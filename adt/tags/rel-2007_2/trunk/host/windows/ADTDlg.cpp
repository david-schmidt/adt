// ADTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ADT.h"
#include "ADTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void AFXAPI DDX_CBStringRoundUP(CDataExchange* pDX, int nIDC, CString& value)
{
	if (pDX->m_bSaveAndValidate)
		DDX_CBString (pDX, nIDC, value);
	else
	{
		HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
		TCHAR sz[16];
		int i = 0;
		while (CB_ERR != ::SendMessage (hWndCtrl, CB_GETLBTEXT, (WPARAM)i, (LPARAM)sz))
		{
			i++;
			if (atol (value) <= atol (sz)) break;
		}
		::SendMessage (hWndCtrl, CB_SETCURSEL, (WPARAM)(i-1), (LPARAM)0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADTDlg dialog

CADTDlg::CADTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADTDlg::IDD, pParent)
	, m_xfer (m_com)
{
	//{{AFX_DATA_INIT(CADTDlg)
	m_strStatus = _T("");
	m_cbPort = _T("");
	m_cbSpeed = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bClosing = FALSE;
	m_bPortEdited = FALSE;
}

void CADTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	if (!pDX->m_bSaveAndValidate) {
		m_cbSpeed.Format (_T("%d"), BaudInt (BaudCBR (atol (m_cbSpeed))));
	}
	//{{AFX_DATA_MAP(CADTDlg)
	DDX_Control(pDX, IDC_CONNECT, m_btnConnect);
	DDX_Text(pDX, IDC_STATUS, m_strStatus);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_cbPort);
	//}}AFX_DATA_MAP
	DDX_CBStringRoundUP(pDX, IDC_COMBO_SPEED, m_cbSpeed);
	DDX_Control(pDX, IDC_PROGRESS5, m_prog[5]);
	DDX_Control(pDX, IDC_PROGRESS4, m_prog[4]);
	DDX_Control(pDX, IDC_PROGRESS3, m_prog[3]);
	DDX_Control(pDX, IDC_PROGRESS2, m_prog[2]);
	DDX_Control(pDX, IDC_PROGRESS1, m_prog[1]);
	if (!pDX->m_bSaveAndValidate) {
		CString str;
		CRect r;
		int bot;
		if (m_com.Connected()) {
			m_bReady = TRUE;
			str.LoadString (IDS_DISCONNECT);
			m_prog[5].GetWindowRect (&r);
		} else {
			m_bReady = FALSE;
			str.LoadString (IDS_CONNECT);
			m_btnConnect.GetWindowRect (&r);
		}
		m_btnConnect.SetWindowText (str);
		bot = r.bottom + 10;
		GetWindowRect (&r);
		r.bottom = bot;
		MoveWindow (r);

		if (!m_xfer.Busy()) {
			str.LoadString (IDS_APPTITLE);
			SetWindowText (str);
		}
	}
}

