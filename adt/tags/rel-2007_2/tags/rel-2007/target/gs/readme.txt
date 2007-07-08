
ADTgs

Apple Disk Transfer for PC-->Apple IIgs disk image transfers

beta version 0.91
mainly this is a February, 2000 update


Note: April, 2004

This package now includes both the MS-DOS and Windows versions
of the part of ADT which runs on the PC:

Windows32 version of PC-side program is adt.exe.
 (by Sean Gugler)

MS-DOS version of PC-side program is now adtdos.exe.
 (by Paul Guertin; ADT creator)

R/



ADTgs is a set of two programs to transfer standard 140k 5.25" .dsk 
disk image files from PC to 5.25" diskettes on an Apple IIgs. The PC
program is named "adt.exe" and the IIgs program is named "ADT".

To use ADTgs91.zip, you download the file to PC, unzip it, and send 
the Apple IIgs part (named "adtgs.dmp") to your IIgs as text. No special
software is needed on the IIgs; you just use the IN#2 command to accept
the text. When the transfer is complete, the BRUNable program ADT is
saved on DOS 3.3 diskette on the IIgs.


ADT runs under DOS 3.3. It will transfer 5.25" DOS 3.3, ProDOS, Pascal,
etc. .dsk disk images so long as the .dsk disk image uses DOS 3.3 sector
ordering-- most .dsk files do. ADTgs will not correctly transfer .PO or
any other ProDOS order disks.


ADTgs uses the built-in IIgs modem serial port and firmware. Version 0.91
is the current beta test release. There are no guarantees.


Beta version .90 did not work on the ROM 3 IIgs. Version .91 should work on
both ROM-01 and ROM 3 machines. The new version adds a few features:

o- It takes care of turning ON access to Slot 2 firmware. (Users no longer
need to go to the Control Panel and set Slot 2 to "Modem Port".)

o- Version .91 lets the user set baudrate (speed) from within the program.


ADTgs is a IIgs-only program.

ADT is by Paul Guertin. This mod for IIgs is by Jeff Hurlburt (1999).

ADTgs is freeware.


Contents of ADTgs91.ZIP archive:

    readme.txt      -- Information and directions
    adtgs.dmp       -- Object code for ADT (Monitor dump format).
    adt.exe         -- MS-DOS executable.


On the PC, use WinZIP to unzip ADTgs91.ZIP




Installation


What you need

1- You will need a null-modem cable (e.g. a IIgs high-speed 
   modem cable and NULL modem adaptor) to connect the IIgs 
   to a PC COM port (or to a modem cable coming from a 
   PC COM port).  Normally, you would connect to COM1 or COM2.

   Note: For more about NULL modem connections, see the Apple II FAQs 
   file Csa2T1TCOM at http://home.swbell.net/rubywand/Csa2T1TCOM.html
   or one of the other FAQs sites.

2- A bootable DOS 3.3 (or Prontodos or ESDOS) diskette with
   at least 12 free sectors.



Transferring ADTgs to your IIgs

     Boot a DOS 3.3 diskette.
 
     On the IIgs, Slot 2 must be set to "Modem Port" and the modem cable
must be plugged into the Modem Port (the serial port near the right side
as you face the front of the IIgs).

     Go to the IIgs Control Panel and check that Slot 2 is set to "Modem Port".
Set Modem Port speed to 300 baud. All of the other settings can be at their
default (checked) settings.



     On the PC, Delfs recommends using the MS-DOS "mode" and "type" commands
to send adtgs.dmp:

PC- Under Windows95, 98, etc., open a DOS window. (Or, you can restart in DOS
mode or just boot DOS.)  Change Dir to get to the directory containing adtgs.dmp.

Enter the following:

mode comX: baud=300 data=8 parity=n

In the above, replace "X" with the com port# (1 or 2) for the com port connected
to the Apple IIgs. For example, you would enter ...

mode com2: baud=300 data=8 parity=n

if the connection is to COM2.

