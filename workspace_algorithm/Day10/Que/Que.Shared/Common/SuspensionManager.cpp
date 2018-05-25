//
// SuspensionManager.cpp
// SuspensionManager 클래스의 구현
//

#include "pch.h"
#include "SuspensionManager.h"

#include <algorithm>

using namespace Que::Common;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

Map<String^, Object^>^ SuspensionManager::_sessionState = ref new Map<String^, Object^>();

const wchar_t* SuspensionManager::sessionStateFilename = L"_sessionState.dat";

std::vector<WeakReference> SuspensionManager::_registeredFrames;

DependencyProperty^ SuspensionManager::FrameSessionStateKeyProperty = nullptr;
DependencyProperty^ SuspensionManager::FrameSessionBaseKeyProperty = nullptr;
DependencyProperty^ SuspensionManager::FrameSessionStateProperty = nullptr;

void SuspensionManager::RegisterDependencyProperties()
{
	if (FrameSessionStateProperty == nullptr)
	{
		FrameSessionStateProperty = DependencyProperty::RegisterAttached("_FrameSessionStateProperty",
			TypeName(IMap<String^, Object^>::typeid), TypeName(SuspensionManager::typeid), nullptr);
	}

	if (FrameSessionBaseKeyProperty == nullptr)
	{
		FrameSessionBaseKeyProperty = DependencyProperty::RegisterAttached("_FrameSessionBaseKeyProperty",
			TypeName(String::typeid), TypeName(SuspensionManager::typeid), nullptr);
	}

	if (FrameSessionStateKeyProperty == nullptr)
	{
		FrameSessionStateKeyProperty = DependencyProperty::RegisterAttached("_FrameSessionStateKeyProperty",
			TypeName(String::typeid), TypeName(SuspensionManager::typeid), nullptr);
	}
}

class ObjectSerializeHelper
{
public:
	// serialize된 형식을 식별하는 데 사용되는 코드입니다.
	enum StreamTypes {
		NullPtrType = 0,

		// 지원되는 IPropertyValue 형식입니다.
		UInt8Type, UInt16Type, UInt32Type, UInt64Type, Int16Type, Int32Type, Int64Type,
		SingleType, DoubleType, BooleanType, Char16Type, GuidType, StringType,

		// 지원되는 추가 형식입니다.
		StringToObjectMapType,

		// 스트림 무결성을 확인하는 데 사용되는 표식 값입니다.
		MapEndMarker
	};
	static String^ ReadString(DataReader^ reader);
	static IMap<String^, Object^>^ ReadStringToObjectMap(DataReader^ reader);
	static Object^ ReadObject(DataReader^ reader);
	static void WriteString(DataWriter^ writer, String^ string);
	static void WriteProperty(DataWriter^ writer, IPropertyValue^ propertyValue);
	static void WriteStringToObjectMap(DataWriter^ writer, IMap<String^, Object^>^ map);
	static void WriteObject(DataWriter^ writer, Object^ object);
};

/// <summary>
/// 현재 세션에 대해 전역 세션 상태에 대한 액세스를 제공합니다. 이 상태는 <see cref="SaveAsync"/> 로
/// serialize되고 <see cref="RestoreAsync"/>로 복원됩니다. 이 경우 값은 다음 중
/// 하나여야 합니다. 정수를 포함하는 boxed 값, 부동 소수점 singles 및 doubles,
/// 와이드 문자, 부울, Strings 및 Guids 또는 Map<String^, Object^> 여기서 맵 값은
/// 동일한 제약 조건에 따라 적용됩니다. 세션 상태는 가능한 한 간단해야 합니다.
/// </summary>
IMap<String^, Object^>^ SuspensionManager::SessionState()
{
	return _sessionState;
}

