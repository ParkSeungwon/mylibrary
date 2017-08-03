OBJ = $(patsubst %.cc, %.o, $(wildcard *.cc))

all : $(OBJ)
	ar rcs libzezeon.a *.o

%.o : %.cc
	g++ -c -std=c++14 $<

install : 
	cp *.h /usr/local/include
	cp libzezeon.a /usr/local/lib

clean : 
	rm *.o *.a
