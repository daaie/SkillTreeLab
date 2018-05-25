//
// SectionPage.xaml.h
// SectionPage 클래스의 선언
//

#pragma once

#include "SectionPage.g.h"

namespace Que
{
	/// <summary>
	/// 그룹 내에 있는 항목의 미리 보기를 비롯해 단일 그룹에 대한 개요를 표시하는
	/// 페이지입니다.
	/// </summary>
	[Windows::UI::Xaml::Data::Bindable]
	public ref class SectionPage sealed
	{
	public:
		SectionPage();

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
		Windows::ApplicationModel::Resources::ResourceLoader^ _resourceLoader;

		void LoadState(Platform::Object^ sender, Que::Common::LoadStateEventArgs^ e);
		void ItemView_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
	};
}
