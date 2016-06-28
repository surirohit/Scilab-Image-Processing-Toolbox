i = imread('test2.jpg');
corners = detectMinEigenFeatures(i,'MinQuality',0.5);
disp(corners);
