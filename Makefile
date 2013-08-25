all:
	mkdir -p build
	cd build && cmake .. && make
	cp -r build/carvr.app .
	rm -rf carvr
	echo "#\!/bin/sh\n`pwd`/build/carvr.app/Contents/MacOS/carvr" > carvr
	chmod a+x carvr

clean:
	rm -rf build carvr.app carvr

check:
	cppcheck --enable=all -q *.cc *.h

upload:
	zip -r carvr.zip carvr.app
	scp carvr.zip mkeeter@mattkeeter.com:~/mattkeeter.com/projects/carvr
