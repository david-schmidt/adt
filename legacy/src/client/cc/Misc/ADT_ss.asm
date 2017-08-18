S-C Assember source file:
1000          .LIST OFF
1005 *---------------------------
1010 * APPLE DISK TRANSFER 1.21
1020 * BY PAUL GUERTIN
1030 * GUERTINP@IRO.UMONTREAL.CA
1040 * OCTOBER 13TH, 1994
1050 * DISTRIBUTE FREELY
1060 *---------------------------
1070 *
1090 * THIS PROGRAM TRANSFERS A 16-SECTOR DISK
1100 * TO A 140K MS-DOS FILE AND BACK. THE FILE
1110 * FORMAT IS COMPATIBLE WITH RANDY SPURLOCK'S
1120 * APL2EM EMULATOR.
1140 * VERSION HISTORY:
1160 * VERSION 1.21 FILLS UNREADABLE SECTORS WITH ZEROS
1180 * VERSION 1.20
1190 * - HAS A CONFIGURATION MENU
1200 * - HAS A DIRECTORY FUNCTION
1210 * - ABORTS INSTANTLY IF USER PUSHES ESCAPE
1220 * - FIXES THE "256 RETRIES" BUG
1230 * - HAS MORE EFFICIENT CRC ROUTINES
1250 * VERSION 1.11 SETS IOBVOL TO 0 BEFORE CALLING RWTS
1270 * VERSION 1.10 ADDS THESE ENHANCEMENTS:
1280 * - DIFFERENTIAL RLE COMPRESSION TO SPEED UP TRANSFER
1290 * - 16-BIT CRC ERROR DETECTION
1300 * - AUTOMATIC RE-READS OF BAD SECTORS
1320 * VERSION 1.01 CORRECTS THE FOLLOWING BUGS:
1330 * - INITIALIZATION ROUTINE CRASHED WITH SOME CARDS
1340 * - FULL 8.3 MS-DOS FILENAMES NOW ACCEPTED
1360 * VERSION 1.00 - FIRST PUBLIC RELEASE
1390 * CONSTANTS
1410 ESC      .EQ $9B           ;ESCAPE KEY
1420 ACK      .EQ $06           ;ACKNOWLEDGE
1430 NAK      .EQ $15           ;NEGATIVE ACKNOWLEDGE
1440 PARMNUM  .EQ 8             ;NUMBER OF CONFIGURABLE PARMS
1460 * ZERO PAGE LOCATIONS (ALL UNUSED BY DOS, BASIC & MONITOR)
1480 MSGPTR   .EQ $6            ;POINTER TO MESSAGE TEXT (2B)
1490 SECPTR   .EQ $8            ;POINTER TO SECTOR DATA  (2B)
1500 TRKCNT   .EQ $1E           ;COUNTS SEVEN TRACKS     (1B)
1510 CRC      .EQ $EB           ;TRACK CRC-16            (2B)
1520 PREV     .EQ $ED           ;PREVIOUS BYTE FOR RLE   (1B)
1530 YSAVE    .EQ $EE           ;TEMP STORAGE            (1B)
1550 * BIG FILES
1570 TRACKS   .EQ $2000         ;7 TRACKS AT 2000-8FFF (28KB)
1580 CRCTBLL  .EQ $9000         ;CRC LOW TABLE         (256B)
1590 CRCTBLH  .EQ $9100         ;CRC HIGH TABLE        (256B)
1610 * MONITOR STUFF
1630 CH       .EQ $24           ;CURSOR HORIZONTAL POSITION
1640 CV       .EQ $25           ;CURSOR VERTICAL POSITION
1650 BASL     .EQ $28           ;BASE LINE ADDRESS
1660 INVFLG   .EQ $32           ;INVERSE FLAG
1670 CLREOL   .EQ $FC9C         ;CLEAR TO END OF LINE
1680 CLREOP   .EQ $FC42         ;CLEAR TO END OF SCREEN
1690 HOME     .EQ $FC58         ;CLEAR WHOLE SCREEN
1700 TABV     .EQ $FB5B         ;SET BASL FROM A
1710 VTAB     .EQ $FC22         ;SET BASL FROM CV
1720 RDKEY    .EQ $FD0C         ;CHARACTER INPUT
1730 NXTCHAR  .EQ $FD75         ;LINE INPUT
1740 COUT1    .EQ $FDF0         ;CHARACTER OUTPUT
1750 CROUT    .EQ $FD8E         ;OUTPUT RETURN
1770 * MESSAGES
1790 MTITLE   .EQ 0             ;TITLE SCREEN
1800 MCONFIG  .EQ 2             ;CONFIGURATION TOP OF SCREEN
1810 MCONFG2  .EQ 4             ;CONFIGURATION BOTTOM OF SCREEN
1820 MPROMPT  .EQ 6             ;MAIN PROMPT
1830 MDIRCON  .EQ 8             ;CONTINUED DIRECTORY PROMPT
1840 MDIREND  .EQ 10            ;END OF DIRECTORY PROMPT
1850 MFRECV   .EQ 12            ;FILE TO RECEIVE:_
1860 MFSEND   .EQ 14            ;FILE TO SEND:_
1870 MRECV    .EQ 16            ;RECEIVING FILE_    (_ = SPACE)
1880 MSEND    .EQ 18            ;SENDING FILE_
1890 MCONFUS  .EQ 20            ;NONSENSE FROM PC
1900 MNOT16   .EQ 22            ;NOT A 16 SECTOR DISK
1910 MERROR   .EQ 24            ;ERROR: FILE_
1920 MCANT    .EQ 26            ;|CAN'T BE OPENED.     (| = CR)
1930 MEXISTS  .EQ 28            ;|ALREADY EXISTS.
1940 MNOT140  .EQ 30            ;|IS NOT A 140K IMAGE.
1950 MFULL    .EQ 32            ;|DOESN'T FIT ON DISK.
1960 MANYKEY  .EQ 34            ;__ANY KEY:_
1970 MDONT    .EQ 36            ;<- DO NOT CHANGE
1980 MABOUT   .EQ 38            ;ABOUT ADT...
1990 MTEST    .EQ 40            ;TESTING DISK FORMAT
2000 MPCANS   .EQ 42            ;AWAITING ANSWER FROM PC
2020 **********************************************************
2040          .OR $803
2045          .TF ADT.SS
2060          JMP START         ;SKIP DEFAULT PARAMETERS
2080 DEFAULT  .DA #5,#0,#1,#5,#1,#0,#0,#0  ;DEFAULT PARM VALUES
2100 *---------------------------------------------------------
2110 * START - MAIN PROGRAM
2120 *---------------------------------------------------------
2130 START    CLD               ;BINARY MODE
2140          JSR $FE84         ;NORMAL TEXT
2150          JSR $FB2F         ;TEXT MODE, FULL WINDOW
2160          JSR $FE89         ;INPUT FROM KEYBOARD
2170          JSR $FE93         ;OUTPUT TO 40-COL SCREEN
2190          LDA #0
2200          STA SECPTR        ;SECPTR ALWAYS PAGE-ALIGNED
2220          STA STDDOS        ;ASSUME STANDARD DOS INITIALLY
2230          LDA $B92E         ;SAVE CONTENTS OF DOS
2240          STA DOSBYTE       ;CHECKSUM BYTES
2250          CMP #$13
2260          BEQ DOSOK1        ;AND DECREMENT STDDOS (MAKING
2270          DEC STDDOS        ;IT NONZERO) IF THE CORRECT
2280 DOSOK1   LDA $B98A         ;BYTES AREN'T THERE
2290          STA DOSBYTE+1
2300          CMP #$B7
2310          BEQ DOSOK2
2320          DEC STDDOS
2340 DOSOK2   JSR MAKETBL       ;MAKE CRC-16 TABLES
2350          JSR PARMDFT       ;RESET PARAMETERS TO DEFAULTS
2360          JSR PARMINT       ;INTERPRET PARAMETERS
2380 REDRAW   JSR TITLE         ;DRAW TITLE SCREEN
2400 MAINLUP  LDY #MPROMPT      ;SHOW MAIN PROMPT
2410          JSR SHOWMSG       ;AT BOTTOM OF SCREEN
2420          JSR RDKEY         ;GET ANSWER
2430          AND #$DF          ;CONVERT TO UPPERCASE
2440 MOD0     BIT $C088         ;CLEAR SSC INPUT REGISTER
2460          CMP #"S"          ;SEND?
2470          BNE KRECV         ;NOPE, TRY RECEIVE
2480          JSR SEND          ;YES, DO SEND ROUTINE
2490          JMP MAINLUP
2510 KRECV    CMP #"R"          ;RECEIVE?
2520          BNE KDIR          ;NOPE, TRY DIR
2530          JSR RECEIVE       ;YES, DO RECEIVE ROUTINE
2540          JMP MAINLUP
2560 KDIR     CMP #"D"          ;DIR?
2570          BNE KCONF         ;NOPE, TRY CONFIGURE
2580          JSR DIR           ;YES, DO DIR ROUTINE
2590          JMP REDRAW
2610 KCONF    CMP #"C"          ;CONFIGURE?
2620          BNE KABOUT        ;NOPE, TRY ABOUT
2630          JSR CONFIG        ;YES, DO CONFIGURE ROUTINE
2640          JSR PARMINT       ;AND INTERPRET PARAMETERS
2650          JMP REDRAW
2670 KABOUT   CMP #$9F          ;ABOUT MESSAGE? ("?" KEY)
2680          BNE KQUIT         ;NOPE, TRY QUIT
2690          LDY #MABOUT       ;YES, SHOW MESSAGE, WAIT
2700          JSR SHOWMSG       ;FOR KEY, AND RETURN
2710          JSR RDKEY
2720          JMP MAINLUP
2740 KQUIT    CMP #"Q"          ;QUIT?
2750          BNE MAINLUP       ;NOPE, WAS A BAD KEY
2760          LDA DOSBYTE       ;YES, RESTORE DOS CHECKSUM CODE
2770          STA $B92E
2780          LDA DOSBYTE+1
2790          STA $B98A
2800          JMP $3D0          ;AND QUIT TO DOS
2830 *---------------------------------------------------------
2840 * DIR - GET DIRECTORY FROM THE PC AND PRINT IT
2850 * PC SENDS 0,1 AFTER PAGES 1..N-1, 0,0 AFTER LAST PAGE
2860 *---------------------------------------------------------
2870 DIR      JSR HOME          ;CLEAR SCREEN
2880          LDA #"D"          ;SEND DIR COMMAND TO PC
2890          JSR PUTC
2910 DIRLOOP  JSR GETC          ;PRINT PC OUTPUT EXACTLY AS
2920          BEQ DIRSTOP       ;IT ARRIVES (PC IS RESPONSIBLE
2930          ORA #$80          ;FOR FORMATTING), UNTIL 00
2940          JSR COUT1         ;RECEIVED
2950          JMP DIRLOOP
2970 DIRSTOP  JSR GETC          ;GET CONTINUATION CHARACTER
2980          BNE DIRCONT       ;NOT 00, THERE'S MORE
3000          LDY #MDIREND      ;NO MORE FILES, WAIT FOR KEY
3010          JSR SHOWMSG       ;AND RETURN
3020          JSR RDKEY
3030          RTS
3050 DIRCONT  LDY #MDIRCON      ;SPACE TO CONTINUE, ESC TO STOP
3060          JSR SHOWMSG
3070          JSR RDKEY
3080          EOR #ESC          ;NOT ESCAPE, CONTINUE NORMALLY
3090          BNE DIR           ;BY SENDING A "D" TO PC
3100          JMP PUTC          ;ESCAPE, SEND 00 AND RETURN
3130 *---------------------------------------------------------
3140 * CONFIG - ADT CONFIGURATION
3150 *---------------------------------------------------------
3160 CONFIG   JSR HOME          ;CLEAR SCREEN
3170          LDY #MCONFIG      ;SHOW CONFIGURATION SCREEN
3180          JSR SHOWM1
3190          LDY #MCONFG2
3200          JSR SHOWMSG       ;IN 2 PARTS BECAUSE >256 CHARS
3220          LDY #PARMNUM-1    ;SAVE PREVIOUS PARAMETERS
3230 SAVPARM  LDA PARMS,Y       ;IN CASE OF ESCAPE
3240          STA OLDPARM,Y
3250          DEY
3260          BPL SAVPARM
3280 *--------------- FIRST PART: DISPLAY SCREEN --------------
3300 REFRESH  LDA #3            ;FIRST PARAMETER IS ON LINE 3
3310          JSR TABV
3320          LDX #0            ;PARAMETER NUMBER
3330          LDY #$FF          ;OFFSET INTO PARAMETER TEXT
3350 NXTLINE  STX LINECNT       ;SAVE CURRENT LINE
3360          LDA #15
3370          STA CH
3380          CLC
3390          LDA PARMSIZ,X     ;GET CURRENT VALUE (NEGATIVE:
3400          SBC PARMS,X       ;LAST VALUE HAS CURVAL=0)
3410          STA CURVAL
3420          LDA PARMSIZ,X     ;X WILL BE EACH POSSIBLE VALUE
3430          TAX               ;STARTING WITH THE LAST ONE
3440          DEX
3460 VALLOOP  CPX CURVAL        ;X EQUAL TO CURRENT VALUE?
3470          BEQ PRINTIT       ;YES, PRINT IT
3480 SKIPCHR  INY               ;NO, SKIP IT
3490          LDA PARMTXT,Y
3500          BNE SKIPCHR
3510          BEQ ENDVAL
3530 PRINTIT  LDA LINECNT       ;IF WE'RE ON THE ACTIVE LINE,
3540          CMP CURPARM       ;THEN PRINT VALUE IN INVERSE
3550          BNE PRTVAL        ;ELSE PRINT IT NORMALLY
3560          LDA #$3F
3570          STA INVFLG
3590 PRTVAL   LDA #$A0          ;SPACE BEFORE & AFTER VALUE
3600          JSR COUT1
3610 PRTLOOP  INY               ;PRINT VALUE
3620          LDA PARMTXT,Y
3630          BEQ ENDPRT
3640          JSR COUT1
3650          JMP PRTLOOP
3660 ENDPRT   LDA #$A0
3670          JSR COUT1
3680          LDA #$FF          ;BACK TO NORMAL
3690          STA INVFLG
3700 ENDVAL   DEX
3710          BPL VALLOOP       ;PRINT REMAINING VALUES
3730          STY YSAVE         ;CLREOL USES Y
3740          JSR CLREOL        ;REMOVE GARBAGE AT EOL
3750          JSR CROUT
3760          LDY YSAVE
3770          LDX LINECNT       ;INCREMENT CURRENT LINE
3780          INX
3790          CPX #PARMNUM
3800          BCC NXTLINE       ;LOOP 8 TIMES
3820          LDA STDDOS        ;IF NON-STANDARD DOS, WRITE
3830          BEQ GETCMD        ;"DO NOT CHANGE" ON SCREEN
3840          LDA #9            ;NEXT TO THE CHECKSUMS OPTION
3850          JSR TABV
3860          LDY #23
3870          STY CH
3880          LDY #MDONT
3890          JSR SHOWM1
3910 *--------------- SECOND PART: CHANGE VALUES --------------
3930 GETCMD   LDA $C000         ;WAIT FOR NEXT COMMAND
3940          BPL GETCMD
3950          BIT $C010
3960          LDX CURPARM       ;CURRENT PARAMETER IN X
3980          CMP #$88
3990          BNE NOTLEFT
4000          DEC PARMS,X       ;LEFT ARROW PUSHED
4010          BPL LEFTOK        ;DECREMENT CURRENT VALUE
4020          LDA PARMSIZ,X
4030          SBC #1
4040          STA PARMS,X
4050 LEFTOK   JMP REFRESH
4070 NOTLEFT  CMP #$95
4080          BNE NOTRGT
4090          LDA PARMS,X       ;RIGHT ARROW PUSHED
4100          ADC #0            ;INCREMENT CURRENT VALUE
4110          CMP PARMSIZ,X
4120          BCC RIGHTOK
4130          LDA #0
4140 RIGHTOK  STA PARMS,X
4150          JMP REFRESH
4170 NOTRGT   CMP #$8B
4180          BNE NOTUP
4190          DEX               ;UP ARROW PUSHED
4200          BPL UPOK          ;DECREMENT PARAMETER
4210          LDX #PARMNUM-1
4220 UPOK     STX CURPARM
4230          JMP REFRESH
4250 NOTUP    CMP #$8A
4260          BEQ ISDOWN
4270          CMP #$A0
4280          BNE NOTDOWN
4290 ISDOWN   INX               ;DOWN ARROW OR SPACE PUSHED
4300          CPX #PARMNUM      ;INCREMENT PARAMETER
4310          BCC DOWNOK
4320          LDX #0
4330 DOWNOK   STX CURPARM
4340          JMP REFRESH
4360 NOTDOWN  CMP #$84
4370          BNE NOTCTLD
4380          JSR PARMDFT       ;CTRL-D PUSHED, RESTORE DEFAULT
4390 NOTESC   JMP REFRESH       ;PARAMETERS
4410 NOTCTLD  CMP #$8D
4420          BEQ ENDCFG        ;RETURN PUSHED, STOP CONFIGURE
4440          CMP #ESC
4450          BNE NOTESC
4460          LDY #PARMNUM-1    ;ESCAPE PUSHED, RESTORE OLD
4470 PARMRST  LDA OLDPARM,Y     ;PARAMETERS AND STOP CONFIGURE
4480          STA PARMS,Y
4490          DEY
4500          BPL PARMRST
4510 ENDCFG   RTS
4530 LINECNT  .HS 00            ;CURRENT LINE NUMBER
4540 CURPARM  .HS 00            ;ACTIVE PARAMETER
4550 CURVAL   .HS 00            ;VALUE OF ACTIVE PARAMETER
4560 OLDPARM  .BS PARMNUM       ;OLD PARAMETERS SAVED HERE
4590 *---------------------------------------------------------
4600 * PARMINT - INTERPRET PARAMETERS
4610 *---------------------------------------------------------
4620 PARMINT  LDY PDSLOT        ;GET SLOT# (0..6)
4630          INY               ;NOW 1..7
4640          TYA
4650          ORA #"0"          ;CONVERT TO ASCII AND PUT
4660          STA MTSLT         ;INTO TITLE SCREEN
4670          TYA
4680          ASL
4690          ASL
4700          ASL
4710          ASL               ;NOW $S0
4720          STA IOBSLT        ;STORE IN IOB
4730          ADC #$89          ;NOW $89+S0
4740          STA MOD5+1        ;SELF-MOD FOR "DRIVES ON"
4760          LDY PDRIVE        ;GET DRIVE# (0..1)
4770          INY               ;NOW 1..2
4780          STY IOBDRV        ;STORE IN IOB
4790          TYA
4800          ORA #"0"          ;CONVERT TO ASCII AND PUT
4810          STA MTDRV         ;INTO TITLE SCREEN
4830          LDY PSSC          ;GET SSC SLOT# (0..6)
4840          INY               ;NOW 1..7
4850          TYA
4860          ORA #"0"          ;CONVERT TO ASCII AND PUT
4870          STA MTSSC         ;INTO TITLE SCREEN
4880          TYA
4890          ASL
4900          ASL
4910          ASL
4920          ASL               ;NOW $S0
4930          ADC #$88
4940          TAX
4950          LDA #$0B          ;COMMAND: NO PARITY, RTS ON,
4960          STA $C002,X       ;DTR ON, NO INTERRUPTS
4970          LDY PSPEED        ;CONTROL: 8 DATA BITS, 1 STOP
4980          LDA BPSCTRL,Y     ;BIT, BAUD RATE DEPENDS ON
4990          STA $C003,X       ;PSPEED
5000          STX MOD0+1        ;SELF-MODS FOR $C088+S0
5010          STX MOD2+1        ;IN MAIN LOOP
5020          STX MOD4+1        ;AND IN GETC AND PUTC
5030          INX
5040          STX MOD1+1        ;SELF-MODS FOR $C089+S0
5050          STX MOD3+1        ;IN GETC AND PUTC
5070          TYA               ;GET SPEED (0..5)
5080          ASL
5090          ASL
5100          ADC PSPEED        ;5*SPEED IN Y, NOW COPY
5110          TAY               ;FIVE CHARACTERS INTO
5120          LDX #4            ;TITLE SCREEN
5130 PUTSPD   LDA SPDTXT,Y
5140          STA MTSPD,X
5150          INY
5160          DEX
5170          BPL PUTSPD
5190          LDY #1            ;CONVERT RETRIES FROM 0..7
5200 TRYLUP   LDX PRETRY,Y      ;TO 0..5,10,128
5210          LDA TRYTBL,X
5220          STA REALTRY,Y
5230          DEY
5240          BPL TRYLUP
5260          LDX #0            ;IF PCKSUM IS 'NO', WE PATCH
5270          LDY #0            ;DOS TO IGNORE ADDRESS AND
5280          LDA PCKSUM        ;DATA CHECKSUM ERRORS
5290          BNE RWTSMOD
5300          LDX DOSBYTE+1
5310          LDY DOSBYTE
5320 RWTSMOD  STX $B98A         ;IS THERE AN APPLE II TODAY
5330          STY $B92E         ;THAT DOESN'T HAVE >=48K RAM?
5340          RTS               ;(YES)
5360 SPDTXT   .AS -"  003 0021 0042 0084 006900291"
5370 BPSCTRL  .DA #$16,#$18,#$1A,#$1C,#$1E,#$1F
5380 TRYTBL   .DA #0,#1,#2,#3,#4,#5,#10,#99
5410 *---------------------------------------------------------
5420 * GETNAME - GET FILENAME AND SEND TO PC
5430 *---------------------------------------------------------
5440 GETNAME  STX DIRECTN       ;TFR DIRECTION (0=RECV, 1=SEND)
5450          LDY PRMPTBL,X
5460          JSR SHOWMSG       ;ASK FILENAME
5470          LDX #0            ;GET ANSWER AT $200
5480          JSR NXTCHAR
5490          LDA #0            ;NULL-TERMINATE IT
5500          STA $200,X
5510          TXA
5520          BNE FNAMEOK
5530          JMP ABORT         ;ABORT IF NO FILENAME
5550 FNAMEOK  LDY #MTEST        ;"TESTING THE DISK"
5560          JSR SHOWMSG
5570          LDA #TRACKS/$100  ;READ TRACK 1 SECTOR 1
5580          STA IOBBUF+1      ;TO SEE IF THERE'S A 16-SECTOR
5590          LDA #1            ;DISK IN THE DRIVE
5600          STA IOBCMD
5610          STA IOBTRK
5620          STA IOBSEC
5630          LDA #IOB/$100
5640          LDY #IOB
5650          JSR $3D9
5660          BCC DISKOK        ;READ SUCCESSFUL
5680          LDY #MNOT16       ;NOT 16-SECTOR DISK
5690          JSR SHOWMSG
5700          LDY #MANYKEY      ;APPEND PROMPT
5710          JSR SHOWM1
5720          JSR AWBEEP
5730          JSR RDKEY         ;WAIT FOR KEY
5740          JMP ABORT         ;AND ABORT
5760 DISKOK   LDY #MPCANS       ;"AWAITING ANSWER FROM PC"
5770          JSR SHOWMSG
5780          LDA #"R"          ;LOAD ACC WITH "R" OR "S"
5790          ADC DIRECTN
5800          JSR PUTC          ;AND SEND TO PC
5810          LDX #0
5820 FNLOOP   LDA $200,X        ;SEND FILENAME TO PC
5830          JSR PUTC
5840          BEQ GETANS        ;STOP AT NULL
5850          INX
5860          BNE FNLOOP
5880 GETANS   JSR GETC          ;ANSWER FROM PC SHOULD BE 0
5890          BNE PCERROR       ;THERE'S A PROBLEM
5910          JSR TITLE         ;CLEAR STATUS
5920          LDX DIRECTN
5930          LDY TFRTBL,X
5940          JSR SHOWMSG       ;SHOW TRANSFER MESSAGE
5960 SHOWFN   LDA #2            ;AND ADD FILENAME
5970          STA MSGPTR+1
5980          LDA #0
5990          STA MSGPTR
6000          TAY
6010          JMP MSGLOOP       ;AND RETURN THROUGH SHOWMSG
6030 PCERROR  PHA               ;SAVE ERROR NUMBER
6040          LDY #MERROR       ;SHOW "ERROR: FILE "
6050          JSR SHOWMSG       ;SHOW FILENAME
6060          JSR SHOWFN
6070          PLA
6080          TAY
6090          JSR SHOWM1        ;SHOW ERROR MESSAGE
6100          LDY #MANYKEY      ;APPEND PROMPT
6110          JSR SHOWM1
6120          JSR AWBEEP
6130          JSR RDKEY         ;WAIT FOR KEY
6140          JMP ABORT         ;AND RESTART
6160 DIRECTN  .HS 00
6170 PRMPTBL  .DA #MFRECV,#MFSEND
6180 TFRTBL   .DA #MRECV,#MSEND
6210 *---------------------------------------------------------
6220 * RECEIVE - MAIN RECEIVE ROUTINE
6230 *---------------------------------------------------------
6240 RECEIVE  LDX #0            ;DIRECTION = PC-->APPLE
6250          JSR GETNAME       ;ASK FOR FILENAME & SEND TO PC
6260          LDA #ACK          ;1ST MESSAGE ALWAYS ACK
6270          STA MESSAGE
6280          LDA #0            ;START ON TRACK 0
6290          STA IOBTRK
6300          STA ERRORS        ;NO DISK ERRORS YET
6320 RECVLUP  STA SAVTRK        ;SAVE CURRENT TRACK
6330          LDX #1
6340          JSR SR7TRK        ;RECEIVE 7 TRACKS FROM PC
6350          LDX #2
6360          JSR RW7TRK        ;WRITE 7 TRACKS TO DISK
6370          LDA IOBTRK
6380          CMP #$23          ;REPEAT UNTIL TRACK $23
6390          BCC RECVLUP
6400          LDA MESSAGE       ;SEND LAST ACK
6410          JSR PUTC
6420          LDA ERRORS
6430          JSR PUTC          ;SEND ERROR FLAG TO PC
6440          JMP AWBEEP        ;BEEP AND END
6470 *---------------------------------------------------------
6480 * SEND - MAIN SEND ROUTINE
6490 *---------------------------------------------------------
6500 SEND     LDX #1            ;DIRECTION = APPLE-->PC
6510          JSR GETNAME       ;ASK FOR FILENAME & SEND TO PC
6520          LDA #ACK          ;SEND INITIAL ACK
6530          JSR PUTC
6540          LDA #0            ;START ON TRACK 0
6550          STA IOBTRK
6560          STA ERRORS        ;NO DISK ERRORS YET
6580 SENDLUP  STA SAVTRK        ;SAVE CURRENT TRACK
6590          LDX #1
6600          JSR RW7TRK        ;READ 7 TRACKS FROM DISK
6610          LDX #0
6620          JSR SR7TRK        ;SEND 7 TRACKS TO PC
6630          LDA IOBTRK
6640          CMP #$23          ;REPEAT UNTIL TRACK $23
6650          BCC SENDLUP
6660          LDA ERRORS
6670          JSR PUTC          ;SEND ERROR FLAG TO PC
6680          JMP AWBEEP        ;BEEP AND END
6710 *---------------------------------------------------------
6720 * SR7TRK - SEND (X=0) OR RECEIVE (X=1) 7 TRACKS
6730 *---------------------------------------------------------
6740 SR7TRK   STX WHAT2DO       ;X=0 FOR SEND, X=1 FOR RECEIVE
6750          LDA #7            ;DO 7 TRACKS
6760          STA TRKCNT
6770          LDA #TRACKS/$100  ;STARTING HERE
6780          STA SECPTR+1
6790          JSR HOMECUR       ;RESET CURSOR POSITION
6810 S7TRK    LDA #$F           ;COUNT SECTORS FROM F TO 0
6820          STA IOBSEC
6830 S7SEC    LDX WHAT2DO       ;PRINT STATUS CHARACTER
6840          LDA SRCHAR,X
6850          JSR CHROVER
6870          LDA WHAT2DO       ;EXECUTE SEND OR RECEIVE
6880          BNE DORECV        ;ROUTINE
6900 *------------------------ SENDING ------------------------
6920          JSR SENDSEC       ;SEND CURRENT SECTOR
6930          LDA CRC           ;FOLLOWED BY CRC
6940          JSR PUTC
6950          LDA CRC+1
6960          JSR PUTC
6970          JSR GETC          ;GET RESPONSE FROM PC
6980          CMP #ACK          ;IS IT ACK?
6990          BEQ SROKAY        ;YES, ALL RIGHT
7000          CMP #NAK          ;IS IT NAK?
7010          BEQ S7SEC         ;YES, SEND AGAIN
7030          LDY #MCONFUS      ;SOMETHING IS WRONG
7040          JSR SHOWMSG       ;TELL BAD NEWS
7050          LDY #MANYKEY      ;APPEND PROMPT
7060          JSR SHOWM1
7070          JSR AWBEEP
7080          JSR RDKEY         ;WAIT FOR KEY
7090          JMP ABORT         ;AND ABORT
7110 *----------------------- RECEIVING -----------------------
7130 DORECV   LDY #0            ;CLEAR NEW SECTOR
7140          TYA
7150 CLRLOOP  STA (SECPTR),Y
7160          INY
7170          BNE CLRLOOP
7190          LDA MESSAGE       ;SEND RESULT OF PREV SECTOR
7200          JSR PUTC
7210          JSR RECVSEC       ;RECEIVE SECTOR
7220          JSR GETC
7230          STA PCCRC         ;AND CRC
7240          JSR GETC
7250          STA PCCRC+1
7260          JSR UNDIFF        ;UNCOMPRESS SECTOR
7280          LDA CRC           ;CHECK RECEIVED CRC VS
7290          CMP PCCRC         ;CALCULATED CRC
7300          BNE RECVERR
7310          LDA CRC+1
7320          CMP PCCRC+1
7330          BEQ SROKAY
7350 RECVERR  LDA #NAK          ;CRC ERROR, ASK FOR RESEND
7360          STA MESSAGE
7370          BNE S7SEC
7390 *------------------ BACK TO COMMON LOOP ------------------
7410 SROKAY   JSR CHRREST       ;RESTORE PREVIOUS STATUS CHAR
7420          INC SECPTR+1      ;NEXT SECTOR
7430          DEC IOBSEC
7440          BPL S7SEC         ;TRACK NOT FINISHED
7450          LDA TRKCNT
7460          CMP #2            ;STARTING LAST TRACK, TURN
7470          BNE NOTONE        ;DRIVE ON, EXCEPT IN THE LAST
7480          LDA SAVTRK        ;BLOCK
7490          CMP #$1C
7500          BEQ NOTONE
7510 MOD5     BIT $C089
7530 NOTONE   DEC TRKCNT
7540          BEQ SREND
7550          JMP S7TRK         ;LOOP UNTIL 7 TRACKS DONE
7560 SREND    RTS
7580 SRCHAR   .AS -"OI"          ;STATUS CHARACTERS: OUT/IN
7590 WHAT2DO  .HS 00
7620 *---------------------------------------------------------
7630 * SENDSEC - SEND CURRENT SECTOR WITH RLE
7640 * CRC IS COMPUTED BUT NOT SENT
7650 *---------------------------------------------------------
7660 SENDSEC  LDY #0            ;START AT FIRST BYTE
7670          STY CRC           ;ZERO CRC
7680          STY CRC+1
7690          STY PREV          ;NO PREVIOUS CHARACTER
7700 SS1      LDA (SECPTR),Y    ;GET BYTE TO SEND
7710          JSR UPDCRC        ;UPDATE CRC
7720          TAX               ;KEEP A COPY IN X
7730          SEC               ;SUBTRACT FROM PREVIOUS
7740          SBC PREV
7750          STX PREV          ;SAVE PREVIOUS BYTE
7760          JSR PUTC          ;SEND DIFFERENCE
7770          BEQ SS3           ;WAS IT A ZERO?
7780          INY               ;NO, DO NEXT BYTE
7790          BNE SS1           ;LOOP IF MORE TO DO
7800          RTS               ;ELSE RETURN
7820 SS2      JSR UPDCRC
7830 SS3      INY               ;ANY MORE BYTES?
7840          BEQ SS4           ;NO, IT WAS 00 UP TO END
7850          LDA (SECPTR),Y    ;LOOK AT NEXT BYTE
7860          CMP PREV
7870          BEQ SS2           ;SAME AS BEFORE, CONTINUE
7880 SS4      TYA               ;DIFFERENCE NOT A ZERO
7890          JSR PUTC          ;SEND NEW ADDRESS
7900          BNE SS1           ;AND GO BACK TO MAIN LOOP
7910          RTS               ;OR RETURN IF NO MORE BYTES
7940 *---------------------------------------------------------
7950 * RECVSEC - RECEIVE SECTOR WITH RLE (NO TIME TO UNDIFF)
7960 *---------------------------------------------------------
7970 RECVSEC  LDY #0            ;START AT BEGINNING OF BUFFER
7980 RC1      JSR GETC          ;GET DIFFERENCE
7990          BEQ RC2           ;IF ZERO, GET NEW INDEX
8000          STA (SECPTR),Y    ;ELSE PUT CHAR IN BUFFER
8010          INY               ;AND INCREMENT INDEX
8020          BNE RC1           ;LOOP IF NOT AT BUFFER END
8030          RTS               ;ELSE RETURN
8040 RC2      JSR GETC          ;GET NEW INDEX
8050          TAY               ;IN Y REGISTER
8060          BNE RC1           ;LOOP IF INDEX <> 0
8070          RTS               ;ELSE RETURN
8100 *---------------------------------------------------------
8110 * UNDIFF -  FINISH RLE DECOMPRESSION AND UPDATE CRC
8120 *---------------------------------------------------------
8130 UNDIFF   LDY #0
8140          STY CRC           ;CLEAR CRC
8150          STY CRC+1
8160          STY PREV          ;INITIAL BASE IS ZERO
8170 UDLOOP   LDA (SECPTR),Y    ;GET NEW DIFFERENCE
8180          CLC
8190          ADC PREV          ;ADD TO BASE
8200          JSR UPDCRC        ;UPDATE CRC
8210          STA PREV          ;THIS IS THE NEW BASE
8220          STA (SECPTR),Y    ;STORE REAL BYTE
8230          INY
8240          BNE UDLOOP        ;REPEAT 256 TIMES
8250          RTS
8280 *---------------------------------------------------------
8290 * RW7TRK - READ (X=1) OR WRITE (X=2) 7 TRACKS
8300 * USES A,X,Y. IF ESCAPE, CALLS ABORT
8310 *---------------------------------------------------------
8320 RW7TRK   STX IOBCMD        ;X=1 FOR READ, X=2 FOR WRITE
8330          LDA #7            ;COUNT 7 TRACKS
8340          STA TRKCNT
8350          LDA #TRACKS/$100  ;START AT BEGINNING OF BUFFER
8360          STA IOBBUF+1
8370          JSR HOMECUR       ;RESET CURSOR POSITION
8390 NEXTTRK  LDA #$F           ;START AT SECTOR F (READ IS
8400          STA IOBSEC        ;FASTER THIS WAY)
8410 NEXTSEC  LDX IOBCMD        ;GET MAX RETRIES FROM
8420          LDA REALTRY-1,X   ;PARAMETER DATA
8430          STA RETRIES
8440          LDA RWCHAR-1,X    ;PRINT STATUS CHARACTER
8450          JSR CHROVER
8470 RWAGAIN  LDA $C000         ;CHECK KEYBOARD
8480          CMP #ESC          ;ESCAPE PUSHED?
8490          BNE RWCONT        ;NO, CONTINUE
8500          JMP SABORT        ;YES, ABORT
8520 RWCONT   LDA #IOB/$100     ;GET IOB ADDRESS IN REGISTERS
8530          LDY #IOB
8540          JSR $3D9          ;CALL RWTS THROUGH VECTOR
8550          LDA #"."          ;CARRY CLEAR MEANS NO ERROR
8560          BCC SECTOK        ;NO ERROR: PUT . IN STATUS
8570          DEC RETRIES       ;ERROR: SOME PATIENCE LEFT?
8580          BPL RWAGAIN       ;YES, TRY AGAIN
8590          ROL ERRORS        ;NO, SET ERRORS TO NONZERO
8600          JSR CLRSECT       ;FILL SECTOR WITH ZEROS
8610          LDA #'*'          ;AND PUT INVERSE * IN STATUS
8630 SECTOK   JSR CHRADV        ;PRINT SECTOR STATUS & ADVANCE
8640          INC IOBBUF+1      ;NEXT PAGE IN BUFFER
8650          DEC IOBSEC        ;NEXT SECTOR
8660          BPL NEXTSEC       ;LOOP UNTIL END OF TRACK
8670          INC IOBTRK        ;NEXT TRACK
8680          DEC TRKCNT        ;LOOP UNTIL 7 TRACKS DONE
8690          BNE NEXTTRK
8700          RTS
8720 RWCHAR   .AS -"RW"          ;STATUS CHARACTERS: READ/WRITE
8730 RETRIES  .HS 00
8740 REALTRY  .HS 0000          ;REAL NUMBER OF RETRIES
8770 *---------------------------------------------------------
8780 * CLRSECT - CLEAR CURRENT SECTOR
8790 *---------------------------------------------------------
8800 CLRSECT  LDA IOBBUF+1      ;POINT TO CORRECT SECTOR
8810          STA CSLOOP+2
8820          LDY #0            ;AND FILL 256 ZEROS
8830          TYA
8840 CSLOOP   STA $FF00,Y
8850          INY
8860          BNE CSLOOP
8870          RTS
8900 *---------------------------------------------------------
8910 * HOMECUR - RESET CURSOR POSITION TO 1ST SECTOR
8920 * CHRREST - RESTORE PREVIOUS CONTENTS & ADVANCE CURSOR
8930 * CHRADV  - WRITE NEW CONTENTS & ADVANCE CURSOR
8940 * ADVANCE - JUST ADVANCE CURSOR
8950 * CHROVER - JUST WRITE NEW CONTENTS
8960 *---------------------------------------------------------
8970 HOMECUR  LDY SAVTRK
8980          INY               ;CURSOR ON 0TH COLUMN
8990          STY CH
9000          JSR TOPNEXT       ;TOP OF 1ST COLUMN
9010          JMP CHRSAVE       ;SAVE 1ST CHARACTER
9030 CHRREST  LDA SAVCHR        ;RESTORE OLD CHARACTER
9040 CHRADV   JSR CHROVER       ;OVERWRITE STATUS CHAR
9050          JSR ADVANCE       ;ADVANCE CURSOR
9060 CHRSAVE  LDY CH
9070          LDA (BASL),Y      ;SAVE NEW CHARACTER
9080          STA SAVCHR
9090          RTS
9110 ADVANCE  INC CV            ;CURSOR DOWN
9120          LDA CV
9130          CMP #21           ;STILL IN DISPLAY?
9140          BCC NOWRAP        ;YES, WE'RE DONE
9150 TOPNEXT  INC CH            ;NO, GO TO TOP OF NEXT
9160          LDA #5            ;COLUMN
9170 NOWRAP   JMP TABV          ;VALIDATE BASL,H
9190 CHROVER  LDY CH
9200          STA (BASL),Y
9210          RTS
9240 *---------------------------------------------------------
9250 * UPDCRC - UPDATE CRC WITH CONTENTS OF ACCUMULATOR
9260 *---------------------------------------------------------
9270 UPDCRC   PHA
9280          EOR CRC+1
9290          TAX
9300          LDA CRC
9310          EOR CRCTBLH,X
9320          STA CRC+1
9330          LDA CRCTBLL,X
9340          STA CRC
9350          PLA
9360          RTS
9390 *---------------------------------------------------------
9400 * MAKETBL - MAKE CRC-16 TABLES
9410 *---------------------------------------------------------
9420 MAKETBL  LDX #0
9430          LDY #0
9440 CRCBYTE  STX CRC           ;LOW BYTE = 0
9450          STY CRC+1         ;HIGH BYTE = INDEX
9470          LDX #8            ;FOR EACH BIT
9480 CRCBIT   LDA CRC
9490 CRCBIT1  ASL               ;SHIFT CRC LEFT
9500          ROL CRC+1
9510          BCS CRCFLIP
9520          DEX               ;HIGH BIT WAS CLEAR, DO NOTHING
9530          BNE CRCBIT1
9540          BEQ CRCSAVE
9550 CRCFLIP  EOR #$21          ;HIGH BIT WAS SET, FLIP BITS
9560          STA CRC           ;0, 5, AND 12
9570          LDA CRC+1
9580          EOR #$10
9590          STA CRC+1
9600          DEX
9610          BNE CRCBIT
9630          LDA CRC           ;STORE CRC IN TABLES
9640 CRCSAVE  STA CRCTBLL,Y
9650          LDA CRC+1
9660          STA CRCTBLH,Y
9670          INY
9680          BNE CRCBYTE       ;DO NEXT BYTE
9690          RTS
9720 *---------------------------------------------------------
9730 * PARMDFT - RESET PARAMETERS TO DEFAULT VALUES (USES AX)
9740 *---------------------------------------------------------
9750 PARMDFT  LDX #PARMNUM-1
9760 DFTLOOP  LDA DEFAULT,X
9770          STA PARMS,X
9780          DEX
9790          BPL DFTLOOP
9800          RTS
9830 *---------------------------------------------------------
9840 * AWBEEP - CUTE TWO-TONE BEEP (USES AXY)
9850 *---------------------------------------------------------
9860 AWBEEP   LDA PSOUND        ;IF SOUND OFF, RETURN NOW
9870          BNE NOBEEP
9880          LDA #$80          ;STRAIGHT FROM APPLE WRITER ][
9890          JSR BEEP1         ;(CANNIBALISM IS THE SINCEREST
9900          LDA #$A0          ;FORM OF FLATTERY)
9910 BEEP1    LDY #$80
9920 BEEP2    TAX
9930 BEEP3    DEX
9940          BNE BEEP3
9950          BIT $C030         ;WHAP SPEAKER
9960          DEY
9970          BNE BEEP2
9980 NOBEEP   RTS
10010 *---------------------------------------------------------
10020 * PUTC - SEND ACC OVER THE SERIAL LINE (AXY UNCHANGED)
10030 *---------------------------------------------------------
10040 PUTC     PHA
10050 PUTC1    LDA $C000
10060          CMP #ESC          ;ESCAPE = ABORT
10070          BEQ SABORT
10080 MOD1     LDA $C089         ;CHECK STATUS BITS
10090          AND #$70
10100          CMP #$10
10110          BNE PUTC1         ;OUTPUT REG FULL, LOOP
10120          PLA
10130 MOD2     STA $C088         ;PUT CHARACTER
10140          RTS
10170 *---------------------------------------------------------
10180 * GETC - GET A CHARACTER FROM SERIAL LINE (XY UNCHANGED)
10190 *---------------------------------------------------------
10200 GETC     LDA $C000
10210          CMP #ESC          ;ESCAPE = ABORT
10220          BEQ SABORT
10230 MOD3     LDA $C089         ;CHECK STATUS BITS
10240          AND #$68
10250          CMP #$8
10260          BNE GETC          ;INPUT REG EMPTY, LOOP
10270 MOD4     LDA $C088         ;GET CHARACTER
10280          RTS
10310 *---------------------------------------------------------
10320 * ABORT - STOP EVERYTHING (CALL SABORT TO BEEP ALSO)
10330 *---------------------------------------------------------
10340 SABORT   JSR AWBEEP        ;BEEP
10350 ABORT    LDX #$FF          ;POP GOES THE STACKPTR
10360          TXS
10370          BIT $C010         ;STROBE KEYBOARD
10380          JMP REDRAW        ;AND RESTART
10410 *---------------------------------------------------------
10420 * TITLE - SHOW TITLE SCREEN
10430 *---------------------------------------------------------
10440 TITLE    JSR HOME          ;CLEAR SCREEN
10450          LDY #MTITLE
10460          JSR SHOWM1        ;SHOW TOP PART OF TITLE SCREEN
10480          LDX #15           ;SHOW SECTOR NUMBERS
10490          LDA #5            ;IN DECREASING ORDER
10500          STA CV            ;FROM TOP TO BOTTOM
10510 SHOWSEC  JSR VTAB
10520          LDA #$20
10530          LDY #38
10540          STA (BASL),Y
10550          LDY #0
10560          STA (BASL),Y
10570          LDA HEXNUM,X
10580          INY
10590          STA (BASL),Y
10600          LDY #37
10610          STA (BASL),Y
10620          INC CV
10630          DEX
10640          BPL SHOWSEC
10660          LDA #"_"          ;SHOW LINE OF UNDERLINES
10670          LDX #38           ;ABOVE INVERSE TEXT
10680 SHOWUND  STA $500,X
10690          DEX
10700          BPL SHOWUND
10710          RTS
10740 *---------------------------------------------------------
10750 * SHOWMSG - SHOW NULL-TERMINATED MESSAGE #Y AT BOTTOM OF
10760 * SCREEN.  CALL SHOWM1 TO SHOW ANYWHERE WITHOUT ERASING
10770 *---------------------------------------------------------
10780 SHOWMSG  STY YSAVE         ;CLREOP USES Y
10790          LDA #0
10800          STA CH            ;COLUMN 0
10810          LDA #22           ;LINE 22
10820          JSR TABV
10830          JSR CLREOP        ;CLEAR MESSAGE AREA
10840          LDY YSAVE
10860 SHOWM1   LDA MSGTBL,Y      ;CALL HERE TO SHOW ANYWHERE
10870          STA MSGPTR
10880          LDA MSGTBL+1,Y
10890          STA MSGPTR+1
10910          LDY #0
10920 MSGLOOP  LDA (MSGPTR),Y
10930          BEQ MSGEND
10940          JSR COUT1
10950          INY
10960          BNE MSGLOOP
10970 MSGEND   RTS
11000 *------------------------ MESSAGES -----------------------
11020 MSGTBL   .DA MSG01,MSG02,MSG03,MSG04,MSG05,MSG06,MSG07
11030          .DA MSG08,MSG09,MSG10,MSG11,MSG12,MSG13,MSG14
11040          .DA MSG15,MSG16,MSG17,MSG18,MSG19,MSG20,MSG21
11050          .DA MSG22
11070 MSG01    .AS -"SSC:S"
11080 MTSSC    .AS -" ,"
11090 MTSPD    .AS -"        "
11100          .AS -" ADT 1.21 "   ;INV
11110          .AS -"    DISK:S"
11120 MTSLT    .AS -" ,D"
11130 MTDRV    .AS -" "
11135          .HS 8D8D8D
11140          .AS "  00000000000000001111111111111111222  "    ;INV
11145          .HS 8D
11150          .AS "  "                                         ;INV
11160 HEXNUM   .AS "0123456789ABCDEF0123456789ABCDEF012  "      ;INV
11165          .HS 8D00
11180 MSG02    .AS -" ADT CONFIGURATION "   ;INV
11185          .HS 8D8D8D
11190          .AS -"DISK SLOT"
11195          .HS 8D
11200          .AS -"DISK DRIVE"
11205          .HS 8D
11210          .AS -"SSC SLOT"
11215          .HS 8D
11220          .AS -"SSC SPEED"
11225          .HS 8D
11230          .AS -"READ RETRIES"
11235          .HS 8D
11240          .AS -"WRITE RETRIES"
11245          .HS 8D
11250          .AS -"USE CHECKSUMS"
11255          .HS 8D
11260          .AS -"ENABLE SOUND"
11265          .HS 00
11280 MSG03    .AS -"USE ARROWS AND SPACE TO CHANGE VALUES,"
11285          .HS 8D
11290          .AS -"RETURN TO ACCEPT, CTRL-D FOR DEFAULTS."
11295          .HS 00
11310 MSG04    .AS -"SEND, RECEIVE, DIR, CONFIGURE, QUIT? "
11315          .HS 00
11320 MSG05    .AS -"SPACE TO CONTINUE, ESC TO STOP: "
11325          .HS 00
11330 MSG06    .AS -"END OF DIRECTORY, TYPE SPACE: "
11335          .HS 00
11350 MSG07    .AS -"FILE TO RECEIVE: "
11355          .HS 00
11360 MSG08    .AS -"FILE TO SEND: "
11365          .HS 00
11380 MSG09    .AS "RECEIVING FILE "
11385          .HS 00
11390 MSG10    .AS "SENDING FILE "
11395          .HS 00
11410 MSG11    .AS -"ERROR:"   ;INV
11420          .AS -" NONSENSE FROM PC."
11425          .HS 00
11440 MSG12    .AS -"ERROR:"   ;INV
11450          .AS -" NOT A 16-SECTOR DISK."
11455          .HS 00
11470 MSG13    .AS -"ERROR:"   ;INV
11480          .AS -" FILE "
11485          .HS 00
11500 MSG14    .HS 8D
11510          .AS -"CAN'T BE OPENED."
11515          .HS 00
11530 MSG15    .HS 8D
11540          .AS -"ALREADY EXISTS."
11545          .HS 00
11560 MSG16    .HS 8D
11570          .AS -"IS NOT A 140K IMAGE."
11575          .HS 00
11590 MSG17    .HS 8D
11600          .AS -"DOESN'T FIT ON DISK."
11605          .HS 00
11620 MSG18    .AS -"  ANY KEY: "
11625          .HS 00
11640 MSG19    .AS -"<- DO NOT CHANGE"
11645          .HS 00
11660 MSG20    .AS -"APPLE DISK TRANSFER 1.21     1994-10-13"
11665          .HS 8D
11670          .AS -"PAUL GUERTIN: GUERTINP@IRO.UMONTREAL.CA"
11675          .HS 00
11690 MSG21    .AS -"TESTING DISK FORMAT."
11695          .HS 00
11710 MSG22    .AS -"AWAITING ANSWER FROM PC."
11715          .HS 00
11740 *----------------------- PARAMETERS ----------------------
11760 PARMSIZ  .DA #7,#2,#7,#6,#8,#8,#2,#2    ;#OPTIONS OF EACH PARM
11780 PARMTXT  .DA #'1+$80,#0,#'2+$80,#0
11782          .DA #'3+$80,#0,#'4+$80,#0
11784          .DA #'5+$80,#0,#'6+$80,#0
11786          .DA #'7+$80,#0
11790          .DA #'1+$80,#0,#'2+$80,#0
11800          .DA #'1+$80,#0,#'2+$80,#0
11802          .DA #'3+$80,#0,#'4+$80,#0
11804          .DA #'5+$80,#0,#'6+$80,#0
11806          .DA #'7+$80,#0
11810          .AS -"300"
11811          .HS 00
11812          .AS -"1200"
11813          .HS 00
11814          .AS -"2400"
11815          .HS 00
11820          .AS -"4800"
11821          .HS 00
11822          .AS -"9600"
11823          .HS 00
11824          .AS -"19200"
11825          .HS 00
11830          .DA #'0+$80,#0,#'1+$80,#0
11832          .DA #'2+$80,#0,#'3+$80,#0
11834          .DA #'4+$80,#0,#'5+$80,#0
11836          .DA #'1+$80,#'0+$80,#0
11838          .DA #'9+$80,#'9+$80,#0
11840          .DA #'0+$80,#0,#'1+$80,#0
11842          .DA #'2+$80,#0,#'3+$80,#0
11844          .DA #'4+$80,#0,#'5+$80,#0
11846          .DA #'1+$80,#'0+$80,#0
11848          .DA #'9+$80,#'9+$80,#0
11850          .AS -"YES"
11852          .HS 00
11854          .AS -"NO"
11856          .HS 00
11860          .AS -"YES"
11862          .HS 00
11864          .AS -"NO"
11866          .HS 00
11880 PARMS
11890 PDSLOT   .DA #5            ;DISK SLOT (6)
11900 PDRIVE   .DA #0            ;DISK DRIVE (1)
11910 PSSC     .DA #1            ;SSC SLOT (2)
11920 PSPEED   .DA #5            ;SSC SPEED (19200)
11930 PRETRY   .DA #1,#0         ;READ/WRITE MAX RETRIES (1,0)
11940 PCKSUM   .DA #0            ;USE RWTS CHECKSUMS? (Y)
11950 PSOUND   .DA #0            ;SOUND AT END OF TRANSFER? (Y)
11970 *-------------------------- IOB --------------------------
11990 IOB      .HS 01            ;IOB TYPE
12000 IOBSLT   .HS 60            ;SLOT*$10
12010 IOBDRV   .HS 01            ;DRIVE
12020          .HS 00            ;VOLUME
12030 IOBTRK   .HS 00            ;TRACK
12040 IOBSEC   .HS 00            ;SECTOR
12050          .DA DCT           ;DEVICE CHAR TABLE POINTER
12060 IOBBUF   .DA TRACKS        ;SECTOR BUFFER POINTER
12070          .HS 0000          ;UNUSED
12080 IOBCMD   .HS 01            ;COMMAND (1=READ, 2=WRITE)
12090          .HS 00            ;ERROR CODE
12100          .HS FE            ;ACTUAL VOLUME
12110          .HS 60            ;PREVIOUS SLOT
12120          .HS 01            ;PREVIOUS DRIVE
12130 DCT      .HS 0001EFD8      ;DEVICE CHARACTERISTICS TABLE
12150 *-------------------------- MISC -------------------------
12170 DOSBYTE  .HS 0000          ;DOS BYTES CHANGED BY ADT
12180 STDDOS   .HS 00            ;ZERO IF "STANDARD" DOS
12190 SAVTRK   .HS 00            ;FIRST TRACK OF SEVEN
12200 SAVCHR   .HS 00            ;CHAR OVERWRITTEN WITH STATUS
12210 MESSAGE  .HS 00            ;SECTOR STATUS SENT TO PC
12220 PCCRC    .HS 0000          ;CRC RECEIVED FROM PC
12230 ERRORS   .HS 00            ;NON0 IF AT LEAST 1 DISK ERROR

SAVE ADT_SS.SC
