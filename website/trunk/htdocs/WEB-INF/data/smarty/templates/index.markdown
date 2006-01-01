Apple Disk Transfer
===================

ADT is the "Apple Disk Transfer" utility. Paul Guertin originally developed
ADT as a way to transfer 5.25" disk image files from PC directly to diskette
on a 64k or larger Apple II and to send 5.25" diskettes to PC where they are
saved as .dsk disk image files. A typical ADT setup is an Apple II with a
serial card or built-in serial port connected via a NULL modem cableto a PC's
COM port with each computer running ADT.

News
====

**31 Dec 2005**: ADT now has a new home at
[BerliOS](http://developer.berlios.de/). BerliOS provides web hosting, a
download service, and a Subversion source code repository. See the [project
page](http://developer.berlios.de/projects/adt/) for all the goodies.

Setting Up and Using ADT
========================

Download [ADT\_2005\_2.zip](http://download.berlios.de/adt/ADT_2005_2.zip) and
unzip the archive. The result is a folder named "ADT\_2005\_2". This includes
a Windows and MS-DOS program, as well as the three most popular Apple II-side
programs.

PC Side
-------

* `adt.exe`: A Windows32 version of the PC-side program by Sean Gugler.

* `adtdos.exe`: An MS-DOS version of the PC-side program by Paul Guertin.

Apple II Side
-------------

* `ADTssc`: This is the original ADT (now at version 1.23). It is intended for
  use on an Apple II equipped with Apple's Super Serial Card (SSC) or a
  compatible card.
  
* `ADTcc`: This is an adaptation of ADT (now at version 1.21) intended 
  for use on an Apple II equipped with the Apple Communications Card 
  and other, generally older, non-SSC cards.
  
* `ADTgs`:   This is an adaptation of ADT intended only for transferring 
  disks from PC to an Apple IIgs. It uses the the built-in IIgs modem 
  serial port and firmware. (Version beta .91)

You will need to transfer one of the Apple II-side ADT programs to the Apple
II. How to do the transfer, setting up a NULL modem connection, and more is
discussed in the included Text documentation files. Also see the
[comp.sys.apple2](news:comp.sys.apple2) Apple II FAQs
[Telecom-1](http://home.swbell.net/rubywand/Csa2T1TCOM.html) page and feel
free to ask questions on the newsgroup.

The ADT\_2005 distribution allows running ADT under Windows or from the MS-DOS
prompt. To start the Win32 version, double-click `adt.exe` or run it from a
Command Prompt.

---

<a href="http://developer.berlios.de" title="BerliOS Developer">
    <img src="http://developer.berlios.de/bslogo.php?group_id=5617"
        width="124px" height="32px" border="0" alt="BerliOS Developer Logo" />
</a>
