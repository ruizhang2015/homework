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
#include "../MTimer.h"
#include "operation.h"
/*
 * partition space with m splits, encrypt points in each grid for nn query 
 * */
using namespace CryptoPP;
using namespace std;
struct Point
{
	double x;
	double y;
};
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
		cerr << "Usage: " << argv[0] << " 1-data_file 2-grid_file 3-query_file 4-rslt_file" << endl;
		return -1;
	}
	vector<vector<Point> > cells;
	ifstream fin(argv[1]);
	if (! fin)
	{
		cerr << "Cannot open data file " << argv[1] << "." << endl;
		return -1;
	}
        MTimer mt0,mt1;
	int num;//number of points in a cell
	Point cor;
	while(fin)
	{
		fin>>num;
		if(!fin.good()) break;
		vector<Point> t;
		for(int i=0;i<num;i++)
		{
			fin>>cor.x;
			fin>>cor.y;
			t.push_back(cor);
		}
		cells.push_back(t);
	}
	fin.close();
	vector<Point> t=cells[0];
        string s=IntToString(t.size());
        char s1[14];
        string passPhrase="123456";
        dataowner owner;
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
        mt0.go();
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
	ifstream fin1(argv[2]);
	if (! fin1)
	{
		cerr << "Cannot open grid file " << argv[2] << "." << endl;
		return -1;
	}
	vector<double> XSplit,YSplit;
	double x;
	while(fin1)
	{
		fin1>>num;
		if(!fin1.good()) break;
		for(int i=0;i<num;i++)
		{
			fin1>>x;
			XSplit.push_back(x);
		}
		fin1>>num;
		if(!fin1.good()) break;
		for(int i=0;i<num;i++)
		{
			fin1>>x;
			YSplit.push_back(x);
		}
	}
	fin1.close();
	ifstream qfin(argv[3]);
	if (! qfin)
	{
		cerr << "Cannot open query file " << argv[3] << "." << endl;
		return -1;
	}
	ofstream fout(argv[4]);
	int in;
        mt1.go();
	while(qfin>>cor.x>>cor.y)
	{
		int indY=-1,indX=-1;
		for(unsigned int i=0;i<YSplit.size();i++)
		{
			if(cor.y>=YSplit[i]&&cor.y<YSplit[i+1])
			{
				indY=i;break;
			}
		}
		for(unsigned int i=0;i<XSplit.size();i++)
		{
			if(cor.x>=XSplit[i]&&cor.x<XSplit[i+1])
			{
				indX=i;break;
			}
		}
		in=indY*(XSplit.size()-1)+indX;
		getNN(cor,cells[in],fout);
	}
	qfin.close();
	mt1.stop();
        mt1.update();
        printf("%15.7f\n",mt0.elapsed+mt1.elapsed/100);
        fout.close();
	return 0;
}