Note: The PC should respond by echoing an abbreviated version of your mode
command on the screen. If you get an error message, it could mean that the
COM port you want to use is committed to some background task. For example,
if you have started a telecom program under Windows and it uses COM2, then,
the mode command will probably not work for COM2 until you shut down the 
telecom program.



IIgs- From the usual Applesoft prompt ("]") enter this command:

IN#2


PC- Enter the following to start the transfer:

type adtgs.dmp >comX:

Again, in the above, replace "X" with the com port# (1 or 2) you are using.


     On your IIgs screen, you should see code being entered. This should go
on for 4-5 minutes and end with the file named "ADT" being saved on the
DOS 3.3 diskette.
 

     Delfs comments that the above method may be limited to AT class PC's.
If it does not seem to work on your PC, you may have to use a PC telecom 
utility to send ADTgs.

If all goes well, skip ahead to "How to Use ADTgs".

If it looks like the problem is with using the MS-DOS mode and type commands,
see "Alternative PC Setup" for another way to send adtgs.dmp.

If you have some other problem, see the "Troubleshooting" section.


------------------------------


Alternative PC Setup

     On the PC, start a comm program, set it for 300 baud N81, and
ASCII-upload (not Xmodem or any other protocol but a straight text
upload). Using HyperTerm, you would click on "Transfer" and do a 
"Send Text File". (See the example below.)

