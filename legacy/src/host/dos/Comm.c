/*    From CompuServe's BPROGB Borland Int'l forum
 *
 *    #:  11879 S4/Turbo C
 *        02-Jun-88  18:54:50
 *    Sb: #11817-VT100
 *    Fm: Jerry Joplin 70441,2627
 *    To: Pete Gontier 72261,1754 (X)
 *
 *    Pete, here are the communication ISR's I've been using.   Hope these
 *    don't overflow the message data base for BPROGB.  :-)
 *    I think all is included to set up COM1/COM2 for receiver interrupts.
 *
 *    Also, these are straight ports from routines in MSKERMIT 2.30 and CKERMIT
 *    for UNIX, which are public domain but copyrighted (c) by Columbia
 *    University.
 */

/*    Last modification, to support other compiler(s):
 *       Pete Gontier, July 20th, 1988
 *    This code is no longer portable to the UNIX systems mentioned.
 *    The function names were in accordance with the TT teletype conventions,
 *    and I couldn't stand for it. So sue me.
 */

/*    Modified by Paul Guertin, Oct. 6, 1994
 *    Added longjmps, removed debug code, changed dobaud to report errors.
 *    Dec. 4 1995: changed "be" from int to unsigned in comm_open()
 */

/*    Modified by Knut Roll-Lund, Nov. 7, 2005
 *    Added more baudrates extended parameters to long to allow high bauds
 */

#include <dos.h>

#define TCICOMM    /* avoid extern declaration in... */
#include "comm.h"  /* comm prototypes */

/** macros for absolute data ************************************************/

#define MDMDAT1 0x03F8            /* Address of modem port 1 data */
#define MDMSTS1 0x03FD            /* Address of modem port 1 status  */
#define MDMCOM1 0x03FB            /* Address of modem port 1 command */
#define MDMDAT2 0x02F8            /* Address of modem port 2 data */
#define MDMSTS2 0x02FD            /* Address of modem port 2 status */
#define MDMCOM2 0x02FB            /* Address of modem port 2 command */
#define MDMINTV 0x000C            /* Com 1 interrupt vector */
#define MDINTV2 0x000B            /* Com 2 interrupt vector */
#define MDMINTO 0x0EF             /* Mask to enable IRQ3 for port 1 */
#define MDINTO2 0x0F7             /* Mask to enable IRQ4 for port 2 */
#define MDMINTC 0x010             /* Mask to Disable IRQ4 for port 1 */
#define MDINTC2 0x008             /* Mask to Disable IRQ3 for port 2 */
#define INTCONT 0x0021            /* 8259 interrupt controller ICW2-3 */
#define INTCON1 0x0020            /* Address of 8259 ICW1 */

#define CBS     5000              /* Communications port buffer size */
#define XOFF    0x13              /* XON/XOFF */
#define XON     0x11

/** globals not externally accessible ***************************************/

static int   dat8250;             /* 8250 data register */
static int   stat8250;            /* 8250 line-status register */
static int   com8250;             /* 8250 line-control register */
static char  en8259;              /* 8259 IRQ enable mask */
static char  dis8259;             /* 8259 IRQ disable mask */
static unsigned int intv;         /* interrupt number to usurp */

static char  buffer[CBS];         /* Communications circular buffer */
static char *inptr;               /* input address of circular buffer */
static char *outptr;              /* output address of circular buffer */
static int   c_in_buf = 0;        /* count of characters received */
static int   xoffpt;              /* amount of buffer that forces XOFF */
static int   xonpt;               /* amount of buffer that unXOFFs */


static void interrupt ( far *oldvec ) ( );
                                  /* vector of previous comm interrupt */

int xonxoff = 0;                          /* auto xon/xoff support flag */
int xofsnt  = 0;                          /* XOFF transmitted flag */
int xofrcv  = 0;                          /* XOFF received flag */

void interrupt serint ( void ) {          /* ISR to receive character */
   *inptr++ = inportb ( dat8250 );        /* Store character in buffer */
   c_in_buf++;                            /* and increment count */
   if ( xonxoff ) {                       /* if xon/xoff auto-support is on */
      if ( c_in_buf > xoffpt && ! xofsnt ) {  /* then if buf nearly full */
         comm_putc ( XOFF );              /* send an XOFF */
         xofsnt = 1;                      /* and say so */
      }
   }
   disable ( );                           /* ints off for ptr change */
   if ( inptr == &buffer[CBS] )           /* Set buffer input pointer */
      inptr = buffer;
   enable ( );
   outportb ( 0x20, 0x20 );               /* Generic EOI to 8259 */
}


void comm_close ( void ) {             /* restore previous settings of 8259 */
    outportb ( INTCONT, dis8259 | inportb ( INTCONT ) );
       /* Disable com interrupt at 8259 */
    setvect ( intv, oldvec );      /* Reset original interrupt vector */
}

