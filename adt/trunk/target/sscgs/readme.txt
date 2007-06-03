ADT (ssc + gs)

ADT for Apple II with Super Serial Card or compatible, or IIgs
version 1.32
ref: ADT folder

by Paul Guertin (pg@sff.net)

December 4th, 1995 - 1999 ...

Apple Disk Transfer (ADT for short) is a set of two programs to
transfer a standard 16-sector Apple II disk to a 140k file on an
MS-DOS or Windows computer, and transfer a standard disk image
file to disk on an Apple II.

ADT 1.32 supports Apple II computers with a Super Serial Card or
the IIgs modem port.  Also supported are cards that are hardware-
compatible with SSC, built-in SSC-compatible serial port hardware.
The IIc+ and //c include SSC-compatible serial ports.  ADT will
scan for a suitable serial port and auto-select it if a custom
configuration has not been saved.

Note that the ADTPro server, written in Java, will service ADT 
clients while running on a broader range of operating systems.
See: http://adtpro.sourceforge.net

ADT is freeware.


                                * * *

Note: June 2007

ADT client has been updated to automatically scan slots for
a suitable serial device and auto-select it.  Slot scans
are executed from highest to lowest slot, and the last one
wins.  If there are both SSC card(s) and an IIgs modem port,
the lowest SSC card will still win.  SSC cards are given 
priority over the IIgs modem port.

David Schmidt

                                * * *

Note: December 2006

ADT client has been updated to work with both SSC and IIgs
hardware.  The ability to save the current configuration
has been built into the ADT program itself, rather than
depending on a separate BASIC utility.  Also, a protocol
error discovered by Joseph Oswalt has been corrected.

David Schmidt

                                * * *

Note: December 30th 2005

Windows version has been updated to allow longer filenames
Some minor manual and readme corrections.

Knut Roll-Lund

                                * * *

Note: November 2005

Windows 32 version has been patched to include
 higher baud rates not thought possible using the
 A2.  Modification and compile by Knut Roll-Lund

DOS version has been patched to include higher
 baud rates not thought possible using the A2.
 Modification and compile by Knut Roll-Lund

The Apple II SSC version was updated by Knut Roll-Lund and EEastman

ee

                                * * *

Note: April, 2004

This package now includes both the MS-DOS and Windows versions
of the part of ADT which runs on the PC:

Windows32 version of PC-side program is adt.exe.
 (by Sean Gugler)

MS-DOS version of PC-side program is now adtdos.exe.
 (by Paul Guertin; ADT creator)

R/

                                * * *


Main contents of ADT distribution:
 adt.exe            -- Windows server executable
 manual.txt         -- ADT documentation
 ADT/adt131.dsk     -- Disk image containing ADT 1.31
 ADT/adtpc.dmp      -- Monitor dump format of ADT (PC format)
 ADT/adtmac.dmp     -- ... Mac line ending format
 ADT/adtunix.dmp    -- ... Unix line ending format
 ADTcc/ADTcc.dmp    -- Object code for Communications 
                       Card version of ADT

                                * * *

What's new in version 1.31:

    1. Integrated a configuration save feature, 
       removed Applesoft configurator

    2. All 'Dir' command processing goes through buffering now

What was new in version 1.30:

    1. Merged SSC and IIgs code, both support 115200 baud

    2. Only distributed as part of ADTPro's built-in 
       bootstraping support

What was new in version 1.23:

    1. Added 115K transfer baud rate to the SSC version

    2. Dir command buffering on 115200 baud

    3. Windows client updated to allow use of more baud rates

    4. DOS client updated to allow the use of more baud rates


What was new in version 1.22:

    1. Modified "About" message and docs.

    2. Otherwise; no changes.


What was new in version 1.21:

    1. If a sector can't be read, it will be filled with zeros
       instead of keeping the old data of 7 tracks ago.

    2. Changed "int be" to "unsigned be" in comm_open(). Thanks
       to Tim G. Foley <timf@interlog.com> for pointing out this
       bug.


What was new in version 1.20:

    1. Added a configuration menu with user-selectable number
       of retries, etc.

    2. Added a "Dir" function to see the contents of the current
       MS-DOS directory.

    3. You can now push Escape at any time to abort a transfer.

    4. In previous versions, a bug made ADT retry 256 times if
       there was more than one bad sector in a 7-track block.
       Fixed.

    5. Faster CRC calculations.

    6. Some cosmetic changes, better error messages. The usual
       stuff you find in a minor revision.


What was new in version 1.11:

    1. In version 1.10, I forgot to set IOBVOL to 0 before calling
       RWTS, resulting in spurious "Not a 16-sector disk" errors.
       Fixed.


What was new in version 1.10:

    1. ADT now uses a simple compression scheme (differential run
       length encoding) that will speed up the transfer of many
       disks.

    2. 16-bit CRC error detection for each sector.

    3. ADT now tries 5 times to get a good read instead of giving
       up at the first error. This may help transferring old,
       flaky disks.

    4. The transfer speed is now always 19200 bps.


What was new in version 1.01:

    1. Bug fix: 12-character (8.3) MS-DOS filenames now accepted.

    2. Serial card initialization did not work with some serial
       cards.

                                * * *

For those upgrading from a previous version of ADT:

Just format a blank disk on your Apple II and use your current ADT
to transfer the file ADT.DSK. This disk image contains ADT (the
BRUN-able program).  On the MS-DOS side, replace your current 
ADT.EXE with the new one from the .zip archive.

                                * * *

