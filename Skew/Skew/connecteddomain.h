#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;

enum ATTR {Text,Picture,h_Title,v_Title,Table,h_Line,v_Line,NoText};

class FConnectedDomain
{
public:
	CvRect rect;
	//vector<CvPoint*> points;
	vector<CvPoint*> Xpoints;
	enum ATTR attr; 
public:
	virtual ~FConnectedDomain()
	{
		//for(int i = 0; i < points.size(); i++)
		//{
		//	if(points[i] != 0) { delete points[i]; points[i] = 0; }
		//}
		for(int i = 0; i < Xpoints.size(); i++)
		{
			if(Xpoints[i] != 0) { delete Xpoints[i]; Xpoints[i] = 0; }
		}

	}
};

class FConnectedDomains
{
public:
	vector<FConnectedDomain*> ConnectedDomains;

public:
	virtual ~FConnectedDomains()
	{
		for(int i = 0; i < ConnectedDomains.size(); i++)
		{
			if(ConnectedDomains[i] != 0) { delete ConnectedDomains[i]; ConnectedDomains[i] = 0; }
		}
	}

	static FConnectedDomains* FromImage(IplImage* image);
	void ConnectDomainToIpl(FConnectedDomains* cd,IplImage* dst);
	void Draw(IplImage* image, unsigned int color = 0xff0000)
	{
		for(int i =0; i <ConnectedDomains.size(); i++)//ConnectedDomains.size()
		{
			FConnectedDomain* fcd = ConnectedDomains[i];

			cvRectangleR(image, fcd->rect, cvScalar(color & 0xff, (color & 0xff00) >> 8, (color & 0xff0000) >> 16, (color & 0xff0000) >> 24));
			
			/*if(fcd->attr==Text)
				cvRectangleR(image, fcd->rect, cvScalar(255, 0, 0, 0));
			else if(fcd->attr==NoText)
				cvRectangleR(image, fcd->rect, cvScalar(0, 0, 255, 0));
			else if(fcd->attr==h_Line)
				cvRectangleR(image, fcd->rect, cvScalar(0, 255, 0, 0));
			else if(fcd->attr==Table)
				cvRectangleR(image, fcd->rect, cvScalar(255, 255, 0, 0));
			else if(fcd->attr==Picture)
				cvRectangleR(image, fcd->rect, cvScalar(0, 255, 255, 0));*/
			
		}
	}
	
	// 交叉合并
	void Combine();
	// 水平合并
	void CombineH();
};

void GetConnectedDomains(void* image, FConnectedDomains*& connectedDomains);
void CombineConnectedDomains(FConnectedDomains* connectedDomains);
void ReleaseConnectedDomains(FConnectedDomains** connectedDomains);
void DrawConnectedDomains(void* image, FConnectedDomains* connectedDomains, unsigned int color);
