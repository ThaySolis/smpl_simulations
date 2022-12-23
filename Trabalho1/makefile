all: VRing

VRing: VRing.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic VRing.o smpl.o rand.o -lm
	
smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

VRing.o: VRing.c smpl.h
	$(COMPILE.c) -g  VRing.c
	
rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o VRing relat saida

