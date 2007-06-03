/* NOTE:  This module adapted from sample code included with Microsoft Visual Studio 4.0
          Samples/SDK/Win32/COMM
In particular, the functions  CComm::Read  and  CComm:Write  are nearly unmodified
from  ReadCommBlock()  and  WriteCommBlock().  Comments in those functions are likely
written by Microsoft staff.

  - Sean Gugler
*/

#include "stdafx.h"
#include "comm.h"
#include "resource.h"

struct {
	int n;
	DWORD cbr;
} Baud [] = {
	{   110, CBR_110   },
	{   300, CBR_300   },
	{   600, CBR_600   },
	{  1200, CBR_1200  },
	{  2400, CBR_2400  },
	{  4800, CBR_4800  },
	{  9600, CBR_9600  },
	{ 14400, CBR_14400 },
	{ 19200, CBR_19200 },
	{ 38400, CBR_38400 },
	{ 56000, CBR_56000 },
	{ 57600, CBR_57600 },
	{115200, CBR_115200},
	{128000, CBR_128000},
	{256000, CBR_256000},
};
#define BAUD_Z (sizeof(Baud)/sizeof(Baud[0]))

int BaudInt (DWORD cbr)
{
	int i;
	for (i=0; i<BAUD_Z-1; i++)
		if (Baud[i].cbr == cbr) break;
	return Baud[i].n;
}

DWORD BaudCBR (int n)
{
	int i;
	for (i=0; i<BAUD_Z-1; i++)
		if (Baud[i].n >= n) break;
	return Baud[i].cbr;
}

/////////////////////////////////////////////////////////////////////////

DWORD FAR PASCAL CommThread (LPVOID lpData)
{
   DWORD       dwEvtMask ;
   CComm       *com = (CComm *)lpData;
   if (!SetCommMask (com->m_hComm, EV_RXCHAR ))
      return ( FALSE ) ;

   while (com->m_bConnected)
   {
		dwEvtMask = 0;
		WaitCommEvent (com->m_hComm, &dwEvtMask, NULL);
		if (dwEvtMask & EV_RXCHAR)
		{
			com->Read();
		}
   }
   com->m_pThread = NULL;
   return 0;
}

/////////////////////////////////////////////////////////////////////////

CComm::CComm ()
{
	m_bConnected = FALSE;
	m_hComm = NULL;
	m_pThread = NULL;
	m_wndNotify = NULL;
}

CComm::~CComm ()
{
	if (m_bConnected) Close(); //TODO: guarantee thread shutdown
}

#define CHECK(func, test) if (!test) {ret=0; m_err = GetLastError(); m_errFunc = _T(#func);} else
#define CHECK_BOOL(func, args) if (!func args) {ret=0; m_err = GetLastError(); m_errFunc = _T(#func);} else

BOOL CComm::Create (int zBufR, int zBufW)
{
	BOOL ret = TRUE;
	m_overlapW.Offset = 0;
	m_overlapW.OffsetHigh = 0;
	m_overlapR.Offset = 0;
	m_overlapR.OffsetHigh = 0;

	if (!m_bufR.Init (zBufR)) {ret = FALSE; } else {
    m_overlapR.hEvent = CreateEvent( NULL,    // no security
                                     TRUE,    // explicit reset req
                                     FALSE,   // initial event reset
                                     NULL ) ; // no name

	if (NULL == m_overlapR.hEvent) {ret = FALSE; } else {
    m_overlapW.hEvent = CreateEvent( NULL,    // no security
                                     TRUE,    // explicit reset req
                                     FALSE,   // initial event reset
                                     NULL ) ; // no name
    if (NULL == m_overlapW.hEvent) {ret = FALSE; } else {
	if (!ret) CloseHandle (m_overlapW.hEvent);
	}
	if (!ret) CloseHandle (m_overlapR.hEvent);
    }}
	return (ret);
}

