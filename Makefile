all:
	mkdir -p build
	cd build && cmake .. && make
	cp build/carvr .

clean:
	rm -rf build carvr
