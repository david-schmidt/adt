/*---------------------------
 * Apple Disk Transfer 1.22
 * By Paul Guertin
 * pg@sff.net
 * October 13th, 1994 - 1999
 * Distribute freely
 *--------------------------*/

/*---------------------------
 * Version 1.23 2005
 * by Knut Roll-Lund
 * November 7th, 2005
 * allow more baudrates
 *--------------------------*/

/*
 * Compile with Turbo C 2.0:
 * TCC ADT.C COMM.C
 */


#include <conio.h>
#include <stdio.h>
#include <dir.h>
#include <io.h>
#include <ctype.h>
#include <setjmp.h>
#include <time.h>
#include "comm.h"

#define ACK 0x06
#define NAK 0x15

int main( int argc, char **argv );
void send_directory( void );
void send_disk( void );
void send_sector( unsigned char *buffer, int part, int track, int sector );
void receive_disk( void );
void receive_sector( unsigned char *buffer, int part, int track, int sector );
void getfname( char *fname );
void comm_puts( char *p );
int set_port( void );
void clear_display( void );
void putsat( int x, int y, char *s );
void give_help( void );
void make_crctable( void );
unsigned do_crc( unsigned char *ptr, int count );


int comport;                    /* serial port details */
long comspeed;                  /* serial port details */
jmp_buf beginning;              /* where to abort when ESC pushed */
unsigned crctable[256];         /* CRC-16 lookup table for speed */


int main( int argc, char **argv )
{
    int command;                            /* character from Apple */

    comport = comspeed = 0;
    argc = argc;                            /* prevent compiler warning */

    while( *++argv ) {
        if( !isdigit( **argv ) ) {          /* argument is not a digit, */
            give_help();                    /* give help and quit */
            return 0;
        }
        if( !(*argv)[1] )                   /* kluge: a number is a port */
            comport = **argv - '0';         /* iff it has only one digit */
        else
            comspeed = atol( *argv );
    }

    make_crctable();

refresh:
    setjmp( beginning );                    /* return here when ESC pushed */

    clrscr();
    textattr( LIGHTGRAY + (BLACK<<4) );
    putsat( 28, 2, "Apple Disk Transfer 1.23" );

    if( set_port() )
        return -1;

    clear_display();
    putsat( 1, 24, "C to change port/speed, ^L to refresh screen, "
                   "Q to quit program." );

    for( ;; ) {
        putsat( 1, 9, "READY." );
        while( !comm_avail() ) {
            if( kbhit() ) {
                switch( getch() ) {
                    case 'C': case 'c':
                        comm_close();
                        comport = comspeed = 0;
                        if( set_port() )
                            return -1;
                        putsat( 1, 9, "READY." );
                        break;
                    case 'Q': case 'q':
                        putsat( 1, 9, "" );
                        putsat( 1, 24, "" );
                        putsat( 1, 20, "Goodbye!\n" );
                        return 0;
                    case 0x0C:              /* ctrl-L = refresh screen */
                        goto refresh;
                }
            }
        }
        putsat( 1, 20, "" );
        command = comm_getc() & 0x7F;
        switch( command ) {
            case 'D': send_directory();  break;
            case 'R': send_disk(); break;
            case 'S': receive_disk(); break;
            default:
                putsat( 1, 20, "ERROR: unknown command received: " );
                cprintf( "%02X", command );
        }
    }
}


void send_directory()
/* Format and send the current of current directory */
{
    int i;
    int tab, line;
    char *dirname = getcwd( NULL, 256 );
    struct ffblk ff;

    putsat( 1, 9, "Sending directory listing   (ESC to abort)" );
    comm_puts( "DIRECTORY OF " );
    comm_puts( dirname );
    if( (strlen( dirname ) + 13) % 40 )
        comm_putc(  '\r' );
    for( i=0; i<40; i++ )
        comm_putc( '-' );

    if( findfirst( "*.*", &ff, 0 ) == 0 ) {
        tab = 0;
        line = (strlen( dirname ) + 13) / 40 + 2;
        for( ;; ) {
            if( kbhit() && getch() == 0x1B )
                break;
            comm_puts( ff.ff_name );
            for( i=strlen( ff.ff_name ); i<12; i++ )
                comm_putc( ' ' );
            if( tab < 2 ) {
                comm_putc( ' ' );
                comm_putc( ' ' );
            }
            if( comm_avail() && comm_getc() == '\0' )
                break;
            if( findnext( &ff ) == -1 )
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
        }
    }
    else
        comm_puts( "NO FILES" );

    comm_putc( '\0' );
    comm_putc( '\0' );
    comm_flush();
    free( dirname );
}


