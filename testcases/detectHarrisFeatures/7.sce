i = imread('test1.jpg');
corners = detectHarrisFeatures(i,'MinQuality',1.1);
disp(corners);
