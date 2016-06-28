/***************************************************
Author : Rohit Suri
***************************************************/
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std; 
extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include "sciprint.h"
    
    /* Calling syntax: [set1,set2,...,setN] = partition(imgSet,groupSizes)
                       [set1,set2,...,setN] = partition(imgSet,groupPercentages)
                       [set1,set2,...,setN] = partition(___,method)                   */

    double findSum(double *array, int n)
    {
        double sum = 0;
        for(int iter=0; iter<n; iter++)
        {
            sum+= array[iter];
        }
        return sum;
    }
    
    int opencv_partition(char *fname, unsigned long fname_len)
    {
        // Error management variables
        SciErr sciErr; 
        
        //------Local variables------//
        int *parentAddr = NULL;
        int *piAddr = NULL;
        int *piLen = NULL;
        int *piChild = NULL;
        int *piGrandChild = NULL;
        char **pstData = NULL;
        int iRows, iCols;
        
        char *objectType = "imageSet";
        bool randomized = 0; // 0 for sequential and  1 for randomized method
        bool sizeProvided; // 0 if percentage and 1 if size provided
        bool zeroWarning = 0;
        double *groupSizesOrPercentages = NULL;
        char **description = NULL;
        int *count = NULL;
        int *tempCount = NULL;
        char ***location = NULL;
        int **countOutput = NULL;
        int **locationIndex = NULL;
        char ***locationOutput = NULL;
        int descriptionCount, partitionCount;
        double sumOfSizesOrPercentages;
        double groupSizesOrPercentagesSum = 0;
        int index;
        //------Check number of parameters------//
        CheckInputArgument(pvApiCtx, 2, 3);
        // Output Arguments checked later
        
        //------Get input arguments------//
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
        if(!isListType(pvApiCtx, piAddr))
        {
            Scierror(999, "Error: Invalid first argument. List Expected.\n");
            return 0;
        }
        
        // Extracting object type and checking if type is imageSet
        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		piLen = (int*)malloc(sizeof(int) * iRows * iCols);

		sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &iRows, &iCols, piLen, NULL);
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

		sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 1, &iRows, &iCols, piLen, pstData);
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
        sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 2, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		piLen = (int*)malloc(sizeof(int) * iRows * iCols);

		sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 2, &iRows, &iCols, piLen, NULL);
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

		sciErr = getMatrixOfStringInList(pvApiCtx, piAddr, 2, &iRows, &iCols, piLen, description);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
        descriptionCount = iRows;
        
        // Extracting Count attribute of input argument
        sciErr = getMatrixOfInteger32InList(pvApiCtx, piAddr, 3, &iRows, &iCols, &count);
        if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
        
        location = (char***) malloc(sizeof(char**) * descriptionCount);
        sciErr = getListItemAddress(pvApiCtx, piAddr, 4, &piChild);
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
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
        if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
        
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &groupSizesOrPercentages);
        if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
        if(iCols == 0)
        {
            Scierror(999, "Error: Please provide a proper groupSizes vector.\n");
            return 0;
        }
        if(iRows!=1)
        {
            Scierror(999, "Error: A one dimensional column vector expected.\n");
            return 0;
        }
        partitionCount = iCols+1;
        CheckOutputArgument(pvApiCtx, 1, partitionCount);
        
        if(*getNbInputArgument(pvApiCtx) == 3)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr); 
            if (sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }

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
            
            if(strcmp("sequential",pstData[0])==0)
            {
                randomized = 0;
            }
            else if(strcmp("randomized",pstData[0])==0)
            {
                randomized = 1;
            }
            else
            {
                Scierror(999, "Error: Argument \"%s\" is not valid.\n",pstData[0]);
                return 0;
            }
        }
        
        sumOfSizesOrPercentages = findSum(groupSizesOrPercentages,partitionCount);
        if(groupSizesOrPercentages[0]>0 && groupSizesOrPercentages[0]<1)
        {
            sizeProvided = 0;
        }
        else if(groupSizesOrPercentages[0]>=1)
        {
            sizeProvided = 1;
        }
        else
        {
            Scierror(999, "Error: Please enter positive values groupSizes argument.\n");
            return 0;
        }
        for(int iter = 0; iter<partitionCount-1;iter++)
        {
            if(sizeProvided && (groupSizesOrPercentages[iter]<1 || groupSizesOrPercentages[iter] - (int)groupSizesOrPercentages[iter]!=0))
            {
                Scierror(999, "Error: Please enter positive whole number values for groupSizes argument.\n");
                return 0;
            }
            else if(!sizeProvided && (groupSizesOrPercentages[iter]<=0 || groupSizesOrPercentages[iter]>=1))
            {
                Scierror(999, "Error: Please enter proper groupPercentages argument. Values should lie between 0-1.\n");
                return 0;
            }
            groupSizesOrPercentagesSum+=groupSizesOrPercentages[iter]; 
        }
        if(!sizeProvided && groupSizesOrPercentagesSum>1)
        {
            Scierror(999, "Error: Sum of values of groupPercentages should be less than or equal to one. \n");
            return 0;
        }
        if(sizeProvided)
        {
            for(int iter = 0; iter<descriptionCount; iter++)
            {
                if(count[iter]<groupSizesOrPercentagesSum)
                {
                    Scierror(999, "Error: The sum of groupSizes arguments cannot be greater than the size of any imageSet object.");
                    return 0;
                }
            }
        }
        
        //------Actual processing------//
        double temp;
        countOutput = (int**) malloc(sizeof(int*) * partitionCount);
        tempCount = (int*) malloc(sizeof(int) * descriptionCount);
        locationIndex = (int**) malloc(sizeof(int*) * descriptionCount);
        for(int iter = 0 ; iter<descriptionCount; iter++)
        {
            locationIndex[iter] = (int*) malloc(sizeof(int) * count[iter]); 
            tempCount[iter] = count[iter];
        }
        for(int iter=0;iter<partitionCount-1; iter++)
        {
            countOutput[iter] = (int*) malloc(sizeof(int) * descriptionCount);
            for(int iter2 = 0; iter2<descriptionCount; iter2++)
            {
                if(sizeProvided)
                {
                    temp = groupSizesOrPercentages[iter];
                }
                else
                {
                    temp = groupSizesOrPercentages[iter] * count[iter2];
                    if(temp - (int)temp >= 0.5)
                    {
                        temp = (int)temp + 1;
                    }
                    else
                    {
                        temp = (int)temp;
                    }
                }
                if(tempCount[iter2]==0)
                {
                    countOutput[iter][iter2] = 0;
                }
                else if(tempCount[iter2]-temp<=0)
                {
                    countOutput[iter][iter2] = tempCount[iter2];
                    tempCount[iter2] = 0;
                    zeroWarning = 1;
                }
                else
                {
                    if(temp==0)
                    {
                        zeroWarning = 1;
                    }
                    countOutput[iter][iter2] = temp;
                    tempCount[iter2] -= temp; 
                }
            }
        }
        countOutput[partitionCount-1] = (int*) malloc(sizeof(int) * descriptionCount);
        for(int iter = 0; iter<descriptionCount; iter++)
        {
            countOutput[partitionCount-1][iter] = tempCount[iter];
        }
        for(int iter = 0;iter<descriptionCount;iter++)
        {
            for(int iter2=0;iter2<count[iter];iter2++)
            {
                locationIndex[iter][iter2] = 0;
            }
        }
        if(randomized)
        {
            srand(time(NULL));
            for(int iter = 0; iter < partitionCount; iter++)
            {
                for(int iter2 = 0; iter2 <descriptionCount; iter2++)
                {
                    temp = countOutput[iter][iter2];
                    while(temp!=0)
                    {
                        index = rand() % count[iter2];
                        if(locationIndex[iter2][index]==0)
                        {
                            locationIndex[iter2][index] = iter+1;
                            temp--;
                        }
                    }
                }
            }
        }
        else
        {
            for(int iter = 0; iter < partitionCount; iter++)
            {
                for(int iter2 = 0; iter2 <descriptionCount; iter2++)
                {
                    temp = countOutput[iter][iter2];
                    index = 0;
                    while(temp!=0)
                    {
                        if(locationIndex[iter2][index]==0)
                        {
                            locationIndex[iter2][index] = iter+1;
                            temp--;
                        }
                        index++;
                    }
                }
            }
        }
        if(zeroWarning)
        {
            sciprint("Warning: One or more sets may have zero images.\n");
        }
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, partitionCount, &parentAddr);
	        if(sciErr.iErr)
	        {
		        printError(&sciErr, 0);
		        return 0;
	        }
        //------Create output arguments------//
        for(int iter=1;iter<=partitionCount;iter++)
        {
            sciErr = createListInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, parentAddr, iter, 4, &piAddr);
	        if(sciErr.iErr)
	        {
		        printError(&sciErr, 0);
		        return 0;
	        }
            sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, 1, 1, 1, &objectType);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, 2, descriptionCount, 1, description); 
            if(sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, 3, descriptionCount, 1, countOutput[iter-1]);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            sciErr = createListInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, 4, descriptionCount, &piChild);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0); 
                return 0; 
            }
            locationOutput = (char***) malloc(sizeof(char**) * descriptionCount);
            for(int i=0;i<descriptionCount;i++)
            {
                locationOutput[i] = (char**) malloc(sizeof(char*) * countOutput[iter-1][i]);
                index = 0;
                for(int j=0;j<count[i];j++)
                {
                    if(locationIndex[i][j]==iter)
                    {
                        locationOutput[i][index] = (char*) malloc(sizeof(char) * (strlen(location[i][j])+1));
                        strncpy(locationOutput[i][index], location[i][j], strlen(location[i][j])+1);
                        index++;
                    }
                }
                sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piChild, i+1, countOutput[iter-1][i], 1, locationOutput[i]);
            }
        }
        //------Return Arguments------//
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
        ReturnArguments(pvApiCtx); 
        return 0; 
    }
/* ==================================================================== */
}
