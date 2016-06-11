
/***************************************************
Author : Rohit Suri
***************************************************/

#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
using namespace std;
extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>


    string formats[] = {"jpeg","jpg","bmp","png","JPEG","JPG","BMP","PNG"};

    vector<vector<string> > descriptionVector;

    const char pathSeparator =
    #ifdef _WIN32
                                '\\';
    #else
                                '/';
    #endif

    bool recursive = false;
    bool listDirError = 0;

    bool isImage(string fname)
    {
        for(int i=0;i<8;i++)
        {
            if(fname.substr(fname.length()-formats[i].length()).compare(formats[i])==0)
                return 1;
        }
        return 0;
    }

    void listDir(string dirName, vector<string> &currentDir)
    {
        DIR *dir;
        struct dirent *entry;
        if(!(dir = opendir(dirName.c_str())))
        {
            Scierror(999,"Cannot open directory for reading. Please check the directory name.\n");
            listDirError = 1;
            return;
        }
        if(!(entry = readdir(dir)))
        {
            Scierror(999,"The directory is empty.\n");
            listDirError = 1;
            return;
        }

        do
        {
            if (entry -> d_type == DT_DIR)
            {
                if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0) continue;
                if(recursive)
                {
                    vector<string> nextDir;
                    nextDir.push_back(entry->d_name);
                    listDir(dirName+pathSeparator+entry->d_name,nextDir);
                }
            }
            else
            {
                if(isImage(entry->d_name))
                {
                    //cout<<dirName<<pathSeparator<<entry->d_name<<endl;
                    currentDir.push_back(dirName+pathSeparator+entry->d_name);
                }
            }
        }while(entry = readdir(dir));
        vector<string>::iterator it = currentDir.begin();
        sort(it+1,currentDir.end());
        descriptionVector.push_back(currentDir);
    }

    /*Calling syntax: imageSet(imageLocation) OR imageSet(imageFolder,'recursive') */

    int opencv_imageSet(char *fname, unsigned long fname_len)
    {
        // Error management variables
        SciErr sciErr;

        //------Local variables------//
        int *piAddr = NULL;
        int *piChild = NULL;
        int *piLen = NULL;
        char **pstData = NULL;
        int iRows, iCols;

        string filePath,fileName,tempPath;
        char **description = NULL;
        char ***location = NULL;
        int *count = NULL;
        int descriptionCount = 0;
        int pos = 0;
        char *objectType = "imageSet";
        //------Check number of parameters------//
        CheckInputArgument(pvApiCtx, 1, 2);
        CheckOutputArgument(pvApiCtx, 1, 1);

        //------Get input arguments------//
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
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

        filePath = string(pstData[0]);

        if(*getNbInputArgument(pvApiCtx) == 2)
        {
            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
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

            if(strcmp("recursive",pstData[0])!=0)
            {
                Scierror(999, "Error: Argument %s is invalid. \n", pstData[0]);
                return 0;
            }
            recursive = true;
        }

        //------Actual processing------//
        while(filePath[filePath.length()-1]==pathSeparator)
        {
            filePath = filePath.substr(0,filePath.length()-1);
        }
        vector<string> nextDir;
        nextDir.push_back(filePath.substr(filePath.find_last_of(pathSeparator)+1));

        listDir(filePath,nextDir);

        if(listDirError)
        {
            listDirError = 0; //Doing this is very important as it is a global
            return 0;
        }
        for(int i=0;i<descriptionVector.size();i++)
        {
            if(descriptionVector[i].size()>1)
                descriptionCount++;

        }
        description = (char**) malloc(sizeof(char*) * descriptionCount);
        count = (int*) malloc(sizeof(int) * descriptionCount);
        location = (char***) malloc(sizeof(char**) * descriptionCount);

        for(int i=0;i<descriptionVector.size();i++)
        {
            if(descriptionVector[i].size()>1)
            {
                description[pos] = (char*) malloc(sizeof(char) * descriptionVector[i][0].length() + 1);
                descriptionVector[i][0].copy(description[pos],descriptionVector[i][0].length());
                description[pos][descriptionVector[i][0].length()] = 0;

                count[pos] = descriptionVector[i].size()-1;

                location[pos] = (char**) malloc(sizeof(char*) * (descriptionVector[i].size()-1));
                for(int j=1;j<descriptionVector[i].size();j++)
                {
                    location[pos][j-1] = (char*) malloc(sizeof(char) * descriptionVector[i][j].length() + 1);
                    descriptionVector[i][j].copy(location[pos][j-1],descriptionVector[i][j].length());
                    location[pos][j-1][descriptionVector[i][j].length()] = 0;
                }
                pos++;
            }
        }

        descriptionVector.clear(); //Doing this is very important as it is a global
        recursive = false; //Doing this is very important as it is a global

        //------Create output arguments------//
        sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 4, &piAddr);
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
        sciErr = createMatrixOfInteger32InList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piAddr, 3, descriptionCount, 1, count);
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

        for(int i=0;i<descriptionCount;i++)
        {
            sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx)+1, piChild, i+1, 1, count[i], location[i]);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        //------Return Arguments------//
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
        ReturnArguments(pvApiCtx);
        return 0;
    }
/* ==================================================================== */
}
