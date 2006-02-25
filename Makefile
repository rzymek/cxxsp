export APACHE_HOME=/home/rzymek/devel/00cxxsp/httpd
export HTDOCS=$(APACHE_HOME)/htdocs

export CXX=g++ -ac -Wall -ansi -fPIC -march=athlon-xp -O3 -mmmx -m3dnow -msse -mfpmath=sse -pipe -funroll-loops -fomit-frame-pointer -pthread 
#export CXX=g++ -g -Wall -ansi -fPIC -march=athlon-xp -O3 -mmmx -m3dnow -msse -mfpmath=sse -pipe -funroll-loops -fomit-frame-pointer -pthread -finstrument-functions -lfnccheck 
#export CXX=g++ -g -O0 -Wall -ansi -pthread -pg

export BOOST_INC=$(PWD)/external/stripped-boost/
export BOOST_LIB=$(PWD)/external/stripped-boost/libs/libstripped-boost.a 
export FLEX_INC=$(PWD)/external/flex/include
export FLEX_LIB=$(PWD)/external/flex/libfl.a

all: 
	$(MAKE) -C external/stripped-boost/libs
	$(MAKE) -C external/flex
	$(MAKE) -C common
	$(MAKE) -C mod_servlet 
	$(MAKE) -C mod_cxxsp
	$(MAKE) -C store/cxxsp
install: all
	$(MAKE) -C common $@
	$(MAKE) -C mod_servlet $@
	$(MAKE) -C mod_cxxsp $@
	$(MAKE) -C store/cxxsp $@
deploy: install
	$(APACHE_HOME)/bin/apachectl stop
	killall httpd || true
	killall -9 httpd || true
	sleep 1
	$(APACHE_HOME)/bin/apachectl start
dbg: install
	killall httpd || true
	killall -9 httpd || true
	$(APACHE_HOME)/bin/httpd -X
clean:
	$(MAKE) -C mod_servlet $@
	$(MAKE) -C mod_cxxsp $@
	$(MAKE) -C store/cxxsp $@
	$(MAKE) -C common $@
	$(MAKE) -C external/flex $@
	#$(MAKE) -C external/stripped-boost/libs $@
back: clean
	zip backups/cxxsp-`date +%Y.%m.%d-%H.%M`.zip -r mod_cxxsp mod_servlet