BOOL CComm::Open (int port, int speed)
{
	int ret = TRUE;
    COMMTIMEOUTS CommTimeOuts;
	CString szCom = _T("COM");

	CWaitCursor w;

	szCom += ('0' + port);
	m_hComm = CreateFile (szCom,
		GENERIC_READ | GENERIC_WRITE,
		0, /* exclusive access */
		NULL, /* no security */
		OPEN_EXISTING, /* required */
		FILE_ATTRIBUTE_NORMAL | 
		    FILE_FLAG_OVERLAPPED, // overlapped I/O
		NULL  /* no hTemplate */
		);
	CHECK (CreateFile, (INVALID_HANDLE_VALUE != m_hComm)) {
	if (!SetState(speed)) {ret=0;} else {
    CHECK_BOOL (SetCommMask, (m_hComm, EV_RXCHAR)) {
    CHECK_BOOL (SetupComm, (m_hComm, 4096, 4096)) {
    CHECK_BOOL (PurgeComm, (m_hComm, PURGE_TXABORT | PURGE_RXABORT |
                                     PURGE_TXCLEAR | PURGE_RXCLEAR )) {
    // set up for overlapped I/O
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
    CHECK_BOOL (SetCommTimeouts, (m_hComm, &CommTimeOuts)) {
    CHECK_BOOL (EscapeCommFunction, (m_hComm, SETDTR)) { 
	m_bConnected = TRUE;
	m_pThread = AfxBeginThread ((AFX_THREADPROC) CommThread,
                                (LPVOID) this,
								THREAD_PRIORITY_ABOVE_NORMAL);
	CHECK (CreateThread, (NULL != m_pThread)) {
	}
	m_bufR.Empty();
	if (!ret) m_bConnected = FALSE;
	}}}}}}
	if (!ret) {CloseHandle (m_hComm); m_hComm = NULL; }
	}
	return (ret);
}

BOOL CComm::Close ()
{
	BOOL ret = TRUE;
	m_bConnected = FALSE;
    CHECK_BOOL (SetCommMask, (m_hComm, 0)) {
    while (m_pThread);
    CHECK_BOOL (EscapeCommFunction, (m_hComm, CLRDTR)) {
    CHECK_BOOL (PurgeComm, (m_hComm, PURGE_TXABORT | PURGE_RXABORT |
                                     PURGE_TXCLEAR | PURGE_RXCLEAR )) {
    CHECK_BOOL (CloseHandle, (m_hComm)) {
	}}}}
	return (ret);
}

BOOL CComm::Getc (BYTE * b)
{
	BOOL ret = FALSE;
	DWORD len = 1;
	LPSTR buf = m_bufR.GetBegin (&len);
	if (len) {
		*b = *buf;
		len = 1;
		ret = TRUE;
	}
	m_bufR.GetEnd (len);
	return (ret);
}

BOOL CComm::Putc (BYTE b)
{
	return Write ((LPCSTR)&b, 1);
}

BOOL CComm::Get  (LPSTR  buf, int * nBytes)
{
	BOOL ret = FALSE;
	DWORD len = 0;
	LPSTR rbuf = m_bufR.GetBegin (&len);
	if (len > (DWORD)*nBytes)
		len = (DWORD)*nBytes;
	if (len) {
		memcpy (buf, rbuf, len);
		*nBytes = len;
		ret = TRUE;
	}
	m_bufR.GetEnd (len);
	return ret;
}

BOOL CComm::Puts (LPCSTR s)
{
	return Write (s, strlen(s));
}

BOOL CComm::Put (LPCSTR buf, int nBytes)
{
	return Write (buf, (DWORD)nBytes);
}

BOOL CComm::Flush ()
{
	return FlushFileBuffers (m_hComm);
}

void CComm::ErrorBox (CWnd * pWnd)
{
	CString str;
	switch (m_err) {
	case ERROR_FILE_NOT_FOUND:
		str.LoadString (IDS_ERR0002);
		break;
	case ERROR_ACCESS_DENIED:
		str.LoadString (IDS_ERR0005);
		break;
	default:
		str.Format ("ERROR 0x%08x from ", m_err);
		str += ErrorCause();
		break;
	}
	pWnd->MessageBox (str);
}

/////////////////////////////////////////////////////////////////////////

BOOL CComm::SetState (int speed)
{
	int ret = TRUE;
    DCB dcb;

    dcb.DCBlength = sizeof(DCB);
	CHECK_BOOL (GetCommState, (m_hComm, &dcb)) {
	dcb.BaudRate = BaudCBR(speed);
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	CHECK_BOOL (SetCommState, (m_hComm, &dcb)) {
	}}
	return (ret);
}

