#include"docSeg.h"
struct ColLine
{
	int start;
	int end;
};
struct ColLines
{
	vector<ColLine*> items;
	int yStart;
	int yEnd;
			
};
void DocSeg::ConnectDomainToIpl(DocSeg* cd,IplImage* dst)
{
	int stride=dst->widthStep;
	cvSetZero(dst);
	cvNot(dst,dst);
	FConnectedDomain* fcd1 = 0;
	//cvSaveImage("d://new4.jpg", dst);
	for(int i = 0; i < cd->fcds->ConnectedDomains.size(); i++)
	 {
		 fcd1 = cd->fcds->ConnectedDomains[i];
	
		 {
			 for(int j=0;j<fcd1->Xpoints.size();j++)
			 {
				 CvPoint *point=(CvPoint*)fcd1->Xpoints[j];
				 unsigned char *dstD=(unsigned char*) dst->imageData+point->y*stride;
				 dstD[point->x]=0;
			 }
		 }
			 
	 }	
	// cvSaveImage("d://new5.jpg",dst);
}

void DocSeg::delBigConnectDomain(DocSeg *connectedDomains)
{
	DocSeg* fcd1 = 0;
	connectedDomains->CalFont();
	FConnectedDomain* fcd = 0;
	int font=connectedDomains->Font;
	font-=2;
	int height=0,width=0;
	int little=300/15;
	for(int i =connectedDomains->fcds->ConnectedDomains.size()-1; i >= 0; i--)
	{
			fcd = connectedDomains->fcds->ConnectedDomains[i];
			height=fcd->rect.height;
			width=fcd->rect.width;
			if (((height>font+little && width>font+little)|| (width>15*height || height>15*width) || (height>15*font || width>15*font)))
			{
				connectedDomains->fcds->ConnectedDomains.erase(connectedDomains->fcds->ConnectedDomains.begin() + i);
		
			}			

	}
}
void DocSeg::delSmallConnectDomain(DocSeg *connectedDomains,int width,int height)
{
	FConnectedDomain* fcd1 = 0;	
	FConnectedDomain* fcd = 0;
	for(int i =connectedDomains->fcds->ConnectedDomains.size()-1; i >= 0; i--)
	{
			fcd = connectedDomains->fcds->ConnectedDomains[i];
			if(fcd->rect.width<width/10 && fcd->rect.height<height/10)
			{
				connectedDomains->fcds->ConnectedDomains.erase(connectedDomains->fcds->ConnectedDomains.begin() + i);
			}
			else if(abs(fcd->rect.width-fcd->rect.height)<=40)
			{
				connectedDomains->fcds->ConnectedDomains.erase(connectedDomains->fcds->ConnectedDomains.begin() + i);
			}

	}
}


void DocSeg::skew(DocSeg *connectedDomains,double *angle1)
{
	
	
	FConnectedDomain* fcd1 = 0;
	 vector<double> angle; 
	 vector<double> angle2;
	 
	 for(int i = 0; i < connectedDomains->fcds->ConnectedDomains.size(); i++)
	{
		fcd1 = connectedDomains->fcds->ConnectedDomains[i];
		double ang=0.0;
		double ang1=0.0;
		int k=0;
		int n=0;
		vector<CvPoint*> XYpoints;		
		double slope=0.0;
		double slope2=0.0;
		getSlope(slope,fcd1->Xpoints);
		getSlope1(slope2,fcd1->Xpoints);
		ang=atan(slope)*180/CV_PI;
		ang1=atan(slope2)*180/CV_PI;
		angle.push_back(ang);
		angle2.push_back(ang1);
	}
	 //double avangle=0.0;
	 //for(int i=0;i<angle.size();i++)
		//avangle+=angle.at(i);
	 //avangle=avangle/angle.size();
	 //*angle1=avangle;
	 //if(abs(avangle)<1) *angle1=0;
	 //if(abs(avangle)>45)
	 //{
		//double avangle2=0.0;
		//for(int i=0;i<angle2.size();i++)
		//	avangle2+=angle2.at(i);
		//avangle2=avangle2/angle2.size();
		//*angle1=avangle2;
		//if(avangle2<0)
		//	*angle1=-(90+avangle2);
		//else
		//	*angle1=90-avangle2;	
		//}	

	double avangle=0.0;	
	int plus=0,minus=0;
	double avangle1=0.0;
	for(int i=0;i<angle.size();i++)
	{
		if(angle[i]<0)
		{
			minus++;
			avangle+=angle[i];
		}
		else 
		{
			plus++;
			avangle1+=angle[i];
		}
	}
	if(plus>minus)
	{	
		avangle1=avangle1/plus;
		*angle1=avangle1;
	}
	else 
	{	
		avangle=avangle/minus;
		*angle1=avangle;
	}
	
	if(abs(*angle1)>45)
	{
	
		double avangle2=0.0;
		double avangle3=0.0;
		plus=0;minus=0;
		for(int i=0;i<angle2.size();i++)
		{
			if(angle2[i]<0)
			{
				minus++;
				avangle2+=angle2[i];
			}
			else 
			{
				plus++;
				avangle3+=angle2[i];
			}
		}
		if(plus>minus)
		{	
			avangle3=avangle3/plus;
			if(avangle3<0)
				*angle1=-(90+avangle3);
			else
				*angle1=90-avangle3;
			
		}
		else 
		{	
			avangle2=avangle2/minus;
			if(avangle2<0)
				*angle1=-(90+avangle2);
			else
				*angle1=90-avangle2;
			
		}
	}
	//FromImage(image);
}
void DocSeg::CalFont()
{
	int min=int(0.06*300);	// 六号字体的高�?
	int array1[150],array2[150];
	int i,j;
	int height;
	memset(array1,0,150*sizeof(int));
	memset(array2,0,150*sizeof(int));

	FConnectedDomain* fcd1 = 0;
	for ( i=0; i<fcds->ConnectedDomains.size(); i++ )
	{
		fcd1 =fcds-> ConnectedDomains[i];
		height=fcd1->rect.height;//-fcd1->rect.y+1;//.m_pBlockIndex[i].bottom-Blocks.m_pBlockIndex[i].top+1;
		if ( height>=min && height<150 )
			array1[height]++;
	}
	array2[min+4]=array1[min]+array1[min+1]+array1[min+2]+array1[min+3]+array1[min+4];
	for (i=min+5;i<150;i++)
		array2[i]=array2[i-1]+array1[i]-array1[i-5];
	j=min+4;
	for ( i=min+5; i<150; i++ )
		if ( array2[i]>array2[j] )
			j=i;
	
	Font=j;	
	//Font=fcds->Font;
}
void DocSeg::setFont(int font)
{
	Font=font;
}
int DocSeg::getFont()
{
	return Font;
}
void DocSeg::distinctionAttr()
{
	int font1=0;
	int font=Font;
	font-=2;
	font1=font/4;
	FConnectedDomain* fcd1 = 0;
	int height=0,width=0;
	int little=300/15;
	int *cle=(int*)malloc(fcds->ConnectedDomains.size()*sizeof(int));
	int k=0;
	for(int i = 0; i < fcds->ConnectedDomains.size(); i++)
	{
		fcd1 = fcds->ConnectedDomains[i];
		height=fcd1->rect.height;
		width=fcd1->rect.width;
		if (((height>font+little && width>font+little)|| (width>15*height || height>15*width) || (height>15*font || width>15*font)))
		{
			if(height<font/2 && width>6*font)
				fcd1->attr=h_Line;
			else if(width<font/2 && height>6*font)
				fcd1->attr=v_Line;
			else
				fcd1->attr=NoText;
		}
		else
			fcd1->attr=Text;
	}	
}

