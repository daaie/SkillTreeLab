//
// RelayCommand.cpp
// RelayCommand 및 관련 클래스의 구현
//

#include "pch.h"
#include "RelayCommand.h"
#include "NavigationHelper.h"

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
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// 현재 상태에서 <see cref="RelayCommand"/>를 실행할 수 있는지 여부를 확인합니다.
/// </summary>
/// <param name="parameter">
/// 명령으로 사용된 데이터입니다. 명령에 전달되어야 할 데이터가 필요하지 않으면 이 개체는 null로 설정됩니다.
/// </param>
/// <returns>이 명령이 실행되면 true를 그렇지 않으면 false를 반환합니다.</returns>
bool RelayCommand::CanExecute(Object^ parameter)
{
	return (_canExecuteCallback) (parameter);
}

/// <summary>
/// 현재 명령 대상에서 <see cref="RelayCommand"/>를 실행합니다.
/// </summary>
/// <param name="parameter">
/// 명령으로 사용된 데이터입니다. 명령에 전달되어야 할 데이터가 필요하지 않으면 이 개체는 null로 설정됩니다.
/// </param>
void RelayCommand::Execute(Object^ parameter)
{
	(_executeCallback) (parameter);
}

/// <summary>
/// <see cref="CanExecuteChanged"/> 이벤트를 발생시키기 위해 사용된 메서드입니다.
/// <see cref="CanExecute"/> 메서드의 반환 값이
/// 변경된 것을 나타냅니다.
/// </summary>
void RelayCommand::RaiseCanExecuteChanged()
{
	CanExecuteChanged(this, nullptr);
}

/// <summary>
/// RelayCommand 클래스 소멸자입니다.
/// </summary>
RelayCommand::~RelayCommand()
{
	_canExecuteCallback = nullptr;
	_executeCallback = nullptr;
};

/// <summary>
/// 항상 실행할 수 있는 새 명령을 만듭니다.
/// </summary>
/// <param name="canExecuteCallback">실행 상태 논리입니다.</param>
/// <param name="executeCallback">실행 논리입니다.</param>
RelayCommand::RelayCommand(std::function<bool(Platform::Object^)> canExecuteCallback,
	std::function<void(Platform::Object^)> executeCallback) :
	_canExecuteCallback(canExecuteCallback),
	_executeCallback(executeCallback)
	{
	}
