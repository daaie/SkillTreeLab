//
// NavigationHelper.h
// NavigationHelper 및 관련 클래스의 선언
//

#pragma once

#include "RelayCommand.h"

namespace Que
{
	namespace Common
	{
		/// <summary>
		///페이지가 상태 로드를 시도할 때 필요한 이벤트 데이터를 유지하는 데 사용되는 클래스입니다.
		/// </summary>
		public ref class LoadStateEventArgs sealed
		{
		public:

			/// <summary>
			/// 이 페이지가 처음 요청되었을 때 <see cref="Frame->Navigate(Type, Object)"/>에 
			/// 전달된 매개 변수 값입니다.
			/// </summary>
			property Platform::Object^ NavigationParameter
			{
				Platform::Object^ get();
			}

			/// <summary>
			/// 이전 세션 동안 이 페이지에 유지된
			/// 사전 상태입니다. 페이지를 처음 방문할 때는 이 값이 null입니다.
			/// </summary>
			property Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ PageState
			{
				Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ get();
			}

		internal:
			LoadStateEventArgs(Platform::Object^ navigationParameter, Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);

		private:
			Platform::Object^ _navigationParameter;
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ _pageState;
		};

		/// <summary>
		/// <see cref="NavigationHelper->LoadState"/> 이벤트를 처리할 메서드를 나타냅니다.
		/// </summary>
		public delegate void LoadStateEventHandler(Platform::Object^ sender, LoadStateEventArgs^ e);

		/// <summary>
		///페이지가 상태 저장을 시도할 때 필요한 이벤트 데이터를 유지하는 데 사용되는 클래스입니다.
		/// </summary>
		public ref class SaveStateEventArgs sealed
		{
		public:

			/// <summary>
			/// serializable 상태로 채워질 빈 사전입니다.
			/// </summary>
			property Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ PageState
			{
				Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ get();
			}

		internal:
			SaveStateEventArgs(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState);

		private:
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ _pageState;
		};

		/// <summary>
		/// <see cref="NavigationHelper->SaveState"/> 이벤트를 처리할 메서드를 나타냅니다.
		/// </summary>
		public delegate void SaveStateEventHandler(Platform::Object^ sender, SaveStateEventArgs^ e);

		/// <summary>
		/// NavigationHelper는 페이지 간의 탐색을 지원합니다. NavigationHelper는 
		/// 앞뒤로 이동하는 데 사용되는 명령을 제공하고 앞뒤로 이동하는 데 사용되는 
		/// 하드웨어 탐색 요청 바로 가기와 Windows Phone의 하드웨어
		/// 뒤로 단추를 처리합니다. 또한 페이지 사이를 탐색할 때 프로세스 수명 관리
		/// 및 상태 관리를 처리하는 SuspensionManager가 통합되어 있습니다.
		/// </summary>
		/// <example>
		/// NavigationHelper를 사용하려면 다음 두 단계를 따르거나
		/// BasicPage 또는 기타 페이지 항목 템플릿(BlankPage가 아니어야 함)으로 시작하십시오.
		/// 
		/// 1) 다음과 같은 위치에서 NavigationHelper의 인스턴스를 만듭니다.
		///		페이지 생성자 같은 위치에 만들고 LoadState 및 
		///		SaveState 이벤트에 대한 콜백을 등록합니다.
		/// <code>
		///		MyPage::MyPage()
		///		{
		///			InitializeComponent();
		///			auto navigationHelper = ref new Common::NavigationHelper(this);
		///			navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &MyPage::LoadState);
		///			navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &MyPage::SaveState);
		///		}
		///		
		///		void MyPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
		///		{ }
		///		void MyPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e)
		///		{ }
		/// </code>
		/// 
		/// 2) 페이지가 참여할 때마다 페이지를 NavigationHelper 호출에 등록합니다.
		///		<see cref="Windows::UI::Xaml::Controls::Page::OnNavigatedTo"/> 
		///		및 <see cref="Windows::UI::Xaml::Controls::Page::OnNavigatedFrom"/> 이벤트를 재정의하여 탐색에 사용됩니다.
		/// <code>
		///		void MyPage::OnNavigatedTo(NavigationEventArgs^ e)
		///		{
		///			NavigationHelper->OnNavigatedTo(e);
		///		}
		///
		///		void MyPage::OnNavigatedFrom(NavigationEventArgs^ e)
		///		{
		///			NavigationHelper->OnNavigatedFrom(e);
		///		}
		/// </code>
		/// </example>
		[Windows::Foundation::Metadata::WebHostHidden]
		[Windows::UI::Xaml::Data::Bindable]
		public ref class NavigationHelper sealed
		{
		public:
			/// <summary>
			/// 뒤로 탐색 기록의 가장 최근 항목으로 이동하기 위한
			/// 뒤로 단추의 명령 속성을 바인딩하는 데 사용되는 <see cref="RelayCommand"/>입니다. 프레임이
			/// 자체적으로 탐색 기록을 관리할 경우에 해당합니다.
			/// 
			/// <see cref="RelayCommand"/>는 가상 메서드 <see cref="GoBack"/>을
			/// 실행 작업으로 사용하고 CanExecute에 대해서는 <see cref="CanGoBack"/>을 사용하도록 설정됩니다.
			/// </summary>
			property RelayCommand^ GoBackCommand
			{
				RelayCommand^ get();
			}

			/// <summary>
			/// 앞으로 탐색 기록의 가장 최근 항목으로 이동하는 데 사용되는 <see cref="RelayCommand"/>입니다. 
			/// 프레임이 자체적으로 탐색 기록을 관리할 경우에 해당합니다.
			/// 
			/// <see cref="RelayCommand"/>는 가상 메서드 <see cref="GoForward"/>를
			/// 실행 작업으로 사용하고 CanExecute에 대해서는 <see cref="CanGoForward"/>를 사용하도록 설정됩니다.
			/// </summary>
			property RelayCommand^ GoForwardCommand
			{
				RelayCommand^ get();
			}

		internal:
			NavigationHelper(Windows::UI::Xaml::Controls::Page^ page,
				RelayCommand^ goBack = nullptr,
				RelayCommand^ goForward = nullptr);

			bool CanGoBack();
			void GoBack();
			bool CanGoForward();
			void GoForward();

			void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);
			void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e);

			event LoadStateEventHandler^ LoadState;
			event SaveStateEventHandler^ SaveState;

		private:
			Platform::WeakReference _page;

			RelayCommand^ _goBackCommand;
			RelayCommand^ _goForwardCommand;

#if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
			Windows::Foundation::EventRegistrationToken _backPressedEventToken;

			void HardwareButton_BackPressed(Platform::Object^ sender,
				Windows::Phone::UI::Input::BackPressedEventArgs^ e);
#else
			bool _navigationShortcutsRegistered;
			Windows::Foundation::EventRegistrationToken _acceleratorKeyEventToken;
			Windows::Foundation::EventRegistrationToken _pointerPressedEventToken;

			void CoreDispatcher_AcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher^ sender,
				Windows::UI::Core::AcceleratorKeyEventArgs^ e);
			void CoreWindow_PointerPressed(Windows::UI::Core::CoreWindow^ sender,
				Windows::UI::Core::PointerEventArgs^ e);
#endif

			Platform::String^ _pageKey;
			Windows::Foundation::EventRegistrationToken _loadedEventToken;
			Windows::Foundation::EventRegistrationToken _unloadedEventToken;
			void OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
			void OnUnloaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

			~NavigationHelper();
		};
	}
}
