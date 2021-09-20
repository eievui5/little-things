all: bin/str-gradient

clean:
	rm -rf bin/

rebuild:
	$(MAKE) clean
	$(MAKE) all

bin/str-gradient: str-gradient.cpp
	@mkdir -p $(@D)
	g++ -O3 -o $@ $^
