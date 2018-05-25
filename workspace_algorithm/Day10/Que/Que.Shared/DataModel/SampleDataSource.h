//
// SampleDataSource.h
// SampleDataSource, SampleDataGroup, SampleDataItem, and SampleDataCommon 클래스의 선언
//

#pragma once

// 이 파일에 의해 정의된 데이터 모델은 강력한 형식 모델의 대표적 예로 제공되어
// 있습니다. 선택한 속성 이름은 표준 항목 템플릿의 데이터 바인딩과 일치합니다.
//
// 응용 프로그램에서는 이 모델을 시작점으로 사용하여 작성하거나
// 모두 삭제하고 필요에 맞게 대체할 수 있습니다. 이 모델을 사용하면
// 응용 프로그램을 처음 시작할 때 App.xaml의 숨김 코드에 있는 데이터 로드 작업을 시작하여 응용 프로그램 반응성이 
// 개선될 수 있습니다.

namespace Que
{
	namespace Data
	{
		ref class SampleDataGroup; // SampleDataItem 및 SampleDataGroup 간의 순환 관계를 확인합니다.

		/// <summary>
		/// 일반 항목 데이터 모델입니다.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataItem sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Platform::String^ Content { Platform::String^ get(); }
			property Platform::String^ ImagePath { Platform::String^ get(); }

			// ICustomPropertyProvider의 구현은 개체에 대한 문자열 표현을 제공하여 액세스 가능성의 자동 이름으로 사용됩니다.
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			SampleDataItem(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description, Platform::String^ content);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::String^ _content;
		};

		/// <summary>
		/// 일반 그룹 데이터 모델입니다.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataGroup sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Windows::Foundation::Collections::IObservableVector<SampleDataItem^>^ Items
			{
				Windows::Foundation::Collections::IObservableVector<SampleDataItem^>^ get();
			}
			property Platform::String^ ImagePath { Platform::String^ get(); }

			// ICustomPropertyProvider의 구현은 개체에 대한 문자열 표현을 제공하여 액세스 가능성의 자동 이름으로 사용됩니다.
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			SampleDataGroup(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::Collections::Vector<SampleDataItem^>^ _items;
		};

		/// <summary>
		/// 정적 json 파일에서 읽은 콘텐츠가 있는 항목 및 그룹의 컬렉션을 만듭니다.
		/// 
		/// SampleDataSource는 프로젝트에 포함된 정적 json 파일에서 읽은 데이터로 
		/// 초기화됩니다. 이렇게 하면 디자인 타임과 런타임 모두에 샘플 데이터가 제공됩니다.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class SampleDataSource sealed
		{
		public:
			property Windows::Foundation::Collections::IObservableVector<SampleDataGroup^>^ Groups
			{
				Windows::Foundation::Collections::IObservableVector<SampleDataGroup^>^ get();
			}

		internal:
			SampleDataSource();
			static concurrency::task<Windows::Foundation::Collections::IIterable<SampleDataGroup^>^> GetGroups();
			static concurrency::task<SampleDataGroup^> GetGroup(Platform::String^ uniqueId);
			static concurrency::task<SampleDataItem^> GetItem(Platform::String^ uniqueId);

		private:
			concurrency::task_completion_event<void> _loadCompletionEvent;
			Platform::Collections::Vector<SampleDataGroup^>^ _groups;
			static concurrency::task<void> Init();

			static SampleDataSource^ _sampleDataSource;
		};
	}
}
