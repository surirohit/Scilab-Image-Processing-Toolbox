i = imread('test2.jpg');
corners = detectHarrisFeatures(i,'MinQuality',0.6,'MinQuality',0.7);
disp(corners);
