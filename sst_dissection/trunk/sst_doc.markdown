SST Technical Documentation
===========================

Saltine's Super Transcopy (SST) is a hacked version of the Essential Data
Duplicator (EDD) bit copy program. The intent is to take the nibble image of a
copy protected disk, and convert it to 2 standard DOS 3.3 disks. These disks
can be transfered and copied by standard DOS 3.3 tools (ADT or COPYA). The
original protected disk can then be recreated also using SST. While SST makes
it possible to transfer some copy protected to a PC or Mac and use them under
an emulator, the process is a bit involved. Ideally, SST would be able to
transfer the image right to the PC, without using two intermediate disks.
Unfortunately, the source code to EDD has been lost, and none of the
developers are available for technical consultation. The SST code is
available, but comments are sparse and, again, the developer is unavailable
for technical consultation. Thus, I decided to reverse engineer SST and EDD to
see if it was possible to modify SST to work in this manner. This document
describes how SST works, with the hopes that this information will allow for
modification of SST, either by me or someone else.

The SST source code uses upper case letters for its symbol names. I use lower
case letters for symbols I've found in EDD. This is mainly because the cross
disassembler I am using uses lower case letters. Besides, upper case was only
used back in the Apple ][ days because many programs only worked with upper
case letters. Times have changed, and using lower case seems to be the norm
for assembly code.

SST Loading
===========

Let's first look at how SST is loaded from disk. The binary file has a start
address of $0B00 and a length of $5500, thus ending at $6000. This memory map
is detailed in this figure:

![SST Initial Memory Map](images/sst.png "SST Initial Memory Map")

The code at $0B00 relocates part of the disk image, and then jumps to $0C00.
The relocatable code is in two sections, $4000-$4FFF and $5000-$5FFF. The
first section, is moved to $B000-$BFFF. I believe this is standard EDD
operation, and contains (mostly) standard EDD code. I say mostly, because a
small part of it was modified for SST, but more on that latter. The second
relocatable section is moved to $D700-$E6FF. I believe this second section was
added for SST, and was not part of EDD. I've named this section SST Overlays,
since it will be overlaid on existing parts of EDD, as described later.

It is worth noting that the destination for the second section, $D700, is in
the area called bank-switched RAM. The address space is shared with the
AppleSoft ROMs. Since bank-switched RAM starts at $D000, I am unclear as to
why it is relocated to $D700. I think EDD may use $D000-$D6FF of bank-switched
RAM. I am also unclear as to why the relocation code does not first ensure
that the bank-switched RAM is writable. I am guessing that the power-on state
of the bank-switched RAM is writable, and hence the relocation code just
relies on this fact. Once these sections are relocated, the code path jumps to
$0C00. The rest of the memory between the relocation code and $0BFF is unused.

SST Operation
=============

The $0C00 is the main entry point for SST. For the most part, this is
unmodified EDD code. SST modified certain text messages, namely the banner on
the first line, and the text for menu option number 1. The memory map during
this main operation is detailed in this figure:

![EDD Memory Map](images/edd.png "EDD Memory Map")

SST hijacks EDD operation at a few key points. The first, is main menu option
number 1. When this option is chosen, execution jumps to $B500. I am unclear
if this region of code was perviously unused by EDD, or if SST just sacrificed
some less used parts of EDD. In any case, the code at $B500 is SST code, not
EDD code. It uses a routine called `MOVE` which enables reading from
bank-switched RAM (BSR), swaps the memory region $E000 through $F500 to $1D00
through $3200, and then disables reading of the BSR. This essentially overlays
SST on part of the EDD code. This EDD code is essentially disabled until
`MOVE` is called again. I assume this EDD code region was chosen for a reason,
but I really cannot say for sure. Back to menu option 1. After jumping to
$B500 and swapping memory regions, the SST code that gets called for menu
option number 1 is a routine called `CHOOSE`. It presents the Copy, Pack, and
Unpack menu options. If Pack or Unpack is chosen, then a flag, simply called
`FLAG` at $B56F, is set that is later check in the hijacked read or write
routine. `FLAG` is set to $01 for Pack and $02 for unpack. After a choice is
made, the EDD code moved to BSR code is swapped back into main memory.
Execution then continues at $1929, a routine which I call `edd_copy`. The
`CHOOSE` menu was added by SST. My guess is that prior to SST, EDD called
`edd_copy` for main menu option number 1 directly.

`edd_copy` is the main disk copy routine. It presents the user with the copy
options (start track, end track, step, etc.). Then, it loops through all the
tracks to copy, alternately reading, analyzing, and writing each track in
turn. SST not only hijacks main menu option number 1, but it also hijacks the
track read and write routines. The hijacked write routine is named,
descriptively `W` is at $B51D. It calls SST's read routine, named `MYWRITE` if
`FLAG` is $01, i.e. if Pack was choosen. Otherwise `W` calls the standard EDD
write routine, named `WRITE` at $1A04. The hijacked read routine is named, not
surprisingly, `R` is at $B517. It calls SST's read routine if `FLAG` is $02,
i.e. if Unpack was choosen Otherwise `R` calls the standard EDD read routine,
named `READ` at $19C0.

SST's read and write routines access standard DOS 3.3 formatted floppies. Thus
it uses the standard DOS 3.3 RWTS routine to read and write DOS 3.3 sectors.
The only problem is that DOS 3.3 typically occupies $B700-$BFFF, and was
trashed during the original relocation code. SST solves this by again using
spare memory in BSR and swapping it in. This time, it uses a routine named
`XCHANGE` which swaps $D700-$DFFF with $B700-$BFFF. This means that the memory
originally relocated up to $D700 is actually a full copy of DOS 3.3. Thus, the
memory map of the SST overlay region from $D700-$E700 is details in this
figure:

![SST Overlay](images/sst_overlay.png "SST Overlay")

Thus, both `R` and `W` do some fancy memory swapping. First they swap in SST
code using `MOVE`. If the standard EDD read or write routines are called, SST
calls `MOVE` again to swap EDD back into main memory. If the SST read or write
routines are used, `XCHANGE` is called to bring DOS 3.3 into memory. Once
SST's read or write routine completes, DOS 3.3 is swapped out, and the EDD is
also swapped out, thus bringing EDD completely back into main memory. It's
quite convoluted, but it actually makes sense when trying to follow the code.

So now we know that how SST hijacks the track read and write routines for
unpacking and packing. Now let's look at the pack operation in detail. First,
the normal EDD read and analyze routines are called. EDD uses a $1C00 byte
buffer from $7800-$93FF for the raw nibble data. EDD also uses a parallel
$1C00 byte buffer from $9400-$AFFF for sync byte information. The first thing
SST's write routine does is pack these two buffers into a single $1C00 byte
buffer. It does this by marking sync bytes in the main nibble byte buffer with
the high bit cleared. I'm not sure why EDD needs two parallel buffers to keep
track of sync information. Perhaps there is other information encoded in the
$9400 buffer that I can't decipher. But if it is important, it just gets
thrown out by SST during the pack phase. Once the nibble data is packed, it is
finally written out to disk. SST not only writes out this complete $1C00 byte
buffer at $7800, but it also writes out the EDD parameter information at
address $B100-$B3FF. This means a total of $1F00 bytes need to be written to
disk. Conveniently, this is 31 sectors, thus, each nibble track takes up 2 DOS
3.3 tracks. The order in which the nibble data is written is described in this
figure:

![SST Disk Structure](images/sst_disk.png "SST Disk Structure")

Thus track 0's nibble data is stored on tracks 0 and 1 of the packed disk.
Track 2's nibble data is stored on tracks 2 and 3 of the packed disk, and so
on.  The figure also lists some key address in the EDD analyze results area.  These addresses can be used to reconstruct the original track nibble data.  Using the information from this last figure, it is possible to unpack SST disks into a .NIB image without actually using SST.