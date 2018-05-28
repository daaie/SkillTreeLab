﻿//
// ItemPage.xaml.h
// ItemPage 클래스의 선언
//

#pragma once

#include "ItemPage.g.h"

namespace Que
{
	/// <summary>
	/// 그룹 내의 단일 항목에 대한 정보를 표시하며 같은 그룹에 속한 다른 항목으로
	/// 전환하는 제스처도 허용하는 페이지입니다.
	/// </summary>
	[Windows::UI::Xaml::Data::Bindable]
	public ref class ItemPage sealed
	{
	public:
		ItemPage();

		static void RegisterDependencyProperties();
		static property Windows::UI::Xaml::DependencyProperty^ DefaultViewModelProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get() { return _defaultViewModelProperty; }
		}

		static property Windows::UI::Xaml::DependencyProperty^ NavigationHelperProperty
		{
			Windows::UI::Xaml::DependencyProperty^ get() { return _navigationHelperProperty; }
		}

		/// <summary>
		/// 이는 강력한 형식의 뷰 모델로 변경될 수 있습니다.
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}
		/// <summary>
		/// NavigationHelper는 각 페이지에서 사용되어 탐색을 지원합니다. 
		/// 프로세스 수명 관리
		/// </summary>
		property Que::Common::NavigationHelper^ NavigationHelper
		{
			Que::Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Que::Common::LoadStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
	};
}