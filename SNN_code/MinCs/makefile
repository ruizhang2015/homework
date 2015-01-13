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
all: dataPro.o MTimer.o operation.o buildPartition MinCsQuery

operation.o: operation.cpp operation.h
	g++ $(CFLAGS) -c operation.cpp -I/research/cryptopp561/ 
MTimer.o: ../MTimer.cpp ../MTimer.h
	g++ $(CFLAGS) -c ../MTimer.cpp -I../
dataPro.o: ../voronoi/dataPro.cpp ../voronoi/dataPro.h
	g++ $(CFLAGS) -c ../voronoi/dataPro.cpp -I../voronoi/
buildPartition: buildPartition.cc 
	g++ $(CFLAGS) -o buildPartition.exe buildPartition.cc operation.o MTimer.o dataPro.o -I/research/cryptopp561/ -L/research/cryptopp561/ -lcryptopp
MinCsQuery: MinCsQuery.cc 
	g++ $(CFLAGS) -o MinCsQuery.exe MinCsQuery.cc operation.o MTimer.o dataPro.o -I/research/cryptopp561/ -L/research/cryptopp561/ -lcryptopp

RTreeBulkLoad: RTreeBulkLoad.cc 
	$(LINK) -o  RTreeBulkLoad.exe RTreeBulkLoad.cc -L/home/yao/usr/lib -I/home/yao/usr/include/spatialindex -lpthread -lspatialindex

olqFindMinMaxParDivOTF: olqFindMinMaxParDivOTF.cc $(LINKLIBS) 
	$(LINK) -o olqFindMinMaxParDivOTF.exe olqFindMinMaxParDivOTF.cc $(LINKLIBS) $(CGALLIBS)

clean:
	rm *.o *.stackdump *.exe