BEGIN_MESSAGE_MAP(CADTDlg, CDialog)
	//{{AFX_MSG_MAP(CADTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, OnSelChangeComboSpeed)
	ON_CBN_KILLFOCUS(IDC_COMBO_PORT, OnKillfocusComboPort)
	ON_CBN_EDITCHANGE(IDC_COMBO_PORT, OnEditchangeComboPort)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_COMM_RX_NOTIFY, OnCommRx)
    ON_MESSAGE(WM_ADT_SENDFILE		, OnAdtSendFile		)
    ON_MESSAGE(WM_ADT_CANTOPEN		, OnAdtCantOpen		)
    ON_MESSAGE(WM_ADT_NOT140K  		, OnAdtNot140K		)
    ON_MESSAGE(WM_ADT_SECTOR   		, OnAdtSector		)
    ON_MESSAGE(WM_ADT_SENT     		, OnAdtSent			)
    ON_MESSAGE(WM_ADT_CRCERR   		, OnAdtCRCerr		)
    ON_MESSAGE(WM_ADT_RECEIVE_FILE	, OnAdtReceiveFile	)
    ON_MESSAGE(WM_ADT_EXISTS   		, OnAdtExists		)
    ON_MESSAGE(WM_ADT_DISKFULL 		, OnAdtDiskFull		)
    ON_MESSAGE(WM_ADT_RECEIVED 		, OnAdtReceived		)
    ON_MESSAGE(WM_ADT_BADHEADER		, OnAdtBadHeader	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADTDlg message handlers

BOOL CADTDlg::Create (UINT nID, CADTCmdLine & cmd)
{
	m_cbPort = cmd.port;
	m_cbSpeed = cmd.speed;
	return CDialog::Create(nID);
}

BOOL CADTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Wizard Sez: Add extra initialization here
	for (int i=1; i<=5; i++) {
		m_prog[i].SetRange (0, 0x6F);
	}
//	m_comboPort.Set
//		CComboBox
	m_strStatus.LoadString (IDS_ADT_READY);
	UpdateData(FALSE);
	m_com.Create (0x8000, 0x8000);
	m_com.SetNotify (this, WM_COMM_RX_NOTIFY);
	m_xfer.SetNotify (this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CADTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CADTDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CADTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Don't allow ESC/RETURN to close app, only close box or its keyboard shortcuts.
void CADTDlg::OnClose() 
{
	m_bClosing = TRUE;	
	CDialog::OnClose();
}
void CADTDlg::OnCancel()
{
	if (m_bClosing)	CDialog::DestroyWindow();
	else OnReset();
}
void CADTDlg::OnOK()
{
	if (m_bClosing)	CDialog::DestroyWindow();
}
void CADTDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}


void CADTDlg::OnConnect() 
{
	BOOL ok;

	if (m_com.Connected())
		ok = m_com.Close();
	else
		ok = m_com.Open (atol(m_cbPort), atol(m_cbSpeed));
	if (!ok)
		m_com.ErrorBox (this);
	UpdateData (FALSE);
}

void CADTDlg::OnSelChangeComboSpeed() 
{
	UpdateData (TRUE);
	Reconnect();
}
void CADTDlg::OnKillfocusComboPort() 
{
	if (m_bPortEdited)
	{
		UpdateData (TRUE);
		Reconnect();
	}
}
void CADTDlg::OnEditchangeComboPort() 
{
	m_bPortEdited = TRUE;
}
void CADTDlg::OnSelchangeComboPort() 
{
	CComboBox * cb = (CComboBox *)GetDlgItem (IDC_COMBO_PORT);
	cb->GetLBText (cb->GetCurSel(), m_cbPort);
	Reconnect();
}

void CADTDlg::Reconnect() 
{
	BOOL ok;
	if (m_com.Connected()) {
		m_com.Close();
		ok = m_com.Open (atol(m_cbPort), atol(m_cbSpeed));
		if (!ok)
			m_com.ErrorBox (this);
	}
	UpdateData (FALSE);
}

void CADTDlg::OnReset()
{
	m_strStatus.LoadString (IDS_ADT_READY);
	UpdateData(FALSE);
	m_xfer.Reset();
 	for (int i=1; i<=5; i++) {
 		m_prog[i].SetPos(0);
 	}
}

LRESULT CADTDlg::OnCommRx (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	BYTE cmd;
	if (m_bReady && !m_xfer.Busy()) {
		m_bReady = FALSE;
		for (int i=1; i<=5; i++) {
			m_prog[i].SetPos(0);
		}
		m_com.Getc (&cmd);
		if (!m_xfer.OnCommand (cmd)) {
			//FUTURE expansion
		}
		m_bReady = m_com.Connected();
	}
	return (ret);
}

LRESULT CADTDlg::OnAdtSector (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	int sector = (int)lParam % 0x70;
	int part = (int)lParam / 0x70;
	m_prog[part+1].SetPos (sector);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtSendFile (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	CString str;
	m_fname = (LPCTSTR)lParam;
	m_strStatus.Format (IDS_ADT_SENDFILE, m_fname);
	UpdateData(FALSE);
	str.LoadString (IDS_APPTITLE);
	str += _T(" - ");
	str += m_fname;
	SetWindowText (str);
	return (ret);
}

LRESULT CADTDlg::OnAdtCantOpen (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	m_strStatus.Format (IDS_ADT_CANTOPEN, m_fname);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtNot140K (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	m_strStatus.Format (IDS_ADT_NOT140K, m_fname);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtBadHeader (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	m_strStatus.LoadString (IDS_ADT_NOT140K);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtSent (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
    m_strStatus = (LPCTSTR)lParam;
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtCRCerr (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
    m_strStatus.Format (IDS_ADT_CRCERR, lParam >> 4, lParam & 0xF);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtReceiveFile (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	CString str;
	m_fname = (LPCTSTR)lParam;
	m_strStatus.Format (IDS_ADT_RECEIVE_FILE, m_fname);
	UpdateData(FALSE);
	str.LoadString (IDS_APPTITLE);
	str += _T(" - ");
	str += m_fname;
	SetWindowText (str);
	return (ret);
}

LRESULT CADTDlg::OnAdtExists (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	m_strStatus.Format (IDS_ADT_EXISTS, m_fname);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtDiskFull (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	m_strStatus.Format (IDS_ADT_DISKFULL, m_fname);
	UpdateData(FALSE);
	return (ret);
}

LRESULT CADTDlg::OnAdtReceived (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
    m_strStatus = (LPCTSTR)lParam;
	UpdateData(FALSE);
	return (ret);
}

#if 0
LRESULT CADTDlg::OnAdt (WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	return (ret);
}
#endif
