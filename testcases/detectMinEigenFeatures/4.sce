i = imread('test1.jpg');
corners = detectMinEigenFeatures(i,'MinQuality',0.08);
disp(corners);
