//
// NavigationHelper.cpp
// NavigationHelper 및 관련 클래스의 구현
//

#include "pch.h"
#include "NavigationHelper.h"
#include "RelayCommand.h"
#include "SuspensionManager.h"

using namespace Que::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
using namespace Windows::Phone::UI::Input;
#endif

/// <summary>
/// <see cref="LoadStateEventArgs"/> 클래스의 새 인스턴스를 초기화합니다.
/// </summary>
/// <param name="navigationParameter">
/// 이 페이지가 처음 요청되었을 때 <see cref="Frame->Navigate(Type, Object)"/>에 
/// 전달된 매개 변수 값입니다.
/// </param>
/// <param name="pageState">
/// 이전 세션 동안 이 페이지에 유지된
/// 사전 상태입니다. 페이지를 처음 방문할 때는 이 값이 null입니다.
/// </param>
LoadStateEventArgs::LoadStateEventArgs(Object^ navigationParameter, IMap<String^, Object^>^ pageState)
{
	_navigationParameter = navigationParameter;
	_pageState = pageState;
}

/// <summary>
/// <see cref"LoadStateEventArgs"/> 클래스의 <see cref="NavigationParameter"/> 속성을 가져옵니다.
/// </summary>
Object^ LoadStateEventArgs::NavigationParameter::get()
{
	return _navigationParameter;
}

/// <summary>
/// <see cref"LoadStateEventArgs"/> 클래스의 <see cref="PageState"/> 속성을 가져옵니다.
/// </summary>
IMap<String^, Object^>^ LoadStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// <see cref="SaveStateEventArgs"/> 클래스의 새 인스턴스를 초기화합니다.
/// </summary>
/// <param name="pageState">serializable 상태로 채워질 빈 사전입니다.</param>
SaveStateEventArgs::SaveStateEventArgs(IMap<String^, Object^>^ pageState)
{
	_pageState = pageState;
}

/// <summary>
/// <see cref"SaveStateEventArgs"/> 클래스의 <see cref="PageState"/> 속성을 가져옵니다.
/// </summary>
IMap<String^, Object^>^ SaveStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// <see cref="NavigationHelper"/> 클래스의 새 인스턴스를 초기화합니다.
/// </summary>
/// <param name="page">탐색에 사용되는 현재 페이지에 대한 참조입니다. 
/// 이 참조는 프레임 조작을 가능하게 하고 키보드 
/// 탐색 요청이 페이지가 전체 창을 차지했을 경우에만 발생하도록 합니다.</param>
NavigationHelper::NavigationHelper(Page^ page, RelayCommand^ goBack, RelayCommand^ goForward) :
_page(page),
_goBackCommand(goBack),
_goForwardCommand(goForward)
{
	// 이 페이지가 시각적 트리의 일부인 경우 두 가지를 변경합니다.
	// 1) 응용 프로그램 뷰 상태를 페이지의 시각적 상태에 매핑
	// 2) Windows에서 앞이나 뒤로 이동하는 데 사용되는
	_loadedEventToken = page->Loaded += ref new RoutedEventHandler(this, &NavigationHelper::OnLoaded);

	//// 페이지가 더 이상 표시되지 않는 경우 동일한 변경을 취소합니다.
	_unloadedEventToken = page->Unloaded += ref new RoutedEventHandler(this, &NavigationHelper::OnUnloaded);
}

NavigationHelper::~NavigationHelper()
{
	delete _goBackCommand;
	delete _goForwardCommand;

	_page = nullptr;
}

/// <summary>
/// 페이지가 가상 트리의 일부일 때 호출됩니다.
/// </summary>
/// <param name="sender">이벤트를 트리거한 인스턴스입니다.</param>
/// <param name="e">이벤트의 발생 조건을 설명하는 이벤트 데이터입니다.</param>
void NavigationHelper::OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	_backPressedEventToken = HardwareButtons::BackPressed +=
		ref new EventHandler<BackPressedEventArgs^>(this,
		&NavigationHelper::HardwareButton_BackPressed);
