function [varargout] = evaluateImageRetrieval(image, IndexImage, ExpectedID, varargin)
	/// varargout(1) = average_precision
	/// varargout(2) = index
	/// varargout(3) = score
	[ lhs rhs ] = argn(0)
	if rhs > 7 then
		error(msprintf("Too many input arguments"))
	end	
	
	if lhs > 3 then
		error(msprintf("Too many output arguments"))
	end
	image_list = mattolist(image)
	
	if lhs == 1 then
		select rhs 
			case 3 then
				
				average_precision= opencv_evalutateImageRetrieval(image_list, IndexImage, ExpectedID)
			
			case 5 then
				
				average_precision = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2))
				
			case 7 then
			
				average_precision = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2), varargin(3), varargin(4))
		end
		
		varargout(1) = average_precision
		
	elseif lhs == 2 then
		select rhs 
			case 3 then
				
				[average_precision, index] = opencv_evalutateImageRetrieval(image_list, IndexImage, ExpectedID)
			
			case 5 then
				
				[average_precision, index] = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2))
				
			case 7 then
			
				[average_precision, index] = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2), varargin(3), varargin(4))
		end
		
		varargout(1) = average_precision
		varargout(2) = index
	
	elseif lhs == 3 then
		select rhs 
			case 3 then
				
				[average_precision, index, score] = opencv_evalutateImageRetrieval(image_list, IndexImage, ExpectedID)
			
			case 5 then
				
				[average_precision, index, score] = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2))
				
			case 7 then
			
				[average_precision, index, score] = opencv_evaluateImageRetrieval(image_list, IndexImage, ExpectedID, varargin(1), varargin(2), varargin(3), varargin(4))
		end
		
		varargout(1) = average_precision
		varargout(2) = index
		varargout(3) = score
			
	end
	
endfunction
