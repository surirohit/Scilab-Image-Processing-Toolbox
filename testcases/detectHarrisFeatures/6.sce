i = imread('test3.jpg');
corners = detectHarrisFeatures(i,'FilterSize',6);
disp(corners);

