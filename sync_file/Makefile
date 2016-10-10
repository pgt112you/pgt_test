CURPATH=$(shell pwd)

all : syncfile 
.PHONY : all

syncfile : tool.o config.o sync_file.o
	g++ -g -lpthread -lcrypto config.o sync_file.o tool.o /usr/local/lib/liblog4cpp.a -I/usr/local/include -o syncfile

tool.o : tool.cpp tool.h
	g++ -c -g tool.cpp -DSYNC_LOG=\"${CURPATH}/log/\" -DLOCKFILE=\"${CURPATH}/pid/syncfile.pid\" -o tool.o

config.o : config.cpp config.h
	g++ -c -g config.cpp -DCONF_FILE=\"${CURPATH}/conf/syncfile.conf\" -o config.o

sync_file.o : sync_file.cpp tool.h config.h
	g++ -c -g sync_file.cpp -DCONF_FILE=\"${CURPATH}/conf/syncfile.conf\" -o sync_file.o

clean : 
	-rm *.o syncfile