void DocSeg::ComBlocks()
{
	int font=0;	
	//CalFont();
	distinctionAttr();
	DelBlock();
	ComText();
	ComVText();	
}
void DocSeg::ComText()
{
	bool found = true;
	int pos=0;
	while(found)
	{
		found = false;
		int index1 = 0;
		FConnectedDomain* fcd = 0;
		FConnectedDomain* fcd2 = 0;

		vector<int> index3;
		
		for(int i = pos; i < fcds->ConnectedDomains.size() - 1; i++)
		{
			fcd = fcds->ConnectedDomains[i];
			
			double fx=0.0,fy=0.0;	
			double L=0.0,R=0.0;
			if(fcd->attr==Text)
			for(int j = i + 1; j < fcds->ConnectedDomains.size(); j++)
			{
				fcd2 = fcds->ConnectedDomains[j];
				
				if(fcd2->attr==Text)
				{
					
					L=max(fcd->rect.x,fcd2->rect.x);
					R=min(fcd->rect.width+fcd->rect.x,fcd2->rect.width+fcd2->rect.x);
					if(L<R)
						fx=0.0;
					else
						fx=L-R;
					L=max(fcd->rect.y,fcd2->rect.y);
					R=min(fcd->rect.height+fcd->rect.y,fcd2->rect.height+fcd2->rect.y);
					if(L<R)
						fy=0.0;
					else
						fy=L-R;
					if(fy==0)
					{
						if(fx<2*Font)//
						{
							index1 = i; found = true;
							index3.push_back(j); 
						}
						//break;
					}
				}
				//}
			}
			if(found == true){ pos=i;break;}
		}

		if(found == true)
		{
			for(int i = 0; i < index3.size(); i++)
			{
				fcd2 = fcds->ConnectedDomains[index3[i]];
				int x=min(fcd->rect.x,fcd2->rect.x);
				int y=min(fcd->rect.y,fcd2->rect.y);
				int width = max(fcd->rect.x + fcd->rect.width, fcd2->rect.x + fcd2->rect.width)-x;
				int height = max(fcd->rect.y + fcd->rect.height, fcd2->rect.y + fcd2->rect.height)-y;
				fcd->rect.x=x;
				fcd->rect.y=y;
				fcd->rect.width = width;
				fcd->rect.height = height;
				for(int k = 0; k < fcd2->Xpoints.size(); k++)
				{
					CvPoint* cp = fcd2->Xpoints[k];
					fcd->Xpoints.push_back(cp);
				}
				fcd2->Xpoints.clear();
				
			}

			for(int i = index3.size() - 1; i >= 0; i--)
			{
				fcds->ConnectedDomains.erase(fcds->ConnectedDomains.begin() + index3[i]);
			}
		}
	}	
}

