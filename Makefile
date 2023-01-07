ROSIDIR		= `basename $(PWD) /`
SUBDIRS		= detectors simDataContainer phantoms sources
WINSUBDIRS	= control statusServer statusClient 
EXAMPLES	= example example2

all:
	for i in $(SUBDIRS) ; do (cd $$i && $(MAKE)) ; done
	for i in $(WINSUBDIRS) ; do (cd $$i && $(MAKE)) ; done
	for i in $(EXAMPLES) ; do (cd $$i && $(MAKE)) ; done

win: 
	for i in $(SUBDIRS) ; do (cd $$i && $(MAKE)) ; done
	for i in $(WINSUBDIRS) ; do (cd $$i && $(MAKE) win) ; done
	for i in $(EXAMPLES) ; do (cd $$i && $(MAKE)) ; done

clean:
	for i in $(SUBDIRS) $(WINSUBDIRS) $(EXAMPLES); \
	do (cd $$i && $(MAKE) clean && rm Makefile) ; done

tarball: clean
	@rm -f ravarLUT/*
	@rm -f example/detector.dat
	@rm -f example/Rosi.h
	@rm -f example/rosi.cpp
	@find ./ -name "*~" -exec rm {} \;
	@tar -C .. --exclude=$(ROSIDIR)/lscatPegs4/* \
	-czf ../rosi-`cat VERSION`.tar.gz $(ROSIDIR)

love:
	@echo not war!

.PHONY: all clean tarball love

