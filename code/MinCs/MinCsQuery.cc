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
/*
 * partition space into m grids, encrypt points in each grid for nn query 
 * */
using namespace std;
using namespace CryptoPP;

#define BOUNDER	100005
struct Point
{
	double x;
	double y;
};

void printCell(vector<vector<Point> > &rslt)
{
	for(unsigned int i=0;i<rslt.size();i++)
	{
		vector<Point> t=rslt[i];
		cout<<t.size();
		for(unsigned int j=0;j<t.size();j++)
		{
			cout<<" "<<t[j].x<<" "<<t[j].y;
		}
		cout<<endl;
	}
}
void getNN(Point cor,vector<Point> & data, ofstream& fout)
{
	Point rslt;
	rslt.x=-1;
	rslt.y=-1;
	double dist=numeric_limits<double>::max();
	if(data.size()>0)
	{
		dist=pow(cor.x-data[0].x,2.0)+pow(cor.y-data[0].y,2.0);
		rslt.x=data[0].x;
		rslt.y=data[0].y;
		for(unsigned int i=1;i<data.size();i++)
		{
			double d=pow(cor.x-data[i].x,2.0)+pow(cor.y-data[i].y,2.0);
			if(d<dist) 
			{	
				dist=d;
				rslt.x=data[i].x;
				rslt.y=data[i].y;
			}
		}
	}
	fout<<rslt.x<<" "<<rslt.y<<endl;
}
int main(int argc, char** argv)
{
	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " 1-grid_file 2-m (num of grid) 3-query_file 4-query_rslt" << endl;
		return -1;
	}
	double upperbd=BOUNDER;
	double lowerbd=0;
	int m=atoi(argv[2]);
	int sqrm=ceil(sqrt(m));
	double cellLen= (upperbd-lowerbd)/sqrm;
	vector<vector<Point> > cellCollection;
	for(int i=0;i<m;i++)
	{
		vector<Point> p;
		cellCollection.push_back(p);
	}
	ifstream fin(argv[1]);
	if (! fin)
	{
		cerr << "Cannot open data file " << argv[1] << "." << endl;
		return -1;
	}
	string passPhrase="123456";
        dataowner owner;
	int num;//number of points in a cell
	Point cor;
	int j=0;
	MTimer mt0,mt1;
	while(fin)
	{
		fin>>num;
		if(!fin.good()) break;
		for(int i=0;i<num;i++)
		{
			fin>>cor.x;
			fin>>cor.y;
			cellCollection[j].push_back(cor);
		}
		j++;
	}
	fin.close();
	vector<Point> t=cellCollection[0];
        string s=IntToString(t.size());
        char s1[14];
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
        mt0.go();   //mt0: timer for decryption process
        string decrypted = owner.DEADecryptString(ciphertext.c_str(), passPhrase.c_str());
        stringstream sstr;
        sstr << decrypted;
        sstr >> num;
	for(int i=0;i<num;i++)
	{
		sstr>>cor.x;
		sstr>>cor.y;
	}
	mt0.stop();
        mt0.update();
	ifstream qfin(argv[3]);
	if (! qfin)
	{
		cerr << "Cannot open query file " << argv[3] << "." << endl;
		return -1;
	}
	ofstream fout(argv[4]);
	mt1.go();
	while(qfin >>cor.x>>cor.y)
	{
		int i=floor(cor.y/cellLen)*sqrm+floor(cor.x/cellLen);
		getNN(cor,cellCollection[i],fout);
	}
	mt1.stop();
	mt1.update();
        printf("%15.7f\n",mt0.elapsed+mt1.elapsed/100);
	qfin.close();
	fout.close();
	return 0;
}
