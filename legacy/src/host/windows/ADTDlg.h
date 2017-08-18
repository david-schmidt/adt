// ADTDlg.h : header file
//
#include "comm.h"
#include "DiskXfer.h"

#define WM_COMM_RX_NOTIFY (WM_USER + 100)

/////////////////////////////////////////////////////////////////////////////
// CADTDlg dialog

class CADTDlg : public CDialog
{
// Construction
public:
	CADTDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CADTDlg)
	enum { IDD = IDD_ADT_DIALOG };
	CButton	m_btnConnect;
	CString	m_strStatus;
	CString	m_cbPort;
	CString	m_cbSpeed;
	//}}AFX_DATA
	CProgressCtrl	m_prog[6];  // [0] is unused

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADTDlg)
	public:
	virtual void OnCancel();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	public:
	BOOL Create (UINT nID, CADTCmdLine & cmd);

// Implementation
protected:
	HICON m_hIcon;
	BOOL  m_bClosing;

	// Generated message map functions
	//{{AFX_MSG(CADTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnClose();
	afx_msg void OnReset();
	afx_msg void OnSelChangeComboSpeed();
	afx_msg void OnKillfocusComboPort();
	afx_msg void OnEditchangeComboPort();
	afx_msg void OnSelchangeComboPort();
	//}}AFX_MSG
    afx_msg LRESULT OnCommRx(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtSendFile		(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtCantOpen		(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtNot140K		(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtSector			(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtSent			(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtCRCerr			(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtReceiveFile	(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtExists			(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtDiskFull		(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtReceived		(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdtBadHeader		(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CComm			m_com;
	CDiskTransfer	m_xfer;
	BOOL			m_bReady;
	CString			m_fname;
	BOOL			m_bPortEdited;

	void Reconnect();
};
