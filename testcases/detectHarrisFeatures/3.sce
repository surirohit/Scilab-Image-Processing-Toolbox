i = imread('test3.jpg');
corners = detectHarrisFeatures(i,'FilterSize',7);
disp(corners);