Example: For Hyperterm, go to the File menu and set up a New Connection
profile. It should be "Direct to COM 1" (or whatever COM port you use.
Configure for 8 bits, No Parity, 1 Stop bit, 300 baud. Set Flow Control
to "None". Other settings- Emulation can be TTY and, in ASCII Setup, 
nothing should be checked in the "ASCII Sending" area. I have just "Wrap
lines that exceed terminal width" checked in ASCII Receiving. Set 
Line Delay to about 400 ms.

     Once you telecom program is set up, you can boot a DOS 3.3 diskette
on the IIgs and set up things as described earlier. From the Applesoft
prompt enter IN#2.

     On the PC comm program, select a Text or ASCII transfer (e.g. like 
Send Text File if using HyperTerm) and pick the file adtgs.dmp to send.

     On your IIgs screen, you should see code being entered. The process
should end with ADT being BSAVEd on your DOS 3.3 diskette.

------------------------------


Troubleshooting


In case there are problems, here are some trouble shooting tips:

1. If you get a beep at every line, the PC comm program is
   probably sending CRLFs to your Apple, which only likes
   CRs. Turn on the "Strip linefeeds on output" option in
   your comm program.

2. If you see total garbage on the Apple II screen, first be
   sure that the IIgs and the PC comm program agree on the
   transfer speed. If they do, your comm program is probably
   doing something funny with the high bit (aka bit 7) of
   each character. Turn off the "Strip high bit on output"
   option.

3. If you are uploading ADT to an unaccelerated Apple IIgs,
   you may need to slow down the transfer. Your comm program
   probably allows you to set a fixed delay between each
   line (aka "line delay"). Increasing the delay to 500ms or
   more may help.

4. If the transfer seemed to go okay but you suspect that some
   parts were corrupted, you can check your copy of ADTgs using
   ADTCHECK. It's a short program you can enter in a few minutes.
   Boot DOS 3.3 and get to the usual Applesoft BASIC prompt.
   Enter NEW and then type in the program. 
   

 10  TEXT : HOME
 20  PRINT  CHR$ (4)"BLOAD ADT,A$4000"
 40  FOR I = 0 TO 2579
 50 Z = Z +  PEEK (16384 + I)
 55  IF I / 500 =  INT (I / 500) THEN  PRINT "CURRENT SUM= ";Z
 60  NEXT I
 70  PRINT "CHECKSUM= ";Z


   Save the program on the same disk as ADT (SAVE ADTCHECK). Now, 
   RUN ADTCHECK and check the numbers you get against the checksums
   listed below. (The program displays intermediate sums and 
   a final sum.)


   Correct sums for ADTgs v.91

       76
       48588
       97886
       151354
       201728
       284376
       290904 (final sum)



5. Naturally, you can always post a question to comp.sys.apple2.


------------------------------





How to use ADTgs


     Immediately after first transferring ADTgs, you can press CTRL-Reset
to get to the Applesoft prompt. It's a good idea to back up ADT on another
diskette.

     ADT requires that target diskettes be formatted. So, before starting,
INIT enough blank 5.25" diskettes to handle your transfers.

     To start a session, on the Apple IIgs boot the DOS 3.3 diskette on
which ADT is located.

     Most likely, there will be no need to go to the IIgs Control Panel.
With version .91, ADTgs takes care of setting Slot 2 as required and Speed
is now setable within the program. If your Control Panel "Modem Port" 
settings are at the default (checkmarked) positions, you are ready to go.

     To start ADTgs you get to the Applesoft prompt and enter BRUN ADT . 
Once the program starts you can select target Drive and set Speed:

Drive- Pressing "V" toggles between Drive 1 and Drive 2. The default target
Drive is Drive 2. (If you use Drive 1, be careful not to overwrite your
ADT diskette!)

Speed- Pressing "S" steps through the Speed settings. The default Speed is 
9600 baud. This works well on both accelerated and non-accelerated IIgs's. 
The 19,200 setting seems to be unstable and is not recommended. (You can 
try 19,200 if you like and decide for yourself.)


     On the PC, you should move the .dsk files you want to transfer to 
the main ADT folder (where adt.exe is located). To run the Win32 version
of ADT run adt.exe. To run the MS-DOS version, run adtdos.exe.

Note: To run ADT under MS-DOS, you _must_ be in DOS, not just a
DOS window. 

     
Running under MS-DOS (adtdos.exe)

     Go to the PC directory which has ADT and enter 'adtdos' to start the 
PC program. It will ask you which serial port to use and the transfer speed.
You can also type these parameters on the MS-DOS command line, 
eg. 'ADT 2 9600'. The PC speed has to match the speed set on the IIgs.


Running under Windows (adt.exe)

     Go to the PC directory which has ADT and double-click on adt.dos. Make
sure the PC speed setting matches the speed set on the IIgs. ...


     Mainly, ADT operation is controlled from the Apple IIgs keyboard. So, 
you can transfer many disks without having to go back and forth between the
two computers.

     When you BRUN ADT on the IIgs, you will see the following menu at the 
bottom of the IIgs screen: 

     RECEIVE DIR SPEED DRIVE QUIT ABOUT

     To select an item, type the highlighted letter of the word. One
keystroke is enough, you don't have to hit Return.


[R] Receive: ADT will ask for a filename. It will then receive that
file from the PC's current directory and write it to the disk in the
target Drive, overwriting any data that may be on the disk. Before
transmitting, ADT ensures that the file exists and is 140k in length.

[D] Dir: ADT will display a list of the files in the current MS-DOS
directory. There is currently no way to change directories from within
ADT.

[S] Speed: As mentioned earlier, this steps through available Speed 
settings. At present, these range from 300 baud through 19,200 baud.

[V] Drive: As mentioned earlier, this sets the target Drive.

[Q] Quit: This quits ADTgs, restores the original Slot 2 setting, and
returns you to the Applesoft BASIC prompt and DOS 3.3.

[A] About: This shows "About ADTgs" message.

During a transfer, you can press ESCape to break off the transfer and
get back to the menu.

If, for some reason, the system hangs, you can press Control-Reset on the
IIgs to exit and get to the Applesoft prompt.



On the PC (under MS-DOS), you can type the following keys:

    C allows you to change the serial port and speed. (If you
    change the speed on one computer, don't forget to make sure
    that the other computer's speed matches.)

    ctrl-L redraws the screen.

    ESC breaks off a transfer

    Q quits ADT.


Meaning of characters on the Apple II status display:
    R: Reading from disk
    W: Writing to disk
    I: Receiving data from PC ("Input")
    O: Sending data to PC ("Output")
    .: Sector read or written correctly
    *: Error while reading or writing sector



Rubywand, 1999, 2000, ...
rubywand@swbell.net


