#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <set>
#include <map>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "operation.h"
#include "../MTimer.h"
#include "../voronoi/dataPro.h"
using namespace std;
using namespace CryptoPP;

#define BORDER 100005
dataPro dp;
struct Point
{
	double x;
	double y;
};
struct Cell
{
	vector<Point> pX;	//vertex sorted by X
	vector<Point> pY;
	vector<int> pi;	//point index
	Point ll;		//lower left corner
	Point ur;
};
void printPar(vector<Cell> & cells, char* d_filename, char* g_filename, int max)
{
	ofstream fout(d_filename);
	ofstream fout1(g_filename);
        for(unsigned int i=0;i<cells.size();++i)
        {
                Cell c=cells[i];
                fout<< max <<endl;
                for(unsigned int j=0;j<c.pi.size();j++)
                {
                        fout << dp.dataset[c.pi[j]].first << " "<<dp.dataset[c.pi[j]].second << endl;
		}
		for(int j=0;j<max-c.pi.size();++j)
			fout <<"-1 -1"<<endl;
		fout1<<c.ll.x<<" "<<c.ll.y<<" "<<c.ur.x<<" "<<c.ur.y<<endl;
	}
	fout.close();
	fout1.close();

}
bool sortX(Point a,Point b)
{
	return (a.x<b.x);
}
bool sortY(Point a,Point b)
{
	return (a.y<b.y);
}
struct entryComp
{
	bool operator()(Cell a,Cell b)
	{
		if(a.pX.size()<b.pX.size()) return true;
		else return false; 
	}
};
void parDCell(vector<int> & cell,double sepLine, vector<int> & left,vector<int> & right, char t)
{
	if(t=='x'){		//using Y
		for(int i=0;i<cell.size();++i){
			if(dp.dataset[cell[i]].first<sepLine){
				left.push_back(cell[i]);
				if(dp.border[cell[i]].maxX>sepLine){ 
					right.push_back(cell[i]);
				}
			}
			else if(dp.dataset[cell[i]].first>sepLine){
				right.push_back(cell[i]);
				if(dp.border[cell[i]].minX<sepLine){ 
					left.push_back(cell[i]);
				}
			}
			else{
				left.push_back(cell[i]);
				right.push_back(cell[i]);
			}
		}
	}				
	else{		//using X
		for(int i=0;i<cell.size();++i){
			if(dp.dataset[cell[i]].second<sepLine){
				left.push_back(cell[i]);
				if(dp.border[cell[i]].maxY>sepLine){ 
					right.push_back(cell[i]);
				}
			}
			else if(dp.dataset[cell[i]].second>sepLine){
				right.push_back(cell[i]);
				if(dp.border[cell[i]].minY<sepLine){ 
					left.push_back(cell[i]);
				}
			}
			else{
				left.push_back(cell[i]);
				right.push_back(cell[i]);
			}
		}
	}				
}
int main(int argc, char** argv)
{
	if (argc != 5){
		cerr << "Usage: " << argv[0] << " 1-data_file 2-#partition 3-out_data_file 4-out_grid_file" << endl;
		return -1;
	}
	double upperbd=BORDER;
	double lowerbd=0;
	int m=atoi(argv[2]);//using 4 value to present one cell
	ifstream fin(argv[1]);
	if (! fin){
		cerr << "Cannot open data file " << argv[1] << "." << endl;
		return -1;
	}
	MTimer mt,mt1,mt2;
	mt.go();
        dp.Process(argv[1]);
	mt.stop();
  	mt.update();
	double ve=mt.elapsed;	//preprocess time
	mt1.go();
	Cell c;
	Point cor;
	for(int i=1;i<dp.vertex.size();++i)
	{
		cor.x =dp.vertex[i].first;
		cor.y =dp.vertex[i].second;
		c.pX.push_back(cor);
		c.pY.push_back(cor);
	}
	sort(c.pX.begin(),c.pX.end(),sortX);
	sort(c.pY.begin(),c.pY.end(),sortY);
	for(int i=0;i<dp.dataset.size();++i)
	{
		c.pi.push_back(i);
	}
	c.ll.x=c.ll.y=lowerbd;
	c.ur.x=c.ur.y=upperbd;
	int step=0;
	//in queue, sort by number of point in a cell, used to find next cell to split
	priority_queue<Cell,vector<Cell>,entryComp> queue;
	queue.push(c);
	while(queue.size()<m)
	{
		Cell t=queue.top();queue.pop();
		int glbMax=numeric_limits<int>::max();
		int glbInd=-1;
		vector<Point> left,right;
		int sign=1;	//1:x -1:y
		int lNum,rNum;
		//in each cell, split it in the "middle"
		int size =t.pX.size();;
		int l,r,m;
		l =r =m =(size-1)/2;
		double middle =t.pX[m].x;
		if((size%2==1)||(middle==t.pX[m+1].x)){
			for(int i=m+1;i<size;++i)
				if(t.pX[i].x!=middle){
					r =i;
					break;
				}
			for(int i=m-1;i>0;--i)
				if(t.pX[i].x!=middle){
					l =i;
					break;
				}
			glbMax =r-l-1;
			glbInd =m;
		}
		else{
			double middle2 =t.pX[m+1].x;
			for(int i=m+1;i<size;++i)
				if(t.pX[i].x!=middle2){
					r =i;
					break;
				}
			for(int i=m-1;i>0;--i)
				if(t.pX[i].x!=middle){
					l =i;
					break;
				}
			int n1=m-l;
			int n2=r-(m+1);
			glbMax =(n1>n2)?n1:n2;
			glbInd =(n1>n2)?m:m+1;
		}
		size =t.pY.size();	//using X
		l =r =m =(size-1)/2;
		middle =t.pY[m].y;
		if((size%2==1)||(middle==t.pY[m+1].y)){
			for(int i=m+1;i<size;++i)
				if(t.pY[i].y!=middle){
					r =i;
					break;
				}
			for(int i=m-1;i>0;--i)
				if(t.pY[i].y!=middle){
					l =i;
					break;
				}
			if(glbMax<r-l-1){
				glbMax =r-l-1;
				glbInd =m;
				sign=-1;
			}
		}
		else{
			int middle2 =t.pY[m+1].y;
			for(int i=m+1;i<size;++i)
				if(t.pY[i].y!=middle2){
					r =i;
					break;
				}
			for(int i=m-1;i>0;--i)
				if(t.pY[i].y!=middle){
					l =i;
					break;
				}
			int n1=m-l;
			int n2=r-(m+1);
			if(n1>n2){
				if(glbMax<n1){
					glbMax =n1;
					glbInd =m;
					sign =-1;
				}
			}
			else{
				if(glbMax<n2){
					glbMax =n2;
					glbInd =m+1;
					sign =-1;
				}
			}
		}
		Cell a,b;
		if(sign==1)	//using Y
		{
			left.insert(left.begin(),t.pX.begin(),t.pX.begin()+glbInd+1);
			right.insert(right.begin(),t.pX.begin()+glbInd,t.pX.end());
			for(int i=glbInd+1;i<t.pX.size();++i){
				if(t.pX[i].x!=t.pX[glbInd].x) break;
				left.push_back(t.pX[i]);
			}
			for(int i=glbInd-1;i>=0;--i){
				if(t.pX[i].x!=t.pX[glbInd].x) break;
				right.insert(right.begin(),t.pX[i]);
			}
			a.pX=left;
			sort(left.begin(),left.end(),sortY);
			a.pY=left;
			a.ll=t.ll;
			a.ur.x=t.pX[glbInd].x;
			a.ur.y=t.ur.y;
			b.pX=right;
			sort(right.begin(),right.end(),sortY);
			b.pY=right;
			b.ll.x=t.pX[glbInd].x;
			b.ll.y=t.ll.y;
			b.ur=t.ur;
			parDCell(t.pi,t.pX[glbInd].x,a.pi,b.pi,'x');
		}
		else
		{
			left.insert(left.begin(),t.pY.begin(),t.pY.begin()+glbInd+1);
			right.insert(right.begin(),t.pY.begin()+glbInd,t.pY.end());
			for(int i=glbInd+1;i<t.pY.size();++i){
				if(t.pY[i].y!=t.pY[glbInd].y) break;
				left.push_back(t.pY[i]);
			}
			for(int i=glbInd-1;i>=0;--i){
				if(t.pY[i].y!=t.pY[glbInd].y) break;
				right.insert(right.begin(),t.pY[i]);
			}
			a.pY=left;
			sort(left.begin(),left.end(),sortX);
			a.pX=left;
			a.ll=t.ll;
			a.ur.x=t.ur.x;
			a.ur.y=t.pY[glbInd].y;
			b.pY=right;
			sort(right.begin(),right.end(),sortX);
			b.pX=right;
			b.ll.x=t.ll.x;
			b.ll.y=t.pY[glbInd].y;
			b.ur=t.ur;
			parDCell(t.pi,t.pY[glbInd].y,a.pi,b.pi,'y');
		}
		queue.push(a);
		queue.push(b);
		
		step++;
	}//end while
	mt1.stop();
        mt1.update();
	
	vector<Cell> cells;
        if(!queue.empty())
        {
                c=queue.top();queue.pop();
                cells.push_back(c);
        }
        int size_Max=c.pi.size();
        int Min=c.pi.size();
        int sum=c.pi.size();
        while(!queue.empty())
        {
                c=queue.top();queue.pop();
                if(c.pi.size()>size_Max) size_Max=c.pi.size();
                if(c.pi.size()<Min) Min=c.pi.size();
		sum+=c.pi.size();
                cells.push_back(c);
        }
	vector<Point> tmp;
	Point p;
	for(int i=0;i<cells[0].pi.size();++i)
	{
		int ind=cells[0].pi[i];
		p.x=dp.dataset[ind].first;
		p.y=dp.dataset[ind].second;
		tmp.push_back(p);
	}
        p.x=p.y=-1;
	int n=size_Max-tmp.size();
	for(unsigned int j=0;j<n;++j)
		tmp.push_back(p);
	printPar(cells, argv[3], argv[4],size_Max);
        string s=IntToString(tmp.size());
        char s1[14];
        string passPhrase="123456";
        dataowner owner;
        mt2.go();
        for(int i=0;i<tmp.size();i++)
        {
                sprintf(s1,"%013.6f",tmp[i].x);
                s.append(" ");
                s.append(s1);
                sprintf(s1,"%013.6f",tmp[i].y);
                s.append(" ");
                s.append(s1);
        }
        string ciphertext = owner.DEAEncryptString(s.c_str(), passPhrase.c_str());
        mt2.stop();
        mt2.update();
	cout <<m<<" ";	
	printf("%15.7f %15.7f %10d %10d %10d %10d %10d\n",ve+mt1.elapsed, mt2.elapsed*cells.size(),sum/cells.size(),Min,size_Max,size_Max*cells.size(), dp.dataset.size());

	return 0;
}