#else
	Page ^page = _page.Resolve<Page>();

	// 키보드 및 마우스 탐색은 전체 창 크기인 경우에만 적용됩니다.
	if (page != nullptr &&
		page->ActualHeight == Window::Current->Bounds.Height &&
		page->ActualWidth == Window::Current->Bounds.Width)
	{
		// 포커스가 필요하지 않도록 창을 직접 수신합니다.
		_acceleratorKeyEventToken = Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated +=
			ref new TypedEventHandler<CoreDispatcher^, AcceleratorKeyEventArgs^>(this,
			&NavigationHelper::CoreDispatcher_AcceleratorKeyActivated);

		_pointerPressedEventToken = Window::Current->CoreWindow->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this,
			&NavigationHelper::CoreWindow_PointerPressed);

		_navigationShortcutsRegistered = true;
	}
#endif
}

/// <summary>
/// 가상 트리에서 페이지를 제거할 때 호출됩니다.
/// </summary>
/// <param name="sender">이벤트를 트리거한 인스턴스입니다.</param>
/// <param name="e">이벤트의 발생 조건을 설명하는 이벤트 데이터입니다.</param>
void NavigationHelper::OnUnloaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
	HardwareButtons::BackPressed -= _backPressedEventToken;
#else
	if (_navigationShortcutsRegistered)
	{
		Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated -= _acceleratorKeyEventToken;
		Window::Current->CoreWindow->PointerPressed -= _pointerPressedEventToken;
		_navigationShortcutsRegistered = false;
	}
#endif

	// 처리기 제거 및 페이지에 대한 참조 해제
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		page->Loaded -= _loadedEventToken;
		page->Unloaded -= _unloadedEventToken;
		delete _goBackCommand;
		delete _goForwardCommand;
		_goForwardCommand = nullptr;
		_goBackCommand = nullptr;
	}
}

#pragma region Navigation support

/// <summary>
/// <see cref="GoBackCommand"/> 속성이 사용하는 메서드로,
/// <see cref="Frame"/>의 뒤로 이동 가능 여부를 확인하기 위해 사용합니다.
/// </summary>
/// <returns>
/// <see cref="Frame"/>에 적어도 하나의 항목이 있을 경우 
/// True입니다(뒤로 탐색 기록 내).
/// </returns>
bool NavigationHelper::CanGoBack()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		return (frame != nullptr && frame->CanGoBack);
	}

	return false;
}

/// <summary>
/// <see cref="GoBackCommand"/> 속성이 사용하는 메서드로,
/// <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/> 메서드를 호출하기 위해 사용합니다.
/// </summary>
void NavigationHelper::GoBack()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		if (frame != nullptr && frame->CanGoBack)
		{
			frame->GoBack();
		}
	}
}

/// <summary>
/// <see cref="GoForwardCommand"/> 속성이 사용하는 메서드로,
/// <see cref="Frame"/>의 앞으로 이동 가능 여부를 확인하기 위해 사용합니다.
/// </summary>
/// <returns>
/// <see cref="Frame"/>에 적어도 하나의 항목이 있을 경우 
/// True입니다(앞으로 탐색 기록 내).
/// </returns>
bool NavigationHelper::CanGoForward()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		return (frame != nullptr && frame->CanGoForward);
	}

	return false;
}

/// <summary>
/// <see cref="GoForwardCommand"/> 속성이 사용하는 메서드로,
/// <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/> 메서드를 호출하기 위해 사용합니다.
/// </summary>
void NavigationHelper::GoForward()
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frame = page->Frame;
		if (frame != nullptr && frame->CanGoForward)
		{
			frame->GoForward();
		}
	}
}

/// <summary>
/// <see cref"NavigationHelper"/> 클래스의 <see cref="GoBackCommand"/> 속성을 가져옵니다.
/// </summary>
RelayCommand^ NavigationHelper::GoBackCommand::get()
{
	if (_goBackCommand == nullptr)
	{
		_goBackCommand = ref new RelayCommand(
			[this](Object^) -> bool
		{
			return CanGoBack();
		},
			[this](Object^) -> void
		{
			GoBack();
		}
		);
	}
	return _goBackCommand;
}