/// <summary>
/// <see cref="Frame"/> 인스턴스를 등록하면 해당 탐색 기록을
/// <see cref="SessionState"/>에 저장하고 여기서 복원합니다. 프레임이 세션 상태 관리에
/// 참여할 경우 프레임은 작성 후 바로 등록해야 합니다.
/// 상태가 지정된 키에 대해 이미 복원된 경우 등록하면
/// 탐색 기록은 바로 복원됩니다. <see cref="RestoreAsync(String)"/>의
/// 후속 호출도 탐색 기록을 복원합니다.
/// </summary>
/// <param name="frame">탐색 기록이 <see cref="SuspensionManager"/>에서 관리되어야 하는
/// <see cref="SuspensionManager"/></param>
/// <param name="sessionStateKey">탐색 관련 정보를 저장하는 데 사용되는 <see cref="SessionState"/>의
/// 고유한 키입니다.</param>
/// <param name="sessionBaseKey">세션의 형식을 식별하는 선택 키입니다.
/// 이것은 여러 응용 프로그램 시작 시나리오의 구분에 사용할 수 있습니다.</param>
void SuspensionManager::RegisterFrame(Frame^ frame, String^ sessionStateKey, String^ sessionBaseKey)
{
	if (frame->GetValue(FrameSessionStateKeyProperty) != nullptr)
	{
		throw ref new FailureException("Frames can only be registered to one session state key");
	}

	if (frame->GetValue(FrameSessionStateProperty) != nullptr)
	{
		throw ref new FailureException("Frames must be either be registered before accessing frame session state, or not registered at all");
	}

	if (sessionBaseKey != nullptr)
	{
		frame->SetValue(FrameSessionBaseKeyProperty, sessionBaseKey);
		sessionStateKey = sessionBaseKey + "_" + sessionStateKey;
	}

	// 종속성 속성을 사용하여 세션 키를 프레임에 연결하고, 탐색 상태를 관리해야 하는
	// 프레임 목록을 유지합니다.
	frame->SetValue(FrameSessionStateKeyProperty, sessionStateKey);
	_registeredFrames.insert(_registeredFrames.begin(), WeakReference(frame));

	// 탐색 상태를 복원할 수 있는지 확인하십시오.
	RestoreFrameNavigationState(frame);
}

/// <summary>
/// <see cref="RegisterFrame"/>으로 <see cref="SessionState"/>에서 이전에 등록된
/// <see cref="Frame"/>의 연결을 끊습니다. 이전에 캡처된 모두 탐색 상태는
/// 제거됩니다.
/// </summary>
/// <param name="frame">탐색 기록이 더 이상 관리되지 않는
/// 인스턴스입니다.</param>
void SuspensionManager::UnregisterFrame(Frame^ frame)
{
	// 세션 상태를 제거하고 탐색 상태가 더 이상 접근할 수 없는 약한 참조와 함께 저장되는
	// 프레임 목록에서 프레임을 제거합니다.
	auto key = safe_cast<String^>(frame->GetValue(FrameSessionStateKeyProperty));
	if (SessionState()->HasKey(key))
	{
		SessionState()->Remove(key);
	}
	_registeredFrames.erase(
		std::remove_if(_registeredFrames.begin(), _registeredFrames.end(), [=](WeakReference& e)
	{
		auto testFrame = e.Resolve<Frame>();
		return testFrame == nullptr || testFrame == frame;
	}),
		_registeredFrames.end()
		);
}

