i = imread('test3.jpg');
corners = detectMinEigenFeatures(i,'FilterSize',6);
disp(corners);

