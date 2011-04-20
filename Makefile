all:
	cd src/shs; $(MAKE) all
	cd src/examples; $(MAKE) all

install:
	cd src/shs; $(MAKE) install
	cd src/examples; $(MAKE) install

clean:
	cd src/shs; $(MAKE) clean
	cd src/examples; $(MAKE) clean

