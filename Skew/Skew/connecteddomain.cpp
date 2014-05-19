#include "connecteddomain.h"

FConnectedDomains* FConnectedDomains::FromImage(IplImage* image)
{
	IplImage* image1 = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopy(image, image1);
	FConnectedDomains* ret = new FConnectedDomains();

	if(image1->nChannels == 1) 
	{
		for(int i = 0; i < image1->height; i++)
		{
			for(int j = 0; j < image1->width; j++)
			{
				int index = i * image1->widthStep + j;
				if(((unsigned char)(image1->imageData[index]) == 255) || (image1->imageData[index] == 100) || (image1->imageData[index] == 101)) 
					continue;

				CvConnectedComp ccc;
				cvFloodFill(image1, cvPoint(j, i), cvScalarAll(100), cvScalarAll(0), cvScalarAll(0), &ccc, 8);

				FConnectedDomain* fcd = new FConnectedDomain();
				fcd->rect = ccc.rect;

			/*	for(int k = ccc.rect.y; k < ccc.rect.y + ccc.rect.height; k++)
				{
					for(int l = ccc.rect.x; l < ccc.rect.x + ccc.rect.width; l++)
					{
						index = k * image1->widthStep + l;
						if(image1->imageData[index] == 100)
						{
							CvPoint* cp = new CvPoint();
							cp->x = l;
							cp->y = k;
							fcd->points.push_back(cp);
							image1->imageData[index] = 101;
						}
					}
				}*/
				for(int l = ccc.rect.x; l < ccc.rect.x + ccc.rect.width; l++)
				{	
					for(int k = ccc.rect.y; k < ccc.rect.y + ccc.rect.height; k++)
					{
						index = k * image1->widthStep + l;
						if(image1->imageData[index] == 100)
						{
							CvPoint* cp = new CvPoint();
							cp->x = l;
							cp->y = k;
							fcd->Xpoints.push_back(cp);
							image1->imageData[index] = 101;
						}
					}
				}
				ret->ConnectedDomains.push_back(fcd);
			}
		}
	}

	cvReleaseImage(&image1);

	return ret;
}

void FConnectedDomains::Combine()
{
	//CalFont();
	bool found = true;
	int pos=0;
	while(found)
	{
		found = false;
		int index1 = 0;
		FConnectedDomain* fcd1 = 0;
		FConnectedDomain* fcd2 = 0;

		vector<int> index3;
		for(int i = pos; i < ConnectedDomains.size() - 1; i++)
		{
			fcd1 = ConnectedDomains[i];
			for(int j = i + 1; j < ConnectedDomains.size(); j++)
			{
				fcd2 = ConnectedDomains[j];
				if(fcd1->rect.x > fcd2->rect.x + fcd2->rect.width) continue;
				if(fcd1->rect.y > fcd2->rect.y + fcd2->rect.height) continue;
				if(fcd1->rect.x + fcd1->rect.width < fcd2->rect.x) continue;
				if(fcd1->rect.y + fcd1->rect.height < fcd2->rect.y) continue;
				index3.push_back(j);
				index1 = i; found = true; //break;
			}
			if(found == true){ pos=i; break;}
		}

		if(found == true)
		{
			for(int i = 0; i < index3.size(); i++)
			{
				fcd2 = ConnectedDomains[index3[i]];

				int x = min(fcd1->rect.x, fcd2->rect.x);
				int y = min(fcd1->rect.y, fcd2->rect.y);
				int width = max(fcd1->rect.x + fcd1->rect.width, fcd2->rect.x + fcd2->rect.width) - x;
				int height = max(fcd1->rect.y + fcd1->rect.height, fcd2->rect.y + fcd2->rect.height) - y;
				fcd1->rect.x = x;
				fcd1->rect.y = y;
				fcd1->rect.width = width;
				fcd1->rect.height = height;
				for(int j = 0; j < fcd2->Xpoints.size(); j++)
				{
					CvPoint* cp = fcd2->Xpoints[j];
					fcd1->Xpoints.push_back(cp);
				}
				fcd2->Xpoints.clear();
			}

			for(int i = index3.size() - 1; i >= 0; i--)
			{
				ConnectedDomains.erase(ConnectedDomains.begin() + index3[i]);
			}
		}
	}
}

void FConnectedDomains::CombineH()
{
	//CalFont();
	bool found = true;
	int pos=0;
	while(found)
	{
		found = false;
		int index1 = 0;
		FConnectedDomain* fcd1 = 0;
		FConnectedDomain* fcd2 = 0;

		vector<int> index3;
		for(int i = pos; i < ConnectedDomains.size() - 1; i++)
		{
			fcd1 = ConnectedDomains[i];
			for(int j = i + 1; j < ConnectedDomains.size(); j++)
			{
				fcd2 = ConnectedDomains[j];
				if(fcd1->rect.y > fcd2->rect.y + fcd2->rect.height) continue;
				if(fcd1->rect.y + fcd1->rect.height < fcd2->rect.y) continue;
				index3.push_back(j);
				index1 = i; found = true; //break;
			}
			if(found == true){ pos=i; break;}
		}

		if(found == true)
		{
			for(int i = 0; i < index3.size(); i++)
			{
				fcd2 = ConnectedDomains[index3[i]];

				int x = min(fcd1->rect.x, fcd2->rect.x);
				int y = min(fcd1->rect.y, fcd2->rect.y);
				int width = max(fcd1->rect.x + fcd1->rect.width, fcd2->rect.x + fcd2->rect.width) - x;
				int height = max(fcd1->rect.y + fcd1->rect.height, fcd2->rect.y + fcd2->rect.height) - y;
				fcd1->rect.x = x;
				fcd1->rect.y = y;
				fcd1->rect.width = width;
				fcd1->rect.height = height;
				for(int j = 0; j < fcd2->Xpoints.size(); j++)
				{
					CvPoint* cp = fcd2->Xpoints[j];
					fcd1->Xpoints.push_back(cp);
				}
				fcd2->Xpoints.clear();
			}

			for(int i = index3.size() - 1; i >= 0; i--)
			{
				ConnectedDomains.erase(ConnectedDomains.begin() + index3[i]);
			}
		}
	}
}

void GetConnectedDomains(void* image, FConnectedDomains*& connectedDomains)
{
	connectedDomains = FConnectedDomains::FromImage((IplImage*)image);
}

void CombineConnectedDomains(FConnectedDomains* connectedDomains)
{
	connectedDomains->Combine();
}

void ReleaseConnectedDomains(FConnectedDomains** connectedDomains)
{
	if((*connectedDomains) != 0)
	{
		FConnectedDomains* fcd = *connectedDomains;
		*connectedDomains = 0;

		delete fcd;
	}
}

void DrawConnectedDomains(void* image, FConnectedDomains* connectedDomains, unsigned int color)
{
	connectedDomains->Draw((IplImage*)image, color);
}

