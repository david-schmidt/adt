; da65 V2.11.0 - (C) Copyright 2000-2005,  Ullrich von Bassewitz
; Created:    2005-12-13 10:04:44
; Input file: SST_DD00
; Page:       1


        .setcpu "6502"
        .include "merlin.inc"
        .ORG    $1D00

CLBOT           := $1610
ORSLT           := $B280                        ; Original slot
ORDRV           := $B281                        ; Original drive
DPSLT           := $B282                        ; Duplicate slot
DPDRV           := $B283                        ; Duplicate drive
XXSLT           := $B284                        ; Some slot number
DCOUNT          := $B285                        ; Disk count
ORTRK           := $B286                        ; Initialized to $FF?
DPTRK           := $B287
TBEG            := $B288
TEND            := $B289
TSTEP           := $B28A
CURTRK          := $B28B
SYNFLG          := $B28C
NIBFLG          := $B28D
CH              := $B500                        ; Called by Pirate a Disk main menu
IN              := $B50D
R               := $B517                        ; Hijacked read routine
W               := $B51D                        ; Hijacked write routine
LEAVE           := $B523
MOVE            := $B527                        ; Swaps RAM and banked-switch RAM
XCH1            := $B539
XCH2            := $B541
MREAD           := $B560
MWRITE          := $B566
VAR1            := $B56C
VAR2            := $B56D
VAR3            := $B56E
FLAG            := $B56F
ORSLT1          := $B570
DPSLT1          := $B571
ORDRV1          := $B572
DPDRV1          := $B573
DCOUNT1         := $B574
TBEG1           := $B575
TEND1           := $B576
TSTEP1          := $B577
NIBFLG1         := $B578
SYNFLG1         := $B579
SIDE            := $B57A
TRK1            := $B57B
NUMBER          := $B57C
SLT             := $B7E9                        ; RWTS slot * 16
DRV             := $B7EA                        ; RWTS drive
VOL             := $B7EB                        ; RWTS volume
TRK             := $B7EC                        ; RWTS track
SECX            := $B7ED                        ; RWTS sector
BUFLO           := $B7F0                        ; RWTS buffer, low
BUFHI           := $B7F1                        ; RWTS buffer, high
RWTS_BYTE_COUNT := $B7F3
CMD             := $B7F4                        ; RWTS command: 0=SEEK, 1=READ, 2=WRITE, 4=FORMAT.
LB87D           := $B87D
DOS_RWTS        := $BD00
KBD             := $C000
KBDSTRB         := $C010
TAPEOUT         := $C020
SPKR            := $C030
TXTCLR          := $C050
TXTSET          := $C051
MIXCLR          := $C052
MIXSET          := $C053
LOWSCR          := $C054
VTAB            := $FC22
HOME            := $FC58
COUT            := $FDED
SETKBD          := $FE89
SETVID          := $FE93
BELL            := $FF3A
OLDRST          := $FF59


XCHANGE:lda     #$B7
        sta     $3D
        lda     #$D7
        sta     $3F
        lda     #$00
        sta     $3C
        sta     $3E
        tay
        lda     #$C0
        sta     $42
        jmp     XCH1
PUP:    lda     #$78
        sta     $3D
        lda     #$94
        sta     $3F
        lda     #$00
        sta     $3C
        sta     $3E
        tay
        rts
UNPACK: jsr     PUP
UPCK1:  lda     ($3C),y
        cmp     #$80
        bcc     SETBIT
        lda     #$00
        jmp     STRBIT
SETBIT: eor     #$80
        sta     ($3C),y
        lda     #$01
STRBIT: sta     ($3E),y
        iny
        bne     UPCK1
        inc     $3D
        inc     $3F
        lda     $3F
        cmp     #$B0
        bne     UPCK1
        rts
CLMID:  lda     #$0C
        sta     $22
        jsr     HOME
        rts
CHKDSK: ldy     #$00
CHK1:   lda     $B230,y
        cmp     DSKCMP,y
        bne     BAD
        iny
        cpy     #$05
        bne     CHK1
        clc
        jmp     GOOD
BAD:    jsr     CLBOT
        jsr     SETCRS
        lda     SIDE
        sta     SIDEA
        ldy     #$00
BAD1:   lda     WDSK,y
        cmp     #$FF
        beq     BAD2
        jsr     COUT
        iny
        jmp     BAD1
BAD2:   jsr     TDO2
        lda     TRK1
        jmp     RETRY
WDSK:   asc     " WRONG DISK!!!! Insert data disk side "
SIDEA:  .byte   $b1, $8d
        asc     " and press return."
        .byte   $ff
GOOD:   jmp     RETIT
RWTS:   lda     #$B1
        sta     SIDE
        lda     TRK
        cmp     #$23
        bcc     RWTS2
        cmp     #$24
        bcs     RWTS1
        jsr     TDO
        jmp     RWTS0
