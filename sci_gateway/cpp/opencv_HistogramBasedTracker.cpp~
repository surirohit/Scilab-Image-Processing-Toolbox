/***************************************************
Author : Tanmay Chaudhari
 ***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "sciprint.h"
#include "../common.h"
//#include "../common.cpp"

	Mat image;
	bool backprojMode = false;
	bool selectObject = false;
	int trackObject = 0;
	Point origin;
	Rect selection;
	int vmin = 10, vmax = 256, smin = 30;
    double xcord[1<<20];
    double ycord[1<<20];
    double width[1<<20];
    double height[1<<20];
    double orientation[1<<20];

	static void onMouse( int event, int x, int y, int, void* )
	{
		if( selectObject )
		{
			selection.x = MIN(x, origin.x);
			selection.y = MIN(y, origin.y);
			selection.width = std::abs(x - origin.x);
			selection.height = std::abs(y - origin.y);

			selection &= Rect(0, 0, image.cols, image.rows);
		}

		switch( event )
		{
			case EVENT_LBUTTONDOWN:
				origin = Point(x,y);
				selection = Rect(x,y,0,0);
				selectObject = true;
				break;
			case EVENT_LBUTTONUP:
				selectObject = false;
				if( selection.width > 0 && selection.height > 0 )
					trackObject = -1;
				break;
		}
	}
	
	int opencv_HistogramBasedTracker()
	{	
		SciErr sciErr;
        int* piAddr = NULL;
        int iRows = 0;
        int iter = 0;
        int iCols = 0;
        int iRet = 0;
        char** pstData = NULL;

        CheckInputArgument(pvApiCtx, 1, 1);
        CheckOutputArgument(pvApiCtx, 1, 2);

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        iRet = getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstData);
        if(iRet)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstData);
            return iRet;
        }
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pstData);
        if(sciErr.iErr)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstData);
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
		
		VideoCapture cap(pstData[0]);
		Rect trackWindow;
		int hsize = 16;
		float hranges[] = {0,180};
		const float* phranges = hranges;
		namedWindow( "Video Player", 0 );
		setMouseCallback( "Video Player", onMouse, 0 );

		Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
		bool paused = false;

		for(;;)
		{
			if( !paused)
			{
				cap >> frame;
				if( frame.empty() )
					break;
			}

			frame.copyTo(image);

			if( !paused )
			{
				cvtColor(image, hsv, COLOR_BGR2HSV);

				if( trackObject )
				{
					int _vmin = vmin, _vmax = vmax;

					inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
							Scalar(180, 256, MAX(_vmin, _vmax)), mask);
					int ch[] = {0, 0};
					hue.create(hsv.size(), hsv.depth());
					mixChannels(&hsv, 1, &hue, 1, ch, 1);

					if( trackObject < 0 )
					{
						Mat roi(hue, selection), maskroi(mask, selection);
						calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
						normalize(hist, hist, 0, 255, NORM_MINMAX);

						trackWindow = selection;
						trackObject = 1;

						histimg = Scalar::all(0);
						int binW = histimg.cols / hsize;
						Mat buf(1, hsize, CV_8UC3);
						for( int i = 0; i < hsize; i++ )
							buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
						cvtColor(buf, buf, COLOR_HSV2BGR);

						for( int i = 0; i < hsize; i++ )
						{
							int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
							rectangle( histimg, Point(i*binW,histimg.rows),
									Point((i+1)*binW,histimg.rows - val),
									Scalar(buf.at<Vec3b>(i)), -1, 8 );
						}
					}

					calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
					backproj &= mask;
					RotatedRect trackBox = CamShift(backproj, trackWindow,
							TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
					if( trackWindow.area() <= 1 )
					{
						int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
						trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
								trackWindow.x + r, trackWindow.y + r) &
							Rect(0, 0, cols, rows);
					}

					if( backprojMode )
						cvtColor( backproj, image, COLOR_GRAY2BGR );
					ellipse( image, trackBox, Scalar(0,0,255), 3, 16 );
					xcord[iter] = trackBox.center.x - trackBox.size.width;
					ycord[iter] = trackBox.center.y - trackBox.size.height;
					width[iter] = 2*trackBox.size.width;
					height[iter] = 2*trackBox.size.height;
					orientation[iter] = trackBox.angle;
					iter++;	
				}

			}
			else if( trackObject < 0 )
				paused = false;

			if( selectObject && selection.width > 0 && selection.height > 0 )
			{
				Mat roi(image, selection);
				bitwise_not(roi, roi);
			}
			imshow( "Video Player", image );
			char c = (char)waitKey(30);
			if( c == 27 )
				break;
			else if(c == 32)
				paused = !paused;
			else;
			waitKey(20);
		}
		destroyAllWindows();

		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 4, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, 1, iter, xcord);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 2, 1, iter, ycord);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 3, 1, iter, width);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 4, 1, iter, height);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, piAddr, 1, 1, iter, orientation);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
		ReturnArguments(pvApiCtx);
		backprojMode = false;
		selectObject = false;
		trackObject = 0;
		return 0;
	}
	/* ==================================================================== */
}
