all: 
	$(MAKE) -C servlet
	$(MAKE) -C module
	$(MAKE) -C examples 

deploy: install
	../httpd/bin/apachectl stop
	sleep 1
	../httpd/bin/apachectl start

install: all
	$(MAKE) -C servlet $@
	$(MAKE) -C module $@
	$(MAKE) -C examples $@
clean:
	$(MAKE) -C servlet $@
	$(MAKE) -C module $@
	$(MAKE) -C examples $@
back: clean
	(cd .. && zip backups/mod_servlet-`date +%Y.%m.%d-%H.%M`.zip -r mod_servlet)
