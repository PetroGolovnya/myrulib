#ifndef __FBTITLEDLG_H__
#define __FBTITLEDLG_H__

#include <wx/wx.h>
#include "FbWindow.h"

class FbTitleDlg : public FbDialog
{
	private:
		class TitlePanel: public wxPanel
		{
			public:
				TitlePanel( wxWindow* parent );
			private:
				wxBoxSizer * m_authors;
				wxBoxSizer * m_series;
				wxBoxSizer * m_genres;
				wxTextCtrl m_title;
		};

		class AuthSubPanel: public wxPanel
		{
			public:
				AuthSubPanel( wxWindow* parent );
			private:
				wxToolBar m_toolbar;
				wxTextCtrl m_text;
		};

	protected:
		wxNotebook * m_notebook;

	public:
		static bool Execute(int book);
		FbTitleDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE  | wxRESIZE_BORDER | wxTAB_TRAVERSAL );
		~FbTitleDlg();
};

#endif // __FBTITLEDLG_H__