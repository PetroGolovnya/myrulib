#include "FbFrameBase.h"
#include "FbConst.h"
#include "FbExportDlg.h"
#include "FbMainFrame.h"
#include "MyRuLibApp.h"
#include "FbFilterDlg.h"
#include "FbColumnDlg.h"

IMPLEMENT_CLASS(FbFrameBase, FbAuiMDIChildFrame)

BEGIN_EVENT_TABLE(FbFrameBase, FbAuiMDIChildFrame)
	EVT_TREE_SEL_CHANGED(ID_MASTER_LIST, FbFrameBase::OnMasterSelected)
	EVT_ACTIVATE(FbFrameBase::OnActivated)
	EVT_TREE_ITEM_COLLAPSING(ID_MASTER_LIST, FbFrameBase::OnTreeCollapsing)
	EVT_MENU(wxID_SAVE, FbFrameBase::OnExternal)
	EVT_MENU(wxID_SELECTALL, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_UNSELECTALL, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_EDIT_COMMENTS, FbFrameBase::OnSubmenu)
	EVT_COMMAND(ID_AUTHOR_INFO, fbEVT_BOOK_ACTION, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_SPLIT_HORIZONTAL, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_SPLIT_VERTICAL, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_SPLIT_NOTHING, FbFrameBase::OnSubmenu)
	EVT_MENU(ID_MODE_TREE, FbFrameBase::OnChangeMode)
	EVT_MENU(ID_MODE_LIST, FbFrameBase::OnChangeMode)
	EVT_MENU(ID_FILTER_SET, FbFrameBase::OnFilterSet)
	EVT_MENU(ID_FILTER_USE, FbFrameBase::OnFilterUse)
	EVT_MENU(ID_DIRECTION, FbFrameBase::OnDirection)
	EVT_MENU(ID_ORDER_AUTHOR, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_TITLE, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_RATING, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_DATE, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_LANG, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_SIZE, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_ORDER_TYPE, FbFrameBase::OnChangeOrder)
	EVT_MENU(ID_SHOW_COLUMNS, FbFrameBase::OnShowColumns)
	EVT_UPDATE_UI(ID_MODE_LIST, FbFrameBase::OnChangeModeUpdateUI)
	EVT_UPDATE_UI(ID_MODE_TREE, FbFrameBase::OnChangeModeUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_HORIZONTAL, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_VERTICAL, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_NOTHING, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_HORIZONTAL, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_VERTICAL, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_SPLIT_NOTHING, FbFrameBase::OnChangeViewUpdateUI)
	EVT_UPDATE_UI(ID_FILTER_USE, FbFrameBase::OnFilterUseUpdateUI)
	EVT_UPDATE_UI(ID_DIRECTION, FbFrameBase::OnDirectionUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_MENU, FbFrameBase::OnMenuOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_AUTHOR, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_TITLE, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_RATING, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_DATE, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_LANG, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_SIZE, FbFrameBase::OnChangeOrderUpdateUI)
	EVT_UPDATE_UI(ID_ORDER_TYPE, FbFrameBase::OnChangeOrderUpdateUI)
    EVT_LIST_COL_CLICK(ID_BOOKS_LISTCTRL, FbFrameBase::OnColClick)
	EVT_COMMAND(ID_EMPTY_BOOKS, fbEVT_BOOK_ACTION, FbFrameBase::OnEmptyBooks)
	EVT_COMMAND(ID_BOOKS_COUNT, fbEVT_BOOK_ACTION, FbFrameBase::OnBooksCount)
END_EVENT_TABLE()

FbFrameBase::FbFrameBase(wxAuiMDIParentFrame * parent, wxWindowID id, const wxString & title) :
	FbAuiMDIChildFrame(parent, id, title),
	m_MasterList(NULL), m_BooksPanel(NULL), m_ToolBar(NULL), m_MasterThread(NULL)
{
	m_filter.Load();
}

FbFrameBase::~FbFrameBase()
{
	if (m_MasterThread) m_MasterThread->Wait();
	wxDELETE(m_MasterThread);
}

bool FbFrameBase::Create(wxAuiMDIParentFrame * parent, wxWindowID id, const wxString & title)
{
	bool res = wxAuiMDIChildFrame::Create(parent, id, title);
	if (res) CreateControls();
	return res;
}

void FbFrameBase::CreateControls()
{
	UpdateMenu();
	UpdateFonts(false);
	ShowFullScreen(IsFullScreen());
	Layout();

	if (m_MasterList)
		m_MasterList->SetFocus();
	else
		m_BooksPanel->SetFocus();
}

