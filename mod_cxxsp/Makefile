all: 
	$(MAKE) -C module
	$(MAKE) -C examples
	$(MAKE) -C cxxsp 
install: all
	$(MAKE) -C module $@
	$(MAKE) -C examples $@
	$(MAKE) -C cxxsp $@
clean:
	$(MAKE) -C module $@
	$(MAKE) -C examples $@
	$(MAKE) -C cxxsp $@
back: clean
	(cd .. && zip backups/mod_cxxsp-`date +%Y.%m.%d-%H.%M`.zip -r mod_cxxsp)