RWTS1:  lda     #$B2
        sta     SIDE
RWTS0:  lda     TRK
        sec
        sbc     #$23
        sta     TRK
RWTS2:  lda     KBD
        cmp     #$9B
        beq     RWTS3
        lda     #$B7
        ldy     #$E8
        jsr     DOS_RWTS
        bcs     RWTS2
        rts
RWTS3:  sec
        rts
READINT:lda     FLAG
        cmp     #$02
        beq     MYREAD
        jmp     MREAD
WRITINT:lda     FLAG
        cmp     #$01
        beq     MYWRITE
        jmp     MWRITE
MYREAD: bit     KBDSTRB
        jsr     XCHANGE
        lda     ORDRV
        sta     DRV
        lda     ORSLT
        asl     a
        asl     a
        asl     a
        asl     a
        sta     SLT
        lda     #$01
        sta     CMD
        jsr     RWPROG
        jmp     LEAVE
MYWRITE:
        jmp     MYWRITEX
;        jsr     XCHANGE
        lda     DPSLT
        asl     a
        asl     a
        asl     a
        asl     a
        sta     SLT
        lda     DPDRV
        sta     DRV
        jsr     PACK
        lda     #$02
        sta     CMD
        jsr     RWPROG
        jmp     LEAVE
RWPROG: lda     ORSLT
        sta     ORSLT1
        lda     DPSLT
        sta     DPSLT1
        lda     ORDRV
        sta     ORDRV1
        lda     DPDRV
        sta     DPDRV1
        lda     DCOUNT
        sta     DCOUNT1
        lda     CURTRK
        sta     TRK1
        lda     VAR2
        beq     RILBEG
        lda     NIBFLG
        sta     NIBFLG1
        lda     SYNFLG
        sta     SYNFLG1
        ldy     #$00
SPARM:  lda     $B300,y
        sta     $2500,y
        iny
        bne     SPARM
RILBEG: lda     TBEG
        sta     TBEG1
        lda     TEND
        sta     TEND1
        lda     TSTEP
        sta     TSTEP1
RETRY:  lda     TRK1
        lsr     a
        sta     TRK
        lda     #$78
        sta     BUFHI
        lda     #$00
        sta     BUFLO
        sta     VOL
        lda     TRK1
        cmp     #$8C
        bcc     T34
        jmp     T35
T34:    lda     #$0F
        sta     SECX
RWLOOP: jsr     RWTS
        inc     BUFHI
        lda     BUFHI
        cmp     #$94
        beq     RWDONE
        dec     SECX
        bpl     RWLOOP
        lda     #$0F
        sta     SECX
        inc     TRK
        jmp     RWLOOP
RWDONE: dec     SECX
        lda     #$B1
        sta     BUFHI
        jsr     RWTS2
        dec     SECX
        inc     BUFHI
        jsr     RWTS2
        dec     SECX
        inc     BUFHI
        jsr     RWTS2
        bcc     REB
        jsr     XCHANGE
        rts
REB:    jmp     CHKDSK
RETIT:  lda     ORSLT
        cmp     DPSLT
        bne     DONE
        lda     ORDRV
        cmp     DPDRV
        bne     DONE
        lda     TRK
        asl     a
        asl     a
        sta     $12
        jsr     XCHANGE
        lda     CURTRK
        jsr     LB87D
        jmp     DONER
DONE:   jsr     XCHANGE
DONER:  lda     FLAG
        cmp     #$02
        bne     DONE2
        jsr     UNPACK
        lda     VAR3
        cmp     #$FF
        beq     DONE2
        lda     CURTRK
        sec
        sbc     TSTEP
        sta     DPTRK
DONE2:  lda     CURTRK
        sta     $12
        lda     #$00
        sta     VAR3
        jsr     CLBOT
        lda     ORDRV1
        sta     ORDRV
        lda     ORSLT1
        sta     ORSLT
        lda     DPSLT1
        sta     DPSLT
        lda     DPDRV1
        sta     DPDRV
        lda     DCOUNT1
        sta     DCOUNT
        lda     TBEG1
        sta     TBEG
        lda     TEND1
        sta     TEND
        lda     TSTEP1
        sta     TSTEP
        lda     VAR2
        beq     RILDON
        lda     NIBFLG1
        sta     NIBFLG
        lda     SYNFLG1
        sta     SYNFLG
        ldy     #$00
RPARM:  lda     $2500,y
        sta     $B300,y
        iny
        bne     RPARM
RILDON: rts
DSKCMP: .byte   $10,$27,$E8,$03,$64
CHOOSE: jsr     CLMID
        lda     #$FF
        sta     VAR3
        sta     ORTRK
        ldy     #$00
        bit     KBDSTRB