void DocSeg::ComVText()
{
	bool found = true;
	int pos=0;
	while(found)
	{
		found = false;
		int index1 = 0;
		FConnectedDomain* fcd = 0;
		FConnectedDomain* fcd2 = 0;

		vector<int> index3;
		
		for(int i =pos; i <fcds-> ConnectedDomains.size() - 1; i++)
		{
			fcd =fcds-> ConnectedDomains[i];
			double fx=0.0,fy=0.0;	
			double L=0.0,R=0.0;
			if(fcd->attr==Text)
			for(int j = i + 1; j < fcds->ConnectedDomains.size(); j++)
			{
				fcd2 = fcds->ConnectedDomains[j];
				
				if(fcd2->attr==Text)
				{
					
					L=max(fcd->rect.x,fcd2->rect.x);
					R=min(fcd->rect.width+fcd->rect.x,fcd2->rect.width+fcd2->rect.x);
					if(L<R)
						fx=0.0;
					else
						fx=L-R;
					L=max(fcd->rect.y,fcd2->rect.y);
					R=min(fcd->rect.height+fcd->rect.y,fcd2->rect.height+fcd2->rect.y);
					if(L<R)
						fy=0.0;
					else
						fy=L-R;
					if(fx==0)
					{
						if(fy<=Font)//ceil((double)font/2)
						{
							index1 = i; found = true;
						    index3.push_back(j); 
							break;
						}
						
					}
				}
				//}
			}
			if(found == true){ pos=i; break;}
		}

		if(found == true)
		{
			for(int i = 0; i < index3.size(); i++)
			{
				fcd2 = fcds->ConnectedDomains[index3[i]];
				int x=min(fcd->rect.x,fcd2->rect.x);
				int y=min(fcd->rect.y,fcd2->rect.y);
				int width = max(fcd->rect.x + fcd->rect.width, fcd2->rect.x + fcd2->rect.width)-x;
				int height = max(fcd->rect.y + fcd->rect.height, fcd2->rect.y + fcd2->rect.height)-y;
				fcd->rect.x=x;
				fcd->rect.y=y;
				fcd->rect.width = width;
				fcd->rect.height = height;
				for(int k = 0; k < fcd2->Xpoints.size(); k++)
				{
					CvPoint* cp = fcd2->Xpoints[k];
					fcd->Xpoints.push_back(cp);
				}
				fcd2->Xpoints.clear();
				
			}

			for(int i = index3.size() - 1; i >= 0; i--)
			{
				fcds->ConnectedDomains.erase(fcds->ConnectedDomains.begin() + index3[i]);
			}
		}
	}
	
}

