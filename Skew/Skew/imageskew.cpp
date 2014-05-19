
#include <stdlib.h>
#include <windows.h>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;

#include "ImportOpenCVLib2012_DPATH.h"


#ifdef __cplusplus
extern "C" {
#endif

bool ImageSkew( void* srcImage, void** desImage )
{
	return true;
}

void ImageDestroy(void** desImage )
{

}

#ifdef __cplusplus
}
#endif

