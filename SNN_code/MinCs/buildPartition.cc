#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <list>
#include <set>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "operation.h"
#include "../MTimer.h"
#include "../voronoi/dataPro.h"
/*
 * partition space into m grids, encrypt points in each grid for nn query 
 * */
using namespace std;
using namespace CryptoPP;	
#define BORDER 100005
struct Point
{
	double x;
	double y;
};
typedef vector<Point> Cell;

/*
 * fill all the partitions with(-1,-1)to make them have equal size for security
 * and print
 **/
void printCell(vector<vector<Point> > &rslt, char* filename, int max)
{
	ofstream fout(filename);
	int n=0;
	for(unsigned int i=0;i<rslt.size();i++)
	{
		vector<Point> t=rslt[i];
		fout<<max<<endl;
		for(unsigned int j=0;j<t.size();j++)
		{
			fout<<t[j].x<<" "<<t[j].y<<endl;
		}
		n=max-t.size();
		for(unsigned int j=0;j<n;j++)    
			fout<<"-1 -1"<<endl;
	}
	fout.close();
}

int main(int argc, char** argv)
{
	
	if (argc != 4)
	{
		cerr << "Usage: " << argv[0] << " 1-data_file 2-m (#partition, a square num) 3-out_grid_file" << endl;
		return -1;
	}
	MTimer mt;
	mt.go();
	dataPro dp;
	dp.Process(argv[1]);
        mt.stop();
        mt.update();
        double ve=mt.elapsed;      //ve: voronoi+preprocessing time
	double upperbd=	BORDER;
	double lowerbd=0;
	int m=atoi(argv[2]);      //m: #partition 
	cout << m<<" ";
	int sqrm=ceil(sqrt(m));
	double cellLen= (upperbd-lowerbd)/sqrm;
	vector<vector<Point> > cellCollection;
	for(int i=0;i<m;i++)
	{
		vector<Point> p;
		cellCollection.push_back(p);
	}	
	MTimer mt1;              //mt1: timer for the grid partition process
	mt1.go();
	for(int j=0;j<dp.dataset.size();++j)
	{
		Point cor;
		pair<double,double> p =dp.dataset[j];
		cor.x =p.first;
		cor.y =p.second;
		double MaxX =dp.border[j].maxX;
		double MinX =dp.border[j].minX;
		double MaxY =dp.border[j].maxY;
		double MinY =dp.border[j].minY;
		int IndX1 =ceil(MaxX/cellLen);
		int IndX0 =floor(MinX/cellLen);
		int IndY1 =ceil(MaxY/cellLen);
		int IndY0 =floor(MinY/cellLen);
		for(int ii=IndY0;ii<IndY1;++ii)
			for(int l=IndX0;l<IndX1;++l)
				cellCollection[ii*sqrm+l].push_back(cor);
	}
        mt1.stop();
	mt1.update();
	int size_Max=cellCollection[0].size();                              
	int Min=cellCollection[0].size();                              
	int sum=cellCollection[0].size();                              
        for(unsigned int i=1;i<cellCollection.size();i++)                   
	{
		if(cellCollection[i].size()<Min) Min=cellCollection[i].size();
		if(cellCollection[i].size()>size_Max) size_Max=cellCollection[i].size();
	 	sum+=cellCollection[i].size();		
	}        
	int n;                                                              
        Point p;                                                            
        p.x=p.y=-1;
                n=size_Max-cellCollection[0].size();
                for(unsigned int j=0;j<n;j++)
                        cellCollection[0].push_back(p);
	printCell(cellCollection,argv[3],size_Max);
	vector<Point> t=cellCollection[0];
	string s=IntToString(t.size());
	char s1[14];
	MTimer mt2;       //mt2: timer for encryption process(one partition)
        string passPhrase="123456";
        dataowner owner;
        mt2.go();
	for(int i=0;i<t.size();i++)
	{
		sprintf(s1,"%013.6f",t[i].x);
		s.append(" ");
		s.append(s1);
		sprintf(s1,"%013.6f",t[i].y);
		s.append(" ");
		s.append(s1);
	}
	string ciphertext = owner.DEAEncryptString(s.c_str(), passPhrase.c_str());
	mt2.stop();
        mt2.update();
	printf("%15.7f %15.7f %10d %10d %10d %14d %14d\n",ve+mt1.elapsed,mt2.elapsed*cellCollection.size(),sum/cellCollection.size(),Min,size_Max,size_Max*cellCollection.size(),dp.dataset.size()); //print: preprocessing_time,process_time,encryption_time,average_size,min_size,max_size,
                                                     //total size(=max_size*#partition),effctive data size
	return 0;
}
