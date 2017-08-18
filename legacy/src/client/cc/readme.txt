
ADTcc

ADT for Apple Communications Card (and other non-SSC cards)
ref: ADTcc folder

by Paul Schlyter


This is an adaptation of Paul Guertin's ADT (Apple Disk Transfer) utility
so it runs on the older Apple Communications Card. That old serial card
supports only two bit rates.  By default these bit rates are 110 and 300 bps,
but the Comm Card can be strapped for 300/1200 bps or 1200/4800 bps. This
strapping is higly recommended, unless you're VERY patient in your
downloads (downloading one 140K disk takes almost 1.5 hours in 300 bps
but only 5 minutes in 4800 bps).  The Apple Comm Card manual describes
how to do this strapping.  ADT for the Comm Card always uses the higher
bit rate and offers no option to select the lower bit rate.

                                * * *


Note: April, 2004

This package now includes both the MS-DOS and Windows versions
of the part of ADT which runs on the PC:

Windows32 version of PC-side program is adt.exe.
 (by Sean Gugler)

MS-DOS version of PC-side program is now adtdos.exe.
 (by Paul Guertin; ADT creator)

R/




ADTcc includes the following files:


ADT_SC.DSK     .DSK image of S-C Asm source for ADT: Super Serial & Comm 
               Card versions


Text files

ADT_ss.asm     ADT for Super Serial Card: S-C Assembler source
ADT_cc.asm     ADT for Comm Card: S-C Assembler source
ADT.dif        Differences between ADT_ss.asm and ADT_cc.asm
ADT_ss.dmp     ADT for Super Serial Card: hex dump of executable
                (note: sends address at start of each line)
ADTcc.dmp      ADT for non SSC Comm Cards: hex dump of executable
                (note: sends address at start of each line)
readme.txt     Directions


                                * * *

Mini-Review

     Finally got a chance to try out your new modified ADT on our II+.
It has a 1983 Computer Associates J13 serial card. The card would not work
with regular ADT because it is not SSC-compatible.

     Using Hyperterm (set to screen out LF's in line enders), the dump file
(e.g. ADTcc.dmp) was sent (Send Text) to the II+ at 300 baud. It seemed to
arrive okay and saved itself as ADT.CC on the DOS 3.3 diskette.

     At 19,200 baud, ADT.CC worked like a champ! There was no problem getting
a PC directory; and, both received and sent disk images checked out.

Rubywand

                                * * *


Installation:

You will need a null-modem cable to connect the PC and the Apple II.
If you don't have one, you can make your own (see the end of this
file) or buy one at your local computer store.

To install ADT on the PC there's probably nothing special to do,
since you've already unzipped the main ADT file. The executable file
is adt.exe (Win32) or adtdos.exe (if running under MS-DOS)..

To install ADT on the Apple II: Boot a DOS 3.3 disk with some free
space, and from BASIC type IN#x, where x is the slot your serial card
is in (typically, x=2). Set the speed to 300 baud.

Then, on the PC, start a comm program, set it for 300 baud N81, and
ASCII-upload (not Xmodem or any other protocol but a straight text
upload) the file "ADTcc.dmp" to the Apple. (Example: Using HyperTerm
Under Windows 95, you would Send Text.)

During the transfer process, the program will enter itself into
the Monitor and save itself to disk under the name "ADT.cc". Here are
some tips to get this to work. If nothing seems to work, you can
always type in the hex dump by hand (yep, it's getting longer with
each new version, but it's still under 2.5k).

     1. If you get a beep at every line, the PC comm program is
        probably sending CRLFs to your Apple, which only likes
        CRs. Turn on the "Strip linefeeds on output" option in
        your comm program.

     2. If you see total garbage on the Apple II screen, first be
        sure that your serial card and the comm program agree on the
        transfer speed. If they do, your comm program is probably
        doing something funny with the high bit (aka bit 7) of
        each character. Turn off the "Strip high bit on output"
        option.

     3. If you are uploading ADT to an unaccelerated Apple II,
        you may need to slow down the transfer. Your comm program
        probably allows you to set a fixed delay between each
        line (aka "line pacing"). A 1.2-second pause worked well
        with a 1 MHz Apple //c. [Note: disk transfers with ADT
        can go as high as 19200 bps, even with a 1 MHz machine. The
        slowness here is due to the way the Apple Monitor works.]


                                * * *

How to use ADT:

On the Apple II, type 'BRUN ADT.cc' from the DOS 3.3 prompt. On the PC,
you can type 'ADT', and the program will ask you which serial port to
use and the transfer speed. You can also type these parameters on the
MS-DOS command line, eg. 'ADT 2 19200'. If you always use the same
serial port and speed, I suggest making a batch file or a 4DOS alias.

Except for the PC configuration (serial port & speed), ADT operation
is entirely controlled from the Apple II keyboard, so you can transfer
many disks without having to go back and forth between the two
computers.

At the bottom of the Apple screen, you will see the following menu:

    Send, Receive, Dir, Configure, Quit?

