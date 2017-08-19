Apple Disk Transfer
===================

ADT is the "Apple Disk Transfer" utility. Paul Guertin originally developed
ADT as a way to transfer 5.25" disk image files from PC directly to diskette
on a 48k or larger Apple II and to send 5.25" diskettes to PC where they are
saved as .dsk disk image files. A typical ADT setup is an Apple II/IIe/IIc/IIgs
with a serial card or built-in serial port connected via a NULL modem cable to
a PC's COM port or a classic Mac's modem port with each computer running ADT. 

Download
========
Download ADT releases [here.](https://github.com/david-schmidt/adt/releases)

News
====
**19 August 2017**: ADT 2.41 is released.  This marks a more concerted effort
to be hosted on github and incorporate a couple of creature comforts: leave 
the transfer screen up at the end of a transfer in order to let the user
review the results, and change the defualt transfer speed to 115.2k, which 
has been the standard for ADTPro client and server for years.

**8 July 2007**: ADT 2007_2 is released.  ADT now can operate with
Laser 128 compatible computers.  MacADT has also been added to the 
distribution.

**3 June 2007**: ADT 2007 is released.  ADT now automatically scans for
and detects appropriate serial hardware.  Bootstrapping text files are
included with different line endings (PC, Unix, Mac) though ADTPro's
[bootstrapping instructions](http://adtpro.sourceforge.net/bootstrap.html#Bootstrapping%20DOS)
are simpler and can even use the cassette ports.

**27 Dec 2006**: ADT 2006 is released.  Some goodies from the 
[ADTPro project](http://adtpro.com/) have been incorporated
into ADT.  See the
[releases](https://github.com/david-schmidt/adt/releases)
page for details and downloads.

**31 Dec 2005**: ADT now has a new home at
[BerliOS](http://developer.berlios.de/). BerliOS provides web hosting, a
download service, and a Subversion source code repository. See the [project
page](http://developer.berlios.de/projects/adt/) for all the goodies.

Setting Up and Using ADT
========================

Download [ADT\_2007\_2.zip](https://github.com/david-schmidt/adt/releases/download/v2.4/ADT_2007_2.zip) and
unzip the archive. The result is a folder named "ADT\_2007\_2". This includes
server programs for Windows, MS-DOS and MacOS, as well as two popular 
Apple II-side programs.

Host Side
-------

* `adt.exe`: A Windows32 version of the PC server program by Sean Gugler.

* `adtdos.exe`: An MS-DOS version of the PC server program by Paul Guertin.

* `MacADT.sit`: Stuffit archive of the Mac server program by Hideki Naito.

Apple II Side
-------------

* `ADT`: This is the original ADT (now at version 2.41). It is intended for
  use on an just about any Apple II or compatible.

You will need to transfer the Apple II-side ADT program to the Apple
II.

The ADTPro project has 
[bootstrapping instructions](http://adtpro.com/bootstrap.html#Bootstrapping%20DOS)
that simplify and speed up the bootstrapping process with new server
software, but it does require Java on the host computer to run. 
Otherwise, the included text documentation files discuss how to set up a
NULL modem connection, do the transfer, and more.
Also see the [comp.sys.apple2](news:comp.sys.apple2) Apple II newsgroup and feel
free to ask questions.

This ADT distribution allows running ADT under Windows or from the MS-DOS
prompt. To start the Win32 version, double-click `adt.exe` or run it from a
Command Prompt.

---