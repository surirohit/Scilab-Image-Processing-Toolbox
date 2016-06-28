function [bag]=bagOfFeatures(imageSet,varargin)
// This function is used to create a Bag-of-Words model (BoW model).
//
// Calling Sequence
//   bag = bagOfFeatures(imgSet);
//   bag = bagOfFeatures(imgSet, Name, Value, ...);
//
// Parameters
// bag: Bag of visual words
// imgSet: An imageSet structure
// VocabularySize: (Optional) Number of visual words (Default- 500)
// StrongestFeatures: (Optional) Fraction of strongest features (Default- 1.0)
// Upright: (Optional) Orientation of SURF feature vector (Default- true)
// Verbose: (Optional) Enable progress display to screen (Default- true)
//
// Description
// BoW model is applied to classify images by treating image features as words. The bagOfFeatures object contains a bag of visual words that help in image classification.
//
// Examples
// imgSet = imageSet(directory,'recursive');
// [trainingSet testSet] = partition(imgSet,[0.8]);
// bag = bagOfFeatures(trainingSet);
//
// Authors
//  Rohit Suri
//  Umang Agrawal

    [lhs rhs]=argn(0);
    if lhs>1
         error(msprintf(" Too many output arguments"));
    elseif rhs<1
        error(msprintf("At least one argument is required"));
    elseif rhs>9
        error(msprintf(" Too many input arguments"));
    end
    imageSetList=imageSetToList(imageSet);
    select rhs
        case 1 then
            bagList=opencv_bagOfFeatures(imageSetList);
        case 3 then
            bagList=opencv_bagOfFeatures(imageSetList,varargin(1),varargin(2));
        case 5 then
            bagList=opencv_bagOfFeatures(imageSetList,varargin(1),varargin(2),varargin(3),varargin(4));
        case 7 then
            bagList=opencv_bagOfFeatures(imageSetList,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6));
        case 9 then
            bagList=opencv_bagOfFeatures(imageSetList,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8));
    end
    bag=struct('FilePath',bagList(2)(1,1),'VocabularySize',bagList(3)(1,1),'StrongestFeatures',bagList(4)(1,1),'Upright',bagList(5)(1,1));
endfunction
