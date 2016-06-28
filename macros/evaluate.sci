function confMat = evaluate(classifier, imageSet)
// This function is used to evaluate an image classifier.
//
// Calling Sequence
// confMat = evaluate(classifier, imgSet)
//
// Parameters
// confMat: Confusion matrix as a result of the evaluation
// classifier: Image category classifer
// imgSet: An imageSet struct to be evaluated
//
// Description
// This function evaluates an image category classifier on an input image set.
//
// Examples
// imgSet = imageSet(directory,'recursive');
// [trainingSet testSet] = partition(imgSet,[0.8]);
// bag = bagOfFeatures(trainingSet);
// categoryClassifier = trainImageCategoryClassifier(trainingSet, bag);
// confMatrix = evaluate(categoryClassifer, testSet);
//
// Authors
//  Rohit Suri
//  Umang Agrawal
	
	classifier_list = classifierToList(classifier)
	imageSet_list = imageSetToList(imageSet)
	
	confMat = opencv_evaluate(classifier_list, imageSet_list)	
	
endfunction