void send_disk()
/* Main send routine */
{
    char fname[13], error[80];
    unsigned char buffer[28672];
    FILE *f;
    int part, track, sector, report;
    time_t tstart, tend;
    int minutes, seconds, cps;

    getfname( fname );
    putsat( 1, 9, "Sending file " );
    putsat( 14, 9, fname );
    cputs( "   (ESC to abort)" );

    f = fopen( fname, "rb" );
    if( !f ) {
        comm_putc( 26 );        /* can't open */
        sprintf( error, "ERROR: File %s can't be opened for input.", fname );
        putsat( 1, 20, error );
        return;
    }
    if( filelength( fileno( f ) ) != 143360L ) {
        comm_putc( 30 );        /* not a 140k image */
        sprintf( error, "ERROR: File %s is not a 140K disk image.", fname );
        putsat( 1, 20, error );
        return;
    }
    comm_putc( 0 );     /* File is now ready */

    time( &tstart );
    clear_display();
    while( comm_getc() != ACK )
        putsat( 1, 20, "ERROR: Bad header byte received." );
    for( part=0; part<5; part++ ) {
        putsat( 70, 12+part, "Reading" );
        fread( buffer, 1, 28672, f );
        putsat( 70, 12+part, "" );
        gotoxy( 12, 12+part );
        for( track=0; track<7; track++ ) {
            for( sector=15; sector>=0; sector-- ) {
                send_sector( buffer, part, track, sector );
                if( sector%2 == 1 )
                    putch( '‏' );
            }
        }
        putsat( 70, 12+part, "OK" );
    }
    report = comm_getc();
    fclose( f );
    time( &tend );

    seconds = (int)difftime( tend, tstart );
    cps = (int)(143360L / seconds);
    minutes = seconds/60;
    seconds = seconds%60;
    if( report )
        sprintf( error, "File %s sent WITH ERRORS in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else
        sprintf( error, "File %s sent successfully in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    putsat( 1, 20, error );
}


void send_sector( unsigned char *buffer, int part, int track, int sector )
/* Send a sector with RLE compression */
{
    int byte, crc, ok;
    unsigned char data, prev, newprev;
    unsigned char *thissector = buffer+(track<<12)+(sector<<8);
    struct text_info textinfo;

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
                while( byte<256 && thissector[byte] == newprev )
                    byte++;
                comm_putc( byte & 0xFF );       /* 256 becomes 0 */
            }
        }
        crc = do_crc( thissector, 256 );
        comm_putc( crc & 0xFF );
        comm_putc( crc >> 8 );
        ok = comm_getc();
        if( ok != ACK ) {
            gettextinfo( &textinfo );           /* save cursor position */
            gotoxy( 1, 20 );
            clreol();
            cprintf( "Track $%02X, sector $%02X: bad CRC",
                7*part+track, sector );
            gotoxy( textinfo.curx, textinfo.cury );

        }
    } while( ok != ACK );
}


