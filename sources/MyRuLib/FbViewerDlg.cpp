#include "FbViewerDlg.h"
#include <wx/artprov.h>
#include "FbConst.h"
#include "MyRuLibApp.h"

BEGIN_EVENT_TABLE( FbViewerDlg, FbDialog )
	EVT_TEXT_ENTER( ID_EDIT_TXT, FbViewerDlg::OnTextEnter )
	EVT_BUTTON( ID_EDIT_BTN, FbViewerDlg::OnBtnClick )
END_EVENT_TABLE()

FbViewerDlg::FbViewerDlg( wxWindow* parent, const wxString& type, const wxString& value, bool relative)
	: FbDialog( parent, wxID_ANY, _("Customize"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER), m_relative(relative)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxString msg = _("Select the application to view files") + COLON + type;
	wxStaticText* stTitle = new wxStaticText( this, wxID_ANY, msg, wxDefaultPosition, wxDefaultSize, 0 );
	stTitle->Wrap( -1 );
	bSizerMain->Add( stTitle, 0, wxALL, 5 );

	wxBoxSizer* bSizerEdit = new wxBoxSizer( wxHORIZONTAL );

	m_filename = new wxTextCtrl( this, ID_EDIT_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_filename->SetValue(value);
	m_filename->SetMinSize( wxSize( 300,-1 ) );
	bSizerEdit->Add( m_filename, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBitmapButton* bpButton = new wxBitmapButton( this, ID_EDIT_BTN, wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizerEdit->Add( bpButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	bSizerMain->Add( bSizerEdit, 1, wxEXPAND, 5 );
/*
	wxCheckBox* cbRelative = new wxCheckBox( this, wxID_ANY, _("Сохранить только относительный путь"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerMain->Add( cbRelative, 0, wxALL, 5 );
*/
	wxStdDialogButtonSizer * sdbSizerBtn = CreateStdDialogButtonSizer( wxOK | wxCANCEL );
	bSizerMain->Add( sdbSizerBtn, 0, wxEXPAND | wxALL, 5 );

	SetSizer( bSizerMain );
	Layout();
	bSizerMain->Fit( this );
	m_filename->SetFocus();

	wxSize size = GetBestSize();
	SetMaxSize(wxSize(-1, size.y));
	SetMinSize(size);
}

void FbViewerDlg::OnBtnClick( wxCommandEvent& event )
{
	wxString title = _("Select the application to view files");
	#ifdef __WIN32__
	wxString wildCard = _("Executable files") + (wxString)wxT(" (*.exe)|*.exe");
	#else
	wxString wildCard;
	#endif
	wxString command = m_filename->GetValue();

	wxFileDialog dlg (
		this,
		title,
		wxEmptyString,
		command,
		wildCard,
		wxFD_OPEN | wxFD_FILE_MUST_EXIST,
		wxDefaultPosition
	);

	if (dlg.ShowModal() == wxID_OK) {
		wxString result = dlg.GetPath();
		if (m_relative) {
			wxFileName filename = result;
			wxFileName database = wxGetApp().GetLibFile();
			filename.MakeRelativeTo(database.GetPath());
			result = filename.GetFullPath();
		}
		m_filename->SetValue(result);
	}
}

wxString FbViewerDlg::GetValue()
{
	return m_filename->GetValue();
}

void FbViewerDlg::OnTextEnter( wxCommandEvent& event )
{
	EndModal(wxID_OK);
}

bool FbViewerDlg::Execute( wxWindow* parent, const wxString& type, wxString &value, bool relative)
{
	FbViewerDlg dlg(parent, type, value, relative);
	bool res = dlg.ShowModal() == wxID_OK;
	if (res) value = dlg.GetValue();
	return res;
}