void FbFrameBase::Localize(bool bUpdateMenu)
{
	SetTitle(GetTitle());
    FbAuiMDIChildFrame::Localize(bUpdateMenu);
//    if (bUpdateMenu) UpdateStatus();

	wxDELETE(m_ToolBar);
	m_ToolBar = CreateToolBar();
	if (m_ToolBar) GetSizer()->Insert(0, m_ToolBar, 0, wxGROW);

	if (m_MasterList) {
		m_MasterList->EmptyColumns();
		CreateColumns();
		m_MasterList->Refresh();
	}
    m_BooksPanel->Localize();
}

void FbFrameBase::CreateBooksPanel(wxWindow * parent)
{
	m_BooksPanel = new FbBookPanel(parent, wxSize(500, 400), GetViewKey(), GetModeKey(), GetId());
}

void FbFrameBase::OnSubmenu(wxCommandEvent& event)
{
	wxPostEvent(m_BooksPanel, event);
}

void FbFrameBase::OnExternal(wxCommandEvent& event)
{
	FbExportDlg::Execute(this, m_BooksPanel);
}

void FbFrameBase::OnEmptyBooks(wxCommandEvent& event)
{
	m_BooksPanel->EmptyBooks();
}

void FbFrameBase::OnMasterSelected(wxTreeEvent & event)
{
	UpdateBooklist();
}

void FbFrameBase::OnChangeOrder(wxCommandEvent& event)
{
	m_BooksPanel->SetOrderID( event.GetId() );
	UpdateBooklist();
}

void FbFrameBase::OnDirection(wxCommandEvent& event)
{
	m_BooksPanel->RevertOrder();
	UpdateBooklist();
}

void FbFrameBase::OnChangeMode(wxCommandEvent& event)
{
	FbListMode mode = event.GetId() == ID_MODE_TREE ? FB2_MODE_TREE : FB2_MODE_LIST;

	int param = GetModeKey();
	if (param) FbParams().SetValue(param, mode);

	if (mode == FB2_MODE_TREE) m_BooksPanel->m_BookList->SetSortedColumn(0);

	m_BooksPanel->SetListMode(mode);
	UpdateBooklist();
}

void FbFrameBase::OnTreeCollapsing(wxTreeEvent & event)
{
	event.Veto();
}

void FbFrameBase::OnActivated(wxActivateEvent & event)
{
//	UpdateStatus();
	event.Skip();
}

void FbFrameBase::UpdateFonts(bool refresh)
{
	if (m_MasterList) {
		m_MasterList->SetFont( FbParams::GetFont(FB_FONT_MAIN) );
		if (refresh) m_MasterList->Update();
	}
	if (m_BooksPanel) {
		m_BooksPanel->UpdateFonts(refresh);
	}
}

void FbFrameBase::UpdateInfo(int id)
{
	if (m_BooksPanel) m_BooksPanel->UpdateInfo(id);
}

void FbFrameBase::OnDirectionUpdateUI(wxUpdateUIEvent & event)
{
	event.Check( m_BooksPanel->m_BookList->GetSortedColumn() < 0 );
}

void FbFrameBase::OnMenuOrderUpdateUI(wxUpdateUIEvent & event)
{
	event.Enable( m_BooksPanel->GetListMode() == FB2_MODE_LIST );
}

void FbFrameBase::OnChangeOrderUpdateUI(wxUpdateUIEvent & event)
{
	if (event.GetId() == m_BooksPanel->GetOrderID()) event.Check(true);
}

wxToolBar * FbFrameBase::CreateToolBar(long style, wxWindowID winid, const wxString& name)
{
	wxToolBar * toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style, name);
	toolbar->SetFont(FbParams::GetFont(FB_FONT_TOOL));
	toolbar->AddTool(wxID_SAVE, _("Export"), wxArtProvider::GetBitmap(wxART_FILE_SAVE), _("Export to external device"));
	toolbar->Realize();
	return toolbar;
}

void FbFrameBase::OnColClick(wxListEvent& event)
{
	UpdateBooklist();
}

void FbFrameBase::OnBooksCount(wxCommandEvent& event)
{
	UpdateStatus(event.GetInt());
}

void FbFrameBase::UpdateStatus(int count)
{
	FbMainFrame * frame = wxDynamicCast(GetMDIParentFrame(), FbMainFrame);
	if (frame) {
		wxString msg = wxString::Format(wxT(" %d "), count);
		msg << wxPLURAL("book", "books", count);
		frame->SetStatus(msg);
	}
}

