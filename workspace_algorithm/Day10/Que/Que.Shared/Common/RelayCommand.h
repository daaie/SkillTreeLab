//
// RelayCommand.h
// RelayCommand 및 관련 클래스의 선언
//

#pragma once

// <summary>
// 대리자를 호출하여 다른 개체에 기능을 릴레이하는 것이 
// 목적인 명령입니다. 
// CanExecute 메서드의 기본 반환 값은 'true'입니다.
// <see cref="RaiseCanExecuteChanged"/> 호출이 필요합니다.
// <see cref="CanExecute"/>가 다른 값으로 반환할 때마다
// </summary>


namespace Que
{
	namespace Common
	{
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class RelayCommand sealed :[Windows::Foundation::Metadata::Default] Windows::UI::Xaml::Input::ICommand
		{
		public:
			virtual event Windows::Foundation::EventHandler<Object^>^ CanExecuteChanged;
			virtual bool CanExecute(Object^ parameter);
			virtual void Execute(Object^ parameter);
			virtual ~RelayCommand();

		internal:
			RelayCommand(std::function<bool(Platform::Object^)> canExecuteCallback,
				std::function<void(Platform::Object^)> executeCallback);
			void RaiseCanExecuteChanged();

		private:
			std::function<bool(Platform::Object^)> _canExecuteCallback;
			std::function<void(Platform::Object^)> _executeCallback;
		};
	}
}