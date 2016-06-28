i = imread('test1.jpg');
corners = detectMinEigenFeatures(i,'MinQuality',1.1);
disp(corners);
