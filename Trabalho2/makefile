all: VCubeV1 VCubeV2

VCubeV1: VCubeV1.o smpl.o rand.o cisj.o
	$(LINK.c) -o $@ -Bstatic VCubeV1.o smpl.o rand.o cisj.o -lm
	
VCubeV2: VCubeV2.o smpl.o rand.o cisj.o
	$(LINK.c) -o $@ -Bstatic VCubeV2.o smpl.o rand.o cisj.o -lm
	
cisj_main: cisj_main.o cisj.o
	$(LINK.c) -o $@ -Bstatic cisj_main.o cisj.o -lm
	
smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

VCubeV1.o: VCubeV1.c smpl.h
	$(COMPILE.c) -g  VCubeV1.c
	
VCubeV2.o: VCubeV2.c smpl.h
	$(COMPILE.c) -g  VCubeV2.c
	
rand.o: rand.c
	$(COMPILE.c) -g rand.c

cisj.o: cisj.c cisj.h
	$(COMPILE.c)  -g cisj.c
	
cisj_main.o: cisj_main.c
	$(COMPILE.c)  -g cisj_main.c
	
clean:
	$(RM) *.o VCubeV1 VCubeV2 relat saida