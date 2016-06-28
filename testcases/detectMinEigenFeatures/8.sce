i = imread('test2.jpg');
corners = detectMinEigenFeatures(i,'ROI',2);
disp(corners);

