//
// SuspensionManager.h
// SuspensionManager 클래스의 선언입니다.
//

#pragma once

namespace Que
{
	namespace Common
	{
		/// <summary>
		/// SuspensionManager는 전역 세션 상태를 capture하여 응용 프로그램에 대한 프로세스
		/// 수명 관리를 간단하게 합니다. 세션 상태는 다양한 조건에 따라 자동으로
		/// 지워지므로 세션 간에 이동하기 쉽지만 응용 프로그램이 충돌하거나
		/// 업그레이드될 때는 삭제해야 하는 정보를 저장하는 데만 사용해야
		/// 합니다.
		/// </summary>
		class SuspensionManager sealed
		{
		public:
			static void RegisterFrame(Windows::UI::Xaml::Controls::Frame^ frame, Platform::String^ sessionStateKey, Platform::String^ sessionBaseKey = nullptr);
			static void UnregisterFrame(Windows::UI::Xaml::Controls::Frame^ frame);
			static concurrency::task<void> SaveAsync();
			static concurrency::task<void> RestoreAsync(Platform::String^ sessionBaseKey = nullptr);
			static Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ SessionState();
			static Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ SessionStateForFrame(
				Windows::UI::Xaml::Controls::Frame^ frame);
			static void RegisterDependencyProperties();

		private:
			static void RestoreFrameNavigationState(Windows::UI::Xaml::Controls::Frame^ frame);
			static void SaveFrameNavigationState(Windows::UI::Xaml::Controls::Frame^ frame);

			static Platform::Collections::Map<Platform::String^, Platform::Object^>^ _sessionState;
			static const wchar_t* sessionStateFilename;

			static std::vector<Platform::WeakReference> _registeredFrames;
			static Windows::UI::Xaml::DependencyProperty^ FrameSessionStateKeyProperty;
			static Windows::UI::Xaml::DependencyProperty^ FrameSessionBaseKeyProperty;
			static Windows::UI::Xaml::DependencyProperty^ FrameSessionStateProperty;
		};
	}
}
