#ifndef TCICOMM
   extern int xonxoff;  /* auto xon/xoff support is on flag */
   extern int portin;   /* com port is open flag */
   extern int xofsnt;   /* XOFF transmitted flag */
   extern int xofrcv;   /* XOFF received flag */
#endif

/* port number parameters for comm_open() */

#define COM1 1
#define COM2 2

/* prototypes for externally called functions */

int  comm_open  ( int port, long baud );
void comm_close ( void );
void comm_flush ( void );
int  comm_avail ( void );
void comm_putc  ( unsigned char c );
int  comm_getc  ( void );