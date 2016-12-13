SNOWGOOSE=../snowgoose
COMPI=../COMPI
OPTITEST=../OPTITEST
#LIBJSON=$(SNOWGOOSE)/libjson
NUC=../NUC
EXTRA_INC = -I$(SNOWGOOSE) -I$(COMPI) -I$(OPTITEST) -I$(NUC)

all: solverJSON.cpp JSONSerializer.o
	c++ -std=c++11 $(EXTRA_INC) -g -c solverJSON.cpp
	c++ -o solverJSON solverJSON.o  JSONSerializer.o ../snowgoose/libjson/libjson.a -lm -ldl -lpthread

JSONSerializer.o: JSONSerializer.cpp JSONSerializer.hpp
	g++ $(EXTRA_INC) -std=c++11 --static -c JSONSerializer.hpp JSONSerializer.cpp

clean:
	rm *.o *.gch solverJSON