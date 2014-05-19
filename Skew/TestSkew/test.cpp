#include <stdlib.h>
#include <Windows.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ImportOpenCVLib2012_DPATH.h"

using namespace cv;

typedef bool (WINAPI *ImageSkew)( void* srcImage, void** desImage );
typedef void (WINAPI *ImageDestroy)(void** desImage );

int main(int argc, char** argv)
{
	HINSTANCE hDll = NULL;
	IplImage* image = NULL;

	hDll = ::LoadLibrary( ("Skew.dll"));
	if ( NULL == hDll )
	{
		::MessageBox( ::GetActiveWindow() , ("找不到Skew.dll"), NULL, MB_OK);
		goto PRO_END;
	}

	ImageSkew	imageSkew = (ImageSkew)GetProcAddress(hDll,"ImageSkew");
	ImageDestroy imageDestroy = (ImageDestroy)GetProcAddress(hDll,"ImageDestroy");

	if ( !imageSkew || !imageDestroy )
	{
		::MessageBox( ::GetActiveWindow() , ("找不到Skew中的 imageSkew imageDestroy"), NULL, MB_OK);
		goto PRO_END;
	}

	image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	if (!image){
		goto PRO_END;
	}

	void* desImage = NULL;

	imageSkew( (void*)image, &desImage);

	if ( desImage)
	{
		cvSaveImage("desImage.bmp", (IplImage*)desImage);
		imageDestroy(&desImage);
	}

PRO_END:

	if (image){
		cvReleaseImage(&image);
	}
	if (hDll){
		::FreeLibrary(hDll);
	}
	

	return 0;
}