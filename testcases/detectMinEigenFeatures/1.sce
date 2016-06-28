i = imread('test1.jpg');
corners = detectMinEigenFeatures(i);
disp(corners);
