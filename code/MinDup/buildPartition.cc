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
#include <list>
#include <algorithm>
#include "operation.h"
#include "../MTimer.h"
#include "../voronoi/dataPro.h"
using namespace std;
using namespace CryptoPP;

#define BOUNDER 100005
dataPro dp;
struct Point
{
	double x;
	double y;
};
struct ICell
{
	int cid;
	vector<Point> pX;//sort by X
};
struct Cell
{
	int cid;
	vector<int> pi;
	vector<double> minX;
	vector<double> maxX;
	vector<double> minY;
	vector<double> maxY;
};
void printPV(vector<vector<Point> > &rslt)
{
	for(unsigned int i=0;i<rslt.size();i++)
		for(unsigned int j=0;j<rslt[i].size();j++)
			cout<<rslt[i][j].x<<" "<<rslt[i][j].y<<endl;
}
void printP(vector<Point> &rslt)
{
	for(unsigned int i=0;i<rslt.size();i++)
		cout<<rslt[i].x<<" "<<rslt[i].y<<endl;
}
void printVec(vector<double> &rslt, ofstream & fout)
{
	fout<<rslt.size();
	for(unsigned int i=0;i<rslt.size();i++)
		fout<<" "<<rslt[i];
		fout<<endl;
}
void printPar(vector<vector<Cell> > & grid, char* filename, int max)
{
	ofstream fout(filename);
	int n;
	for(unsigned int i=0;i<grid.size();i++)
	{
		vector<Cell> t=grid[i];
		for(unsigned int j=0;j<t.size();j++)
		{
			Cell c=t[j];
			fout<<max<<endl;
			n=max-c.pi.size();
			for(unsigned int m=0;m<c.pi.size();m++)
				fout<<dp.dataset[c.pi[m]].first<<" "<<dp.dataset[c.pi[m]].second<<endl;
			for(unsigned int m=0;m<n;m++)
				fout<<"-1 -1"<<endl;
		}	
	}
	fout.close();
}
bool sortX(Point a,Point b)
{
	return (a.x<b.x);
}
bool sortY(Point a,Point b)
{
	return (a.y<b.y);
}
struct entry
{
	int cid;
	int pNum;//num of point
	double posX;//lower left point of cell
	double posY;//lower left point of cell
};
struct entryComp
{
	bool operator()(entry a,entry b)
	{
		if(a.pNum<b.pNum) return true;
		else return false; 
	}
};
void findCandiCells(entry en,vector<vector<ICell> > &grid, vector<vector<Cell> > &dgrid, vector<ICell> &candiX, vector<ICell> &candiY, vector<Cell>&dcandiX, vector<Cell>&dcandiY,vector<double> & XSplit,vector<double> & YSplit, int &indX,int &indY)
{

	for(indX=0;(unsigned int)indX<XSplit.size();++indX)
	{
		if(XSplit[indX]==en.posX) break;
	}
	for(indY=0;(unsigned int)indY<YSplit.size();++indY)
	{
		if(YSplit[indY]==en.posY) break;
	}
	for(unsigned int i=0;i<grid[indY].size();++i){
		candiX.push_back(grid[indY][i]);	//along X
		dcandiX.push_back(dgrid[indY][i]);	//along X
	}
	for(unsigned int i=0;i<grid.size();++i){
		candiY.push_back(grid[i][indX]);	
		dcandiY.push_back(dgrid[i][indX]);	//along Y
	}
}
//now use linear scan to find the seperate line, better solution is using binary search
void parCell(vector<Point> & cell,double sepLine, vector<Point> & left,vector<Point> &  right, char t)
{
	
	for(unsigned int i=0;i<cell.size();i++)
	{
		if(t=='x')
		{
			if(cell[i].x<sepLine)
				left.push_back(cell[i]);
			else if(cell[i].x>sepLine)
				right.push_back(cell[i]);
			else 
			{
				left.push_back(cell[i]);
				right.push_back(cell[i]);
			}
		}
		else
		{		
			if(cell[i].y<sepLine)
				left.push_back(cell[i]);
			else if(cell[i].y>sepLine)
				right.push_back(cell[i]);
			else 
			{
				left.push_back(cell[i]);
				right.push_back(cell[i]);
			}
		}	
	}
}
void parDCell(Cell & cell,double sepLine, Cell & left,Cell & right, char t)
{
	if(t=='x'){		//using Y
		for(int i=0;i<cell.pi.size();++i){
			if(dp.dataset[cell.pi[i]].first<sepLine){
				left.pi.push_back(cell.pi[i]);
				left.minX.push_back(dp.border[cell.pi[i]].minX);
				left.maxX.push_back(dp.border[cell.pi[i]].maxX);
				left.minY.push_back(dp.border[cell.pi[i]].minY);
				left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				if(dp.border[cell.pi[i]].maxX>sepLine){ 
					right.pi.push_back(cell.pi[i]);
					right.minX.push_back(dp.border[cell.pi[i]].minX);
					right.maxX.push_back(dp.border[cell.pi[i]].maxX);
					right.minY.push_back(dp.border[cell.pi[i]].minY);
					right.maxY.push_back(dp.border[cell.pi[i]].maxY);
				}
			}
			else if(dp.dataset[cell.pi[i]].first>sepLine){
				right.pi.push_back(cell.pi[i]);
				right.minX.push_back(dp.border[cell.pi[i]].minX);
				right.maxX.push_back(dp.border[cell.pi[i]].maxX);
				right.minY.push_back(dp.border[cell.pi[i]].minY);
				right.maxY.push_back(dp.border[cell.pi[i]].maxY);
				if(dp.border[cell.pi[i]].minX<sepLine){ 
					left.pi.push_back(cell.pi[i]);
					left.minX.push_back(dp.border[cell.pi[i]].minX);
					left.maxX.push_back(dp.border[cell.pi[i]].maxX);
					left.minY.push_back(dp.border[cell.pi[i]].minY);
					left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				}
			}
			else{
				left.pi.push_back(cell.pi[i]);
				left.minX.push_back(dp.border[cell.pi[i]].minX);
				left.maxX.push_back(dp.border[cell.pi[i]].maxX);
				left.minY.push_back(dp.border[cell.pi[i]].minY);
				left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				right.pi.push_back(cell.pi[i]);
				right.minX.push_back(dp.border[cell.pi[i]].minX);
				right.maxX.push_back(dp.border[cell.pi[i]].maxX);
				right.minY.push_back(dp.border[cell.pi[i]].minY);
				right.maxY.push_back(dp.border[cell.pi[i]].maxY);
			}
		}
	}				
	else{		//using X
		for(int i=0;i<cell.pi.size();++i){
			if(dp.dataset[cell.pi[i]].second<sepLine){
				left.pi.push_back(cell.pi[i]);
				left.minX.push_back(dp.border[cell.pi[i]].minX);
				left.maxX.push_back(dp.border[cell.pi[i]].maxX);
				left.minY.push_back(dp.border[cell.pi[i]].minY);
				left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				if(dp.border[cell.pi[i]].maxY>sepLine){ 
					right.pi.push_back(cell.pi[i]);
					right.minX.push_back(dp.border[cell.pi[i]].minX);
					right.maxX.push_back(dp.border[cell.pi[i]].maxX);
					right.minY.push_back(dp.border[cell.pi[i]].minY);
					right.maxY.push_back(dp.border[cell.pi[i]].maxY);
				}
			}
			else if(dp.dataset[cell.pi[i]].second>sepLine){
				right.pi.push_back(cell.pi[i]);
				right.minX.push_back(dp.border[cell.pi[i]].minX);
				right.maxX.push_back(dp.border[cell.pi[i]].maxX);
				right.minY.push_back(dp.border[cell.pi[i]].minY);
				right.maxY.push_back(dp.border[cell.pi[i]].maxY);
				if(dp.border[cell.pi[i]].minY<sepLine){ 
					left.pi.push_back(cell.pi[i]);
					left.minX.push_back(dp.border[cell.pi[i]].minX);
					left.maxX.push_back(dp.border[cell.pi[i]].maxX);
					left.minY.push_back(dp.border[cell.pi[i]].minY);
					left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				}
			}
			else{
				left.pi.push_back(cell.pi[i]);
				left.minX.push_back(dp.border[cell.pi[i]].minX);
				left.maxX.push_back(dp.border[cell.pi[i]].maxX);
				left.minY.push_back(dp.border[cell.pi[i]].minY);
				left.maxY.push_back(dp.border[cell.pi[i]].maxY);
				right.pi.push_back(cell.pi[i]);
				right.minX.push_back(dp.border[cell.pi[i]].minX);
				right.maxX.push_back(dp.border[cell.pi[i]].maxX);
				right.minY.push_back(dp.border[cell.pi[i]].minY);
				right.maxY.push_back(dp.border[cell.pi[i]].maxY);
			}
		}
	}				
}
void parCell(Cell & cell,double sepLine, int & left,int &  right, char t)
{
	left =right =0;
	int ll,lr,rl,rr;	//ll: left end > sepLine
	vector<double>* e1;
	vector<double>* e2;
	if(t=='x'){		//using Y
		e1 =&(cell.minX);
		e2 =&(cell.maxX);
		if(e1->size()==1)//ll,lr
		{
			if((*e1)[0]<sepLine)
			{	
				ll=1;lr=0;
			}
			else 
			{
				ll=0;lr=1;
			}	
		}
		else{
			int st=-1,ed=e1->size(),rslt=-1;
			while(ed!=(st+1))
			{
				if((*e1)[(st+ed)/2]<sepLine)
				{
					st=(st+ed)/2;
				}
				else if((*e1)[(st+ed)/2]>sepLine)
				{
					ed=(st+ed)/2;
				}
				else 
				{
					rslt=(st+ed)/2;
					break;
				}
			}
			if(rslt==-1)
			{
				ll=st+1;
			}
			else
			{
				int l,r;
				l=r=rslt;
				for(int i=rslt-1;i>=0;i--)
				{	
					if((*e1)[i]!=sepLine){
						l=i;
						break;
					}
				}
				ll=l+1;
			}
		}
		if(e2->size()==1)//rl,rr
		{
			if((*e2)[0]<=sepLine)
			{	
				rl=1;rr=0;
			}
			else 
			{
				rl=0;rr=1;
			}	
		}
		else{
			int st=-1,ed=e2->size(),rslt=-1;
			while(ed!=(st+1))
			{
				if((*e2)[(st+ed)/2]<sepLine)
				{
					st=(st+ed)/2;
				}
				else if((*e2)[(st+ed)/2]>sepLine)
				{
					ed=(st+ed)/2;
				}
				else 
				{
					rslt=(st+ed)/2;
					break;
				}
			}
			if(rslt==-1)
			{
				rl=st+1;
				rr=e2->size()-rl;
			}
			else
			{
				int l,r;
				l=r=rslt;
				for(int i=rslt;i<e2->size();i++)
				{	
					if((*e2)[i]!=sepLine){
						r=i;
						break;
					}
				}
				rr=e2->size()-r;
			}
		}
	}				
	else{		//using X
		e1 =&(cell.minY);
		e2 =&(cell.maxY);
		if(e1->size()==1)//ll,lr
		{
			if((*e1)[0]<sepLine)
			{	
				ll=1;lr=0;
			}
			else 
			{
				ll=0;lr=1;
			}	
		}
		else{
			int st=-1,ed=e1->size(),rslt=-1;
			while(ed!=(st+1))
			{
				if((*e1)[(st+ed)/2]<sepLine)
				{
					st=(st+ed)/2;
				}
				else if((*e1)[(st+ed)/2]>sepLine)
				{
					ed=(st+ed)/2;
				}
				else 
				{
					rslt=(st+ed)/2;
					break;
				}
			}
			if(rslt==-1)
			{
				ll=st+1;
			}
			else
			{
				int l,r;
				l=r=rslt;
				for(int i=rslt-1;i>=0;i--)
				{	
					if((*e1)[i]!=sepLine){
						l=i;
						break;
					}
				}
				ll=l+1;
			}
		}
		if(e2->size()==1)//rl,rr
		{
			if((*e2)[0]<=sepLine)
			{	
				rl=1;rr=0;
			}
			else
			{
				rl=0;rr=1;
			}	
		}
		else{
			int st=-1,ed=e2->size(),rslt=-1;
			while(ed!=(st+1))
			{
				if((*e2)[(st+ed)/2]<sepLine)
				{
					st=(st+ed)/2;
				}
				else if((*e2)[(st+ed)/2]>sepLine)
				{
					ed=(st+ed)/2;
				}
				else 
				{
					rslt=(st+ed)/2;
					break;
				}
			}
			if(rslt==-1)
			{
				rl=st+1;
				rr=e2->size()-rl;
			}
			else
			{
				int l,r;
				l=r=rslt;
				for(int i=rslt;i<e2->size();i++)
				{	
					if((*e2)[i]!=sepLine){
						r=i;
						break;
					}
				}
				rr=e2->size()-r;
			}
		}
	}		
	left =ll;
	right =rr;
}
int findOtherMax(vector<vector<Cell > > & dgrid,int ind,char t)
{
	unsigned int max=0;
	if(t=='y')
	{
		for(unsigned int i=0;i<dgrid.size();i++)
		{
			if(i==(unsigned int)ind) continue;
			for(unsigned int j=0;j<dgrid[i].size();j++)
				if(max<dgrid[i][j].pi.size())
					max=dgrid[i][j].pi.size();
		}
	}
	else
	{
		for(unsigned int i=0;i<dgrid.size();i++)
		{
			for(unsigned int j=0;j<dgrid[i].size();j++)
			{
				if(j==(unsigned int)ind) continue;
				if(max<dgrid[i][j].pi.size())
					max=dgrid[i][j].pi.size();
			}
		}
	}	
	return (int)max;
}
void splitCells(vector<ICell> &candiX, vector<ICell> &candiY,vector<Cell> &dcandiX,vector<Cell> &dcandiY,vector<vector<ICell> > & grid,vector<vector<Cell> > & dgrid, int indX,int indY,int & glbID,vector<double> & XSplit,vector<double> & YSplit,set<int> &deadCell,priority_queue<entry,vector<entry>,entryComp> &queue)
{
	int glbMaxX=numeric_limits<int>::max();	//smallest tmpMax1
	int glbMaxY=numeric_limits<int>::max();
	double glbValueX;			//best splitLine
	double glbValueY;
	vector<vector<Point> > glbParX;
	vector<vector<Point> > glbParY;
	vector<Cell>  dglbParX;
	vector<Cell>  dglbParY;
	int sign=1;	//1:x -1:y
	set<double> sortedX;	//vertex along x axis is sorted by y
	for(unsigned int i=0;i<candiX.size();++i)
	{	
		vector<Point> tmp=candiX[i].pX;
		for(unsigned int j=0;j<tmp.size();j++)
			sortedX.insert(tmp[j].y);
	}
	for(set<double>::iterator sitX=sortedX.begin();sitX!=sortedX.end();sitX++)
	{//most expensive part, need to be optimized
		double sepLine=*sitX;
		vector<vector<Point> > tmpPar;
		int tmpMax1=0;		//max among all candiX
		for(unsigned int i=0;i<candiX.size();i++)
		{
			int left,right;
			if(candiX[i].pX.size()==0) continue;
			parCell(dcandiX[i],sepLine,left,right,'y');	//parallel to X
			int tmpMax=left>right?left:right;
			if(tmpMax>tmpMax1)
				tmpMax1=tmpMax;
		}
		if(tmpMax1<glbMaxX)		//glbMax: record the smallest tmpMax1; the smaller the better 
		{
			glbMaxX=tmpMax1;
			glbValueX=sepLine;
		}
	}
	int otherMaxX=findOtherMax(dgrid,indY,'y');
	if(glbMaxX<otherMaxX) glbMaxX=otherMaxX;
	set<double> sortedY;//point along y axis is sorted by x
	for(unsigned int i=0;i<candiY.size();i++)
	{	
		vector<Point> tmp=candiY[i].pX;
		for(unsigned int j=0;j<tmp.size();j++)
			sortedY.insert(tmp[j].x);
	}
	for(set<double>::iterator sitY=sortedY.begin();sitY!=sortedY.end();sitY++)
	{	
		double sepLine=*sitY;
		vector <vector<Point> > tmpPar;
		int tmpMax1=0;
		for(unsigned int i=0;i<candiY.size();i++)
		{
			int left,right;
			if(candiY[i].pX.size()==0) continue;
			parCell(dcandiY[i],sepLine,left,right,'x');
			int tmpMax=left>right?left:right;
			if(tmpMax>tmpMax1)
				tmpMax1=tmpMax;
		}
		if(tmpMax1<glbMaxY) 
		{
			glbMaxY=tmpMax1;
			glbValueY=sepLine;
		}
	}
	int otherMaxY=findOtherMax(dgrid,indX,'x');
	if(glbMaxY<otherMaxY) glbMaxY=otherMaxY;
	if(glbMaxX>glbMaxY) sign=-1;		//cutting using Y 
	if(sign==1)			//cutting using X
	{	
		for(unsigned int i=0;i<candiX.size();i++)
		{
			vector<Point> left,right;
			Cell dleft,dright;
			parCell(candiX[i].pX,glbValueX,left,right,'y');
			parDCell(dcandiX[i],glbValueX,dleft,dright,'y');
			glbParX.push_back(left);
			glbParX.push_back(right);
			dglbParX.push_back(dleft);
			dglbParX.push_back(dright);
		}
		vector<ICell> left,right;
		vector<Cell> dleft,dright;
		for(unsigned int i=0;i<glbParX.size();i=i+2)
		{
			ICell l;
			Cell dl;
			l.cid=glbID;
			dl=dglbParX[i];
			dl.cid=glbID;
			glbID++;
			l.pX=glbParX[i];
			sort(dl.minX.begin(),dl.minX.end());
			sort(dl.minY.begin(),dl.minY.end());
			sort(dl.maxX.begin(),dl.maxX.end());
			sort(dl.maxY.begin(),dl.maxY.end());
			left.push_back(l);
			dleft.push_back(dl);
			l.cid=glbID;
			dl=dglbParX[i+1];
			dl.cid=glbID;
			glbID++;
			l.pX=glbParX[i+1];
			sort(dl.minX.begin(),dl.minX.end());
			sort(dl.minY.begin(),dl.minY.end());
			sort(dl.maxX.begin(),dl.maxX.end());
			sort(dl.maxY.begin(),dl.maxY.end());
			right.push_back(l);
			dright.push_back(dl);
		}
		for(unsigned int i=0;i<dgrid[indY].size();++i)
		{
			deadCell.insert(dgrid[indY][i].cid);	//old die
			grid[indY][i]=right[i];	
			dgrid[indY][i]=dright[i];		//insert(replaced by) right	
			entry e;				//new(right) enqueue
			e.cid=dright[i].cid;
			e.pNum=dright[i].pi.size();
			e.posX=XSplit[i];
			e.posY=glbValueX;
			queue.push(e);
		}
		grid.insert(grid.begin()+indY,left);		//insert new cell
		dgrid.insert(dgrid.begin()+indY,dleft);
		for(unsigned int i=0;i<dgrid[indY].size();++i)	
		{
			entry e;				//new(left) enqueue
			e.cid=dgrid[indY][i].cid;
			e.pNum=dgrid[indY][i].pi.size();
			e.posX=XSplit[i];
			e.posY=YSplit[indY];
			queue.push(e);
		}
		YSplit.insert(YSplit.begin()+indY+1,glbValueX);
	}
	else
	{
		for(unsigned int i=0;i<candiY.size();i++)
		{
			vector<Point> left,right;
			Cell dleft,dright;
			parCell(candiY[i].pX,glbValueY,left,right,'x');
			parDCell(dcandiY[i],glbValueY,dleft,dright,'x');
			glbParY.push_back(left);
			glbParY.push_back(right);
			dglbParY.push_back(dleft);
			dglbParY.push_back(dright);
		}
		vector<ICell> left,right;
		vector<Cell> dleft,dright;
		for(unsigned int i=0;i<glbParY.size();i=i+2)
		{
			ICell l;
			Cell dl;
			l.cid=glbID;
			dl=dglbParY[i];
			dl.cid=glbID;
			glbID++;
			sort(dl.minX.begin(),dl.minX.end());
			sort(dl.minY.begin(),dl.minY.end());
			sort(dl.maxX.begin(),dl.maxX.end());
			sort(dl.maxY.begin(),dl.maxY.end());
			l.pX=glbParY[i];
			left.push_back(l);
			dleft.push_back(dl);
			l.cid=glbID;
			dl=dglbParY[i+1];
			dl.cid=glbID;
			glbID++;
			sort(dl.minX.begin(),dl.minX.end());
			sort(dl.minY.begin(),dl.minY.end());
			sort(dl.maxX.begin(),dl.maxX.end());
			sort(dl.maxY.begin(),dl.maxY.end());
			l.pX=glbParY[i+1];
			right.push_back(l);
			dright.push_back(dl);
		}
		for(unsigned int i=0;i<dgrid.size();i++)
		{
			deadCell.insert(dgrid[i][indX].cid);
			grid[i][indX]=right[i];	
			dgrid[i][indX]=dright[i];	
			entry e;
			e.cid=dright[i].cid;
			e.pNum=dright[i].pi.size();
			e.posX=glbValueY;
			e.posY=YSplit[i];
			queue.push(e);
		}
		for(unsigned int i=0;i<grid.size();i++)
		{
			grid[i].insert(grid[i].begin()+indX,left[i]);
			dgrid[i].insert(dgrid[i].begin()+indX,dleft[i]);
			entry e;
			e.cid=dgrid[i][indX].cid;
			e.pNum=dgrid[i][indX].pi.size();
			e.posX=XSplit[indX];
			e.posY=YSplit[i];
			queue.push(e);
		}
		XSplit.insert(XSplit.begin()+indX+1,glbValueY);
	}
}
void getPartition(vector<vector<ICell> > & grid,vector<vector<Cell> >& dgrid,vector<double> & XSplit,vector<double> & YSplit, int m, int &glbID)
{
	int step=0;
	priority_queue<entry,vector<entry>,entryComp> queue;//sort by number of point in a cell, used to find next cell to split
	entry e;
	e.cid=dgrid[0][0].cid;
	e.pNum=dgrid[0][0].pi.size();
	e.posX=XSplit[0];
	e.posY=YSplit[0];
	queue.push(e);
	set<int> deadCell;//cells been splitted, will be useless afterwards.
	set<int>::iterator sit;
	while(grid.size()*grid[0].size()<m)
	{
		entry en;
		do 
		{
			en=queue.top();queue.pop();
			sit=deadCell.find(en.cid);
		} while(sit!=deadCell.end());
		int indX,indY;
		vector<ICell> candiX, candiY;
		vector<Cell> dcandiX, dcandiY;
		findCandiCells(en,grid,dgrid,candiX,candiY,dcandiX,dcandiY,XSplit,YSplit,indX,indY);
		splitCells(candiX,candiY,dcandiX,dcandiY,grid,dgrid,indX,indY,glbID,XSplit,YSplit,deadCell,queue);
		step++;
	}//end while
}

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		cerr << "Usage: " << argv[0] << " 1-data_file "<<" 2-#Partition 3-out_data_file 4-out_grid_file" << endl;
		return -1;
	}
	double upperbd=BOUNDER;
	double lowerbd=0;
	int m=atoi(argv[2]);	//m: #partition
	MTimer mt,mt1,mt2;
	mt.go();
	dp.Process(argv[1]);
        mt.stop();
        mt.update();
        double ve=mt.elapsed;      //ve: voronoi+preprocessing time
        mt1.go();
	int glbID=0;
	Cell c;
	c.cid=glbID;
	ICell ic;
	ic.cid =glbID;
	glbID++;
	for(int i=0;i<dp.dataset.size();++i)
	{
		c.pi.push_back(i);
		c.minX.push_back(dp.border[i].minX);
		c.minY.push_back(dp.border[i].minY);
		c.maxX.push_back(dp.border[i].maxX);
		c.maxY.push_back(dp.border[i].maxY);
	}
	for(int i=1;i<dp.vertex.size();++i)
	{
		Point cor;
		cor.x =dp.vertex[i].first;
		cor.y =dp.vertex[i].second;
		ic.pX.push_back(cor);
	}
	sort(c.minX.begin(),c.minX.end());
	sort(c.minY.begin(),c.minY.end());
	sort(c.maxX.begin(),c.maxX.end());
	sort(c.maxY.begin(),c.maxY.end());
	vector<vector<Cell> > dgrid;//store partition: cell
	vector<Cell> tmp;
	tmp.push_back(c);
	dgrid.push_back(tmp);
	
	vector<vector<ICell> > grid;//store partition: cell
	vector<ICell> itmp;
	itmp.push_back(ic);
	grid.push_back(itmp);
	vector<double> XSplit;
	vector<double> YSplit;
	XSplit.push_back(lowerbd);
	XSplit.push_back(upperbd);
	YSplit.push_back(lowerbd);
	YSplit.push_back(upperbd);
	getPartition(grid,dgrid,XSplit,YSplit,m,glbID);
	mt1.stop();
	mt1.update();
	unsigned int i;
	unsigned int j;
	unsigned int size_Max=dgrid[0][0].pi.size();
	unsigned int Min=dgrid[0][0].pi.size();
	unsigned int sum=0;
        for(i=0;i<dgrid.size();i++)
        {
                vector<Cell> t=dgrid[i];
                for(j=0;j<t.size();j++)
                {
                        Cell c=t[j];
                        if(c.pi.size()>size_Max) size_Max=c.pi.size();
                        if(c.pi.size()<Min) Min=c.pi.size();
			sum+=c.pi.size();
                }
        }
    	printPar(dgrid,argv[3],size_Max);
        ofstream fout(argv[4]);
        printVec(XSplit,fout);
        printVec(YSplit,fout);
	fout.close();
	
	vector<Point> t;
	for(int i=0;i<dgrid[0][0].pi.size();++i){
		Point cor;
		cor.x =dp.dataset[dgrid[0][0].pi[i]].first;
		cor.y =dp.dataset[dgrid[0][0].pi[i]].second;
		t.push_back(cor);
	}
	Point p;
	p.x=p.y=-1;
	int n=size_Max-t.size();
	for(unsigned int j=0;j<n;j++)
		t.push_back(p);
        string s=IntToString(t.size());
        char s1[14];
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
	cout << i*j<<" ";
	printf("%15.7f %15.7f %10d %10d %10d %10d %10d\n",ve+mt1.elapsed,mt2.elapsed*i*j,sum/(i*j),Min,size_Max,size_Max*i*j,dp.dataset.size());

	return 0;
}
