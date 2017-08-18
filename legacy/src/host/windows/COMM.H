#ifndef _COMM_H
#define _COMM_H

#include "ringbuf.h"

int   BaudInt (DWORD cbr);
DWORD BaudCBR (int   n);

class CComm
{
public:
	CComm ();
   ~CComm ();
	BOOL Create (int zBufR, int zBufW);
	void SetNotify (CWnd * wndNotify, DWORD msgNotify);
	BOOL Open (int port, int speed);
	BOOL Close ();
	BOOL Connected ();
	LPCTSTR ErrorCause ();
	BOOL Getc (BYTE * b);
	BOOL Putc (BYTE b);
	BOOL Gets (LPSTR  buf, int nBytes);
	BOOL Puts (LPCSTR s);
	BOOL Get  (LPSTR  buf, int * nBytes);
	BOOL Put  (LPCSTR buf, int nBytes);
	BOOL Flush ();
	void ErrorBox (CWnd * pWnd);


protected:
	BOOL SetState (int speed);
	BOOL Read ();
	BOOL Write (LPCSTR lpByte , DWORD dwBytesToWrite);

protected:
	BOOL   m_bConnected;
	HANDLE m_hComm;
	HWND  m_wndNotify;
	DWORD m_msgNotify;
	CWinThread * m_pThread;
	OVERLAPPED m_overlapR;
	OVERLAPPED m_overlapW;
	CString m_errFunc;
	DWORD m_err;
	CRingBuffer m_bufR;

friend DWORD FAR PASCAL CommThread (LPVOID lpData);
};

inline BOOL CComm::Connected ()
{ return m_bConnected; }
inline LPCTSTR CComm::ErrorCause ()
{ return m_errFunc; }
inline void CComm::SetNotify (CWnd * p, DWORD m)
{ m_wndNotify = p->GetSafeHwnd(); m_msgNotify = m; }

#endif //_COMM_H
