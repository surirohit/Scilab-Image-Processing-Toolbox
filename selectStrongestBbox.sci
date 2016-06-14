//[selectedBbox selectedScore]=selectStrongestBbox(bBox,score)
//[selectedBbox selectedScore selectedIndex]=selectStrongestBbox(bBox,score,name,value)
function [selectedBbox,selectedScore,varargout]=selectStrongestBbox(bBox,score,varargin)
    
    [lhs,rhs]=argn(0)
    //To check the number of input and output arguments
    if rhs<2 then
         error(msprintf(" Not enough input arguments"))
    elseif rhs>6 then
         error(msprintf(" Too many input arguments to the function"))
    elseif lhs<2 then
         error(msprintf(" Not enough output arguments"))
    elseif lhs>3 then
         error(msprintf(" Too many output arguments"))
    end
    
    [bBoxRows bBoxCols]=size(bBox);
    [scoreRows scoreCols]=size(score);
    //To ensure matrices size 
    if ~bBoxCols==4 then
        error(msprintf("bounding box matrix must be M*4"))
    elseif ~scoreCols==1 then
          error(msprintf(" score matrix must be M*1"))
    elseif ~bBoxRows==scoreRows then
          error(msprintf(" The number of bounding box and score do not match"))
    elseif ~isreal(bBox)
        error(msprintf(" Wrong input argument,complex matrix is not expected"))
    end
    //To ensure both width and height of the box are positive
    for i=1:bBoxRows
        if bBox(i,3)<0 | bBox(i,4)<0
            error(msprintf(" The width and height of the bounded box must be positive"))
        end
    end
    
    [bBoxRows bBoxCols]=size(bBox);
    [scoreRows scoreCols]=size(score);
    //setting to default values
    ratioType=1;
    overlapThreshold=0.5;
    
    //checking for the name,value pairs if any
    for i=1:2:rhs-2
       if strcmpi(varargin(i),"RatioType")==0 then
            i=i+1;
            if strcmpi(varargin(i),'union')== 0 then
                ratioType=1;//1-union
            elseif  strcmpi(varargin(i),'min')==0 then
                ratioType=0;//0-min
            else
                error(msprintf(" wrong value for the ratio type,it must be union or min"))
            end
        elseif strcmpi(varargin(i),'OverlapThreshold')==0 then
            i=i+1;
            if 0<=varargin(i) & varargin(i)<=1 then
                overlapThreshold=varargin(i)
            else
                error(msprintf("wrong value for the overlapThreshold,it must be in between 0 and 1"))
            end
        else
            error(msprintf(_(" Wrong value for input argument")));
        end
    end
    
    selection=ones(size(bBox,1),1);	
    areaOfBox = bBox(:,3).*bBox(:,4);	
    
    //upperleft corner
    x1 = bBox(:,1); 
    y1 = bBox(:,2);
    
    //lowerright corner
    x2 = bBox(:,1)+bBox(:,3);
    y2 = bBox(:,2)+bBox(:,4);
    for i = 1:bBoxRows 
        if selection(i)
            for j = (i+1):bBoxRows 
                if selection(j)
                    width = min(x2(i), x2(j)) - max(x1(i), x1(j)); 
                    height = min(y2(i), y2(j)) - max(y1(i), y1(j));
                    if width < 0 | height < 0
                        continue;
                    end 
                    intersectionArea = width * height; 
                    if ratioType
                        overlapRatio = intersectionArea/(areaOfBox(i)+areaOfBox(j)-intersectionArea); 
                    else
                        overlapRatio = intersectionArea/min(areaOfBox(i), areaOfBox(j)); 
                    end
                    
                    if overlapRatio > overlapThreshold 
                        if score(i)<score(j)
                            selection(i)=0;
                        else
                            selection(j)=0;
                        end
                    end
                end
            end
        end
    end
    k=1;
    for i=1:bBoxRows
        if selection(i)
            selectionIndex(k)=i;
            k=k+1;
        end
    end
    //disp(selectionIndex);
    [selectedIndexRows selectedIndexRows]=size(selectionIndex)
    for i=1:selectedIndexRows
        for j=i+1:selectedIndexRows
            if selectionIndex(i)>selectionIndex(j)
                temp=selectionIndex(i);
                selectionIndex(i)=selectionIndex(j)
                selectionIndex(j)=temp;
             end
        end
    end
    //disp("selectional indexes");
    //disp(selectionIndex);
    selectedBbox=bBox(selectionIndex,:);
    selectedScore=score(selectionIndex,:);
    varargout(1)=selectionIndex;
endfunction

