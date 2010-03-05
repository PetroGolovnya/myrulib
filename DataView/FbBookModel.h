#ifndef __FBBOOKMODEL_H__
#define __FBBOOKMODEL_H__

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/wxsqlite3.h>
#include <wx/arrimpl.cpp>

class FbBookModelCashe;

class FbBookModel: public wxDataViewVirtualListModel
{
    public:
        enum COL
        {
            COL_ROWID,
            COL_TITLE,
            COL_AUTHOR,
            COL_GENRE,
            COL_NUMBER,
            COL_RATING,
            COL_TYPE,
            COL_LANG,
            COL_SIZE,
            COL_MAX,
        };

        FbBookModel(const wxString &filename);

        virtual ~FbBookModel();

        // implementation of base class virtuals to define model

        virtual unsigned int GetColumnCount() const
        {
            return COL_MAX;
        }

        virtual wxString GetColumnType( unsigned int col ) const
        {
            return wxT("string");
        }

        virtual void GetValueByRow( wxVariant &variant, unsigned int row, unsigned int col ) const;
        virtual bool GetAttrByRow( unsigned int row, unsigned int col, wxDataViewItemAttr &attr ) const;
        virtual bool SetValueByRow( const wxVariant &variant, unsigned int row, unsigned int col ) { return false; };

    public:

    private:
        long Init(const wxString &filename);

    private:
        FbBookModelCashe * m_datalist;
};

class FbBookModelData
{
    public:
        FbBookModelData(const wxSQLite3ResultSet &res);
        FbBookModelData(const FbBookModelData &data);
        wxString GetValue(unsigned int col);
        bool IsOk() { return m_rowid; };
    public:
        unsigned int m_rowid;
        wxArrayString m_values;
};

WX_DECLARE_OBJARRAY(FbBookModelData, FbBookModelArray);

class FbBookModelCashe: private FbBookModelArray
{
	public:
        FbBookModelCashe(const wxString &filename);
        FbBookModelData FindRow(unsigned int row);
        unsigned int GetCount();
        wxString GetValue(unsigned int col);
	private:
        wxSQLite3Database m_database;
        unsigned int m_rowid;
};

#endif // __FBBOOKMODEL_H__
