
INFO = SST.info

DA65 = da65 -i $(INFO)
C3 = --comments 3
DECODE_TXT = ./decode_txt.rb
DEP = $(INFO) $(DECODE_TXT) Makefile
MARKDOWN = markdown

GENERATED_SRC = SST.asm SST_B00.asm SST_B000.asm SST_D700.asm SST_DD00.asm

all: disassembly doc sst

disassembly: $(GENERATED_SRC)

SST.asm: SST_B00 $(DEP)
	$(DA65) $(C3) -S 0xB00 SST | $(DECODE_TXT) > SST.asm

SST_B00.asm: SST_B00 $(DEP)
	$(DA65) $(C3) -S 0xB00 SST_B00 | $(DECODE_TXT) > SST_B00.asm

SST_B000.asm: SST_B000 $(INFO)
	$(DA65) $(C3) -o SST_B000.asm -S 0xB000 SST_B000

SST_D700.asm: SST_D700 SST_D700.info $(DEP)
	da65 $(C3) -i SST_D700.info -S 0x1400 SST_D700 | $(DECODE_TXT) > SST_D700.asm

SST_DD00.asm: SST_DD00 SST_D700.info $(DEP)
	da65 -i SST_D700.info -S 0x1D00 SST_DD00 | $(DECODE_TXT) > SST_DD00.asm

doc: sst_doc.html

sst_doc.html: sst_doc.markdown
	$(MARKDOWN) sst_doc.markdown > sst_doc.html

sst: SSTCC65 SSTDLD

SSTCC65: sst_cc65
	cat SST_PREFIX sst_cc65 > SSTCC65

sst_cc65: sst_cc65.asm
	ca65 -l sst_cc65.asm
	ld65 -t none -o sst_cc65 sst_cc65.o 

SSTDLD: sst_new
	cat SST_PREFIX sst_new > SSTDLD

sst_new: sst_new.asm
	ca65 -l sst_new.asm
	ld65 -t none -o sst_new sst_new.o

sst_clean:
	$(RM) SSTCC65 SSTDLD sst_new sst_cc65 *.o *.lst

clean:
	$(RM) $(GENERATED_SRC) sst_doc.html