void receive_disk()
/* Main receive routine */
{
    char fname[13], error[80];
    unsigned char buffer[28672];
    FILE *f;
    int part, track, sector, report;
    time_t tstart, tend;
    int minutes, seconds, cps;

    getfname( fname );
    putsat( 1, 9, "Receiving file " );
    putsat( 16, 9, fname );
    cputs( "   (ESC to abort)" );

    if( !access( fname, 0 ) ) {
        comm_putc( 28 );        /* File exists */
        sprintf( error, "ERROR: File %s already exists.", fname );
        putsat( 1, 20, error );
        return;
    }
    if( (f = fopen( fname, "wb" )) == NULL ) {
        comm_putc( 26 );        /* Can't open */
        sprintf( error, "ERROR: File %s can't be opened for output.", fname );
        putsat( 1, 20, error );
        return;
    }
    for( track=0; track<35; track++ ) {
        if( fwrite( buffer, 1, 4096, f ) != 4096 ) {
            comm_putc( 30 );    /* Disk full */
            sprintf( error, "ERROR: File %s doesn't fit on disk.", fname );
            putsat( 1, 20, error );
            fclose( f );
            return;
        }
    }
    rewind( f );
    comm_putc( 0 );         /* File is now ready */

    time( &tstart );
    clear_display();
    while( comm_getc() != ACK )
        putsat( 1, 20, "ERROR: Bad header byte received." );
    for( part=0; part<5; part++ ) {
        gotoxy( 12, 12+part );
        for( track=0; track<7; track++ ) {
            for( sector=15; sector>=0; sector-- ) {
                receive_sector( buffer, part, track, sector );
                if( sector%2 == 1 )
                    putch( '‏' );
            }
        }
        fwrite( buffer, 1, 28672, f );
        putsat( 70, 12+part, "OK" );
    }
    report = comm_getc();
    fclose( f );
    time( &tend );

    seconds = (int)difftime( tend, tstart );
    cps = (int)(143360L / seconds);
    minutes = seconds/60;
    seconds = seconds%60;
    if( report )
        sprintf( error, "File %s received WITH ERRORS in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    else
        sprintf( error, "File %s received successfully in %d:%02d (%d cps).",
            fname, minutes, seconds, cps );
    putsat( 1, 20, error );
}


void receive_sector( unsigned char *buffer, int part, int track, int sector )
/* Receive a sector with RLE compression */
{
    int byte;
    unsigned received_crc, computed_crc;
    unsigned char data, prev, crc1, crc2;
    unsigned char *thissector = buffer+(track<<12)+(sector<<8);
    struct text_info textinfo;

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
                } while( byte<256 && byte!=data );
            }
        }
        crc1 = comm_getc();
        crc2 = comm_getc();

        received_crc = crc1 + 256*crc2;
        computed_crc = do_crc( thissector, 256 );
        if( received_crc != computed_crc ) {
            gettextinfo( &textinfo );           /* save cursor position */
            gotoxy( 1, 20 );
            clreol();
            cprintf( "Track %02X, sector %02X: bad CRC"
                " (expected %04X, got %04X)",
                7*part+track, sector, computed_crc, received_crc );
            gotoxy( textinfo.curx, textinfo.cury );
            comm_putc( NAK );
        }
    } while( received_crc != computed_crc );
    comm_putc( ACK );
}


void getfname( char *fname )
/* Receive a null-terminated MS-DOS filename from the Apple */
{
    int i;

    for( i=0; i<=13; i++ ) {
        fname[i] = comm_getc() & 0x7F;
        if( !fname[i] )
            break;
    }
}


void comm_puts( char *p )
/* Like puts() but through the comm port */
{
    while( *p )
        comm_putc( *p++ );
}


int set_port()
/* Show comport and comspeed values, asking for them if they're zero */
/* Return -1 on failure, 0 on success */
{
retry:
    putsat( 1, 5, "Serial port: " );
    if( comport )
        putch( comport + '0' );
    else {
        scanf( "%d", &comport );
        if( !comport )              /* return & quit if port = 0 */
            return -1;
    }

    putsat( 1, 6, "Speed: " );
    if( comspeed )
        cprintf( "%ld", comspeed );
    else
        scanf( "%D", &comspeed );

    if( !comm_open( comport, comspeed ) ) {
        cprintf( "\n\nUnable to open port %d at %ld bps.\n\n",
            comport, comspeed );
        comport = comspeed = 0;
        goto retry;                 /* so sue me */
    }
    return 0;
}


void clear_display()
/* Clear the disk transfer status */
{
    int part;

    for( part=1; part<=5; part++ ) {
        putsat( 4, 11+part, "Part " );
        putch( '0'+part );
        cputs( ": תתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתת" );
    }
}


void putsat( int x, int y, char *s )
/* Show a string at x,y, also erasing to end of line */
{
    gotoxy( x, y );
    clreol();
    cputs( s );
}


void give_help()
/* Text printed when called with illegal arguments */
{
    puts( "\n"
          "Apple Disk Transfer 1.23 -- November 7th, 2005\n"
          "based on, modified to allow more baudrates\n"
          "Apple Disk Transfer 1.22 -- October 13th, 1994\n"
          "By Paul Guertin (pg@sff.net)\n"
          "Distribute freely\n"
          "\n"
          "This program transfers a 16-sector Apple II disk\n"
          "to a 140k MS-DOS file and back.  The file format\n"
          "is compatible with Randy Spurlock's APL2EM emulator.\n"
          "\n"
          "Syntax: ADT [comport] [comspeed]\n"
          "See readme.txt for details.\n" );
}


void make_crctable()
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


unsigned do_crc( unsigned char *ptr, int count )
/* Return the CRC of ptr[0]..ptr[count-1] */
{
    unsigned crc = 0;

    while( count-- )
        crc = (crc<<8) ^ crctable[(crc>>8) ^ *ptr++];
    return crc;
}