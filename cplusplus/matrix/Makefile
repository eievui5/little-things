all: bin/matrix

clean:
	rm -rf bin/

rebuild:
	$(MAKE) clean
	$(MAKE) all

bin/matrix: matrix.cpp
	@mkdir -p $(@D)
	g++ -O3 -o $@ $^
