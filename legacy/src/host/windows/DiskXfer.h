#ifndef _DISKXFER_H
#define _DISKXFER_H

#include <afxmt.h>
#include "comm.h"

#define WM_ADT_CANTOPEN		(WM_USER +  1)
#define WM_ADT_NOT140K  	(WM_USER +  2)
#define WM_ADT_SECTOR   	(WM_USER +  3)
#define WM_ADT_SENT     	(WM_USER +  4)
#define WM_ADT_CRCERR   	(WM_USER +  5)
#define WM_ADT_RECEIVE_FILE (WM_USER +  6)
#define WM_ADT_EXISTS   	(WM_USER +  7)
#define WM_ADT_DISKFULL 	(WM_USER +  8)
#define WM_ADT_RECEIVED 	(WM_USER +  9)
#define WM_ADT_BADHEADER	(WM_USER + 10)
#define WM_ADT_SENDFILE		(WM_USER + 11)

class CDiskTransfer
{
public:
	CDiskTransfer (CComm & rcom);
   ~CDiskTransfer ();

	void Reset();
	BOOL OnCommand (BYTE cmd);
	void SetNotify (CWnd * wndNotify);
	BOOL Busy ();


protected:
	void StartThread ();
	void StopThread (BOOL wait = 1);

	void send_directory( void );
	void send_disk( void );
	void send_sector( unsigned char *buffer, int part, int track, int sector );
	void receive_disk( void );
	void receive_sector( unsigned char *buffer, int part, int track, int sector );
	void getfname( char *fname );
	void make_crctable( void );
	unsigned do_crc( unsigned char *ptr, int count );

	char comm_getc( void );
	void comm_putc( char c );
	void comm_puts( char *p );
	void comm_flush();


	unsigned crctable[256];         /* CRC-16 lookup table for speed */
    unsigned char buffer[28672];    /* 0x7000 == 7 tracks of 16 sectors of 256 bytes */
    char fname[_MAX_FNAME], error[80];
	CComm & m_com;
	HWND m_wnd;
	BOOL m_run;
	CWinThread * m_thread;
	CSemaphore m_hSemThread;
	CSingleLock m_semThread;
	void (CDiskTransfer::*m_threadFunc) ();

friend DWORD DiskTransferThread (LPVOID lpData);
};

inline void CDiskTransfer::SetNotify (CWnd * p)
{ m_wnd = p->GetSafeHwnd(); }
inline BOOL CDiskTransfer::Busy ()
{ return NULL != m_threadFunc; }

#endif //_DISKXFER_H