/// <summary>
/// <see cref"NavigationHelper"/> 클래스의 <see cref="GoForwardCommand"/> 속성을 가져옵니다.
/// </summary>
RelayCommand^ NavigationHelper::GoForwardCommand::get()
{
	if (_goForwardCommand == nullptr)
	{
		_goForwardCommand = ref new RelayCommand(
			[this](Object^) -> bool
		{
			return CanGoForward();
		},
			[this](Object^) -> void
		{
			GoForward();
		}
		);
	}
	return _goForwardCommand;
}

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
/// <summary>
/// 뒤로 단추 누름을 처리하고 루트 프레임 기록을 통해 탐색합니다.
/// </summary>
void NavigationHelper::HardwareButton_BackPressed(Object^ sender, BackPressedEventArgs^ e)
{
	if (this->GoBackCommand->CanExecute(nullptr))
	{
		e->Handled = true;
		this->GoBackCommand->Execute(nullptr);
	}
}
#else
/// <summary>
/// 이 페이지가 활성화되고 전체 창 크기로 표시된 경우 Alt 키 조합 등
/// 시스템 키를 포함한 모든 키 입력에서 호출됩니다. 페이지에 포커스가 없으면
/// 페이지 간 키보드 탐색을 검색하는 데 사용됩니다.
/// </summary>
/// <param name="sender">이벤트를 트리거한 인스턴스입니다.</param>
/// <param name="e">이벤트의 발생 조건을 설명하는 이벤트 데이터입니다.</param>
void NavigationHelper::CoreDispatcher_AcceleratorKeyActivated(CoreDispatcher^ sender,
	AcceleratorKeyEventArgs^ e)
{
	sender; // 사용하지 않는 매개 변수
	auto virtualKey = e->VirtualKey;

	// 왼쪽 화살표, 오른쪽 화살표 또는 전용 이전 또는 다음 키를 눌렀을 때만 더
	// 조사합니다.
	if ((e->EventType == CoreAcceleratorKeyEventType::SystemKeyDown ||
		e->EventType == CoreAcceleratorKeyEventType::KeyDown) &&
		(virtualKey == VirtualKey::Left || virtualKey == VirtualKey::Right ||
		virtualKey == VirtualKey::GoBack || virtualKey == VirtualKey::GoForward))
	{
		auto coreWindow = Window::Current->CoreWindow;
		auto downState = Windows::UI::Core::CoreVirtualKeyStates::Down;
		bool menuKey = (coreWindow->GetKeyState(VirtualKey::Menu) & downState) == downState;
		bool controlKey = (coreWindow->GetKeyState(VirtualKey::Control) & downState) == downState;
		bool shiftKey = (coreWindow->GetKeyState(VirtualKey::Shift) & downState) == downState;
		bool noModifiers = !menuKey && !controlKey && !shiftKey;
		bool onlyAlt = menuKey && !controlKey && !shiftKey;

		if ((virtualKey == VirtualKey::GoBack && noModifiers) ||
			(virtualKey == VirtualKey::Left && onlyAlt))
		{
			// 이전 키 또는 Alt+왼쪽 화살표를 누르면 뒤로 탐색
			e->Handled = true;
			GoBackCommand->Execute(this);
		}
		else if ((virtualKey == VirtualKey::GoForward && noModifiers) ||
			(virtualKey == VirtualKey::Right && onlyAlt))
		{
			// 다음 키 또는 Alt+오른쪽 화살표를 누르면 앞으로 탐색
			e->Handled = true;
			GoForwardCommand->Execute(this);
		}
	}
}

