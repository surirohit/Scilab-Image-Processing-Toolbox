i = imread('test3.jpg');
corners = detectMinEigenFeatures(i,'FilterSize',7);
disp(corners);
