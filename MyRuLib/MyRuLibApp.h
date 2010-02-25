#ifndef __MYRULIBAPP_H__
#define __MYRULIBAPP_H__

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/filename.h>
#include "FbDatabase.h"
#include "FbLocale.h"

class MyRuLibApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	bool OpenDatabase(const wxString &filename);
	const wxString GetAppData();
	const wxString GetAppPath();
private:
	void OpenLog();
	wxFileName GetDatabaseFilename(FbDatabase &database);
	wxFileName GetLocaleFilename(FbConfigDatabase &config);
	void SetAppData(const wxString &filename);
	void LoadBlankImage();
private:
	wxCriticalSection m_section;
	wxString m_datafile;
	FbLocale m_locale;
};

DECLARE_APP(MyRuLibApp)

#endif // __MYRULIBAPP_H__