/// <summary>
/// 지정된 <see cref="Frame"/>에 연결된 세션 상태에 대한 저장소를 제공합니다.
/// <see cref="RegisterFrame"/>으로 이미 등록된 프레임에는
/// 세션 상태가 <see cref="SessionState"/>의 일부로 자동으로 저장 및
/// 복원됩니다. 등록되지 않은 프레임에는 탐색 캐시에서
/// 삭제된 페이지를 복원할 때 유용할 수 있는 임시 상태가
/// 있습니다.
/// </summary>
/// <remarks>앱은 <see cref="NavigationHelper"/>를 사용하도록 선택하여
/// 프레임 세션 상태로 직접 작업하는 대신 페이지별 상태를 관리할 수도 있습니다.</remarks>
/// <param name="frame">세션 상태가 필요한 인스턴스입니다.</param>
/// <returns><see cref="SessionState"/>와 동일한 serialization 메커니즘을 따르는 상태
/// 컬렉션입니다.</returns>
IMap<String^, Object^>^ SuspensionManager::SessionStateForFrame(Frame^ frame)
{
	auto frameState = safe_cast<IMap<String^, Object^>^>(frame->GetValue(FrameSessionStateProperty));

	if (frameState == nullptr)
	{
		auto frameSessionKey = safe_cast<String^>(frame->GetValue(FrameSessionStateKeyProperty));
		if (frameSessionKey != nullptr)
		{
			// 등록된 프레임은 해당 세션 상태를 반영합니다.
			if (!_sessionState->HasKey(frameSessionKey))
			{
				_sessionState->Insert(frameSessionKey, ref new Map<String^, Object^>());
			}
			frameState = safe_cast<IMap<String^, Object^>^>(_sessionState->Lookup(frameSessionKey));
		}
		else
		{
			// 등록되지 않은 프레임에는 임시 상태가 있습니다.
			frameState = ref new Map<String^, Object^>();
		}
		frame->SetValue(FrameSessionStateProperty, frameState);
	}
	return frameState;
}

void SuspensionManager::RestoreFrameNavigationState(Frame^ frame)
{
	auto frameState = SessionStateForFrame(frame);
	if (frameState->HasKey("Navigation"))
	{
		frame->SetNavigationState(safe_cast<String^>(frameState->Lookup("Navigation")));
	}
}

void SuspensionManager::SaveFrameNavigationState(Frame^ frame)
{
	auto frameState = SessionStateForFrame(frame);
	frameState->Insert("Navigation", frame->GetNavigationState());
}

/// <summary>
/// 현재 <see cref="SessionState"/>를 저장합니다. <see cref="RegisterFrame"/>에 등록된
/// 모든 <see cref="Frame"/> 인스턴스는 현재 탐색 스택도 유지합니다.
/// 따라서 활성 <see cref="Page"/>에 해당 상태를 저장할 수 있는 기회를
/// 제공합니다.
/// </summary>
/// <returns>세션 상태가 저장된 시기를 반영하는 비동기 작업입니다.</returns>
task<void> SuspensionManager::SaveAsync(void)
{
	// 등록된 모든 프레임에 대한 탐색 상태를 저장합니다.
	for (auto && weakFrame : _registeredFrames)
	{
		auto frame = weakFrame.Resolve<Frame>();
		if (frame != nullptr) SaveFrameNavigationState(frame);
	}

	// 공유 상태에 대해 비동기적으로 액세스하지 못하도록 세션 상태를 동기적으로
	// serialize합니다.
	auto sessionData = ref new InMemoryRandomAccessStream();
	auto sessionDataWriter = ref new DataWriter(sessionData->GetOutputStreamAt(0));
	ObjectSerializeHelper::WriteObject(sessionDataWriter, _sessionState);

	// 세션 상태가 비동기적으로 캡처되면 결과를 디스크에 쓰는 비동기
	// 프로세스를 시작합니다.
	return task<unsigned int>(sessionDataWriter->StoreAsync()).then([=](unsigned int)
	{
		return ApplicationData::Current->LocalFolder->CreateFileAsync(StringReference(sessionStateFilename),
			CreationCollisionOption::ReplaceExisting);
	})
		.then([=](StorageFile^ createdFile)
	{
		return createdFile->OpenAsync(FileAccessMode::ReadWrite);
	})
		.then([=](IRandomAccessStream^ newStream)
	{
		return RandomAccessStream::CopyAsync(
			sessionData->GetInputStreamAt(0), newStream->GetOutputStreamAt(0));
	})
		.then([=](UINT64 copiedBytes)
	{
		(void) copiedBytes; // 사용하지 않는 매개 변수
		return;
	});
}

