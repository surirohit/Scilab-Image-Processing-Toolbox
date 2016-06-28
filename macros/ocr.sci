function [ txt ] = ocr(image)
// This function is used to identify the text in an image.
//
// Calling Sequence
//   results = ocr(image);
//
// Parameters
// results: OCR Text Struct which contains text, characterBoundingBoxes, words and wordConfidences
// image: Input image to the OCR
//
// Description
// OCR stands for Optical Character Recognition. It is used for scanning documents which contain text and to convert them into documents that can be edited.
//
// Examples
// image = imread('sample.jpg');
// results = ocr(image);
//
// Authors
//  Rohit Suri

	image_list = mattolist(image)

	[ text, characterBoundingBoxes, words, WordConfidences ] = opencv_ocr(image_list)

	txt = struct("Text", text, "characterBoundingBoxes", characterBoundingBoxes, "Words", words, "WordConfidences", WordConfidences)

endfunction
