#include "stdafx.h"
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <ctype.h>
#include <time.h>
#include "DiskXfer.h"

#define ACK 0x06
#define NAK 0x15

DWORD DiskTransferThread (LPVOID lpData)
{
	CDiskTransfer * xfer = (CDiskTransfer*)lpData;

	xfer->m_semThread.Lock();
	//xfer->m_thread->SuspendThread();
	while (xfer->m_run) {
		if (xfer->m_threadFunc) {
			(xfer->*(xfer->m_threadFunc)) ();
		}
		xfer->m_threadFunc = NULL;
		xfer->m_semThread.Lock();
		//xfer->m_thread->SuspendThread();
	}
	xfer->m_thread = NULL;
	return 0;
}

void CDiskTransfer::StartThread()
{
	m_run = TRUE;
	m_threadFunc = NULL;
	m_thread = AfxBeginThread ((AFX_THREADPROC) DiskTransferThread, (LPVOID) this);
}
void CDiskTransfer::StopThread (BOOL wait /* = 1 */)
{
	m_run = FALSE;
	m_semThread.Unlock();
	//m_thread->ResumeThread();
	if (wait) while (m_thread);
}

CDiskTransfer::CDiskTransfer (CComm & rcom)
	: m_com (rcom)
	, m_semThread (&m_hSemThread, TRUE) /*start locked*/
{
	StartThread();
    make_crctable();
}

CDiskTransfer::~CDiskTransfer ()
{
	StopThread();
}

void CDiskTransfer::Reset ()
{
	StopThread();
	StartThread();
}

BOOL CDiskTransfer::OnCommand (BYTE cmd)
{
	BOOL ret = TRUE;
	switch (cmd & 0x7F) {
	default:  ret = FALSE;		break;
	case 'D': m_threadFunc = send_directory;	break;
    case 'R': m_threadFunc = send_disk;			break;
    case 'S': m_threadFunc = receive_disk;		break;
	}
	if (ret) m_semThread.Unlock();
	//if (ret) m_thread->ResumeThread();
	return (ret);
}

void CDiskTransfer::send_directory()
/* Format and send the current of current directory */
{
    int i;
    int tab, line;
    char *dirname = getcwd( NULL, 256 );
    struct _finddata_t ff;
	long hff;

#if DOS_ADT
    putsat( 1, 9, "Sending directory listing   (ESC to abort)" );
#endif
    comm_puts( "DIRECTORY OF " );
    comm_puts( dirname );
    if( (strlen( dirname ) + 13) % 40 )
        comm_putc( '\r' );
    for( i=0; i<40; i++ )
        comm_putc( '-' );

    hff = _findfirst( "*.*", &ff );
    if( hff > 0 ) {
		i = 0;
        tab = 0;
        line = (strlen( dirname ) + 13) / 40 + 2;
        while (m_run) {
#if DOS_ADT
            if( kbhit() && getch() == 0x1B )
                break;
            comm_puts( ff.name );
            for( i=strlen( ff.name ); i<12; i++ )
                comm_putc( ' ' );
            if( tab < 2 ) {
                comm_putc( ' ' );
                comm_putc( ' ' );
            }
            if( m_com.Getc(&ch) && ch == '\0' )
                break;
            if( _findnext( hff, &ff ) == -1 )
                break;
            if( ++tab > 2 ) {
                tab = 0;
                if( ++line > 20 ) {
                    line = 0;
                    comm_putc( '\0' );
                    comm_putc( '\1' );
                    if( comm_getc() == '\0' )
                        break;
                }
            }
#else
			if (i && i+strlen (ff.name) > 40) {
				while (i++ < 40)
					comm_putc( ' ' );
				i=0;
				line++;
			}
			if (line > 20) {
				line = 0;
				comm_putc ('\0');
				comm_putc ('\1');
                if( comm_getc() == '\0' )
                    break;
            }
			line += (strlen(ff.name) / 40);
			i += (strlen(ff.name) % 40);
            comm_puts( ff.name );
            if( _findnext( hff, &ff ) == -1 )
                break;
			do {
				if (i==40) {
					i = 0;
					line++;
					break;
				}
                comm_putc( ' ' );
			} while (++i%14);
#endif
        }
    }
    else
        comm_puts( "NO FILES" );

	_findclose (hff);
    comm_putc( '\0' );
    comm_putc( '\0' );
    comm_flush();
    free( dirname );
}

