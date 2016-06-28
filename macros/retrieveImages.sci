function [varargout] = retrieveImages(QueryImage, IndexImage, varargin)
	/// varargout(1) = index
	/// varargout(2) = score
	/// varargout(3) = Imageword
	
	[ lhs rhs ] = argn(0)
	
	if rhs > 6 then
		error(msprintf("Too many input arguments"))
	end
	
	if lhs > 3 then
		error(msprintf("Too many output arguments"))
	end
	
	QueryImage_list = mattolist(QueryImage)
	
	if lhs == 1 then
		select rhs
			case 2 then
				
				index = opencv_retrieveImages(QueryImage_list, IndexImage)
			
			case 4 then
				
				index = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2))
			
			case 6 then
				index = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2), varagin(3), varargin(4))
		end
		
		varargout(1) = index
	
	elseif lhs == 2 then
		select rhs
			case 2 then
				
				[index, score] = opencv_retrieveImages(QueryImage_list, IndexImage)
			
			case 4 then
				
				[index, score] = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2))
			
			case 6 then
				[index, score] = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2), varagin(3), varargin(4))
		end
		
		varargout(1) = temp
		varargout(2) = score
		
	elseif lhs == 3 then
		select rhs
			case 2 then
				
				[index, score, Imageword] = opencv_retrieveImages(QueryImage_list, IndexImage)
			
			case 4 then
				
				[index, score, Imageword] = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2))
			
			case 6 then
				[index, score, Imageword] = opencv_retrieveImages(QueryImage_list, IndexImage, varargin(1), varargin(2), varagin(3), varargin(4))
		end
		
		varargout(1) = index
		varargout(2) = score
		del = struct("WordIndex", Imageword(1), "Location", Imageword(2), "Vocab_size", Imageword(3), "Count", Imageword(4))
		varargout(3) = del
	end
	
endfunction	
