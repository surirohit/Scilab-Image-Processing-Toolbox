i = imread('test2.jpg');
corners = detectHarrisFeatures(i,'MinQuality',0.5);
disp(corners);