void DocSeg::DelBlock()
{
	FConnectedDomain* fcd = 0;
	for(int i =fcds->ConnectedDomains.size()-1; i >= 0; i--)
	{
			fcd = fcds->ConnectedDomains[i];
			if(fcd->rect.width<6 && fcd->rect.height<6)
			{
				fcds->ConnectedDomains.erase(fcds->ConnectedDomains.begin() + i);
				//i--;
			}

	}	
}
void DocSeg::ComLines(Lines &lines,int h,int v)
{
	int *store=new int[lines.num];
	if  ( !store ) return;
	memset(store,0,lines.num*sizeof(int));
	for(int i=0;i<lines.num-1;i++)
	{
		if ( store[i] ) continue;
		for(int j=i+1;j<lines.num;j++)
		{
			if(store[j]) continue;
			int y11=lines.lines[i].y0-v,y12=lines.lines[i].y1+v;
			int x11=lines.lines[i].x0-h,x12=lines.lines[i].x1+h;
			int y21=lines.lines[j].y0,y22=lines.lines[j].y1;
			int x21=lines.lines[j].x0,x22=lines.lines[j].x1;
			if ( ((y11>=y21 && y11<=y22) || (y21>=y11 && y21<=y12)) &&
				((x11>=x21 && x11<=x22) || (x21>=x11 && x21<=x12)) )
			{
				store[j]=1;
				if ( lines.lines[i].x0>lines.lines[j].x0 ) lines.lines[i].x0=lines.lines[j].x0;
				if ( lines.lines[i].x1<lines.lines[j].x1 ) lines.lines[i].x1=lines.lines[j].x1;
				if ( lines.lines[i].y0>lines.lines[j].y0 ) lines.lines[i].y0=lines.lines[j].y0;
				if ( lines.lines[i].y1<lines.lines[j].y1 ) lines.lines[i].y1=lines.lines[j].y1;
				//if(x12<x22) hlines.lines[i].length=x22-hlines.lines[i].start;
				
				
			}
		}
	}
	int j=0;
	for (int i=0; i<lines.num; i++ )
		if ( !store[i] )
		{
			if ( i!=j ) lines.lines[j]=lines.lines[i];
			j++;
		}
		lines.num=j;

}
void DocSeg::sort(Lines &lines)
{
	for ( int i=0; i<lines.num; i++ )
	{
		int min=i;
		for ( int j=i+1; j<lines.num; j++ )
			if ( lines.lines[min].pixN>lines.lines[j].pixN )
				min=j;
		Line temp=lines.lines[min];
		lines.lines[min]=lines.lines[i];
		lines.lines[i]=temp;
	}
}
void DocSeg::GetCross(Lines &line1,Lines &line2,crossTable &crossT)
{
	for ( int i=0; i<line1.num; i++ )
		for ( int j=0; j<line2.num; j++ )
		{
			int site=i*crossT.v_num+j;
			int x11=line1.lines[i].x0,x12=line1.lines[i].x1;
			int y11=line1.lines[i].y0,y12=line1.lines[i].y1;
			int x21=line2.lines[j].x0,x22=line2.lines[j].x1;
			int y21=line2.lines[j].y0,y22=line2.lines[j].y1;
			//change 2014.01.06
			crossT.tab[site].x=line2.lines[j].pixN;
			crossT.tab[site].y=line1.lines[i].pixN;
			crossT.tab[site].h=i;
			crossT.tab[site].v=j;
			if ( ((y11<=y22 && y11>=y21) || (y12<=y22 && y12>=y21)) &&
				 ((x21<=x12 && x21>=x11) || (x22<=x12 && x22>=x11)) )
			{
				//crossT.tab[site].x=line2.lines[j].pixN;
				//crossT.tab[site].y=line1.lines[i].pixN;
				crossT.tab[site].val=1;
				//crossT.tab[site].h=i;
				//crossT.tab[site].v=j;
			}
			else crossT.tab[site].val=0;
		}
}
void DocSeg::DelLines(Lines &line1,Lines &line2)
{
	int *store=new int[line1.num];
	if  ( !store ) return;
	memset(store,0,line1.num*sizeof(int));
	int *store1=new int[line2.num];
	if  ( !store1 ) return;
	memset(store1,0,line2.num*sizeof(int));
	for ( int i=0; i<line1.num; i++ )
	{
		if(store[i]) continue;
		for ( int j=0; j<line2.num; j++ )
		{
			int x11=line1.lines[i].x0,x12=line1.lines[i].x1;
			int y11=line1.lines[i].y0,y12=line1.lines[i].y1;
			int x21=line2.lines[j].x0,x22=line2.lines[j].x1;
			int y21=line2.lines[j].y0,y22=line2.lines[j].y1;
			if ( ((y11<=y22 && y11>=y21) || (y12<=y22 && y12>=y21)) &&
				 ((x21<=x12 && x21>=x11) || (x22<=x12 && x22>=x11)) )
			{
				store[i]++;//=1;
				//break;
			}
			
		}
	}
	//
	
	for ( int j=0; j<line2.num; j++ )
	{	
		if(store1[j]) continue;
		for ( int i=0; i<line1.num; i++ )
		{
				int x11=line1.lines[i].x0,x12=line1.lines[i].x1;
				int y11=line1.lines[i].y0,y12=line1.lines[i].y1;
				int x21=line2.lines[j].x0,x22=line2.lines[j].x1;
				int y21=line2.lines[j].y0,y22=line2.lines[j].y1;
				if ( ((y11<=y22 && y11>=y21) || (y12<=y22 && y12>=y21)) &&
					 ((x21<=x12 && x21>=x11) || (x22<=x12 && x22>=x11)) )
				{
					store1[j]++;//=1;
					//break;
				}
			
			}
		}
	int j=0;
	for (int i=0; i<line1.num; i++ )
		if ( store[i]>1 )
		{
			if ( i!=j ) line1.lines[j]=line1.lines[i];
			j++;
		}
		line1.num=j;

		
	j=0;
	for (int i=0; i<line2.num; i++ )
		if ( store1[i] >1)
		{
			if ( i!=j ) line2.lines[j]=line2.lines[i];
			j++;
		}
		line2.num=j;
}
int DocSeg::FindLine(IplImage* image,CvRect rect)
{
	IplImage* image1 = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopy(image, image1);
	int stride=image1->widthStep;
	bool flag=0;
	Lines *hline=new Lines(100);
	Lines *vline=new Lines(100);
	//Lines hline(100),vline(100);	
	int num=0;
	int st=0;
	int k=0;

	//

	for(int i=0;i<rect.height;i++)
	{
		unsigned char *src=(unsigned char*)image1->imageData+(rect.y+i)*stride;
		src+=rect.x;
		
		num=0;
		st=0;
		for(int j=0;j<rect.width;j++)
		{
			
			if(src[j]==0)//black
			/*{*/	num++;/*	flag=0;}*/
			else if(src[j]==255)// && flag 
			{	
				
				if(j-st>1.5*Font)//*Font 3*font
				{
					if(hline->num>=hline->size)
						hline->Increase(10);
					hline->lines[k].y0=rect.y+i;
					hline->lines[k].y1=rect.y+i;
					hline->lines[k].x0=st+rect.x;
					hline->lines[k].x1=j+rect.x;//j-st=length;
					hline->lines[k].pixN=1;
					/*{
						for(int l=st+rect.x;l<=j+rect.x;l++)
						{	
							CvPoint* cp = new CvPoint();
							cp->x =l;
							cp->y = rect.y+i;
							hline->Xpoints.push_back(cp);
						}
					}*/
					hline->num++;
					k++;
					st=j;
				}
				num=0;
				st=j;				
			}			
		}
		if(rect.width-st>1.5*Font)//3*Font 3*font
		{
			if(hline->num>=hline->size)
				hline->Increase(10);
			hline->lines[k].y0=rect.y+i;
			hline->lines[k].y1=rect.y+i;
			hline->lines[k].x0=st+rect.x;
			hline->lines[k].x1=rect.width+rect.x;//rect.width-st=length,rect.width-1;
			hline->lines[k].pixN=1;
			/*{
				for(int l=st+rect.x;l<=rect.width+rect.x;l++)
				{	
					CvPoint* cp = new CvPoint();
					cp->x =l;
					cp->y = rect.y+i;
					hline->Xpoints.push_back(cp);
				}
			}*/
			hline->num++;
			k++;
		}
	}

	//取垂直直�?
	k=0;
	num=0;
	st=0;
	for(int i=0;i<rect.width;i++)
	{
		num=0;
		st=0;
		for(int j=0;j<rect.height;j++)
		{
			unsigned char *src=(unsigned char*)image1->imageData+(rect.y+j)*stride;
			src+=rect.x+i;
			if(src[0]==0)
			num++;
			else if(src[0]==255)
			{
				if(j-st>1.5*Font)//3*Font3*font
				{
					if(vline->num>=vline->size)
						vline->Increase(10);
					vline->lines[k].x0=rect.x+i;
					vline->lines[k].x1=rect.x+i;
					vline->lines[k].y0=st+rect.y;			
					vline->lines[k].y1=j+rect.y;
					vline->lines[k].pixN=1;
					/*{
					for(int l=st+rect.y;l<=j+rect.y;l++)
					{	
						CvPoint* cp = new CvPoint();
						cp->x =rect.x+i;
						cp->y = l;
						vline->Xpoints.push_back(cp);
					}
					}*/
					vline->num++;
					k++;
					st=j;
				}
				num=0;
				st=j;
			}
		}
		
		if(rect.height-st>1.5*Font)//3*Font 3*fon
		{
			if(vline->num>=vline->size)
				vline->Increase(10);			
			vline->lines[k].x0=rect.x+i;
			vline->lines[k].x1=rect.x+i;
			vline->lines[k].y0=st+rect.y;
			vline->lines[k].y1=rect.height+rect.y;
			vline->lines[k].pixN=1;
			/*{
					for(int l=st+rect.y;l<=rect.height+rect.y;l++)
					{	
						CvPoint* cp = new CvPoint();
						cp->x =rect.x+i;
						cp->y = l;
						vline->Xpoints.push_back(cp);
					}
			}*/
			vline->num++;
			k++;					
		}
	}
	
	
	//ComLines1(hlines,2*Font,3,h_lines);
	ComLines(*hline,2*Font,3);//
	ComLines(*vline,3,2*Font);
	ComLines(*hline,0,0);//
	ComLines(*vline,0,0);
	
	if(hline->num>1 && vline->num>1)//表格
	{
		for ( int i=0; i<hline->num; i++ )
		{
			hline->lines[i].pixN=(hline->lines[i].y0+hline->lines[i].y1)/2;
		}
		for (int i=0; i<vline->num; i++ )
		{
			vline->lines[i].pixN=(vline->lines[i].x0+vline->lines[i].x1)/2;
		}
		sort(*hline);//
		sort(*vline);//
		
		DelLines(*hline,*vline);
		crossTable *crossT=new crossTable(hline->num,vline->num);
		//得到交叉表格
		GetCross(*hline,*vline,*crossT);
		crossTables.push_back(crossT);
		hlines.push_back(hline);
		vlines.push_back(vline);
		return 1;

	}
	else//if(hlines.num<=1 && vlines.num<=1)
	{

		return 2;//是图�?

	}	
}

