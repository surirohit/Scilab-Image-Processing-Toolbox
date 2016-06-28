function [outputImg]=imread(imagePath)
    outputList=opencv_imread(imagePath);
    for i=1:size(outputList)
        outputImg(:,:,i)= outputList(i);
    end
endfunction
