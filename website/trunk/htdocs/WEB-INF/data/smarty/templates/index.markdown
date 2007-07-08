Apple Disk Transfer
===================

ADT is the "Apple Disk Transfer" utility. Paul Guertin originally developed
ADT as a way to transfer 5.25" disk image files from PC directly to diskette
on a 64k or larger Apple II and to send 5.25" diskettes to PC where they are
saved as .dsk disk image files. A typical ADT setup is an Apple II/IIe/IIc/IIgs
with a serial card or built-in serial port connected via a NULL modem cable to
a PC's COM port or a classic Mac's modem port with each computer running ADT.

News
====
**8 July 2007**: ADT 2007_2 is released.  ADT now can operate with
Laser 128 compatible computers.  MacADT has also been added to the 
distribution.

**3 June 2007**: ADT 2007 is released.  ADT now automatically scans for
and detects appropriate serial hardware.  Bootstrapping text files are
included with different line endings (PC, Unix, Mac) though ADTPro's
[bootstrapping instructions](http://adtpro.sourceforge.net/bootstrap.html#Bootstrapping%20DOS)
are simpler and can even use the cassette ports.

**27 Dec 2006**: ADT 2006 is released.  Some goodies from the 
[ADTPro project](http://adtpro.sourceforge.net/) have been incorporated
into ADT.  See the
[download](http://developer.berlios.de/project/showfiles.php?group_id=5617)
page for details and downloads.

**31 Dec 2005**: ADT now has a new home at
[BerliOS](http://developer.berlios.de/). BerliOS provides web hosting, a
download service, and a Subversion source code repository. See the [project
page](http://developer.berlios.de/projects/adt/) for all the goodies.

Setting Up and Using ADT
========================

Download [ADT\_2007\_2.zip](http://download.berlios.de/adt/ADT_2007_2.zip) and
unzip the archive. The result is a folder named "ADT\_2007\_2". This includes
server programs for Windows, MS-DOS and MacOS, as well as two popular 
Apple II-side programs.

Host Side
-------

* `adt.exe`: A Windows32 version of the PC-side program by Sean Gugler.

* `adtdos.exe`: An MS-DOS version of the PC-side program by Paul Guertin.

* `MacADT.sit`: Stuffit archive of the host program by Hideki Naito.

Apple II Side
-------------

* `ADT`: This is the original ADT (now at version 1.33). It is intended for
  use on an just about any Apple II or compatible.

* `ADTcc`: This is an adaptation of ADT (now at version 1.21) intended 
  for use on an Apple II equipped with the Apple Communications Card 
  and other, generally older, non-SSC cards.

You will need to transfer one of the Apple II-side ADT programs to the Apple
II.

The ADTPro project has 
[bootstrapping instructions](http://adtpro.sourceforge.net/bootstrap.html#Bootstrapping%20DOS)
that simplify and speed up the bootstrapping process with new server
software, but it does require Java on the host computer to run. 
Otherwise, the included Text documentation files discuss how to set up a
NULL modem connection, do the transfer, and more.
Also see the [comp.sys.apple2](news:comp.sys.apple2) Apple II FAQs
[Telecom-1](http://home.swbell.net/rubywand/Csa2T1TCOM.html) page and feel
free to ask questions on the newsgroup.

This ADT distribution allows running ADT under Windows or from the MS-DOS
prompt. To start the Win32 version, double-click `adt.exe` or run it from a
Command Prompt.

---

<a href="http://developer.berlios.de" title="BerliOS Developer">
    <img src="http://developer.berlios.de/bslogo.php?group_id=5617"
        width="124px" height="32px" border="0" alt="BerliOS Developer Logo" />
</a>