/// <summary>
/// 이전에 저장된 <see cref="SessionState"/>를 복원합니다. <see cref="RegisterFrame"/>으로 등록된 모든
/// <see cref="Frame"/> 인스턴스도 이전의 탐색 상태를 복원합니다.
/// 따라서 활성 <see cref="Page"/>에 해당 상태를 복원할 수 있는 기회를
/// 제공합니다.
/// </summary>
/// <param name="sessionBaseKey">세션의 형식을 식별하는 선택 키입니다.
/// 이것은 여러 응용 프로그램 시작 시나리오의 구분에 사용할 수 있습니다.</param>
/// <returns>세션 상태를 읽은 시기를 반영하는 비동기 작업입니다.
/// <see cref="SessionState"/>의 내용은 이 작업이 완료될 때까지 신뢰해서는
/// 안 됩니다.</returns>
task<void> SuspensionManager::RestoreAsync(String^ sessionBaseKey)
{
	_sessionState->Clear();

	task<StorageFile^> getFileTask(ApplicationData::Current->LocalFolder->GetFileAsync(StringReference(sessionStateFilename)));
	return getFileTask.then([=](StorageFile^ stateFile)
	{
		task<BasicProperties^> getBasicPropertiesTask(stateFile->GetBasicPropertiesAsync());
		return getBasicPropertiesTask.then([=](BasicProperties^ stateFileProperties)
		{
			auto size = unsigned int(stateFileProperties->Size);
			if (size != stateFileProperties->Size) throw ref new FailureException("Session state larger than 4GB");
			task<IRandomAccessStreamWithContentType^> openReadTask(stateFile->OpenReadAsync());
			return openReadTask.then([=](IRandomAccessStreamWithContentType^ stateFileStream)
			{
				auto stateReader = ref new DataReader(stateFileStream);
				return task<unsigned int>(stateReader->LoadAsync(size)).then([=](unsigned int bytesRead)
				{
					(void) bytesRead; // 사용하지 않는 매개 변수
					// 세션 상태를 deserialize합니다.
					Object^ content = ObjectSerializeHelper::ReadObject(stateReader);
					_sessionState = (Map<String^, Object^>^)content;

					// 등록된 모든 프레임을 저장된 상태로 복원합니다.
					for (auto && weakFrame : _registeredFrames)
					{
						auto frame = weakFrame.Resolve<Frame>();
						if (frame != nullptr && safe_cast<String^>(frame->GetValue(FrameSessionBaseKeyProperty)) == sessionBaseKey)
						{
							frame->ClearValue(FrameSessionStateProperty);
							RestoreFrameNavigationState(frame);
						}
					}
				}, task_continuation_context::use_current());
			});
		});
	});
}

#pragma region Object serialization for a known set of types

void ObjectSerializeHelper::WriteString(DataWriter^ writer, String^ string)
{
	writer->WriteByte(StringType);
	writer->WriteUInt32(writer->MeasureString(string));
	writer->WriteString(string);
}

void ObjectSerializeHelper::WriteProperty(DataWriter^ writer, IPropertyValue^ propertyValue)
{
	switch (propertyValue->Type)
	{
	case PropertyType::UInt8:
		writer->WriteByte(StreamTypes::UInt8Type);
		writer->WriteByte(propertyValue->GetUInt8());
		return;
	case PropertyType::UInt16:
		writer->WriteByte(StreamTypes::UInt16Type);
		writer->WriteUInt16(propertyValue->GetUInt16());
		return;
	case PropertyType::UInt32:
		writer->WriteByte(StreamTypes::UInt32Type);
		writer->WriteUInt32(propertyValue->GetUInt32());
		return;
	case PropertyType::UInt64:
		writer->WriteByte(StreamTypes::UInt64Type);
		writer->WriteUInt64(propertyValue->GetUInt64());
		return;
	case PropertyType::Int16:
		writer->WriteByte(StreamTypes::Int16Type);
		writer->WriteUInt16(propertyValue->GetInt16());
		return;
	case PropertyType::Int32:
		writer->WriteByte(StreamTypes::Int32Type);
		writer->WriteUInt32(propertyValue->GetInt32());
		return;
	case PropertyType::Int64:
		writer->WriteByte(StreamTypes::Int64Type);
		writer->WriteUInt64(propertyValue->GetInt64());
		return;
	case PropertyType::Single:
		writer->WriteByte(StreamTypes::SingleType);
		writer->WriteSingle(propertyValue->GetSingle());
		return;
	case PropertyType::Double:
		writer->WriteByte(StreamTypes::DoubleType);
		writer->WriteDouble(propertyValue->GetDouble());
		return;
	case PropertyType::Boolean:
		writer->WriteByte(StreamTypes::BooleanType);
		writer->WriteBoolean(propertyValue->GetBoolean());
		return;
	case PropertyType::Char16:
		writer->WriteByte(StreamTypes::Char16Type);
		writer->WriteUInt16(propertyValue->GetChar16());
		return;
	case PropertyType::Guid:
		writer->WriteByte(StreamTypes::GuidType);
		writer->WriteGuid(propertyValue->GetGuid());
		return;
	case PropertyType::String:
		WriteString(writer, propertyValue->GetString());
		return;
	default:
		throw ref new InvalidArgumentException("Unsupported property type");
	}
}

