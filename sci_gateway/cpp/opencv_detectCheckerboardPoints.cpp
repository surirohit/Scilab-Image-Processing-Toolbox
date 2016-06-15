/***************************************************
Author      : Rohit Suri
***************************************************/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include <sciprint.h>
    #include "../common.h"

    int opencv_detectCheckerboardPoints(char *fname, unsigned long fname_len)
    {
        // Error management variables

        SciErr sciErr;

        //------Local variables------//

        int *piAddr;
        int *piChild;
        int *piGrandChild;
        int iItem;
        int iItemInner;
        int argCount;
        int iRows,iCols;
        int *piLen;
        char **pstData;

        int imgCount[2]={0};
        int returnSize[2];
        vector<Mat> inputs;
        Mat inp;
        vector<vector<Point2f> > chessboardPoints;
        vector<Size> chessboardSize;
        vector<Point2f> v;
        bool detected;
        int *usedImages;
        int ***points;
        int count = 0,pointsPos;
        //------Check number of parameters------//

        CheckInputArgument(pvApiCtx, 1, 2);
		CheckOutputArgument(pvApiCtx, 2, 3);
        argCount = *getNbInputArgument(pvApiCtx);

        //------Get input arguments------//

        for(int iter=0;iter<argCount;iter++)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx,iter+1,&piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if(isStringType(pvApiCtx,piAddr))
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                if(iRows!=1)
                {
                    Scierror(999, "Incorrect type of input argument(s).\n");
                    return 0;
                }
                imgCount[iter]=iCols;
            }
            else if(isListType(pvApiCtx,piAddr))
            {
                sciErr = getListItemAddress(pvApiCtx,piAddr,1,&piChild);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                if(isIntegerType(pvApiCtx,piChild) || isDoubleType(pvApiCtx,piChild))
                {
                    sciErr = getListItemNumber(pvApiCtx,piAddr,&iItem);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    if(iItem==1 || iItem==3)
                    {
                        for(int i=1;i<=iItem;i++)
                        {
                            sciErr = getListItemAddress(pvApiCtx,piAddr,i,&piChild);
                            if(sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 0;
                            }
                            if(isIntegerType(pvApiCtx,piChild) || isDoubleType(pvApiCtx,piChild))
                            {
                                continue;
                            }
                            else
                            {
                                Scierror(999, "Incorrect type of input argument(s).\n");
                                return 0;
                            }
                        }
                        imgCount[iter]=1;
                    }
                    else
                    {
                        Scierror(999, "Incorrect type of input argument(s).\n");
                        return 0;
                    }
                }
                else if(isListType(pvApiCtx,piChild))
                {
                    sciErr = getListItemNumber(pvApiCtx,piAddr,&iItem);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    for(int i=1;i<=iItem;i++)
                    {
                        sciErr = getListItemAddress(pvApiCtx,piAddr,i,&piChild);
                        if(sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 0;
                        }
                        if(isListType(pvApiCtx,piChild))
                        {
                            sciErr = getListItemNumber(pvApiCtx,piChild,&iItemInner);
                            if(sciErr.iErr)
                            {
                                printError(&sciErr, 0);
                                return 0;
                            }
                            if(iItemInner==1 || iItemInner==3)
                            {
                                for(int j=1;j<=iItemInner;j++)
                                {
                                    sciErr = getListItemAddress(pvApiCtx,piChild,j,&piGrandChild);
                                    if(sciErr.iErr)
                                    {
                                        printError(&sciErr, 0);
                                        return 0;
                                    }
                                    if(isIntegerType(pvApiCtx,piGrandChild) || isDoubleType(pvApiCtx,piGrandChild))
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        Scierror(999, "Incorrect type of input argument(s).\n");
                                        return 0;
                                    }
                                }
                            }
                            else
                            {
                                Scierror(999, "Incorrect type of input argument(s).\n");
                                return 0;
                            }
                        }
                        else
                        {
                            Scierror(999, "Incorrect type of input argument(s).\n");
                            return 0;
                        }
                    }
                    imgCount[iter]=iItem;
                }
                else
                {
                    Scierror(999, "Incorrect type of input argument(s).\n");
                    return 0;
                }
            }
            else
            {
                Scierror(999, "Incorrect type of input argument(s).\n");
                return 0;
            }
        }
        if(argCount==2 && imgCount[0]!=imgCount[1])
        {
            Scierror(999,"For Stereo Calibration, both image sets must have same number of images.\n");
            return 0;
        }
        for(int iter=0;iter<argCount;iter++)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx,iter+1,&piAddr);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            if(isStringType(pvApiCtx,piAddr))
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                piLen = (int*) malloc(sizeof(int) * iRows * iCols);

                sciErr = getMatrixOfString(pvApiCtx,  piAddr,  &iRows,  &iCols,  piLen,  NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                pstData = (char**) malloc(sizeof(char*) * iRows * iCols);
                for(int iterPstData = 0; iterPstData < iRows * iCols; iterPstData++)
                {
                    pstData[iterPstData] = (char*) malloc(sizeof(char) * piLen[iterPstData] + 1);
                }

                sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }

                for(int i=0;i<iCols;i++)
                {
                    inp = imread(pstData[i]);
                    if(!inp.data)
                    {
                        Scierror(999,"Cannot read image \'%s\'. Aborting.\n",pstData[i]);
                        return 0;
                    }
                    inputs.push_back(inp);
                }
            }
            else
            {
                sciErr = getListItemAddress(pvApiCtx,piAddr,1,&piChild);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                if(isIntegerType(pvApiCtx,piChild) || isDoubleType(pvApiCtx,piChild))
                {
                    retrieveImageFromAddress(inp,piAddr);
                    inputs.push_back(inp);
                }
                else
                {
                    sciErr = getListItemNumber(pvApiCtx,piAddr,&iItem);
                    if(sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }
                    for(int i=1;i<=iItem;i++)
                    {
                        sciErr = getListItemAddress(pvApiCtx,piAddr,i,&piChild);
                        if(sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            return 0;
                        }
                        retrieveImageFromAddress(inp,piChild);
                        inputs.push_back(inp);
                    }
                }
            }
        }

        //------Actual processing------//

        for(int iter=0;iter<inputs.size();iter++)
        {
            v.clear();
            detected = 0;
            for(int i=14;i>=3;i--)
            {
                for(int j=14;j>=i;j--)
                {
                    Size boardsize(i,j);
                    detected = findChessboardCorners(inputs[iter],boardsize,v);
                    if(detected)
                    {
                        chessboardPoints.push_back(v);
                        chessboardSize.push_back(Size(i+1,j+1));
                        break;
                    }
                }
                if(detected)
                    break;
            }
            if(!detected)
            {
                chessboardPoints.push_back(v);
            }
        }
        if(chessboardSize.size()==0)
        {
            Scierror(999, "No chessboards detected. Aborting.");
            return 0;
        }
        Size compare = chessboardSize[0];
        for(int i=1;i<chessboardSize.size();i++)
        {
            if(compare.height!=chessboardSize[i].height || compare.width!=chessboardSize[i].width)
            {
                Scierror(999, "Chessboards of different sizes detected. Aborting.");
                return 0;
            }
        }
        usedImages= (int*) malloc(sizeof(int)*imgCount[0]);
        for(int i=0;i<imgCount[0];i++)
        {
            if(chessboardPoints[i].size() == 0)
            {
                usedImages[i] = 0;
            }
            else
            {
                usedImages[i] = 1;
            }
        }
        if(argCount==2)
        {
            for(int i=0;i<imgCount[0];i++)
            {
                if(chessboardPoints[i+imgCount[0]].size() == 0)
                {
                    usedImages[i] = 0;
                }
            }
        }
        for(int i=0;i<imgCount[0];i++)
        {
            if(usedImages[i]==1)
            {
                count++;
            }
        }
        points = (int***) malloc(sizeof(int**)*argCount);
        for(int i=0;i<argCount;i++)
        {
            points[i] = (int**) malloc(sizeof(int*)*count);
        }
        pointsPos = 0;
        for(int i=0;i<imgCount[0];i++)
        {
            if(usedImages[i])
            {
                points[0][pointsPos] = (int*) malloc(sizeof(int) * chessboardPoints[i].size() * 2);
                if(argCount==2)
                {
                    points[1][pointsPos] = (int*) malloc(sizeof(int) * chessboardPoints[i].size() * 2);
                }
                for(int j=0;j<chessboardPoints[i].size();j++)
                {
                    points[0][pointsPos][j] = chessboardPoints[i][j].x;
                    points[0][pointsPos][chessboardPoints[i].size()+j] = chessboardPoints[i][j].y;
                    if(argCount==2)
                    {
                        points[1][pointsPos][j] = chessboardPoints[imgCount[0]+i][j].x;
                        points[1][pointsPos][chessboardPoints[i].size()+j] = chessboardPoints[imgCount[0]+i][j].y;
                    }
                }
                pointsPos++;
            }
        }
        returnSize[0] = compare.width;
        returnSize[1] = compare.height;
        //------Create output arguments------//
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, argCount, &piAddr);
        if(sciErr.iErr)
	    {
		    printError(&sciErr, 0);
		    return 0;
	    }
        for(int i=0;i<argCount;i++)
        {
            sciErr = createListInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, i+1, pointsPos, &piChild);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            for(int j=0;j<pointsPos;j++)
            {
                sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piChild, j+1, (returnSize[0]-1)*(returnSize[1]-1), 2, points[i][j]);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
            }
        }
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx)+2, 1, 2, returnSize);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx)+3, 1, imgCount[0], usedImages);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        //------Return Arguments------//
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3;
        ReturnArguments(pvApiCtx);
		return 0;
    }
    /* ==================================================================== */
}
