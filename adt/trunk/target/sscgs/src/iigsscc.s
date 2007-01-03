;
; ADTPro - Apple Disk Transfer ProDOS
; Copyright (C) 2006 by David Schmidt
; david__schmidt at users.sourceforge.net
;
; This program is free software; you can redistribute it and/or modify it 
; under the terms of the GNU General Public License as published by the 
; Free Software Foundation; either version 2 of the License, or (at your 
; option) any later version.
;
; This program is distributed in the hope that it will be useful, but 
; WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
; or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
; for more details.
;
; You should have received a copy of the GNU General Public License along 
; with this program; if not, write to the Free Software Foundation, Inc., 
; 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
;

;---------------------------------------------------------
; INITZGS - Set up IIgs SCC chip (kills firmware and GSOS)
; Note - we assume interrupts are turned off in the 
; calling code.  if not, initzgs should be surrounded by
; sei and cli instructions.
;---------------------------------------------------------
initzgs:
	jsr	initzscc
	jsr	patchzgs
	rts

;---------------------------------------------------------
; zccp - Send accumulator out the SCC serial port
;---------------------------------------------------------
zccp:
	sta	tempa
	stx	tempx

zsend:	lda	gscmdb		; rr0

	tax
	and	#%00000100	; test bit 2 (hardware handshaking)
	beq	zsend
	txa
	and	#%00100000	; test bit 5 (ready to send?)
	beq	zsend

exit0:	lda	tempa		; get char to send
	sta	gsdatab		; send the character

exit:	ldx	tempx
	lda	tempa
	rts

tempa:	.byte	1
tempx:	.byte	1


;---------------------------------------------------------
; zccg - Get a character from the SCC serial port (XY unchanged)
;---------------------------------------------------------

zccg:
	lda	gscmdb		; DUMMY READ TO RESET 8530 POINTER TO 0

pollscc:
	lda	$C000
	cmp	#esc		; escape = abort
	bne	sccnext
	jmp	pabort

sccnext:
	lda	gscmdb		; READ 8530 READ REGISTER 0
	and	#$01		; BIT 0 MEANS RX CHAR AVAILABLE
	cmp	#$01
	bne	pollscc

				; THERE'S A CHAR IN THE 8530 RX BUFFER
pullIt:
	lda	#$01		; SET 'POINTER' TO rr1
	sta	gscmdb
	lda	gscmdb		; READ THE 8530 READ REGISTER 1
	and	#$20		; CHECK FOR bit 5=RX OVERRUN
	beq	itsOK
	ldx	#$30		; Clear Receive overrun
	stx	gscmdb
	ldx	#$00
	stx	gscmdb

itsOK:
	lda	#$08		; WE WANT TO READ rr8
	sta	gscmdb		; SET 'POINTER' TO rr8
	lda	gscmdb		; READ rr8
	rts

;---------------------------------------------------------
; initzscc - initialize the Modem Port
; (Channel B is modem port, A is printer port)
;---------------------------------------------------------

initzscc:
	clc
	lda	pspeed		; 0 = 300, 1 = 1200, 2 = 2400, 3 = 4800, 4 = 9600, 5=19200, 6=115200
	sta	baud
	inc	baud		; ADT's notion of baud and the SCC's are off by 1

	lda	gscmdb		; Hit rr0 once to sync up

	ldx	#9		; wr9
	lda	#resetb		; Load constant to reset Ch B
	stx	gscmdb
	sta	gscmdb
	NOP			; SCC needs 11 pclck to recover

	ldx	#3		; wr3
	lda	#%11000000	; 8 data bits, receiver disabled
	stx	gscmdb		; could be 7 or 6 or 5 data bits
	sta	gscmdb		; for 8 bits, bits 7,6 = 1,1

	ldx	#5		; wr5
	lda	#%01100010	; DTR enabled 0=/HIGH, 8 data bits
	stx	gscmdb		; no BRK, xmit disabled, no SDLC
	sta	gscmdb		; RTS *MUST* be disabled, no crc

	ldx	#14		; wr14
	lda	#%00000000	; null cmd, no loopback
	stx	gscmdb		; no echo, /DTR follows wr5
	sta	gscmdb		; Baud Rate Gen source is XTAL or RTxC

	lda	pspeed		; Check desired baud rate
	cmp	#$06		; Is it 115200?
	beq	gofast		; Yes - special processing

	ldx	#4		; wr4
	lda	#%01000100	; X16 clock mode,
	stx	gscmdb		; 1 stop bit, no parity
	sta	gscmdb		; could be 1.5 or 2 stop bits
				; 1.5 set bits 3,2 to 1,0
				; 2   set bits 3,2 to 1,1

	ldx	#11		; wr11
	lda	#wr11bbrg	; Load constant to write
	stx	gscmdb
	sta	gscmdb

	JSR	TIMECON		; Set up wr12 and wr13
				; to set baud rate

