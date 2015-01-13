#ifndef _DATAPRO
#define _DATAPRO
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

#define BORDER 100005
struct bentry{
	double maxX;
	double maxY;
	double minX;
	double minY;
};

class dataPro{
	public:
	vector<pair<double,double> > dataset; 
	vector<bentry> border; 
	//vector<vector<int> > regions; 
	vector<pair<double,double> > vertex;
	//map<pair<int,int>,vector<int> > edges;
	//vector<vector<int> > adjv;	//adjacent vertex	
	public:
	dataPro(){};
	int Process(char*);
};
#endif