Installation:

You will need a null-modem cable to connect the PC and the Apple II.
If you don't have one, you can make your own (see the end of this
file) or buy one at your local computer store.

Note: if you have a genuine Apple Super Serial Card, you do not need a
null modem cable. Simply set the jumper on the card to "terminal"
rather than "modem", and use a standard "straight-through" cable.
Thanks to Greg Bennett for this tip.

To install ADT on the PC: there's probably nothing special to do,
since you've already unzipped the main ADT file. The executable file is 
adt.exe (Win32) or adtdos.exe (if running under MS-DOS).

To install ADT on the Apple II: Boot a DOS 3.3 disk with some free
space, and from BASIC type IN#x, where x is the slot your serial card
is in (typically, x=2). Set the speed to 300 baud by typing the
three characters "<ctrl-A>6B".

Then, on the PC, start a comm program, set it for 300 baud N81, and
ASCII-upload (not Xmodem or any other protocol but a straight text
upload) the file "adtpc.dmp" to the Apple. (Example: Using HyperTerm
Under Windows 95, you would Send Text.)

During the transfer process, the program will enter itself into
the Monitor and save itself to disk under the name "ADT". Here are
some tips to get this to work. If nothing seems to work, you can
always type in the hex dump by hand (yep, it's getting longer with
each new version, but it's still under 2.5k).

     1. If you get a beep at every line, the PC comm program is
        probably sending CRLFs to your Apple, which only likes
        CRs. Turn on the "Strip linefeeds on output" option in
        your comm program.

     2. If you see total garbage on the Apple II screen, first be
        sure that the SSC and the comm program agree on the
        transfer speed. If they do, your comm program is probably
        doing something funny with the high bit (aka bit 7) of
        each character. Turn off the "Strip high bit on output"
        option.

     3. If you are uploading ADT to an unaccelerated Apple II,
        you may need to slow down the transfer. Your comm program
        probably allows you to set a fixed delay between each
        line (aka "line pacing"). A 1.2-second pause worked well
        with a 1 MHz Apple //c. [Note: disk transfers with ADT
        are done at 19200 bps, even with a 1 MHz machine. The
        slowness here is due to the way the Apple Monitor works.]

Now, make sure your Apple is in BASIC and ASCII-upload the file
"adtcfg.bas". Again, the program will enter itself and be saved to
disk under the name "ADT CONFIGURATION". Since BASIC has to parse each
line after it has been entered, you will probably need to increase the
"line pacing" value. Just to be sure it will work, I set it to 3
seconds.

                                * * *

How to use ADT:

On the Apple II, type 'BRUN ADT' from the DOS 3.3 prompt. On the PC,
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
the Apple II.  ADTPro (http://adtpro.sourceforge.net) will.


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


                                * * *

Speed comparison for ADT 1.01, 1.11, and 1.22 (Apple to PC, 19200 bps)

Disk                        1.01    1.11    1.22
-------------------------   ----    ----    ----
DOS 3.3 System Master:      1:42    1:33    0:59   (times are min:sec)
AW data disk, 1k free:      1:42    2:13    1:30
Eamon adventure #12:        1:42    1:24    0:48
Karateka (cracked):         1:42    1:54    1:15


Speed of ADT 1.23 at 115200b to:
Disk                        1.23
-------------------------   ----
Karateka (cracked):          :40


1.01 always takes 1:42 since it does not use a compression algorithm.
1.11 does RLE compression and CRC error checking (this explains why it
     is sometimes slower than 1.01).
1.22 also does RLE and has a faster CRC routine.

                                * * *

Recompilation / reassembly:

On the PC: to compile ADT with Turbo C 2.0, use: "TCC ADT.C COMM.C".
Recompilation is required only if you modify the program.

On the Apple II: The ca65 assembler (part of the CC65 compiler suite:
http://www.cc65.org) will assemble the project.  A complete
development environment is available from http://adt.berlios.de.

                                * * *

ADT is based on the program SENDDISK by Rich Williamson
(glitch@eskimo.com). In fact, version 0.01 of ADT was a patch to
SENDDISK to make it transfer files from the PC to the Apple. I wish
to acknowledge Mr. Williamson's contribution to ADT.

                                * * *

The following is taken from the SENDDISK readme file, and may
help you configure your hardware.

> Super Serial card DIP settings: (19200 baud)
> SW1-1 off      SW2-1 on
>   1-2 off        2-2 on
>   1-3 off        2-3 off
>   1-4 off        2-4 on
>   1-5 on         2-5 off
>   1-6 on         2-6 off
>   1-7 on         2-7 off
>
> Put the TERMINAL/MODEM jumper pointing up at 'MODEM'
>
> Serial cable wiring
> PC (DB-9)     Apple (DB-25)
>
>         2 - 2
>         3 - 3
>         4 - 8
>         5 - 7

Note on the setting of SW2-6 Interrupt: This is a hard switch it
disables or enables interrupts from the SSC to the CPU. ADT doesn't
use interrupt so it is sensible to disable interrupts but if you are
using other comm programs with the SSC it is likely that interrupt is
used, then the SW2-6 must be on. As it is, having SW2-6 on doesn't
affect ADT. So if it is not completely out of the question to use
other comm programs set SW2-6 on.

                                * * *

ADT is maintained at http://adt.berlios.de.

=EOF=