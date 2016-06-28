function imageList=mattolist(imageMat)
	[rows cols channels]=size(imageMat);
    imageList=list();
    for i=1:channels
        imageList($+1)=imageMat(:,:,i);
    end
endfunction