; Enables
	ORA	#%00000001	; Enable Baud Rate Generator
	ldx	#14		; wr14
	stx	gscmdb
	sta	gscmdb		; write value
	jmp	initcommon

gofast:
	ldx	#4		; wr4
	lda	#%10000100	; X32 clock mode,
	stx	gscmdb		; 1 stop bit, no parity
	sta	gscmdb		; could be 1.5 or 2 stop bits
				; 1.5 set bits 3,2 to 1,0
				; 2   set bits 3,2 to 1,1

	ldx	#11		; wr11
	lda	#wr11bxtal	; Load constant to write
	stx	gscmdb
	sta	gscmdb

initcommon:
	lda	#%11000001	; 8 data bits, Rx enable
	ldx	#3
	stx	gscmdb
	sta	gscmdb		; write value

	lda	#%01101010	; DTR enabled; Tx enable
	ldx	#5
	stx	gscmdb
	sta	gscmdb		; write value

; Enable Interrupts

	ldx	#15		; wr15

; The next line is commented out. This driver wants
; interrupts when GPi changes state, ie. the user
; on the BBS may have hung up. You can write a 0
; to this register if you don't need any external
; status interrupts. Then in the IRQIN routine you
; won't need handling for overruns; they won't be
; latched. See the Zilog Tech Ref. for details.

;	lda	#%00100000	; Allow ext. int. on CTS/HSKi

	lda	#%00000000	; Disallow ext. int. on DCD/GPi

	stx	gscmdb
	sta	gscmdb

	ldx	#0
	lda	#%00010000	; Reset ext. stat. ints.
	stx	gscmdb
	sta	gscmdb		; write it twice

	stx	gscmdb
	sta	gscmdb

	ldx	#1		; wr1
	lda	#%00000000	; Wait Request disabled
	stx	gscmdb		; Allow IRQs on Rx all & ext. stat
	sta	gscmdb		; No transmit interrupts (b1)

	lda	gscmdb		; READ TO RESET channelB POINTER TO 0
	lda	#$09
	sta	gscmdb		; SET 'POINTER' TO wr9
	lda	#$00
	sta	gscmdb		; Anti BluRry's syndrome medication 

	rts			; We're done!


; TIMECON: Set time constant bytes in wr12 & wr13
; (In other words, set the baud rate.)

TIMECON:
	ldy	baud
	lda	#12
	sta	gscmdb
	lda	baudl-1,y	; Load time constant low
	sta	gscmdb

	lda	#13
	sta	gscmdb
	lda	baudh-1,y	; Load time constant high
	sta	gscmdb
	rts

; Table of values for different baud rates for internal baud
; rate generator-clocked speeds. There is
; a low byte and a high byte table.

baudl:	.byte	126	;300 bps (1)
	.byte	94	;1200 (2)
	.byte	46	;2400 (3)
	.byte	22	;4800 (4)
	.byte	10	;9600 (5)
	.byte	4	;19200 (6)
	.byte	1	;38400 (7)
	.byte	0	;57600 (8)

baudh:	.byte	1	;300 bps (1)
	.byte	0	;1200 (2)
	.byte	0	;2400 (3)
	.byte	0	;4800 (4)
	.byte	0	;9600 (5)
	.byte	0	;19200 (6)
	.byte	0	;38400 (7)
	.byte	0	;57600 (8)

;---------------------------------------------------------
; resetzgs - Clean up SCC every time we hit the main loop
;---------------------------------------------------------
resetzgs:
	lda	gscmdb	; READ TO RESET channelB POINTER TO 0
	rts

;---------------------------------------------------------
; PATCHZGS - Patch the entry point of putc and getc over
;           to the IIgs versions
;---------------------------------------------------------
patchzgs:
	lda	#<zccp
	sta	putc+1
	lda	#>zccp
	sta	putc+2

	lda	#<zccg
	sta	getc+1
	lda	#>zccg
	sta	getc+2

	lda	#<resetzgs
	sta	resetio+1
	lda	#>resetzgs
	sta	resetio+2

	rts

;---------------------------------------------------------
; Default SCC baud rate
;---------------------------------------------------------
baud:	.byte 6	;1=300, 2=1200, 3=2400
		;4=4800, 5=9600, 6=19200
		;7=38400, 8=57600.

;---------------------------------------------------------
; Apple IIgs SCC Z8530 registers and constants
;---------------------------------------------------------

gscmdb		=	$C038
gsdatab		=	$C03A

gscmda		=	$C039
gsdataa		=	$C03B

reseta		=	%11010001	; constant to reset Channel A
resetb		=	%01010001	; constant to reset Channel B
wr11a		=	%11010000	; init wr11 in Ch A
wr11bxtal	=	%00000000	; init wr11 in Ch B - use external clock
wr11bbrg	=	%01010000	; init wr11 in Ch B - use baud rate generator
