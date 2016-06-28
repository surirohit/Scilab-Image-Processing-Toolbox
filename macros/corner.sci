function [corners]=corner(image,varargin)
// This function is used to find corner points in an image.
//
// Calling Sequence
// c = corner(I);
// c = corner(I, Name, Value, ...);
//
// Parameters
// c: Structure of corner points
// I: Input image to corner()
// MinQuality: (Optional) Minimum accepted quality of corners (Default- 0.01)
// FilterSize: (Optional) Dimension of Gaussian Filter (Default: 5)
// ROI: (Optional) Rectangular region for corner detection
// SensitivityFactor: (Optional) SensitivityFactor of Harris algorithm (Default- 0.04)
// Method: (Optional) Method used to detect corner points- Harris or MinimumEigenValue (Default- Harris)
//
// Description
// This function detects corners in an image I. These corner points are used to extract features and hence recognize the contents of an image.
//
// Examples
// I = imread('sample.jpg');
// c = corner(I);
//
// I = imread('sample.jpg');
// c = corner(I,'Method','MinimumEigenValue');
//
// Authors
//  Rohit Suri
//  Sridhar Reddy
    [lhs rhs]=argn(0);
    if lhs>1
         error(msprintf(" Too many output arguments"));
    elseif rhs<1
        error(msprintf("Not enough  input arguments"));
    elseif rhs>9
        error(msprintf(" Too many input arguments"));
    elseif modulo(rhs,2)==0
       error(msprintf("Either Argument Name or its Value missing"));
    end
    imageList=mattolist(image);
    select rhs-1
        case 0 then
            [corners metric count]=opencv_corner(imageList);
        case 2 then
            [corners metric count]=opencv_corner(imageList,varargin(1),varargin(2));
        case 4 then
            [corners metric count]=opencv_corner(imageList,varargin(1),varargin(2),varargin(3),varargin(4));
        case 6 then
            [corners metric count]=opencv_corner(imageList,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6));
        case 8 then
            [corners metric count]=opencv_corner(imageList,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8));
    end
endfunction