/* NOTE: taken mostly from Microsoft Sample code "TTY.C" */
BOOL CComm::Read ()
{
	BOOL ret = TRUE;
	COMSTAT    ComStat;
	DWORD      dwErrorFlags;
	DWORD      nBytes;
	DWORD      dwError;

	do {
		// only try to read number of bytes in queue 
		if (!ClearCommError (m_hComm, &dwErrorFlags, &ComStat)) {
		}
		nBytes = ComStat.cbInQue;

		if (nBytes > 0)
		{
			LPSTR buf = m_bufR.PutBegin (&nBytes);
			if (!ReadFile (m_hComm, buf, nBytes, &nBytes, &m_overlapR))
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					OutputDebugString("\n\rIO Pending");
					// We have to wait for read to complete. 
					// This function will timeout according to the
					// CommTimeOuts.ReadTotalTimeoutConstant variable
					// Every time it times out, check for port errors
					while (!GetOverlappedResult (m_hComm, &m_overlapR, &nBytes, TRUE))
					{
						dwError = GetLastError();
						if(dwError == ERROR_IO_INCOMPLETE)
							// normal result if not finished
							continue;
						else
						{
							ClearCommError (m_hComm, &dwErrorFlags, &ComStat);
#if 0
							// an error occurred, try to recover
							wsprintf( szError, "<CE-%u>", dwError ) ;
							WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
							ClearCommError( COMDEV( npTTYInfo ), &dwErrorFlags, &ComStat ) ;
							if ((dwErrorFlags > 0) && DISPLAYERRORS( npTTYInfo ))
							{
								wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
								WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
							}
#endif
							break;
						}
							
					}
						
				}
				else
				{
					// some other error occurred
		
					nBytes = 0 ;
					ClearCommError (m_hComm, &dwErrorFlags, &ComStat);
#if 0
					if ((dwErrorFlags > 0) && DISPLAYERRORS( npTTYInfo ))
					{
						wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
						WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
					}
#endif
				}
			}
			m_bufR.PutEnd (nBytes);
			if (m_wndNotify)
				::PostMessage (m_wndNotify, m_msgNotify, 0, (LPARAM)nBytes);
		}
   
	} while ( nBytes > 0 ) ;
	return (ret);
} // end of ReadCommBlock()

/* NOTE: taken mostly from Microsoft Sample code "TTY.C" */
BOOL CComm::Write (LPCSTR lpByte , DWORD dwBytesToWrite)
{

	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	DWORD   	dwError;
	COMSTAT     ComStat;


	fWriteStat = WriteFile (m_hComm, lpByte, dwBytesToWrite,
	                       &dwBytesWritten, &m_overlapW);

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests 
	// (up to several thousand bytes) will normally be accepted 
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if (!fWriteStat) 
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			// We should wait for the completion of the write operation
			// so we know if it worked or not

			// This is only one way to do this. It might be beneficial to 
			// the to place the writing operation in a separate thread 
			// so that blocking on completion will not negatively 
			// affect the responsiveness of the UI

			// If the write takes long enough to complete, this 
			// function will timeout according to the
			// CommTimeOuts.WriteTotalTimeoutConstant variable.
			// At that time we can check for errors and then wait 
			// some more.

			while(!GetOverlappedResult (m_hComm, &m_overlapW, &dwBytesWritten, TRUE))
			{
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
					// normal result if not finished
					continue;
				else
				{
					// an error occurred, try to recover
					ClearCommError (m_hComm, &dwErrorFlags, &ComStat);
#if 0
					wsprintf( szError, "<CE-%u>", dwError ) ;
					WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
					ClearCommError( COMDEV( npTTYInfo ), &dwErrorFlags, &ComStat ) ;
					if ((dwErrorFlags > 0) && DISPLAYERRORS( npTTYInfo ))
					{
						wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
						WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
					}
#endif
					break;
				}
			}
		}
		else
		{						 	
			// some other error occurred

			ClearCommError (m_hComm, &dwErrorFlags, &ComStat);
#if 0
			if ((dwErrorFlags > 0) && DISPLAYERRORS( npTTYInfo ))
			{
				wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
				WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
			}
#endif
			return ( FALSE );
		}
	}
	return ( TRUE ) ;

} // end of WriteCommBlock()

