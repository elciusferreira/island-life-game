all:
	clear && mkdir -p build && cd build && cmake -DCMAKE_C_COMPILER=/usr/bin/clang-3.8 -DCMAKE_CXX_COMPILER=/usr/bin/clang++-3.8 .. && make

run:
	./build/PROGRAM
clean:
	rm -rf ./build/
