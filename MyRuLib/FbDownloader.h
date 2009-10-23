#ifndef __FBDOWNLOADER_H__
#define __FBDOWNLOADER_H__

#include <wx/wx.h>
#include <wx/url.h>
#include <wx/sstream.h>

class FbDownloader: public wxThread
{
	public:
		static void Start();
		static void Stop();
		bool IsRunning();
	protected:
		virtual void *Entry();
	private:
		static wxCriticalSection sm_queue;
		static bool sm_running;

};

#endif // __FBDOWNLOADER_H__