void DocSeg::getSlope(double &slope,vector<CvPoint*> XYpoints)
{
	double sumX=0.0,sumX2=0.0;
	double sumY=0.0,sumXY=0.0;
	int nCount=XYpoints.size();
	for(int i=0;i<nCount;i++)
	{
		sumX+=((CvPoint *)XYpoints.at(i))->x;
		sumX2+=((CvPoint *)XYpoints.at(i))->x*((CvPoint *)XYpoints.at(i))->x;
	}
	for(int i=0;i<nCount;i++)
	{
		sumY+=((CvPoint *)XYpoints.at(i))->y;		
	}
	for(int i=0;i<nCount;i++)
	{
		sumXY+=((CvPoint *)XYpoints.at(i))->x*((CvPoint *)XYpoints.at(i))->y;		
	}
	slope = ( (nCount*sumXY - sumX*sumY) / (nCount*sumX2 - sumX * sumX));  

}
void DocSeg::getSlope1(double &slope,vector<CvPoint*> XYpoints)
{
	double sumX=0.0,sumX2=0.0;
	double sumY=0.0,sumXY=0.0;
	int nCount=XYpoints.size();
	for(int i=0;i<nCount;i++)
	{
		sumX+=((CvPoint *)XYpoints.at(i))->y;
		sumX2+=((CvPoint *)XYpoints.at(i))->y*((CvPoint *)XYpoints.at(i))->y;
	}
	for(int i=0;i<nCount;i++)
	{
		sumY+=((CvPoint *)XYpoints.at(i))->x;		
	}
	for(int i=0;i<nCount;i++)
	{
		sumXY+=((CvPoint *)XYpoints.at(i))->x*((CvPoint *)XYpoints.at(i))->y;		
	}
	slope = ( (nCount*sumXY - sumX*sumY) / (nCount*sumX2 - sumX * sumX));  

}
void DelSmallConnectDomain( DocSeg*connectedDomains,int width,int heigth)
{
	connectedDomains->delSmallConnectDomain(connectedDomains,width,heigth);
}
void DelBigConnectDomain(DocSeg *connectedDomains)
{
	connectedDomains->delBigConnectDomain(connectedDomains);
}
void ConnetionsToImage(DocSeg* cd,IplImage* dst)
{
	cd->ConnectDomainToIpl(cd,dst);

}
double Skew(IplImage* image)
{
	
	IplImage *morph = cvCreateImage(cvSize(image->width, image->height), image->depth, image->nChannels);
	DocSeg* connectedDomains=new DocSeg();
	GetConnectedDomains(image,connectedDomains->fcds);
	DelBigConnectDomain(connectedDomains);
	ConnetionsToImage(connectedDomains,morph);
	//cvSaveImage("d://mor.jpg",morph);
	IplConvKernel* element = cvCreateStructuringElementEx(3, 3, 0, 0,CV_SHAPE_RECT, 0);
    cvErode(morph, morph, element, 5);
	GetConnectedDomains(morph,connectedDomains->fcds);
	DelSmallConnectDomain(connectedDomains,image->width,image->height);
	double angle=0.0;
	DocSeg::skew(connectedDomains,&angle);
	ReleaseConnectedDomains(&connectedDomains->fcds);
	return angle;
}

