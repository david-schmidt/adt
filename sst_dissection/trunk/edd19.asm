   1          ORG $1D00
   2          OBJ $3D00
   3 READ     EQU $19C0
   4 WRITE    EQU $1A04
   5 ORSLT    EQU $B280
   6 DPSLT    EQU $B282
   7 ORDRV    EQU $B281
   8 DPDRV    EQU $B283
   9 DCOUNT   EQU $B285
  10 TBEG     EQU $B288
  11 TEND     EQU $B289
  12 TSTEP    EQU $B28A
  13 NIBFLG   EQU $B28D
  14 SYNFLG   EQU $B28C
  15 PARM     EQU $B300
  16 PARM1    EQU $2500
  17 DRV      EQU $B7EA
  18 SLT      EQU $B7E9
  19 TRK      EQU $B7EC
  20 SEC      EQU $B7ED
  21 BUFHI    EQU $B7F1
  22 BUFLO    EQU $B7F0
  23 CMD      EQU $B7F4
  24 VOL      EQU $B7EB
  25 CLBOT    EQU $1610
  26 XCHANGE  LDA #$B7
  27          STA $3D
  28          LDA #$D7
  29          STA $3F
  30          LDA #$0
  31          STA $3C
  32          STA $3E
  33          TAY 
  34          LDA #$C0
  35          STA $42
  36          JMP XCH1
  37 PUP      LDA #$78
  38          STA $3D
  39          LDA #$94
  40          STA $3F
  41          LDA #$0
  42          STA $3C
  43          STA $3E
  44          TAY 
  45          RTS 
  46 UNPACK   JSR PUP
  47 UPCK1    LDA ($3C),Y
  48          CMP #$80
  49          BLT SETBIT
  50          LDA #$0
  51          JMP STRBIT
  52 SETBIT   EOR #$80
  53          STA ($3C),Y
  54          LDA #$1
  55 STRBIT   STA ($3E),Y
  56          INY 
  57          BNE UPCK1
  58          INC $3D
  59          INC $3F
  60          LDA $3F
  61          CMP #$B0
  62          BNE UPCK1
  63          RTS 
  64 CLMID    LDA #$C
  65          STA $22
  66          JSR $FC58
  67          RTS 
  68 CHKDSK   LDY #$0
  69 CHK1     LDA $B230,Y
  70          CMP DSKCMP,Y
  71          BNE BAD
  72          INY 
  73          CPY #$5
  74          BNE CHK1
  75          CLC 
  76          JMP GOOD
  77 BAD      JSR CLBOT
  78          JSR SETCRS
  79          LDA SIDE
  80          STA SIDEA
  81          LDY #$0
  82 BAD1     LDA WDSK,Y
  83          CMP #$FF
  84          BEQ BAD2
  85          JSR $FDED
  86          INY 
  87          JMP BAD1
  88 BAD2     JSR TDO2
  89          LDA TRK1
  90          JMP RETRY
  91 WDSK     ASC " WRONG DISK!!!! Insert data disk side "
  92 SIDEA    HEX B18D
  93          ASC " and press return."
  94          HEX FF
  95 GOOD     JMP RETIT
  96 RWTS     LDA #$B1
  97          STA SIDE
  98          LDA TRK
  99          CMP #$23
 100          BLT RWTS2
 101          CMP #$24
 102          BGE RWTS1
 103          JSR TDO
 104          JMP RWTS0
 105 RWTS1    LDA #$B2
 106          STA SIDE
 107 RWTS0    LDA TRK
 108          SEC 
 109          SBC #$23
 110          STA TRK
 111 RWTS2    LDA $C000
 112          CMP #$9B
 113          BEQ RWTS3
 114          LDA #$B7
 115          LDY #$E8
 116          JSR $BD00
 117          BCS RWTS2
 118          RTS 
 119 RWTS3    SEC 
 120          RTS 
 121 READINT  LDA FLAG
 122          CMP #$2
 123          BEQ MYREAD
 124          JMP MREAD
 125 WRITINT  LDA FLAG
 126          CMP #$1
 127          BEQ MYWRITE
 128          JMP MWRITE
 129 MYREAD   BIT $C010
 130          JSR XCHANGE
 131          LDA ORDRV
 132          STA DRV
 133          LDA ORSLT
 134          ASL 
 135          ASL 
 136          ASL 
 137          ASL 
 138          STA SLT
 139          LDA #$1
 140          STA CMD
 141          JSR RWPROG
 142          JMP LEAVE
 143 MYWRITE  JSR XCHANGE
 144          LDA DPSLT
 145          ASL 
 146          ASL 
 147          ASL 
 148          ASL 
 149          STA SLT
 150          LDA DPDRV
 151          STA DRV
 152          JSR PACK
 153          LDA #$2
 154          STA CMD
 155          JSR RWPROG
 156          JMP LEAVE
 157 RWPROG   LDA ORSLT
 158          STA ORSLT1
 159          LDA DPSLT
 160          STA DPSLT1
 161          LDA ORDRV
 162          STA ORDRV1
 163          LDA DPDRV
 164          STA DPDRV1
 165          LDA DCOUNT
 166          STA DCOUNT1
 167          LDA $B28B
 168          STA TRK1
 169          LDA VAR2
 170          BEQ RILBEG
 171          LDA NIBFLG
 172          STA NIBFLG1
 173          LDA SYNFLG
 174          STA SYNFLG1
 175          LDY #$0
 176 SPARM    LDA PARM,Y
 177          STA PARM1,Y
 178          INY 
 179          BNE SPARM
 180 RILBEG   LDA TBEG
 181          STA TBEG1
 182          LDA TEND
 183          STA TEND1
 184          LDA TSTEP
 185          STA TSTEP1
 186 RETRY    LDA TRK1
 187          LSR 
 188          STA TRK
 189          LDA #$78
 190          STA BUFHI
 191          LDA #$00
 192          STA BUFLO
 193          STA VOL
 194          LDA TRK1
 195          CMP #$8C
 196          BLT T34
 197          JMP T35
 198 T34      LDA #$F
 199          STA SEC
 200 RWLOOP   JSR RWTS
 201          INC BUFHI
 202          LDA BUFHI
 203          CMP #$94
 204          BEQ RWDONE
 205          DEC SEC
 206          BPL RWLOOP
 207          LDA #$F
 208          STA SEC
 209          INC TRK
 210          JMP RWLOOP
 211 RWDONE   DEC SEC
 212          LDA #$B1
 213          STA BUFHI
 214          JSR RWTS2
 215          DEC SEC
 216          INC BUFHI
 217          JSR RWTS2
 218          DEC SEC
 219          INC BUFHI
 220          JSR RWTS2
 221          BCC REB
 222          JSR XCHANGE
 223          RTS 
 224 REB      JMP CHKDSK
 225 RETIT    LDA ORSLT
 226          CMP DPSLT
 227          BNE DONE
 228          LDA ORDRV
 229          CMP DPDRV
 230          BNE DONE
 231          LDA TRK
 232          ASL 
 233          ASL 
 234          STA $12
 235          JSR XCHANGE
 236          LDA $B28B
 237          JSR $B87D
 238          JMP DONER
 239 DONE     JSR XCHANGE
 240 DONER    LDA FLAG
 241          CMP #$2
 242          BNE DONE2
 243          JSR UNPACK
 244          LDA VAR3
 245          CMP #$FF
 246          BEQ DONE2
 247          LDA $B28B
 248          SEC 
 249          SBC $B28A
 250          STA $B287
 251 DONE2    LDA $B28B
 252          STA $12
 253          LDA #$00
 254          STA VAR3
 255          JSR CLBOT
 256          LDA ORDRV1
 257          STA ORDRV
 258          LDA ORSLT1
 259          STA ORSLT
 260          LDA DPSLT1
 261          STA DPSLT
 262          LDA DPDRV1
 263          STA DPDRV
 264          LDA DCOUNT1
 265          STA DCOUNT
 266          LDA TBEG1
 267          STA TBEG
 268          LDA TEND1
 269          STA TEND
 270          LDA TSTEP1
 271          STA TSTEP
 272          LDA VAR2
 273          BEQ RILDON
 274          LDA NIBFLG1
 275          STA NIBFLG
 276          LDA SYNFLG1
 277          STA SYNFLG
 278          LDY #$0
 279 RPARM    LDA PARM1,Y
 280          STA PARM,Y
 281          INY 
 282          BNE RPARM
 283 RILDON   RTS 
 284 DSKCMP   HEX 1027E80364
 285 CHOOSE   JSR CLMID
 286          LDA #$FF
 287          STA VAR3
 288          STA $B286
 289          LDY #$0
 290          BIT $C010
 291 CH1      LDA CHTXT,Y
 292          CMP #$FF
 293          BEQ CH2
 294          JSR $FDED
 295          INY 
 296          JMP CH1
 297 CH2      LDA $C000
 298          BPL CH2
 299          CMP #$9B
 300          BNE CHH
 301          RTS 
 302 CHH      CMP #$B1
 303          BLT CH2
 304          CMP #$B4
 305          BGE CH2
 306          SEC 
 307          SBC #$B1
 308          STA FLAG
 309          CMP #$2
 310          BNE CH5
 311          JSR $FC58
 312          BIT $C010
 313          LDY #$00
 314 CH3      LDA PARMTXT,Y
 315          CMP #$FF
 316          BEQ CH4
 317          JSR $FDED
 318          INY 
 319          JMP CH3
 320 CH4      LDA $C000
 321          BPL CH4
 322          CMP #$9B
 323          BNE CHI
 324          RTS 
 325 CHI      CMP #$B1
 326          BLT CH4
 327          CMP #$B3
 328          BGE CH4
 329          SEC 
 330          SBC #$B1
 331          STA VAR2
 332          JMP CH6
 333 CH5      LDA #$0
 334          STA VAR2
 335 CH6      BEQ ZYX
 336          JSR XCHANGE
 337          JSR $A00
 338          JSR XCHANGE
 339 ZYX      BIT $C010
 340 CHTXT    ASC "             1) Copy                                 2) Pack                                 3) Unpack"
 341          HEX FF
 342 PARMTXT  ASC "             1) Packed parms                         2) Your parms"
 343          HEX FF
 344 INITEXT  ASC "    <O>riginal or <D>uplicate drive?"
 345          HEX FF
 346 PAK      ASC "        Press a key when ready"
 347          HEX FF
 348 CLR      ASC "                                                                               "
 349          HEX FF
 350 TDO      JSR SETCRS
 351          LDY #$0
 352 TDO1     LDA TDOTXT,Y
 353          CMP #$20
 354          BEQ TDO2
 355          JSR $FDED
 356          INY 
 357          JMP TDO1
 358 TDO2     BIT $C010
 359 TDO3     LDA $C000
 360          BPL TDO3
 361          JSR CLBOT
 362          RTS 
 363 TDOTXT   ASC "  Turn DATA disk over and press RETURN"
 364 INIT     JSR CLBOT
 365          JSR XCHANGE
 366          BIT $C010
 367          JSR SETCRS
 368          LDY #$0
 369 PINIT    LDA INITEXT,Y
 370          CMP #$FF
 371          BEQ INIT2
 372          JSR $FDED
 373          INY 
 374          JMP PINIT
 375 INIT2    LDA $C000
 376          BPL INIT2
 377          BIT $C010
 378          CMP #"O"
 379          BEQ INORG
 380          CMP #"D"
 381          BEQ INDUP
 382          JMP INIT2
 383 INORG    LDA ORDRV
 384          STA $B7EA
 385          LDA ORSLT
 386          ASL 
 387          ASL 
 388          ASL 
 389          ASL 
 390          STA $B7E9
 391          JMP INITIT
 392 INDUP    LDA DPDRV
 393          STA $B7EA
 394          LDA DPSLT
 395          ASL 
 396          ASL 
 397          ASL 
 398          ASL 
 399          STA $B7E9
 400 INITIT   BIT $C010
 401          JSR CLBOT
 402          JSR SETCRS
 403          LDY #$0
 404 INITIT1  LDA PAK,Y
 405          CMP #$FF
 406          BEQ INITIT2
 407          JSR $FDED
 408          INY 
 409          JMP INITIT1
 410 INITIT2  LDA $C000
 411          BPL INITIT2
 412          BIT $C010
 413          LDA #$0
 414          STA $B7EB
 415          LDA #$4
 416          STA CMD
 417          JSR RWTS
 418          JSR CLBOT
 419          JSR XCHANGE
 420          RTS 
 421 SETCRS   LDA #$0
 422          STA $24
 423          LDA #$16
 424          STA $25
 425          JMP $FC22
 426 PACK     JSR PUP
 427 PCK1     LDA ($3E),Y
 428          ASL 
 429          ASL 
 430          ASL 
 431          ASL 
 432          ASL 
 433          ASL 
 434          ASL 
 435          EOR ($3C),Y
 436          STA ($3C),Y
 437          INY 
 438          BNE PCK1
 439          INC $3D
 440          INC $3F
 441          LDA $3F
 442          CMP #$B0
 443          BNE PCK1
 444          RTS 
 445 T35      JSR TDO
 446          LDA #$0
 447          STA SEC
 448          LDA #$1
 449          STA TRK
 450          LDA #$B2
 451          STA SIDE
 452 T35LP    JSR RWTS
 453          BCC T35LP1
 454          RTS 
 455 T35LP1   INC TRK
 456          INC TRK
 457          INC BUFHI
 458          LDA BUFHI
 459          CMP #$94
 460          BNE T35LP
 461          LDA #$B1
 462          STA BUFHI
 463          JSR RWTS
 464          INC BUFHI
 465          INC TRK
 466          INC TRK
 467          JSR RWTS
 468          INC BUFHI
 469          INC TRK
 470          INC TRK
 471          JSR RWTS
 472          JMP REB
 473          ORG $B500
 474          OBJ $4500
 475 CH       JSR MOVE
 476          JSR CHOOSE
 477          JSR MOVE
 478          JMP $1929
 479          RTS 
 480 IN       JSR MOVE
 481          JSR INIT
 482          JSR MOVE
 483          RTS 
 484 R        JSR MOVE
 485          JMP READINT
 486 W        JSR MOVE
 487          JMP WRITINT
 488 LEAVE    JSR MOVE
 489          RTS 
 490 MOVE     LDA #$E0
 491          STA $3D
 492          LDA #$1D
 493          STA $3F
 494          LDA #$0
 495          STA $3C
 496          STA $3E
 497          LDA #$F5
 498          STA $42
 499 XCH1     BIT $C083
 500          BIT $C083
 501          LDY #$0
 502 XCH2     LDA ($3C),Y
 503          STA VAR1
 504          LDA ($3E),Y
 505          STA ($3C),Y
 506          LDA VAR1
 507          STA ($3E),Y
 508          INY 
 509          BNE XCH2
 510          INC $3D
 511          INC $3F
 512          LDA $3D
 513          CMP $42
 514          BNE XCH2
 515          BIT $C081
 516          RTS 
 517 MREAD    JSR MOVE
 518          JMP READ
 519 MWRITE   JSR MOVE
 520          JMP WRITE
 521 VAR1     DFS 1
 522 VAR2     DFS 1
 523 VAR3     DFS 1
 524 FLAG     DFS 1
 525 ORSLT1   DFS 1
 526 DPSLT1   DFS 1
 527 ORDRV1   DFS 1
 528 DPDRV1   DFS 1
 529 DCOUNT1  DFS 1
 530 TBEG1    DFS 1
 531 TEND1    DFS 1
 532 TSTEP1   DFS 1
 533 NIBFLG1  DFS 1
 534 SYNFLG1  DFS 1
 535 SIDE     DFS 1
 536 TRK1     DFS 1
 537 NUMERR   DFS 1
 538          END 