CH1:    lda     CHTXT,y
        cmp     #$FF
        beq     CH2
        jsr     COUT
        iny
        jmp     CH1
CH2:    lda     KBD
        bpl     CH2
        cmp     #$9B
        bne     CHH
        rts
CHH:    cmp     #$B1
        bcc     CH2
        cmp     #$B4
        bcs     CH2
        sec
        sbc     #$B1
        sta     FLAG
        cmp     #$02
        bne     CH5
        jsr     HOME
        bit     KBDSTRB
        ldy     #$00
CH3:    lda     PARMTXT,y
        cmp     #$FF
        beq     CH4
        jsr     COUT
        iny
        jmp     CH3
CH4:    lda     KBD
        bpl     CH4
        cmp     #$9B
        bne     CHI
        rts
CHI:    cmp     #$B1
        bcc     CH4
        cmp     #$B3
        bcs     CH4
        sec
        sbc     #$B1
        sta     VAR2
        jmp     CH6
CH5:    lda     #$00
        sta     VAR2
CH6:    bit     KBDSTRB
        rts
        ;        0        1         2         3         4
        ;        1234567890123456789012345678901234567890
CHTXT:  asc     "             1) Copy                    "
        asc     "             2) Dave's Pack             "
        asc     "             3) Dave's Unpack"
        .byte   $ff
PARMTXT:
        asc     "             1) Packed parms            "
        asc     "             2) Your parms"
        .byte   $ff
INITEXT:
        asc     "    <O>riginal or <D>uplicate drive?"
        .byte   $ff
PAK:    asc     "        Press a key when ready"
        .byte   $ff
CLR:    asc     "                                        "
        asc     "                                       "
        .byte   $ff
TDO:    jsr     SETCRS
        ldy     #$00
TDO1:   lda     TDOTXT,y
        cmp     #$20
        beq     TDO2
        jsr     COUT
        iny
        jmp     TDO1
TDO2:   bit     KBDSTRB
TDO3:   lda     KBD
        bpl     TDO3
        jsr     CLBOT
        rts
TDOTXT: asc     "  Turn DATA disk over and press RETURN"
INIT:   jsr     CLBOT
        jsr     XCHANGE
        bit     KBDSTRB
        jsr     SETCRS
        ldy     #$00
PRINT:  lda     INITEXT,y
        cmp     #$FF
        beq     INIT2
        jsr     COUT
        iny
        jmp     PRINT
INIT2:  lda     KBD
        bpl     INIT2
        bit     KBDSTRB
        cmp     #$CF
        beq     INORG
        cmp     #$C4
        beq     INDUP
        jmp     INIT2
INORG:  lda     ORDRV
        sta     DRV
        lda     ORSLT
        asl     a
        asl     a
        asl     a
        asl     a
        sta     SLT
        jmp     INITIT
INDUP:  lda     DPDRV
        sta     DRV
        lda     DPSLT
        asl     a
        asl     a
        asl     a
        asl     a
        sta     SLT
INITIT: bit     KBDSTRB
        jsr     CLBOT
        jsr     SETCRS
        ldy     #$00
INITIT1:lda     PAK,y
        cmp     #$FF
        beq     INITIT2
        jsr     COUT
        iny
        jmp     INITIT1
INITIT2:lda     KBD
        bpl     INITIT2
        bit     KBDSTRB
        lda     #$00
        sta     VOL
        lda     #$04
        sta     CMD
        jsr     RWTS
        jsr     CLBOT
        jsr     XCHANGE
        rts
SETCRS: lda     #$00
        sta     $24
        lda     #$16
        sta     $25
        jmp     VTAB
PACK:   jsr     PUP
PCK1:   lda     ($3E),y
        asl     a
        asl     a
        asl     a
        asl     a
        asl     a
        asl     a
        asl     a
        eor     ($3C),y
        sta     ($3C),y
        iny
        bne     PCK1
        inc     $3D
        inc     $3F
        lda     $3F
        cmp     #$B0
        bne     PCK1
        rts
T35:    jsr     TDO
        lda     #$00
        sta     SECX
        lda     #$01
        sta     TRK
        lda     #$B2
        sta     SIDE
T35LP:  jsr     RWTS
        bcc     T35LP1
        rts
T35LP1: inc     TRK
        inc     TRK
        inc     BUFHI
        lda     BUFHI
        cmp     #$94
        bne     T35LP
        lda     #$B1
        sta     BUFHI
        jsr     RWTS
        inc     BUFHI
        inc     TRK
        inc     TRK
        jsr     RWTS
        inc     BUFHI
        inc     TRK
        inc     TRK
        jsr     RWTS
        jmp     REB
        
MYWRITEX:
        jsr     BELL
        lda     CURTRK
        sta     $12
        jmp     LEAVE
        
        .res    $2400-*

