all:
	cd src/libshs; $(MAKE) all
	cd src/examples; $(MAKE) all

install:
	cd src/libshs; $(MAKE) install
	cd src/examples; $(MAKE) install

clean:
	cd src/libshs; $(MAKE) clean
	cd src/examples; $(MAKE) clean