/// <summary>
/// 이 페이지가 활성화되고 전체 창 크기로 표시된 경우 모든 마우스 클릭, 터치 스크린 탭
/// 또는 이와 같은 상호 작용에 대해 호출됩니다. 브라우저 스타일의 다음 및 이전 마우스 단추 클릭을
/// 검색하여 페이지 간에 탐색하는 데 사용됩니다.
/// </summary>
/// <param name="sender">이벤트를 트리거한 인스턴스입니다.</param>
/// <param name="e">이벤트의 발생 조건을 설명하는 이벤트 데이터입니다.</param>
void NavigationHelper::CoreWindow_PointerPressed(CoreWindow^ sender, PointerEventArgs^ e)
{
	auto properties = e->CurrentPoint->Properties;

	// 왼쪽 화살표, 오른쪽 화살표 및 가운데 화살표 단추와 함께 누르는 단추를 무시합니다.
	if (properties->IsLeftButtonPressed ||
		properties->IsRightButtonPressed ||
		properties->IsMiddleButtonPressed)
	{
		return;
	}

	// 뒤로 또는 앞으로를 누르면(동시 아님) 해당 방향으로 탐색합니다.
	bool backPressed = properties->IsXButton1Pressed;
	bool forwardPressed = properties->IsXButton2Pressed;
	if (backPressed ^ forwardPressed)
	{
		e->Handled = true;
		if (backPressed)
		{
			if (GoBackCommand->CanExecute(this))
			{
				GoBackCommand->Execute(this);
			}
		}
		else
		{
			if (GoForwardCommand->CanExecute(this))
			{
				GoForwardCommand->Execute(this);
			}
		}
	}
}
#endif

#pragma endregion

#pragma region Process lifetime management

/// <summary>
/// 이 페이지가 프레임에 표시되려고 할 때 호출됩니다.
/// </summary>
/// <param name="e">페이지에 도달한 방법을 설명하는 이벤트 데이터입니다. Parameter
/// 속성은 표시할 그룹을 지정합니다.</param>
void NavigationHelper::OnNavigatedTo(NavigationEventArgs^ e)
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frameState = SuspensionManager::SessionStateForFrame(page->Frame);
		_pageKey = "Page-" + page->Frame->BackStackDepth;

		if (e->NavigationMode == NavigationMode::New)
		{
			// 탐색 스택에 새 페이지를 추가할 때 앞으로 탐색에 대한 기존 상태를
			// 지웁니다.
			auto nextPageKey = _pageKey;
			int nextPageIndex = page->Frame->BackStackDepth;
			while (frameState->HasKey(nextPageKey))
			{
				frameState->Remove(nextPageKey);
				nextPageIndex++;
				nextPageKey = "Page-" + nextPageIndex;
			}

			// 탐색 매개 변수를 새 페이지에 전달합니다.
			LoadState(this, ref new LoadStateEventArgs(e->Parameter, nullptr));
		}
		else
		{
			// 일시 중단된 상태를 로드하고 캐시에서 삭제된 페이지를 다시 만드는 것과
			// 같은 전략을 사용하여 탐색 매개 변수와 유지된 페이지 상태를 페이지로
			// 전달합니다.
			LoadState(this, ref new LoadStateEventArgs(e->Parameter, safe_cast<IMap<String^, Object^>^>(frameState->Lookup(_pageKey))));
		}
	}
}

/// <summary>
/// 이 페이지가 프레임에 더 이상 표시되지 않을 때 호출됩니다.
/// </summary>
/// <param name="e">페이지에 도달한 방법을 설명하는 이벤트 데이터입니다. Parameter
/// 속성은 표시할 그룹을 지정합니다.</param>
void NavigationHelper::OnNavigatedFrom(NavigationEventArgs^ e)
{
	Page ^page = _page.Resolve<Page>();
	if (page != nullptr)
	{
		auto frameState = SuspensionManager::SessionStateForFrame(page->Frame);
		auto pageState = ref new Map<String^, Object^>();
		SaveState(this, ref new SaveStateEventArgs(pageState));
		frameState->Insert(_pageKey, pageState);
	}
}
#pragma endregion