function classifier_list = classifierToList(classifier)
	/// classifier is a struct
	
	if ~isstruct(classifier)
		error(msprintf("Structure of classifier required\n"));
	end
	
	classifier_list = list("classifier",classifier.ClassifierLocation, classifier.BagofFeaturesLocation, classifier.Description);
	
endfunction
