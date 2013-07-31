all:
	mkdir -p build
	cd build && cmake .. && make
	cp -r build/carvr.app .

clean:
	rm -rf build carvr.app
