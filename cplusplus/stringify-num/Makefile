all: bin/stringify-num

clean:
	rm -rf bin/

rebuild:
	$(MAKE) clean
	$(MAKE) all

bin/stringify-num: src/main.cpp src/stringify-num.cpp
	@mkdir -p $(@D)
	g++ -O3 -o $@ $^