void CombineBlocks(DocSeg* connectedDomains)
{
	connectedDomains->ComBlocks();
}
void DiscTableOrPicture(IplImage* image,DocSeg* connectedDomains)
{
	int flag=0;
	for(int i=0;i<connectedDomains->fcds->ConnectedDomains.size();i++)
	{
		FConnectedDomain* fcd = connectedDomains->fcds->ConnectedDomains[i];
		if(fcd->attr==NoText)
		{	
			flag=connectedDomains->FindLine(image,fcd->rect);
			if(flag==1)//表格
				fcd->attr=Table;
			else
				fcd->attr=Picture;
		}
	}
	
}
void DrawLines(IplImage* image,DocSeg* connectedDomains)
{
	
	connectedDomains->DrawLine(image);
}



int GetForms(int* &rectArray,DocSeg* connectedDomains,int *&numLen,int &Num)
{
	vector<vector<CvRect*> >  rectS;
	int num=connectedDomains->crossTables.size();
	int size=0;
	for(int i=0;i<num;i++)
	{
		int x=0,y=0;
		bool flag=0;
		int k=0;
		int n=0,m=0;
		int site=0,site1=0,site2=0,site3=0;
		crossTable *crossT=connectedDomains->crossTables[i];
		vector<CvRect*>  rectSS;
		for(int j=0;j<crossT->h_num;j++)
		{
			 for( k=0;k<crossT->v_num;k++)
			 {	
				 flag=0;
				site1=j*crossT->v_num+k;
				if(crossT->tab[site1].val==1)
				{
					for(m=k+1;m<crossT->v_num;m++)
					{
						site3=j*crossT->v_num+m;
						if(crossT->tab[site3].val==1)
						{
							for(n=j+1;n<crossT->h_num;n++)
							{
								site2=n*crossT->v_num+m;
								if(crossT->tab[site2].val==1)
								{
									site=n*crossT->v_num+k;
									if(crossT->tab[site].val==1)
									{
							
										CvRect *rect=new CvRect();
										rect->x=crossT->tab[site1].x;
										rect->y=crossT->tab[site1].y;
										rect->width=crossT->tab[site2].x-crossT->tab[site1].x+1;
										rect->height=crossT->tab[site2].y-crossT->tab[site1].y+1;
										rectSS.push_back(rect);
										flag=1;//
										k=m-1;
										break;
									}
								}
								
							}
						}
						if(flag) break;
					}
					
				
				}
			}
			 
		}	
		//size+=rectSS.size();
		rectS.push_back(rectSS);
	}
	Num=rectS.size();
	numLen=new int[rectS.size()]();
	for(int j=0;j<rectS.size();j++)
	{
		size+=rectS[j].size();
		numLen[j]=rectS[j].size();
	}
	rectArray=new int[size*4]();
	int renum=0;
	for(int j=0;j<rectS.size();j++)
	{
		vector<CvRect*>  rectT=rectS[j];
		for(int i=0;i<rectT.size();i++)
		{
			rectArray[renum++]=rectT[i]->x;
			rectArray[renum++]=rectT[i]->y;
			rectArray[renum++]=rectT[i]->width;
			rectArray[renum++]=rectT[i]->height;
		}
		
	}
	
	//return 10;
	return size;
}

int GetHorizonLines(int* &Hlines,DocSeg* connectedDomains,int *&BlinesNum,int &Num)
{
	Num=connectedDomains->hlines.size();
	int size=0;
	int lNum=0;
	BlinesNum=new int[Num]();
	for(int i=0;i<Num;i++)
	{	
		size+=connectedDomains->hlines[i]->num;
		BlinesNum[i]=connectedDomains->hlines[i]->num;
	}
	
	Hlines=new int[size*4]();
	for(int i=0;i<Num;i++)
	{
		Lines *line=connectedDomains->hlines[i];
		for(int j=0;j<line->num;j++)
		{
			Hlines[lNum++]=line->lines[j].x0;
			Hlines[lNum++]=line->lines[j].y0;
			Hlines[lNum++]=line->lines[j].x1;
			Hlines[lNum++]=line->lines[j].y1;
		}
	}
	return size;
}
int GetVerticalLines(int* &Vlines,DocSeg* connectedDomains,int *&BlinesNum,int &Num)
{
	Num=connectedDomains->vlines.size();
	int size=0;
	int lNum=0;
	BlinesNum=new int[Num]();
	for(int i=0;i<Num;i++)
	{	
		size+=connectedDomains->vlines[i]->num;
		BlinesNum[i]=connectedDomains->vlines[i]->num;
	}
	
	Vlines=new int[size*4]();
	for(int i=0;i<Num;i++)
	{
		Lines *line=connectedDomains->vlines[i];
		for(int j=0;j<line->num;j++)
		{
			Vlines[lNum++]=line->lines[j].x0;
			Vlines[lNum++]=line->lines[j].y0;
			Vlines[lNum++]=line->lines[j].x1;
			Vlines[lNum++]=line->lines[j].y1;
		}
	}
	return size;
}

