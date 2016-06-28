i = imread('test1.jpg');
corners = detectHarrisFeatures(i);
disp(corners);
