all: BEBCVCube

BEBCVCube: BEBCVCube.o smpl.o rand.o cisj.o token_agrupada.o int_set.o
	$(LINK.c) -o $@ -Bstatic BEBCVCube.o smpl.o rand.o cisj.o token_agrupada.o int_set.o -lm
	
cisj_main: cisj_main.o cisj.o
	$(LINK.c) -o $@ -Bstatic cisj_main.o cisj.o -lm
	
smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c
	
token_agrupada.o: token_agrupada.c token_agrupada.h
	$(COMPILE.c)  -g token_agrupada.c
	
int_set.o: int_set.c int_set.h
	$(COMPILE.c)  -g int_set.c

BEBCVCube.o: BEBCVCube.c smpl.h
	$(COMPILE.c) -g  BEBCVCube.c
	
rand.o: rand.c
	$(COMPILE.c) -g rand.c

cisj.o: cisj.c cisj.h
	$(COMPILE.c)  -g cisj.c
	
cisj_main.o: cisj_main.c
	$(COMPILE.c)  -g cisj_main.c
	
clean:
	$(RM) *.o BEBCVCube relat saida