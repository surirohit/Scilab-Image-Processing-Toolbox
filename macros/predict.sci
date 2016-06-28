function classifierDescription = predict(Categoryclassifier, image)
// This function is used to predict the category of an image using an image classifier.
//
// Calling Sequence
// label = predict(classifier, image)
//
// Parameters
// label: Evaluated label of the input image
// classifier: Image category classifier
// image: Input image
//
// Description
// This function predicts the category of an image based on the category classifier provided.
//
// Examples
// imgSet = imageSet(directory,'recursive');
// [trainingSet testSet] = partition(imgSet,[0.8]);
// bag = bagOfFeatures(trainingSet);
// categoryClassifier = trainImageCategoryClassifier(trainingSet, bag);
// image = imread('sample.jpg');
// label = predict(categoryClassifier, image);
//
// Authors
//  Rohit Suri
//  Umang Agrawal
	
	image_list = mattolist(image)
	Categoryclassifier_list = classifierToList(Categoryclassifier);
	
	classifierDescription = opencv_predict(Categoryclassifier_list, image_list)
	
endfunction
