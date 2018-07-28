name=dwmst

LDFLAGS = -L/usr/lib -lX11
options = -g -Wall -Wextra -pedantic -std=c++14

sources=$(wildcard *.cpp )

default: $(name)
	
$(name): $(sources) 
	mkdir -p bin/
	g++ $(LDFLAGS) $(options) -o bin/$@ $^

install:	
	cp bin/dwmst /usr/local/bin/dwmst

clean:
	 rm -rf bin *.o
 