int GetCells2(IplImage* image,CvRect rect,GridCell *&gc,vector<CellIndex> &trueCells)
{
	
	DocSeg* connectedDomains=new DocSeg();
	GetConnectedDomains(image,connectedDomains->fcds);
	connectedDomains->CalFont();
	CombineConnectedDomains(connectedDomains->fcds);
	
	int n=connectedDomains->FindLine(image,rect);
	if(n==1)
	{
		vector<ColLines*> cLines;
		vector<ColLines*> rLines;
		int thi=0;
		for(int ci=0;ci<connectedDomains->hlines.size();ci++)
		{
			for(int hi=0;hi<connectedDomains->hlines[ci]->num;hi++)
			{
				Line hl=connectedDomains->hlines[ci]->lines[hi];
				ColLine *s=new ColLine();
				ColLines *t=new ColLines();
				s->start=hl.x0;
				s->end=hl.x1;
				t->items.push_back(s);
				t->yStart=hl.y0;
				t->yEnd=hl.y1;
				for(int ti=hi+1;ti<connectedDomains->hlines[ci]->num;ti++)
				{
					Line tl=connectedDomains->hlines[ci]->lines[ti];
					//hl.y0==tl.y0 && hl.y1==tl.y1
					if((hl.y0+hl.y1)/2==(tl.y0+tl.y1)/2 || hl.y0==tl.y0 || hl.y1==tl.y1)
					{
						if( (s->start>=tl.x0 && s->start<=tl.x1)|| (s->end>=tl.x0 && s->end<=tl.x1))
						{
							s->start=min(s->start,tl.x0);
							s->end=max(s->end,tl.x1);
							//t->items.push_back(s);
							
						}
						else
						{
							//t->items.push_back(s);
							ColLine *s1=new ColLine();
							s1->start=tl.x0;
							s1->end=tl.x1;
							t->items.push_back(s1);
							//hi++;
						}
						hi++;
					}
					else
					{
						//t->items.push_back(s);
						break;
					}
				}
				cLines.push_back(t);
			}
		}
	
		for(int ci=0;ci<connectedDomains->vlines.size();ci++)
		{
			for(int hi=0;hi<connectedDomains->vlines[ci]->num;hi++)
			{
				Line hl=connectedDomains->vlines[ci]->lines[hi];
				ColLine *s=new ColLine();
				ColLines *t=new ColLines();
				s->start=hl.y0;
				s->end=hl.y1;
				t->items.push_back(s);
				t->yStart=hl.x0;
				t->yEnd=hl.x1;
				for(int ti=hi+1;ti<connectedDomains->vlines[ci]->num;ti++)
				{
					Line tl=connectedDomains->vlines[ci]->lines[ti];
					int cflag=0;
					//hl.x0==tl.x0 && hl.x1==tl.x1
					//(hl.x0==tl.x0 && abs(hl.x1-tl.x1)<=1)|| (hl.x1==tl.x1 && abs(hl.x0-tl.x0)<=1)

					if((hl.x0+hl.x1)/2==(tl.x0+tl.x1)/2 || hl.x0==tl.x0 || hl.x1==tl.x1 || hl.x0==tl.x1 || hl.x1==tl.x0)
					{
						if( (s->start>=tl.y0 && s->start<=tl.y1)|| (s->end>=tl.y0 && s->end<=tl.y1))
						{
							s->start=min(s->start,tl.y0);
							s->end=max(s->end,tl.y1);
							//hi++;
						}
						else
						{
							ColLine *s1=new ColLine();
							s1->start=tl.y0;
							s1->end=tl.y1;
							t->items.push_back(s1);
							
						}
						hi++;
						//cflag=1;
					}
					//if((hl.x0+hl.x1)/2==(tl.x0+tl.x1)/2 ||(tl.x0>=hl.x0 && tl.x1<=hl.x1))//|| hl.x0==tl.x0 || hl.x1==tl.x1 || hl.x0==tl.x1 || hl.x1==tl.x0)
					//	cflag=1;
					//else if(tl.x0<=hl.x1 && tl.x0>=hl.x0)
					//{
					//	t->yStart=hl.x0;
					//	t->yEnd=max(hl.x1,tl.x1);
					//	cflag=1;
					//}
					//else if(tl.x1<=hl.x1 && tl.x1>=hl.x0)
					//{
					//	t->yStart=min(hl.x0,tl.x0);
					//	t->yEnd=hl.x1;
					//	cflag=1;
					//}
					//else if(hl.x0>=tl.x0 && hl.x1<=tl.x1)
					//{
					//	t->yStart=tl.x0;
					//	t->yEnd=tl.x1;
					//	cflag=1;
					//}
					//if(cflag)
					//{
					//	ColLine *s1=new ColLine();
					//	s1->start=tl.y0;
					//	s1->end=tl.y1;
					//	t->items.push_back(s1);
					//	hi++;
					//}
					else
					{
						
						//t->items.push_back(s);
						break;
					}
				}
				rLines.push_back(t);
			}
		}
		
		//添加网格
		//{
			//GridCell *gc=new GridCell(cLines.size(),rLines.size());
			gc=new GridCell(cLines.size(),rLines.size());
			for(int g1=0;g1<rLines.size();g1++)
				gc->x[g1]=(rLines[g1]->yStart+rLines[g1]->yEnd)/2;
			for(int g1=0;g1<cLines.size();g1++)
				gc->y[g1]=(cLines[g1]->yStart+cLines[g1]->yEnd)/2;
		//}
		crossTable *crossT=new crossTable(cLines.size(),rLines.size());
		int flag=0;
		for ( int i=0; i<cLines.size(); i++ )
		for ( int j=0; j<rLines.size(); j++ )
		{
			int site=i*crossT->v_num+j;
			flag=0;
			for(int ii=0;ii<cLines[i]->items.size();ii++)
			{
				
				for(int jj=0;jj<rLines[j]->items.size();jj++)
				{
					
					int x11=cLines[i]->items[ii]->start,x12=cLines[i]->items[ii]->end;
					int y11=cLines[i]->yStart,y12=cLines[i]->yEnd;
					int x21=rLines[j]->yStart,x22=rLines[j]->yEnd;
					int y21=rLines[j]->items[jj]->start,y22=rLines[j]->items[jj]->end;
					//change 2014.01.06
					crossT->tab[site].x=(rLines[j]->yStart+rLines[j]->yEnd)/2;
					crossT->tab[site].y=(cLines[i]->yStart+cLines[i]->yEnd)/2;
					crossT->tab[site].h=i;
					crossT->tab[site].v=j;
					if ((((y11<=y22 && y11>=y21) || (y12<=y22 && y12>=y21)) &&
					((x21<=x12 && x21>=x11) || (x22<=x12 && x22>=x11))) || y11-y22==1 )//添加了y11-y22==1,离的比较近的
					{
						//if(ii>0)
							crossT->tab[site].vali=ii+1;
						//if(jj)
							crossT->tab[site].val=jj+1;
						//else
						//crossT->tab[site].val=1;
						flag=1;
						
						break;						
					}
					else crossT->tab[site].val=0;
				}
				if(flag) break;
			}
			
		}
		
	int size=0;
	int x=0,y=0;	
	int k=0;
	int n=0,m=0;
	int cNum=0;
	int site=0,site1=0,site2=0,site3=0;	
	int flag1=0;	
	//FILE *file=fopen("d://rindex1.txt","w");	
	for(int j=0;j<crossT->h_num;j++)
		{
			 for(  k=0;k<crossT->v_num;k++)
			 {	
				flag=0;
				site1=j*crossT->v_num+k;
				if(crossT->tab[site1].val>=1/* || crossT->tab[site1].val==2*/)
				{
					for(int m=j+1;m<crossT->h_num;m++)
					{
						site2=m*crossT->v_num+k;
						if(crossT->tab[site2].val>=1 &&(crossT->tab[site1].v==crossT->tab[site2].v && crossT->tab[site1].val== crossT->tab[site2].val))
						{
							for(int n=k+1;n<crossT->v_num;n++)
							{
								flag1=0;
								site3=j*crossT->v_num+n;
								site=m*crossT->v_num+n;
								if(crossT->tab[site].val>=1 && crossT->tab[site3].val>=1 &&(crossT->tab[site].v==crossT->tab[site3].v && crossT->tab[site].val== crossT->tab[site3].val)
									&&(crossT->tab[site1].h==crossT->tab[site3].h && crossT->tab[site1].vali== crossT->tab[site3].vali)
									&&(crossT->tab[site2].h==crossT->tab[site].h && crossT->tab[site2].vali== crossT->tab[site].vali))
								{
									int y1=0,yy1=0;
									int x1=0,xx1=0;;
									for(y1=0;y1<gc->row;y1++)
										if(gc->y[y1]==crossT->tab[site1].y) break;
									for(yy1=0;yy1<gc->row;yy1++)
										if(gc->y[yy1]==crossT->tab[site].y) break;
									for(x1=0;x1<gc->col;x1++)
										if(gc->x[x1]==crossT->tab[site1].x) break;
									for(xx1=0;xx1<gc->col;xx1++)
										if(gc->x[xx1]==crossT->tab[site].x) break;
									int start=0;
									CellIndex temp((yy1-y1)*(xx1-x1));
									int ci=0;
									for(int y11=y1;y11<yy1;y11++)
									{
										start=y11*(gc->col-1)+x1;
										
										
										for(int z=start;z<start+xx1-x1;z++)
										{
										
											temp.element[ci++]=z;
											
										   //fprintf(file,"%d,",z);
										}
										
										
									
									}
									trueCells.push_back(temp);
									flag=1;
									k=n-1;
									//fprintf(file,"%d,\n",0);
									break;									
								}
							}
						}
						if(flag) break;
						
					}
				}
			 }
		}
	
	ReleaseConnectedDomains(&connectedDomains->fcds);
	//return cNum;
	return trueCells.size();
	}	
	else
		return -1;
}

void  ReleaseMemory(unsigned char*& data)
{
	if(data != 0)
	{

		free (data);
		data = 0;
	}
}