int  dobaud ( long baudrate ) {             /* parses baud integer to mask,
                                             * re-inits port accordingly */
   unsigned char portval;
   unsigned char blo, bhi;
   switch  ( baudrate ) {          /* Baud rate LSB's and MSB's */
    /* case 50:     bhi = 0x9;  blo = 0x00;  break; */
    /* case 75:     bhi = 0x6;  blo = 0x00;  break; */
    /* case 110:    bhi = 0x4;  blo = 0x17;  break; */
    /* case 150:    bhi = 0x3;  blo = 0x00;  break; */
       case 300:    bhi = 0x1;  blo = 0x80;  break;
    /* case 600:    bhi = 0x0;  blo = 0xC0;  break; */
       case 1200:   bhi = 0x0;  blo = 0x60;  break;
    /* case 1800:   bhi = 0x0;  blo = 0x40;  break; */
    /* case 2000:   bhi = 0x0;  blo = 0x3A;  break; */
       case 2400:   bhi = 0x0;  blo = 0x30;  break;
       case 4800:   bhi = 0x0;  blo = 0x18;  break;
       case 9600:   bhi = 0x0;  blo = 0x0C;  break;
       case 19200:  bhi = 0x0;  blo = 0x06;  break;
       case 38400:  bhi = 0x0;  blo = 0x03;  break;
       case 57600:  bhi = 0x0;  blo = 0x02;  break;
       case 115200: bhi = 0x0;  blo = 0x01;  break;
       default:
           return -1;
   }
   portval = inportb ( com8250 );         /* read Line-Control Reg val */
   outportb ( com8250, portval | 0x80 );  /* set high bit for baud init */
   outportb ( dat8250, blo );             /* Send LSB for baud rate */
   outportb ( dat8250 + 1, bhi );         /* Send MSB for baud rate */
   outportb ( com8250, portval );         /* Reset initial value at LCR */
   return 0;
}


/*************************************************************************/

#include <bios.h>
#include <setjmp.h>
#include <conio.h>

extern jmp_buf beginning;

/* installs comm interrupts */

int comm_open ( int portid, long speed )
{
   unsigned be = biosequip ( ); /* to get # installed serial ports */
   be <<= 4;                    /* shift-wrap high bits off */
   be >>= 13;                   /* shift down to low bits */
   if ( be >= portid ) {
      if ( portid == 1 ) {
          dat8250  = MDMDAT1;
          stat8250 = MDMSTS1;
          com8250  = MDMCOM1;
          dis8259  = MDMINTC;
          en8259   = MDMINTO;
          intv = MDMINTV;
      }
      else if ( portid == 2 ) {
          dat8250  = MDMDAT2;
          stat8250 = MDMSTS2;
          com8250  = MDMCOM2;
          dis8259  = MDINTC2;
          en8259   = MDINTO2;
          intv = MDINTV2;
      }
      else
         return ( 0 );
      if( dobaud ( speed ) )              /* set baud */
         return 0;
      inptr = outptr = buffer;            /* set circular buffer values */
      c_in_buf = 0;
      oldvec = getvect ( intv );          /* Save old int vector */
      setvect  ( intv, serint );          /* Set up SERINT as com ISR */
      outportb ( com8250,     0x3 );      /* 8 bits no parity */
      outportb ( com8250 + 1, 0xb );      /* Assert OUT2, RTS, and DTR */
      inportb  ( dat8250 );
      outportb ( dat8250 + 1, 0x1 );      /* Receiver-Data-Ready int */
      outportb ( INTCONT, en8259 & inportb ( INTCONT ) );
                                          /* Enable 8259 interrupts */
      xoffpt = CBS / 50 * 49;             /* chars in buff to send XOFF */
      xonpt  = CBS - xoffpt;              /* chars in buff to send XON */
   }
   else
      be = 0;
   return ( be );
}


int comm_avail ( )       /* returns # characters available in buffer */
{
    return ( c_in_buf );
}


void comm_putc ( unsigned char c )       /* sends char out port */
{
    while ( ( inportb ( stat8250 ) & 0x20 ) == 0 )
       if( kbhit() && getch() == 0x1B )
           longjmp( beginning, 1 );       /* Wait til transmitter is ready */
    outportb ( dat8250, c );              /* then send it */
}


int comm_getc ( )                         /* gets char from buffer */
{
    int c;
    register char * ptr;
    if ( c_in_buf < xonpt && xofsnt ) {   /* Check if we need to send */
       xofsnt = 0;                        /* an XON to the host after */
       comm_putc ( XON );                 /* we had to send an XOFF */
    }
    while  ( c_in_buf == 0 )              /* If character not ready */
        if( kbhit() && getch() == 0x1B )
            longjmp( beginning, 2 );      /* then wait til one is   */
    ptr = outptr;
    c = *ptr++;                     /* Get next character in circular buff */
    if ( ptr == &buffer[CBS] )      /* Check for end of circular buffer */
        ptr = buffer;               /* start from bottom of buff */
    disable ( );                    /* no interrupts during pointer manips */
    outptr = ptr;                   /* set character output pointer */
    c_in_buf--;                     /* and decrement the character count */
    enable ( );                     /* then allow interrupts to continue */
    return ( c );                   /* Return the character */}

void comm_flush ( )                 /* flushes all chars out of buffer */
{
    if ( xofsnt ) {                 /* Check if XON needs to be sent */
       xofsnt = 0;
       comm_putc ( XON );
    }
    disable ( );                    /* no interrupts during pointer manips */
    inptr = outptr = buffer;        /* reset buffer pointers */
    c_in_buf = 0;                   /* and indicate no chars received */
    enable ( );
}
