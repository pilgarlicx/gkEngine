﻿// PropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PropertiesDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog dialog


CPropertiesDialog::CPropertiesDialog( const CString &title,XmlNodeRef &node,CWnd* pParent /*=NULL*/, bool bShowSearchBar /*=false*/)
	: CXTResizeDialog(CPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertiesDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bShowSearchBar = bShowSearchBar;
	m_title = title;
	m_node = node;
}


void CPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertiesDialog, CXTResizeDialog)
	//{{AFX_MSG_MAP(CPropertiesDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()	
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog message handlers

BOOL CPropertiesDialog::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();

	SetWindowText( m_title );

	CRect rc;
	GetClientRect( rc );

	m_wndProps.Create( WS_CHILD|WS_VISIBLE|WS_BORDER,rc,this );
	m_wndProps.SetUpdateCallback( functor(*this,&CPropertiesDialog::OnPropertyChange) );

	if (m_bShowSearchBar)
	{
		int inputH = 18;

		m_searchLabel.Create( "Search:", WS_CHILD|WS_VISIBLE,rc, this, NULL );
		m_searchLabel.ModifyStyleEx( WS_EX_CLIENTEDGE, 0 );

		m_input.Create( WS_CHILD|WS_VISIBLE|ES_WANTRETURN|ES_AUTOHSCROLL|WS_TABSTOP,rc, this, NULL );
		m_input.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(SYSTEM_FONT)) );
		m_input.ModifyStyleEx( 0,WS_EX_STATICEDGE );
		m_input.SetWindowText( "" );

		m_wndProps.MoveWindow( rc.left+4,rc.top+4,rc.right-8,rc.bottom-24-inputH,true );
	}
	else
	{
		m_wndProps.MoveWindow( rc.left+4,rc.top+4,rc.right-8,rc.bottom-24,true );
	}

	if (m_node)
	{	
		m_wndProps.CreateItems( m_node );
	}

	AutoLoadPlacement( "Dialogs\\PropertyDlg" );
	ConfigureLayout();
	if (m_bShowSearchBar)
		m_input.SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPropertiesDialog::PreTranslateMessage(MSG* msg)
{
	if (msg->message == WM_KEYUP)
	{
		if (m_bShowSearchBar && (&m_input==GetFocus()))
		{
			CString result;
			m_input.GetWindowText(result);

			m_wndProps.RestrictToItemsContaining(result);
		}
	}

	return CXTResizeDialog::PreTranslateMessage(msg);
}


void CPropertiesDialog::OnDestroy() 
{
	CXTResizeDialog::OnDestroy();
}


void CPropertiesDialog::OnClose()
{
	// reset search bar information
	m_wndProps.RestrictToItemsContaining("");

	m_wndProps.RemoveAllItems();
	CXTResizeDialog::OnClose();
}


void CPropertiesDialog::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);

	if (m_wndProps.m_hWnd)
	{
		ConfigureLayout();
	}
}

void CPropertiesDialog::ConfigureLayout()
{
	CRect rc;
	GetClientRect( rc );
	if (m_bShowSearchBar)
	{
		int inputH = 18;

		m_searchLabel.MoveWindow( rc.left+4,rc.bottom-20-inputH,50,inputH,SWP_NOZORDER );	
		m_input.MoveWindow( rc.left+58,rc.bottom-20-inputH,rc.right-4-58,inputH,SWP_NOZORDER );	
		m_wndProps.MoveWindow( rc.left+4,rc.top+4,rc.right-8,rc.bottom-28-inputH,true );
	}
	else
	{
		m_wndProps.MoveWindow( rc.left+4,rc.top+4,rc.right-8,rc.bottom-24,true );
	}
}

void CPropertiesDialog::OnPropertyChange( IVariable *pVar )
{
	if (m_varCallback)
		m_varCallback( pVar );
}

void CPropertiesDialog::OnCancel()
{
	DestroyWindow();
}