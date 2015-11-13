.SILENT:
AT=ApplicMateriel
LT=LibrairieVillages
ST=ServeurVillages
OBJS=LibrairieVillages.o
CPP=c++ -Wno-deprecated -I$(AT) -I$(LT) -I$(ST)
all:	$(OBJS)	Main ApplicMateriel.out ServeurVillages.out

LibrairieVillages.o:	$(LT)/LibrairieVillages.cxx $(LT)/LibrairieVillages.h
	echo Creation de LibrairieVillages.o ...
	$(CPP) $(LT)/LibrairieVillages.cxx -c

Main:	LibrairieVillages.o $(LT)/Main.cxx
	echo Creation de Main ...
	$(CPP) $(LT)/Main.cxx $(OBJS) -o Main

ApplicMateriel.out: $(AT)/ApplicMateriel.cxx $(AT)/ApplicMateriel.h
	echo Creation de ApplicMateriel ...
	$(CPP) $(AT)/ApplicMateriel.cxx $(OBJS) -lpthread -D_REENTRANT -lnsl -o ApplicMateriel.out

ServeurVillages.out:        $(ST)/ServeurVillages.cxx $(ST)/ServeurVillages.h
	echo Creation de ServeurVillages ...
	$(CPP)  $(ST)/ServeurVillages.cxx $(OBJS) -lpthread -D_REENTRANT -lnsl -o ServeurVillages.out

clean:
	rm -f $(OBJS) core Main ApplicMateriel.out ServeurVillages.out

clobber:	clean
	rm -f tags $(PROGRAM) *.log
