function trainCascadeObjectDetector(outputFile,positiveInstances,negativeImages,varargin)
    //outputfile- ex.xml
    //positiveInstances- array of structure with field names path(string) and Bbox(4 length vector);
    //negativeImages- path to negative images folder
    //varargin- name,value pairs (Eg:numPos,numNeg,numStages,minHitRate,maxFalseAlarmRate,featureType,w,h)
    [lhs rhs]=argn(0);
    if rhs<3 then
         error(msprintf(" Not enough input arguments"))
    elseif rhs>23 then
         error(msprintf(" Too many input arguments to the function"))
    elseif modulo(rhs-3,2)
         error(msprintf(" wrong number of input arguments,name-value pairs not macthed"))
    end
    
    //validating variables
    [path,fname,extension]=fileparts(outputFile)
    if strcmp(extension,".xml") then
        error(msprintf(" wrong input argument #1,must be a string with an XML extension"))
    elseif ~isdir(negativeImages)
        error(msprintf(" wrong input argument #3,existing directory expected"))
    end
    
    //default values
    numPos=4;
    numNeg=2;
    numStages=30;
    precalcValBufSize=512
    precalcIdxBufSize=512
    featureType="HAAR"
    minHitRate=0.995
    maxFalseAlarmRate=0.5
    w=20
    h=20
    
    for i=1:2:rhs-3
       if strcmpi(varargin(i),"numPos")==0 then
            i=i+1;
            numPos=varargin(i);
            if numPos<0 then
                error(msprintf(" numPos value must be positive"))
            end
        elseif strcmpi(varargin(i),'numNeg')==0 then
            i=i+1;
            numNeg=varargin(i);
            if numNeg<0 then
                error(msprintf(" numNeg value must be positive"))
            end
            
        elseif strcmpi(varargin(i),'numStages')==0 then
            i=i+1;
            numStages=varargin(i);
            if numStages<0 then
                error(msprintf(" numStages value must be positive"))
            end
            
        elseif strcmpi(varargin(i),'precalcValBufSize')==0 then
            i=i+1;
            precalcValBufSize=varargin(i);
            if precalcValBufSize<0 then
                error(msprintf(" precalcValBufSize value must be positive"))
            end
            
        elseif strcmpi(varargin(i),'precalcIdxBufSize')==0 then
            i=i+1;
            precalcIdxBufSize=varargin(i);
            if precalcIdxBufSize<0 then
                error(msprintf(" precalcIdxBufSize value must be positive"))
            end
            
        elseif strcmpi(varargin(i),'featureType')==0 then
            i=i+1;
            featureType=varargin(i);
            if strcmpi(featureType,'haar') & strcmpi(featureType,'lbp') & strcmpi(featureType,'hog')
                error(msprintf(" wrong input argument #%d,featureType not matched",i));
            end
            
        elseif strcmpi(varargin(i),'minHitRate')==0 then
            i=i+1;
            minHitRate=varargin(i);
            if minHitRate<0 | minHitRate>1 then
                error(msprintf(" minHitRate value must lie in between 0 and 1"))
            end
            
        elseif strcmpi(varargin(i),'maxFalseAlarmRate')==0 then
            i=i+1;
            maxFalseAlarmRate=varargin(i);
            if maxFalseAlarmRate<0  | minFalseRate>1 then
                error(msprintf(" maxFalseAlarmRate value must lie in between 0 and 1"))
            end
            
        elseif strcmpi(varargin(i),'w')==0 then
            i=i+1;
            w=varargin(i);
            if h<0 then
                error(msprintf(" w value must be positive"))
            end
            
        elseif strcmpi(varargin(i),'h')==0 then
            i=i+1;
            h=varargin(i);
            if h<0 then
                error(msprintf(" h value must be positive"))
            end
        else
            error(msprintf(_(" Wrong value for input argument #%d",i)));
        end
    end
    
    [noOfPositiveInstances nCols]=size(positiveInstances);
    fields=fieldnames(positiveInstances);
    fd = mopen('positive.txt','wt');
    for i=1:noOfPositiveInstances
        mfprintf(fd,'%s 1',getfield(fields(1),positiveInstances(i)));
        for j=1:4
            mfprintf(fd,' %d',getfield(fields(2),positiveInstances(i)));
        end
        mfprintf(fd,'\n');
    end
    mclose(fd);
    
    disp("Creating positive samples:");
    cmd=sprintf("opencv_createsamples -info positive.txt -num%d -vec positive.vec -w %d -h %d",numPos,w,h);
    unix_w(cmd);
    if isdir(negativeImages)
        if getos()=="Linux"
            temp=strcat(["ls ",negativeImages])
        elseif getos()=="Windows"
            temp=strcat(["dir ",negativeImages])
        end
        s=unix_g(temp);
        [noOfFilesInFolder noOfCols]=size(s);
        fd = mopen('negative.txt','wt');
        for i=1:noOfFilesInFolder
            [path,fname,extension]=fileparts(s(i))
            if ~strcmp(extension,".jpg") | ~strcmp(extension,".jpeg") | ~strcmp(extension,".png") | ~strcmp(extension,".bmp")
                mfprintf(fd,'%s\n',s(i));
            end
        end
     end
     disp("Training Cascade");
     cmd=sprintf("opencv_traincascade -data %s -vec positive.vec -bg negative.txt -numPos %d -numNeg %d -numStages %d -precalcValBufSize %d -precalcIdxBufSize %d -featureType %s -minHitRate %d -maxFalseAlarmRate %d -w %d -h %d",outputFile,numPos,numNeg,numStages,precalcValBufSize,precalcIdxBufSize,featureType,minHitRate,maxFalseAlarmRate,w,h);
     unix_w(cmd);
endfunction;
