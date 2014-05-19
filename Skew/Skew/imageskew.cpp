
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "docSeg.h"

using namespace cv;

#include "ImportOpenCVLib2012_DPATH.h"

const double  PI = 3.14159;

IplImage* RotateImage( IplImage* srcImage , double angle );

#ifdef __cplusplus
extern "C" {
#endif

bool WINAPI ImageSkew( void* srcImage, void** desImage )
{
	*desImage = NULL;

	IplImage* pCvSrcImage  = (IplImage*)srcImage;
	if( !pCvSrcImage || pCvSrcImage->nChannels != 1){
		return false;
	} 

	double dAngle = Skew(pCvSrcImage);

	if ( dAngle > 0.001 || dAngle < -0.001 )
	{
	//	bStatus = RotateImage(d);
		IplImage* pCvDesImage = RotateImage(  pCvSrcImage ,  dAngle );
		*desImage = (void*)pCvDesImage;
	}

	return true;
}

void WINAPI ImageDestroy(void** desImage )
{

}

#ifdef __cplusplus
}
#endif


IplImage* RotateImage( IplImage* srcImage , double angle )
{
	if ( !srcImage ){
		return NULL;
	}

	IplImage* dst = NULL;

	float m[6];
	double n1 = 0;
	double n2 = 0;

	n1 = fabs( sin( -angle * PI / 180) * srcImage->width );
	n2 = fabs( cos( -angle * PI / 180) * srcImage->height );
	int newheight = (int)(n1 + n2);

	n1 = fabs(( sin(-angle * PI / 180) * srcImage->height));
	n2 = fabs(( cos(-angle * PI / 180) * srcImage->width));
	int newwidth = (int)(n1 + n2);

	dst = cvCreateImage(cvSize(newwidth, newheight), srcImage->depth, srcImage->nChannels );
	cvSetZero(dst);

	//保证原图可以任意角度旋转的最小尺寸  

	int tempLength = (int)(sqrt((double)srcImage->width * srcImage->width + (double)srcImage->height * srcImage->height) + 10);
	int tempX = (tempLength + 1) / 2 - srcImage->width / 2;
	int tempY = (tempLength + 1) / 2 - srcImage->height / 2;

	IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), srcImage->depth, srcImage->nChannels );

	cvSetZero(temp);
	cvNot(temp, temp);

	//将原图复制到临时图像tmp中心  
	CvRect rect;
	rect.x = tempX;
	rect.y = tempY;
	rect.width = srcImage->width;
	rect.height = srcImage->height;

	cvSetImageROI(temp, rect);

	cvCopy( srcImage, temp, 0);
	cvResetImageROI(temp);

	int w = tempLength;
	int h = tempLength;

	m[0] = (float)(1 * cos(-angle * PI / 180));
	m[1] = (float)(1 * sin(-angle * PI / 180));
	m[2] = w * 0.5f;// x+width/2;  
	m[3] = -m[1];
	m[4] = m[0];
	m[5] = h * 0.5f;//y+heigth/2; 

	CvMat cm = cvMat( 2, 3, CV_32F , m );

	cvGetQuadrangleSubPix(temp, dst, &cm);

	cvReleaseImage( &temp);

	return dst;
}