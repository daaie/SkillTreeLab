//
// HubPage.xaml.cpp
// HubPage 클래스의 구현
//

#include "pch.h"
#include "HubPage.xaml.h"
#include "SectionPage.xaml.h"
#include "ItemPage.xaml.h"

using namespace Que;
using namespace Que::Common;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// 유니버셜 허브 응용 프로그램 프로젝트 템플릿에 대한 설명은 http://go.microsoft.com/fwlink/?LinkID=391958에 나와 있습니다.

HubPage::HubPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &HubPage::LoadState);
}

DependencyProperty^ HubPage::_navigationHelperProperty = nullptr;
DependencyProperty^ HubPage::_defaultViewModelProperty = nullptr;

void HubPage::RegisterDependencyProperties()
{
	if (_navigationHelperProperty == nullptr)
	{
		_navigationHelperProperty = DependencyProperty::Register("NavigationHelper",
			TypeName(Common::NavigationHelper::typeid), TypeName(HubPage::typeid), nullptr);
	}

	if (_defaultViewModelProperty == nullptr)
	{
		_defaultViewModelProperty =
			DependencyProperty::Register("DefaultViewModel",
			TypeName(IObservableMap<String^, Object^>::typeid), TypeName(HubPage::typeid), nullptr);
	}
}


/// <summary>
/// 이 <see cref="Page"/>의 보기 모델을 가져옵니다.
/// 이는 강력한 형식의 뷰 모델로 변경될 수 있습니다.
/// </summary>
IObservableMap<String^, Object^>^ HubPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

/// <summary>
/// trivial 뷰 모델로 사용하기 위해 제작된 <see cref="NavigationHelper"/>의
/// 구현을 가져옵니다.
/// </summary>
NavigationHelper^ HubPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

/// 이 섹션에서 제공되는 메서드는 다음을 허용하는 데에만 사용됩니다.
/// 페이지의 탐색 메서드에 응답하기 위해 NavigationHelper.
/// 
/// 페이지별 논리는 다음에 대한 이벤트 처리기에 있어야 합니다. 
/// <see cref="NavigationHelper::LoadState"/>
/// 및 <see cref="NavigationHelper::SaveState"/>입니다.
/// 탐색 매개 변수는 LoadState 메서드
/// 및 이전 세션 동안 유지된 페이지 상태에서 사용할 수 있습니다. 

void HubPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void HubPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

/// <summary>
/// 탐색 중 전달된 콘텐츠로 페이지를 채웁니다. 이전 세션의 페이지를
/// 다시 만들 때 저장된 상태도 제공됩니다.
/// </summary>
/// <param name="sender">
/// 대개 <see cref="NavigationHelper"/>인 이벤트 소스
/// </param>
/// <see cref="Frame::Navigate(Type, Object)"/>에 전달되는 탐색 매개 변수와
/// 이전 세션 동안 이 페이지에 유지된 상태 사전을 제공하는
/// 이벤트 데이터입니다. 페이지를 처음 방문할 때는 이 상태가 null입니다.< /param>
void HubPage::LoadState(Object^ sender, LoadStateEventArgs^ e)
{
	(void) sender;	// 사용하지 않는 매개 변수
	(void) e;	// 사용하지 않는 매개 변수

	// TODO: 문제 도메인에 적합한 데이터 모델을 만들어 샘플 데이터를 바꿉니다.
	Data::SampleDataSource::GetGroup("Group-4")
		.then([this](Data::SampleDataGroup^ Section3Items)
	{
		DefaultViewModel->Insert("Section3Items", Section3Items);
	}, task_continuation_context::use_current());
}

/// <summary>
/// HubSection 머리글을 클릭할 때 호출됩니다.
/// </summary>
/// <param name="sender">머리글이 클릭된 HubSection이 포함되는 허브입니다.</param>
/// <param name="e">클릭이 시작된 방법을 설명하는 이벤트 데이터입니다.</param>
void HubPage::Hub_SectionHeaderClick(Object^ sender, HubSectionHeaderClickEventArgs^ e)
{
	HubSection^ section = e->Section;
	auto group = section->DataContext;

	// 해당하는 대상 페이지로 이동합니다. 필요한 정보를 탐색 매개 변수로
	// 전달하여 새 페이지를 구성합니다.
	Frame->Navigate(TypeName(SectionPage::typeid), safe_cast<Data::SampleDataGroup^>(group)->UniqueId);
}

/// <summary>
/// 섹션 내의 항목을 클릭할 때 호출됩니다.
/// </summary>
/// <param name="sender">GridView 또는 ListView
/// GridView(또는 응용 프로그램이 기본 뷰 상태인 경우 ListView)입니다.</param>
/// <param name="e">클릭된 항목을 설명하는 이벤트 데이터입니다.</param>
void HubPage::ItemView_ItemClick(Object^ sender, ItemClickEventArgs^ e)
{
	(void) sender;	// 사용하지 않는 매개 변수

	// 해당하는 대상 페이지로 이동합니다. 필요한 정보를 탐색 매개 변수로
	// 전달하여 새 페이지를 구성합니다.
	auto itemId = safe_cast<Data::SampleDataItem^>(e->ClickedItem)->UniqueId;
	Frame->Navigate(TypeName(ItemPage::typeid), itemId);
}
