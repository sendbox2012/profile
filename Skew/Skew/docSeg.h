#pragma once


#include <opencv2/opencv.hpp>
#include <vector>
#include "connecteddomain.h"
using namespace std;
struct cross
{
   int x,y;					// 坐标
   int h,v;					// 横,竖线
   int val;				// 交点是否存在
   int vali;//
};
struct crossTable
{
	int h_num,v_num;
	cross *tab;
	crossTable(int n1,int n2)
	{
		h_num=n1;
		v_num=n2;
		tab=new cross[n1*n2];
	}
	~crossTable()
	{
		if ( tab )
			delete tab;
	}
};
struct Line
{
	int x0;
	int x1;
	int y0;
	int y1;
	int pixN;
	
};
struct Lines
{
	int num;
	int size;
	Line *lines;
	//vector<CvPoint*> Xpoints;
	Lines(int n)
	{
		size=n;
		num=0;
		lines=(Line*)malloc(sizeof(Line)*n);

	}
	Lines()
	{
		size=100;
		num=0;
		lines=(Line*)malloc(sizeof(Line)*100);
	}
	void Increase(int added_num)           // 数组增加一定元素
	{
		size+=added_num;
		Line *temp=(Line*)realloc(lines,size*sizeof(Line));
		if ( temp ) lines=temp;
		else { free(lines); lines=NULL; }
	}
};
//struct Cell
//{
//	int left;
//	int top;
//	int right;
//	int bottom;
//	int tAttr;
//	int bAttr;
//	int rAttr;
//	int lAttr;
//};
struct GridCell
{
	int row;
	int col;
	int *x;
	int *y;
	GridCell()
	{
		row=100;
		col=100;
		x=(int*)malloc(sizeof(int)*100);
		y=(int*)malloc(sizeof(int)*100);

	}
	GridCell(int n,int m)
	{
		row=n;
		col=m;
		x=(int*)malloc(sizeof(int)*m);
		y=(int*)malloc(sizeof(int)*n);

	}
};
struct CellIndex
{
	int *element;
	int num;
	CellIndex()
	{
		num=100;
		element=(int*)malloc(sizeof(int)*100);
	}
	CellIndex(int n)
	{
		num=n;
		element=(int*)malloc(sizeof(int)*n);
	}
};

class DocSeg
{
public:
	int Font;
	vector<Lines*> hlines;
	vector<Lines*> vlines;
	vector<crossTable*> crossTables;
	IplImage *oriImage;
	FConnectedDomains *fcds;
private:
	void ComLines(Lines &hlines,int h,int v);	
	void sort(Lines &hlines);
	void DelLines(Lines &line1,Lines &line2);
	static void getSlope(double &slope,vector<CvPoint*> XYpoints);
	static void getSlope1(double &slope,vector<CvPoint*> XYpoints);
public:
	void DrawLine(IplImage* image)
	{
		for(int j=0;j<hlines.size();j++)
		{
			Lines *lines=hlines[j];
			for(int i=0;i<lines->num;i++)	
				cvLine(image,cvPoint(lines->lines[i].x0,lines->lines[i].pixN),cvPoint(lines->lines[i].x1,lines->lines[i].pixN),cvScalar(0,  0,255, 0));
		
		}
		for(int j=0;j<vlines.size();j++)
		{
			Lines *lines=vlines[j];
			for(int i=0;i<lines->num;i++)	
			   cvLine(image,cvPoint(lines->lines[i].pixN,lines->lines[i].y0),cvPoint(lines->lines[i].pixN,lines->lines[i].y1),cvScalar(255,0, 0,  0));

		}
		
	}
	void ConnectDomainToIpl(DocSeg* cd,IplImage* dst);
	void setFont(int font);
	int  getFont();
	
	void CalFont(/*int *font*/);
	void distinctionAttr();
	void ComText();
	void ComVText();
	void DelBlock();//删除特别小的块
	void ComBlocks();
	void GetCross(Lines &hlines,Lines &vlines,crossTable &crossT);
	int FindLine(IplImage* image,CvRect rect);
	static void  skew(DocSeg *connectedDomains,double *angle1);
	void delSmallConnectDomain(DocSeg *connectedDomains,int width,int heigth);
	void delBigConnectDomain(DocSeg *connectedDomains);
	
};

double Skew(IplImage* image);
void CombineBlocks(DocSeg* connectedDomains);
void DiscTableOrPicture(IplImage* image,DocSeg* connectedDomains);
void DrawLines(IplImage* image,DocSeg* connectedDomains);
int GetForms(int* &rectArray,DocSeg* connectedDomains,int *&numLen,int &Num);
int GetHorizonLines(int* &Hlines,DocSeg* connectedDomains,int *&BlinesNum,int &Num);
int GetVerticalLines(int* &Vlines,DocSeg* connectedDomains,int *&BlinesNum,int &Num);
void  ReleaseMemory(unsigned char*& data);
int GetCells2(IplImage* image,CvRect rect,GridCell *&gc,vector<CellIndex> &trueCells);
void DelBigConnectDomain(DocSeg *connectedDomains);
void ConnetionsToImage(DocSeg* cd,IplImage* dst);