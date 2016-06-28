function [imagePoints,boardSize,varargout]=detectCheckerboardPoints(input1,varargin)
// This function is used to detect a checkerboard pattern in an image.
//
// Calling Sequence
// [imagePoints boardSize] = detectCheckerboardPoints(I);
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(imageFileNames);
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(images);
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(imageFileNames1,imageFileNames2);
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(images1,images2);
//
// Parameters
// imagePoints: Matrix containing checkerboard points.
// boardSize: Matrix (1x2) containing boardSize
// imagesUsed: Images used to detect checkerboard in case of multiple input images
// imageFileNames: Path to image files
// images: Input images
//
// Description
// This function detects a checkerboard in an image and returns the points of the checkeboard corners.
//
// Examples
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(imageFileNames);
//
// [imagePoints boardSize imagesUsed] = detectCheckerboardPoints(images1,images2);
//
// Authors
//  Rohit Suri

    [lhs rhs]=argn(0);
    if lhs>3
         error(msprintf(" Too many output arguments"));
    elseif rhs>2
        error(msprintf(" Too many input arguments"));
    end
    select rhs
        case 1 then
            [points boardSize usedImages]=opencv_detectCheckerboardPoints(input1);
        case 2 then
            [points boardSize usedImages]=opencv_detectCheckerboardPoints(input1,varargin(1));
    end
    for i=1:size(points(1))
        for j=1:size(points(1)(1),'r')
            imagePoints(:,:,i)=points(1)(i);
        end
    end
    if lhs==3 then
        len=size(usedImages,'c');
        varargout(1)=matrix(usedImages,len,1);
    end
endfunction
