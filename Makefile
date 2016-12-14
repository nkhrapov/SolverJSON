SNOWGOOSE=../snowgoose
COMPI=../COMPI
OPTITEST=../OPTITEST
#LIBJSON=$(SNOWGOOSE)/libjson
NUC=../NUC
EXTRA_INC = -I$(SNOWGOOSE) -I$(COMPI) -I$(OPTITEST) -I$(NUC)

all: solverJSON.cpp JSONSerializer.hpp
	g++ -I../snowgoose -I../COMPI -I../OPTITEST -I../NUC -std=c++11 --static -c solverJSON.cpp
	g++ -o solverJSON solverJSON.o ../snowgoose/libjson/libjson.a -lm -ldl -lpthread
#	g++ -std=c++11 $(EXTRA_INC) -g -c solverJSON.cpp
#	g++ -o solverJSON solverJSON.o  JSONSerializer.o ../snowgoose/libjson/libjson.a -lm -ldl -lpthread

#JSONSerializer.o: JSONSerializer.cpp JSONSerializer.hpp
#	g++ $(EXTRA_INC) -std=c++11 --static -c JSONSerializer.hpp JSONSerializer.cpp

clean:
	rm *.o *.gch solverJSON
