i = imread('test2.jpg');
corners = detectMinEigenFeatures(i,'ROI',[10 10 100 100]);
disp(corners);