void CDiskTransfer::send_disk()
/* Main send routine */
{
    FILE *f;
    int part, track, sector, report;
    time_t tstart, tend;
    int minutes, seconds, cps;

	memset (buffer, 0, sizeof(buffer));

    getfname( fname );
#if DOS_ADT
    putsat( 1, 9, "Sending file " );
    putsat( 14, 9, fname );
    cputs( "   (ESC to abort)" );
#else
	::PostMessage (m_wnd, WM_ADT_SENDFILE, 0, (LPARAM)fname);
#endif

    f = fopen( fname, "rb" );
    if( !f ) {
        comm_putc( 26 );        /* can't open */
#if DOS_ADT
        sprintf( error, "ERROR: File %s can't be opened for input.", fname );
        putsat( 1, 20, error );
#else
		::PostMessage (m_wnd, WM_ADT_CANTOPEN, 0, 0);
#endif
        return;
    }
    if( filelength( fileno( f ) ) != 143360L ) {
        comm_putc( 30 );        /* not a 140k image */
#if DOS_ADT
        sprintf( error, "ERROR: File %s is not a 140K disk image.", fname );
        putsat( 1, 20, error );
#else
		::PostMessage (m_wnd, WM_ADT_NOT140K, 0, 0);
#endif
        return;
    }
    comm_putc( 0 );     /* File is now ready */

    time( &tstart );
    //clear_display();
    while( comm_getc() != ACK )
		::PostMessage (m_wnd, WM_ADT_BADHEADER, 0, 0);
//        putsat( 1, 20, "ERROR: Bad header byte received." );
    for( part=0; part<5; part++ ) {
//        putsat( 70, 12+part, "Reading" );
        fread( buffer, 1, 28672, f );
//        putsat( 70, 12+part, "" );
//        gotoxy( 12, 12+part );
        for( track=0; track<7; track++ ) {
            for( sector=15; sector>=0; sector-- ) {
                send_sector( buffer, part, track, sector );
				if (!m_run) goto bail;
#if DOS_ADT
                if( sector%2 == 1 )
                    putch( 'þ' );
#else
				::PostMessage (m_wnd, WM_ADT_SECTOR, 0, part * 0x70 + track * 16 + (15-sector));
#endif
            }
        }
//        putsat( 70, 12+part, "OK" );
    }
    report = comm_getc();
bail:
    fclose( f );
    time( &tend );

    seconds = (int)difftime( tend, tstart );
    cps = (int)(143360L / seconds);
    minutes = seconds/60;
    seconds = seconds%60;
	if (!m_run)
        sprintf( error, "%s send ABORTED after %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else if( report )
        sprintf( error, "%s sent WITH ERRORS in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else
        sprintf( error, "%s sent successfully in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
#if DOS_ADT
    putsat( 1, 20, error );
#else
	::PostMessage (m_wnd, WM_ADT_SENT, 0, (LPARAM)error);
#endif
}


void CDiskTransfer::send_sector( unsigned char *buffer, int part, int track, int sector )
/* Send a sector with RLE compression */
{
    int byte, crc, ok;
    unsigned char data, prev, newprev;
    unsigned char *thissector = buffer+(track<<12)+(sector<<8);
//    struct text_info textinfo;

    do {
        prev = 0;
        for( byte=0; byte<256; ) {
            newprev = thissector[byte];
            data = newprev - prev;
            prev = newprev;
            comm_putc( data );
            if( data )
                byte++;
            else {
                while( m_run && byte<256 && thissector[byte] == newprev )
                    byte++;
                comm_putc( byte & 0xFF );       /* 256 becomes 0 */
            }
			if (!m_run) return;
        }
        crc = do_crc( thissector, 256 );
        comm_putc( crc & 0xFF );
        comm_putc( crc >> 8 );
        ok = comm_getc();
        if( ok != ACK ) {
#if DOS_ADT
            gettextinfo( &textinfo );           /* save cursor position */
            gotoxy( 1, 20 );
            clreol();
            cprintf( "Track $%02X, sector $%02X: bad CRC",
                7*part+track, sector );
            gotoxy( textinfo.curx, textinfo.cury );
#else
			::PostMessage (m_wnd, WM_ADT_CRCERR, 0, part * 0x70 + track * 16 + sector);
#endif
        }
    } while( ok != ACK );
}


void CDiskTransfer::receive_disk()
/* Main receive routine */
{
    FILE *f;
    int part, track, sector, report;
    time_t tstart, tend;
    int minutes, seconds, cps;

	memset (buffer, 0, sizeof(buffer));

    getfname( fname );
#if DOS_ADT
    putsat( 1, 9, "Receiving file " );
    putsat( 16, 9, fname );
    cputs( "   (ESC to abort)" );
#else
	::PostMessage (m_wnd, WM_ADT_RECEIVE_FILE, 0, (LPARAM)fname);
#endif

    if( !access( fname, 0 ) ) {
        comm_putc( 28 );        /* File exists */
#if DOS_ADT
        sprintf( error, "ERROR: File %s already exists.", fname );
        putsat( 1, 20, error );
#else
		::PostMessage (m_wnd, WM_ADT_EXISTS, 0, 0);
#endif
        return;
    }
    if( (f = fopen( fname, "wb" )) == NULL ) {
        comm_putc( 26 );        /* Can't open */
#if DOS_ADT
        sprintf( error, "ERROR: File %s can't be opened for output.", fname );
        putsat( 1, 20, error );
#else
		::PostMessage (m_wnd, WM_ADT_CANTOPEN, 0, 0);
#endif
        return;
    }
    for( track=0; track<35; track++ ) {
        if( fwrite( buffer, 1, 4096, f ) != 4096 ) {
            comm_putc( 30 );    /* Disk full */
#if DOS_ADT
            sprintf( error, "ERROR: File %s doesn't fit on disk.", fname );
            putsat( 1, 20, error );
#else
			::PostMessage (m_wnd, WM_ADT_DISKFULL, 0, 0);
#endif
            fclose( f );
            return;
        }
    }
    rewind( f );
    comm_putc( 0 );         /* File is now ready */

    time( &tstart );
//    clear_display();
    while( comm_getc() != ACK )
		::PostMessage (m_wnd, WM_ADT_BADHEADER, 0, 0);
//        putsat( 1, 20, "ERROR: Bad header byte received." );
    for( part=0; part<5; part++ ) {
//        gotoxy( 12, 12+part );
        for( track=0; track<7; track++ ) {
            for( sector=15; sector>=0; sector-- ) {
                receive_sector( buffer, part, track, sector );
				if (!m_run) goto bail;
#if DOS_ADT
                if( sector%2 == 1 )
                    putch( 'þ' );
#else
				::PostMessage (m_wnd, WM_ADT_SECTOR, 0, part * 0x70 + track * 16 + (15-sector));
#endif
            }
        }
        fwrite( buffer, 1, 28672, f );
//        putsat( 70, 12+part, "OK" );
    }
    report = comm_getc();
bail:
    fclose( f );
    time( &tend );

    seconds = (int)difftime( tend, tstart );
    cps = (int)(143360L / seconds);
    minutes = seconds/60;
    seconds = seconds%60;
    if (!m_run)
        sprintf( error, "%s receive ABORTED after %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else if( report )
        sprintf( error, "%s received WITH ERRORS in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else
        sprintf( error, "%s received successfully in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
#if DOS_ADT
    putsat( 1, 20, error );
#else
	::PostMessage (m_wnd, WM_ADT_RECEIVED, 0, (LPARAM)error);
#endif
}


void CDiskTransfer::receive_sector( unsigned char *buffer, int part, int track, int sector )
/* Receive a sector with RLE compression */
{
    int byte;
    unsigned received_crc, computed_crc;
    unsigned char data, prev, crc1, crc2;
    unsigned char *thissector = buffer+(track<<12)+(sector<<8);
//    struct text_info textinfo;

    do {
        prev = 0;
        for( byte=0; byte<256; ) {
            data = comm_getc();
            if( data ) {
                prev += data;
                thissector[byte++] = prev;
            }
            else {
                data = comm_getc();
                do {
                    thissector[byte++] = prev;
                } while( m_run && byte<256 && byte!=data );
            }
			if (!m_run) return;
        }
        crc1 = comm_getc();
        crc2 = comm_getc();

        received_crc = crc1 + 256*crc2;
        computed_crc = do_crc( thissector, 256 );
        if( received_crc != computed_crc ) {
#if DOS_ADT
            gettextinfo( &textinfo );           /* save cursor position */
            gotoxy( 1, 20 );
            clreol();
            cprintf( "Track %02X, sector %02X: bad CRC"
                " (expected %04X, got %04X)",
                7*part+track, sector, computed_crc, received_crc );
            gotoxy( textinfo.curx, textinfo.cury );
#else
			::PostMessage (m_wnd, WM_ADT_CRCERR, 0, part * 0x70 + track * 16 + sector);
#endif
            comm_putc( NAK );
        }
    } while( received_crc != computed_crc );
    comm_putc( ACK );
}


void CDiskTransfer::getfname( char *fname )
/* Receive a null-terminated MS-DOS filename from the Apple */
{
    int i;

    for( i=0; i<_MAX_FNAME; i++ ) {
        fname[i] = comm_getc() & 0x7F;
        if( !fname[i] )
            break;
    }
}


void CDiskTransfer::make_crctable()
/* Fill the crctable[] array needed by do_crc */
{
    int byte, bit;
    unsigned crc;

    for( byte=0; byte<256; byte++ ) {
        crc = byte<<8;
        for( bit=0; bit<8; bit++ )
            crc = (crc & 0x8000 ? (crc<<1)^0x1021 : crc<<1);
        crctable[byte] = crc;
    }
}


unsigned CDiskTransfer::do_crc( unsigned char *ptr, int count )
/* Return the CRC of ptr[0]..ptr[count-1] */
{
    unsigned short crc = 0;

    while( count-- )
        crc = (crc<<8) ^ crctable[(crc>>8) ^ *ptr++];
    return crc;
}

char CDiskTransfer::comm_getc( void )
{
	BYTE c = '\0';
	while (m_run && !m_com.Getc (&c));
	return (char)c;
}
void CDiskTransfer::comm_putc( char c )
{
	m_com.Putc (c);
}
void CDiskTransfer::comm_puts( char *p )
{
	m_com.Puts (p);
}
void CDiskTransfer::comm_flush()
{
	m_com.Flush();
}

