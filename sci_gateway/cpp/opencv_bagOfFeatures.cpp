/****************************************************************************************************
*	Author:	Umang Agrawal               																															*
*	Code:		bagOfFeatues.cpp																																					*
*	Function Format:	bag = bagOfFeatures(imageSet, Oprional Arguments Name, Value)							      *
*					Arguments:				Name																Value																*
*											1. VocabularySize							any integer value greater than 0								*
*											2. StrongestFeatures					pecentage of best features to be Selected				*
*											3. Upright										Bool 0 or 1																			*
*											4. Verbose										Bool 0 or 1																			*
****************************************************************************************************/

#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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

  bool response_comparator(const KeyPoint& p1, const KeyPoint& p2) {
      return p1.response > p2.response;
  }

	int opencv_bagOfFeatures(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;

		int inp_params = 0;

    int *piAddr = NULL;
    int *piAddr1 = NULL;
		int *piAddr2 = NULL;
    int *piAddr3 = NULL;
    int *piAddr4 = NULL;
		int *piLen = NULL;
		int *piChild = NULL;
		char **pstData = NULL;
		int iRows, iCols;
		char **arg = NULL;
		int count_ver = 0, count_up = 0, count_strength = 0, count_vocab = 0;

		int descriptionCount;
		char *objectType = "bagOfFeatures";
		char **description = NULL;
		int *count = NULL;
		double vocab_size = 500, verbose = 1, upright = 1, strength = 1;
		char ***location = NULL;
		char **filepath = NULL;
		char *file = "Bag-Of-Features.yml";
		int key_size=0, v_key_size=0;

		Mat image;
		vector<KeyPoint> keypoints;
		vector<KeyPoint> valid_key;
		Mat feature_des;
		Mat dictionary;
		Mat featuresUnclustered;

		CheckInputArgument(pvApiCtx, 1, 9);						//Check on Number of Input Arguments
		CheckOutputArgument(pvApiCtx, 1, 1);						//Check on Number of Output Arguments

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if(!isListType(pvApiCtx, piAddr1))
    {
        Scierror(999, "Error: Invalid first argument. List Expected.\n");
        return 0;
    }

    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 1, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 1, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int iter = 0 ; iter < iRows * iCols ; iter++)
    {
      pstData[iter] = (char*)malloc(sizeof(char) * (piLen[iter] + 1));//+ 1 for null termination
    }
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 1, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }

    if(!(strcmp(pstData[0],"imageSet")==0))
    {
        Scierror(999, "Error: The input argument is not of type imageSet.\n");
        return 0;
    }

    // Extracting Description attribute of input argument
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    description = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int iter = 0 ; iter < iRows * iCols ; iter++)
    {
      description[iter] = (char*)malloc(sizeof(char) * (piLen[iter] + 1));//+ 1 for null termination
    }
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr1, 2, &iRows, &iCols, piLen, description);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    descriptionCount = iRows;

    // Extracting Count attribute of input argument
    sciErr = getMatrixOfInteger32InList(pvApiCtx, piAddr1, 3, &iRows, &iCols, &count);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    location = (char***) malloc(sizeof(char**) * descriptionCount);
    sciErr = getListItemAddress(pvApiCtx, piAddr1, 4, &piChild);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    for(int iter = 1; iter<=descriptionCount; iter++)
    {
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, iter, &iRows, &iCols, NULL, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      piLen = (int*)malloc(sizeof(int) * iRows * iCols);
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, iter, &iRows, &iCols, piLen, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      location[iter-1] = (char**)malloc(sizeof(char*) * iRows * iCols);
      for(int colIter = 0 ; colIter < iRows * iCols ; colIter++)
      {
        location[iter-1][colIter] = (char*)malloc(sizeof(char) * (piLen[colIter] + 1));//+ 1 for null termination
      }
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, iter, &iRows, &iCols, piLen, location[iter-1]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
    }

		/*sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		if( !isStringType(pvApiCtx, piAddr2))
		{
			Scierror(999, "%s: Wrong type of argument for Location. A string is expected.\n", fname);
			return 0;
		}
		sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		piLen = (int*)malloc(sizeof(int) * iRows * iCols);
		//second call to retrieve the length of the string
		sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			free(piLen);
			return 0;
		}
		filepath = (char**)malloc(sizeof(char*) * iRows * iCols);
		for(int j=0;j< iRows * iCols; j++)
		{
			filepath[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));
		}

		//third call to retrieve data
		sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, filepath);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			free(piLen);
			free(filepath);
			return 0;
		}*/

    inp_params = *getNbInputArgument(pvApiCtx);
    for(int i=2; i<=inp_params; i++)
    {
      if( inp_params%2 == 0)
			{
				Scierror(999,"Either Argument Name or its Value missing\n");
				return 0;
			}
      sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddr3);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
      //Check for Argument type
			if( !isStringType(pvApiCtx, piAddr3))
			{
				Scierror(999, "%s: Wrong type of argument for Name of Optional Argument. A string is expected.\n", fname);
				return 0;
			}
			//Matrix of Stings
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			piLen = (int*)malloc(sizeof(int) * iRows * iCols);
			//second call to retrieve the length of the string
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, NULL);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(piLen);
				return 0;
			}
			arg = (char**)malloc(sizeof(char*) * iRows * iCols);
			for(int j=0;j< iRows * iCols; j++)
			{
				arg[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));
			}

			//third call to retrieve data
			sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, arg);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				free(piLen);
				free(arg);
				return 0;
			}

      if(strcmp(arg[0],"VocabularySize") == 0)
      {
				if( count_vocab != 0)
				{
					Scierror(999,"Voabulary Size has been called twice.\n");
					return 0;
				}
				free(arg);
				free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
        if( !(isDoubleType(pvApiCtx, piAddr4)||isIntegerType(pvApiCtx, piAddr4)))
        {
          Scierror(999,"Not a valid type of value for VocabularySize.\n");
          return 0;
        }
        //Reading the Value of the argument
        if(getScalarDouble(pvApiCtx, piAddr4, &vocab_size))
        {
          Scierror(999,"Not a valid type of value for VocabularySize.\n");
          return 0;
        }
        if(vocab_size < 0)
        {
          Scierror(999,"Enter a valid value for Vacabulary Size\n");
          return 0;
        }
				i++;
        count_vocab += 1;
      }
      else if(strcmp(arg[0],"StrongestFeatures") == 0)
      {
        if( count_strength != 0)
				{
					Scierror(999,"Strength Features has been called twice.\n");
					return 0;
				}
				free(arg);
				free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
        if( !isDoubleType(pvApiCtx, piAddr4) )
        {
          Scierror(999,"Not a valid type of value for StrongestFeatures.\n");
          return 0;
        }
        //Reading the Value of the argument
        if(getScalarDouble(pvApiCtx, piAddr4, &strength))
        {
          Scierror(999,"Not a valid type of value for StrongestFeatures.\n");
          return 0;
        }
        if( strength<0 || strength>=1 )
        {
          Scierror(999,"Enter a valid value for Strongest Features (between 0 and 1)\n");
          return 0;
        }
				i++;
        count_strength += 1;
      }
      else if(strcmp(arg[0],"Verbose") == 0)
      {
        if( count_ver != 0)
				{
					Scierror(999,"Verbose has been called twice.\n");
					return 0;
				}
				free(arg);
				free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
        if( !(isDoubleType(pvApiCtx, piAddr4)||isIntegerType(pvApiCtx, piAddr4)))
        {
          Scierror(999,"Not a valid type of value for Verbose.\n");
          return 0;
        }
        //Reading the Value of the argument
        if(getScalarDouble(pvApiCtx, piAddr4, &verbose))
        {
          Scierror(999,"Not a valid type of value for Verbose.\n");
          return 0;
        }
        if( !(verbose == 1|| verbose == 0) )
        {
          Scierror(999,"Enter a valid value for Verbose (Either 0 or 1)\n");
          return 0;
        }
				i++;
        count_ver += 1;
      }
      else if(strcmp(arg[0],"Upright") == 0)
      {
        if( count_up != 0)
				{
					Scierror(999,"Upright has been called twice.\n");
					return 0;
				}
				free(arg);
				free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
        if(sciErr.iErr)
        {
          printError(&sciErr, 0);
          return 0;
        }
        if( !(isDoubleType(pvApiCtx, piAddr4)||isIntegerType(pvApiCtx, piAddr4)))
        {
          Scierror(999,"Not a valid type of value for Upright.\n");
          return 0;
        }
        //Reading the Value of the argument
        if(getScalarDouble(pvApiCtx, piAddr4, &upright))
        {
          Scierror(999,"Not a valid type of value for Upright.\n");
          return 0;
        }
        if(!(upright == 1 || upright == 0))
        {
          Scierror(999,"Enter a valid value for Upright (Either 0 or 1)\n");
          return 0;
        }
				i++;
        count_up += 1;
      }
      else
      {
        Scierror(999,"Invalid Argument Name\n");
        return 0;
      }
    }

		filepath = (char**)malloc(sizeof(char*)*1*1);
		filepath[0] = (char*)malloc(sizeof(char)*20);
		strcpy(filepath[0],file);
		/*int flag=1;
		for(int i=0; location[0][0][i]; i++)
		{
			common = location[0][0][i];
			for(int j=0; j<descriptionCount; j++)
			{
				if(location[j][0][i] != common)
					flag = 0;
			}
			if(flag == 0)
			{
				common_len = i;
				break;
			}
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										}
		strncpy(filepath[0], location[0][0], common_len);
		strcat(filepath[0], file);
		*/

		SurfFeatureDetector detector(1, 4, 2, 1, int(upright));
		Ptr<DescriptorExtractor> extractor = new SurfDescriptorExtractor(1, 4, 2, 1, int(upright));
		if(int(verbose))
		{
			sciprint("Creating Bag-Of-Features from %d image sets.\n\n",descriptionCount);
			for(int i=0; i<descriptionCount; i++)
				sciprint("Image set %d: %s\n",i+1,description[i]);
			sciprint("\nExtracting SURF Features from each image set.\n\n");
		}

		for( int i=0; i<descriptionCount; i++)
		{
			if(int(verbose))
				sciprint("Extracting features from %d images in image set %d ...",count[i],i+1);
			key_size = 0;
			v_key_size = 0;
			for( int j=0; j<count[i]; j++)
			{
				valid_key.clear();
				keypoints.clear();
				image = imread(location[i][j],0);
				detector.detect(image, keypoints);
				sort(keypoints.begin(), keypoints.end(), response_comparator);
				for( int k=0; k<(keypoints.size()*strength); k++)
				{
					valid_key.push_back(keypoints[k]);
				}
				extractor->compute(image, valid_key, feature_des);
				featuresUnclustered.push_back(feature_des);
				key_size += keypoints.size();
				v_key_size += valid_key.size();
			}
			if(int(verbose))
			{
				sciprint("done. Extracted %d features.\n",key_size);
				sciprint("Keeping %f percent of the strongest features.\n",(strength)*100);
				sciprint("Net Extracted features : %d\n\n",v_key_size);
			}
		}

		if(vocab_size > featuresUnclustered.rows)
			vocab_size = featuresUnclustered.rows;
		TermCriteria tc(CV_TERMCRIT_ITER, 100, 0.001);
	  int retries = 3;
		BOWKMeansTrainer bowTrainer(int(vocab_size), tc, retries, KMEANS_PP_CENTERS);

		if(int(verbose))
		{
			sciprint("Using K-Means Clustering to create a %d word visual vocabulary.\n",int(vocab_size));
			sciprint("Number of Features : %d\n",featuresUnclustered.rows);
			sciprint("Number of Clusters : %d\n\n",int(vocab_size));
		}

		dictionary = bowTrainer.cluster(featuresUnclustered);

		if(int(verbose))
			sciprint("Finished creating Bag-Of-Features\n");

		FileStorage fs(filepath[0], FileStorage::WRITE);
	  fs<<"dictionary"<<dictionary;
	  fs.release();

    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 5, &piAddr);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, 1, 1, &objectType);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 2, 1, 1, filepath);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 3, 1, 1, &vocab_size);
    if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 4, 1, 1, &strength);
    if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 5, 1, 1, &upright);
    if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		ReturnArguments(pvApiCtx);
		return 0;
  }
}