To select an item, simply type the first letter of the word. One
keystroke is enough, you don't have to hit Return.

Send: ADT will ask for an MS-DOS filename and then start reading the
disk in the specified slot and drive and transmit it to the PC, which
will store it in that file. ADT will not overwrite an existing file.
All files are stored in the current directory and are 143,360 bytes
in length.

The MS-DOS file format is as follows:
          T0S0B0 T0S0B1 ... T0S0BFF T0S1B0 ... ... T22SFBFF
which is the format used by the apl2em emulator by Randy Spurlock.


Receive: ADT will ask for a filename. It will then receive that file
from the PC's current directory and write it to the disk in slot 6
drive 1, overwriting any data that may be on the disk. THE DISK IN
SLOT 6 DRIVE 1 MUST BE 16-SECTOR FORMATTED. Before transmitting, ADT
ensures that the file exists and is 140k in length. (I didn't add
signature bytes because I wanted to stay compatible with the apl2em
format.)


Dir: ADT will display a list of the files in the current MS-DOS
directory. There is currently no way to change directories from within
ADT.


Configure: this takes you to the ADT configuration menu. Use the up
and down arrow keys to select a parameter (Apple ][+ users, use space
instead). The left and right arrow keys change the value of the
current parameter. Return saves the displayed values and exits. Escape
exits without saving the changes. Finally, ctrl-D resets all
parameters to the default values (you can change the default values by
using the BASIC program ADT CONFIGURATION). Here's an explanation of
each parameter:

    Disk slot, disk drive, SSC slot: self-explanatory. The
    defaults are slot 6, drive 1, and the SSC in slot 2.

    SSC speed: use the highest speed that works. The default is
    19200, and it works with my 1 MHz Apple //e.

    Read retries: number of times RWTS will try to read a sector
    before giving up. I suggest 1 or 2. If you have a
    particularly weak disk, and don't mind the recalibrating
    noise, you can increase it (also consider adjusting your
    drive speed, and cleaning the drive head).

    Write retries: same, but for writing to a disk. The default
    is 0: if I have trouble writing to a disk, even only once, I
    usually trash the disk. My data is worth more than $0.50.

    Use checksums: DOS puts a checksum at the end of each sector
    as an error-detection measure. If the checksum is wrong, it
    means the data is probably corrupt and RWTS will issue an I/O
    error. Setting this parameter to NO will defeat this check:
    even if the checksum is wrong, the sector will be read
    normally. This may be of some help in transferring very old
    disks on which a few bits have . It will also allow ADT to
    copy disks where the only protection scheme is a different
    checksum algorithm.

        When you modify this parameter, ADT patches the copy of
    DOS in memory. If you're using a non-standard version of DOS,
    the code that checks for checksum errors may have been moved
    to a different address. In that case, ADT will show the words
    "DO NOT CHANGE" next to the parameter. (It will not prevent
    you from changing it, however... If you really want to
    overwrite two random bytes in your custom DOS, go ahead, but
    you assume all the risks!)

        This parameter was suggested by Andy McFadden.

    Enable sounds: YES means ADT will beep when a transfer is
    finished, and on some other occasions. If this parameter is
    set to NO, ADT will be totally silent. By the way, some of
    you old-timers may recognize the beep: it's the same beep
    used in my all-time favorite word processor, Apple Writer II
    by Paul Lutus.


Quit: This quits ADT, returning to DOS 3.3.


?: Shows "About ADT" message.



On the PC, you can type the following keys:

    C allows you to change the serial port and speed (if you
    change the speed on one computer, don't forget to do the same
    for the other).

    ctrl-L redraws the screen.

    Q quits ADT.


Even though ADT runs under DOS 3.3, it will also transfer ProDOS disks
as well as Pascal disks, because they are all in 16-sector format.

On the PC, a disk will always arrive as a DOS 3.3 ordered .dsk disk
image file. You should include ".dsk" at the end of the name for any
disk you send or add ".dsk" to the file name on the PC. 

On the Apple II, a transferred .dsk disk image file is converted
directly to 5.25" diskette.

ADT will not transfer DOS 3.2 (13-sector) disks to PC; and, It will not
transfer protected disks to PC unless the protection scheme is based on
the DOS checksums only (and even then, there's no guarantee the disk will
work in the emulator).

ADT will not correctly transfer ProDOS-order (.po) disk image files to
the Apple II.


Meaning of characters on the Apple II status display:
    R: Reading from disk
    W: Writing to disk
    I: Receiving data from PC ("Input")
    O: Sending data to PC ("Output")
    .: Sector read or written correctly
    *: Error while reading or writing sector

ABORTING: You can press the escape key at any time during the transfer
and everything will stop. You will be back to the ADT menu. This is
true for the Apple and the PC. When you are aborting a transfer, stop
the _sending_ computer first. If you first stop the receiving
computer, it will continue to receive unexpected data and may become
confused.

ADT CONFIGURATION: use this BASIC program to change the default
parameter values. Its operation is self-explanatory. Just follow
the prompts.


