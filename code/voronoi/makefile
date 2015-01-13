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
all: dataPro 0504dataPro

dataPro: dataPro.h dataPro.cpp 
	g++ $(CFLAGS) -o dataPro.exe dataPro.cpp dataPro.h 
0504dataPro: dataPro.h 0504dataPro.cpp 
	g++ $(CFLAGS) -o 0504dataPro.exe 0504dataPro.cpp dataPro.h 

clean:
	rm *.o *.stackdump *.exe