int FbFrameBase::GetBookCount()
{
	return 0;
//	return (m_BooksPanel) ? m_BooksPanel->m_BookList->GetCount() : 0;
}

void FbFrameBase::ShowFullScreen(bool show)
{
	if (m_ToolBar) m_ToolBar->Show(!show);
	Layout();
}

bool FbFrameBase::IsFullScreen()
{
	wxTopLevelWindow * frame = (wxTopLevelWindow*) wxGetApp().GetTopWindow();
	return frame->IsFullScreen();
}

FbFrameBase::MenuBar::MenuBar()
{
	Append(new MenuFile,   _("&File"));
	Append(new MenuLib,    _("&Library"));
	Append(new MenuFrame,  _("&Catalog"));
	Append(new MenuBook,   _("&Books"));
	Append(new MenuView,   _("&View"));
	Append(new MenuSetup,  _("&Tools"));
	Append(new MenuWindow, _("&Window"));
	Append(new MenuHelp,   _("&?"));
}

wxMenuBar * FbFrameBase::CreateMenuBar()
{
	return new MenuBar;
}

void FbFrameBase::OnFilterUseUpdateUI(wxUpdateUIEvent & event)
{
	event.Check(m_filter.IsEnabled());
}

void FbFrameBase::OnFilterSet(wxCommandEvent& event)
{
	bool ok = FbFilterDlg::Execute(m_filter);
	if (ok) UpdateBooklist();
}

void FbFrameBase::OnFilterUse(wxCommandEvent& event)
{
	FbParams().SetValue(FB_USE_FILTER, 0);
	m_filter.Enable(not m_filter.IsEnabled());
	UpdateBooklist();
}

int FbFrameBase::GetModeKey()
{
	switch (GetId()) {
		case ID_FRAME_AUTHOR: return FB_MODE_AUTHOR;
		case ID_FRAME_GENRES: return FB_MODE_GENRES;
		case ID_FRAME_FOLDER: return FB_MODE_FOLDER;
		case ID_FRAME_SEARCH: return FB_MODE_SEARCH;
		case ID_FRAME_DOWNLD: return FB_MODE_DOWNLD;
		case ID_FRAME_SEQUEN: return FB_MODE_SEQUEN;
		default: return 0;
	}
}

int FbFrameBase::GetViewKey()
{
	switch (GetId()) {
		case ID_FRAME_AUTHOR: return FB_VIEW_AUTHOR;
		case ID_FRAME_GENRES: return FB_VIEW_GENRES;
		case ID_FRAME_FOLDER: return FB_VIEW_FOLDER;
		case ID_FRAME_SEARCH: return FB_VIEW_SEARCH;
		case ID_FRAME_DOWNLD: return FB_VIEW_DOWNLD;
		case ID_FRAME_SEQUEN: return FB_VIEW_SEQUEN;
		default: return 0;
	}
}

void FbFrameBase::OnChangeModeUpdateUI(wxUpdateUIEvent & event)
{
	FbListMode mode = m_BooksPanel->GetListMode();
	switch (event.GetId()) {
		case ID_MODE_LIST: if (mode == FB2_MODE_LIST) event.Check(true); break;
		case ID_MODE_TREE: if (mode == FB2_MODE_TREE) event.Check(true); break;
	}
}

void FbFrameBase::OnChangeViewUpdateUI(wxUpdateUIEvent & event)
{
	FbViewMode mode = m_BooksPanel->GetViewMode();
	switch (event.GetId()) {
		case ID_SPLIT_HORIZONTAL: if (mode == FB2_VIEW_HORISONTAL) event.Check(true); break;
		case ID_SPLIT_VERTICAL: if (mode == FB2_VIEW_VERTICAL) event.Check(true); break;
		case ID_SPLIT_NOTHING: if (mode == FB2_VIEW_NOTHING) event.Check(true); break;
	}
}

void FbFrameBase::OnShowColumns(wxCommandEvent& event)
{
	wxArrayInt columns;
	m_BooksPanel->m_BookList->GetColumns(columns);
	bool ok = FbColumnDlg::Execute(this, columns);
	if (ok) {
		m_BooksPanel->CreateColumns(columns);
		wxString text = FbColumns::Get(columns);
		FbParams().SetText(GetId(), FB_BOOK_COLUMNS, text);
	}
}

void FbFrameBase::UpdateBooklist()
{
	if (m_MasterList) m_BooksPanel->Reset(m_MasterList->GetInfo(), m_filter);
}