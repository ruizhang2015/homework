CC       = g++
CCOPTS   = -c -O
LINK     = g++
LINKOPTS = -lm -O3
CFLAGS=-O3 -Wall
CGALFLAGS= $(CGAL_CXXFLAGS) $(LONG_NAME_PROBLEM_CXXFLAGS)
LIBPATH= $(CGAL_LIBPATH)
LDFLAGS = \
          $(LONG_NAME_PROBLEM_LDFLAGS) \
          $(CGAL_LDFLAGS)
all: MTimer.o dataPro.o operation.o buildPartition MinDupQuery

MTimer.o: ../MTimer.cpp ../MTimer.h
	g++ $(CFLAGS) -c ../MTimer.cpp -I../
operation.o: operation.cpp operation.h
	g++ $(CFLAGS) -c operation.cpp -I/research/cryptopp561/ 
dataPro.o: ../voronoi/dataPro.cpp ../voronoi/dataPro.h
	g++ $(CFLAGS) -c ../voronoi/dataPro.cpp -I../voronoi/
buildPartition: buildPartition.cc 
	g++ $(CFLAGS) -o buildPartition.exe buildPartition.cc operation.o MTimer.o dataPro.o  -I/research/cryptopp561/ -L/research/cryptopp561/ -lcryptopp
MinDupQuery : MinDupQuery.cc 
	g++ $(CFLAGS) -o MinDupQuery.exe MinDupQuery.cc operation.o MTimer.o -I/research/cryptopp561/ -L/research/cryptopp561/ -lcryptopp

clean:
	rm *.o *.stackdump *.exe
