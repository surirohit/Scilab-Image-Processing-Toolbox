/***************************************************
Author : Rohit Suri
TODO   : Extract confidences for each character
       : Calculate Position of words
***************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <tesseract/baseapi.h>
using namespace cv; 
using namespace std;
using namespace tesseract;
extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include "sciprint.h"
    #include "../common.h"
    
    /*Calling syntax: ocr(I) */

    int opencv_ocr(char *fname, unsigned long fname_len)
    {
        // Error management variables
        SciErr sciErr; 
        
        //------Local variables------//        
        Mat sourceImage;
        TessBaseAPI tesseract;
        int *wordConfidences = NULL;
        int wordCount = 0, characterCount = 0, count=0, coordinate;
        string word="";
        char *text = NULL;
        char **words = NULL;
        char *boxInformation = NULL;
        int *characterBoundingBoxes = NULL;
        int characterBoundingBoxesPos = 0;
        //------Check number of parameters------//
        CheckInputArgument(pvApiCtx, 1, 1); 
        CheckOutputArgument(pvApiCtx, 1, 4); 
        
        //------Get input arguments------//
        retrieveImage(sourceImage, 1); 
                
        //------Actual processing------//
        
        tesseract.Init(NULL, "eng", OEM_TESSERACT_ONLY);
        tesseract.SetPageSegMode(PSM_SINGLE_BLOCK);
        tesseract.SetVariable("tessedit_char_whitelist","0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()-_=+[]{}:'\";\|,.<>/");
        tesseract.SetImage(sourceImage.data, sourceImage.cols, sourceImage.rows, sourceImage.elemSize(), sourceImage.step);
        wordConfidences = tesseract.AllWordConfidences();
        boxInformation = tesseract.GetBoxText(0);
        for(int iter = 0; ;iter++)
        {
            if(wordConfidences[iter]!=-1)
            {
                wordCount++;
            }
            else
            {
                break;          
            }
        }
        
        words = (char**) malloc(sizeof(char*)*wordCount);
        text = tesseract.GetUTF8Text();
        for(int iter = 0; count < wordCount; iter++)
        {
            
            if(count == wordCount)
            {
                break;
            }
            if(text[iter]==' ' || text[iter]=='\n')
            {
                words[count] = (char*) malloc(sizeof(char)*word.length()+1);
                for(int char_iter=0; char_iter<word.length();char_iter++)
                {
                    words[count][char_iter] = word[char_iter];
                }
                words[count][word.length()] = '\0';
                word = "";
                count++;
                
            }
            else
            {
                characterCount++;
                word = word + text[iter];
            }
        }
        characterBoundingBoxes = (int*) malloc(sizeof(int)*characterCount*4);
        
        for( int iter = 0; characterBoundingBoxesPos < characterCount ; iter++)
        {
            while( boxInformation[iter]!=' ')
            {
                iter++;
            }
            iter++;
            coordinate = 0;
            while(boxInformation[iter]!=' ')
            {
                coordinate = 10 * coordinate + boxInformation[iter] - '0';
                iter++;
            }
            characterBoundingBoxes [characterBoundingBoxesPos] = coordinate;
            coordinate = 0;
            iter++;
            while(boxInformation[iter]!=' ')
            {
                coordinate = 10 * coordinate + boxInformation[iter] - '0';
                iter++;
            }
            characterBoundingBoxes [characterCount + characterBoundingBoxesPos] = coordinate;
            coordinate = 0;
            iter++;
            while(boxInformation[iter]!=' ')
            {
                coordinate = 10 * coordinate + boxInformation[iter] - '0';
                iter++;
            }
            characterBoundingBoxes [2 * characterCount + characterBoundingBoxesPos] = coordinate;
            coordinate = 0;
            iter++;
            while(boxInformation[iter]!=' ')
            {
                coordinate = 10 * coordinate + boxInformation[iter] - '0';
                iter++;
            }
            characterBoundingBoxes [3 * characterCount + characterBoundingBoxesPos] = coordinate;
            characterBoundingBoxesPos++;
            iter+=2;
        }
        //------Create output arguments------//
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &text);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 2, characterCount, 4, characterBoundingBoxes);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 3, wordCount, 1, words);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        sciErr = createMatrixOfInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 4, wordCount, 1, wordConfidences); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        //------Return Arguments------//
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1; 
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2; 
        AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3;
        AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx)+4;
        ReturnArguments(pvApiCtx); 
        return 0; 
    }
/* ==================================================================== */
}