void ObjectSerializeHelper::WriteStringToObjectMap(DataWriter^ writer, IMap<String^, Object^>^ map)
{
	writer->WriteByte(StringToObjectMapType);
	writer->WriteUInt32(map->Size);
	for (auto && pair : map)
	{
		WriteObject(writer, pair->Key);
		WriteObject(writer, pair->Value);
	}
	writer->WriteByte(MapEndMarker);
}

void ObjectSerializeHelper::WriteObject(DataWriter^ writer, Object^ object)
{
	if (object == nullptr)
	{
		writer->WriteByte(NullPtrType);
		return;
	}

	auto propertyObject = dynamic_cast<IPropertyValue^>(object);
	if (propertyObject != nullptr)
	{
		WriteProperty(writer, propertyObject);
		return;
	}

	auto mapObject = dynamic_cast<IMap<String^, Object^>^>(object);
	if (mapObject != nullptr)
	{
		WriteStringToObjectMap(writer, mapObject);
		return;
	}

	throw ref new InvalidArgumentException("Unsupported data type");
}

String^ ObjectSerializeHelper::ReadString(DataReader^ reader)
{
	int length = reader->ReadUInt32();
	String^ string = reader->ReadString(length);
	return string;
}

IMap<String^, Object^>^ ObjectSerializeHelper::ReadStringToObjectMap(DataReader^ reader)
{
	auto map = ref new Map<String^, Object^>();
	auto size = reader->ReadUInt32();
	for (unsigned int index = 0; index < size; index++)
	{
		auto key = safe_cast<String^>(ReadObject(reader));
		auto value = ReadObject(reader);
		map->Insert(key, value);
	}
	if (reader->ReadByte() != StreamTypes::MapEndMarker)
	{
		throw ref new InvalidArgumentException("Invalid stream");
	}
	return map;
}

Object^ ObjectSerializeHelper::ReadObject(DataReader^ reader)
{
	auto type = reader->ReadByte();
	switch (type)
	{
	case StreamTypes::NullPtrType:
		return nullptr;
	case StreamTypes::UInt8Type:
		return reader->ReadByte();
	case StreamTypes::UInt16Type:
		return reader->ReadUInt16();
	case StreamTypes::UInt32Type:
		return reader->ReadUInt32();
	case StreamTypes::UInt64Type:
		return reader->ReadUInt64();
	case StreamTypes::Int16Type:
		return reader->ReadInt16();
	case StreamTypes::Int32Type:
		return reader->ReadInt32();
	case StreamTypes::Int64Type:
		return reader->ReadInt64();
	case StreamTypes::SingleType:
		return reader->ReadSingle();
	case StreamTypes::DoubleType:
		return reader->ReadDouble();
	case StreamTypes::BooleanType:
		return reader->ReadBoolean();
	case StreamTypes::Char16Type:
		return (char16_t) reader->ReadUInt16();
	case StreamTypes::GuidType:
		return reader->ReadGuid();
	case StreamTypes::StringType:
		return ReadString(reader);
	case StreamTypes::StringToObjectMapType:
		return ReadStringToObjectMap(reader);
	default:
		throw ref new InvalidArgumentException("Unsupported property type");
	}
}

#pragma endregion
