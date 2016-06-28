i = imread('test3.jpg');
corners = detectHarrisFeatures(i,'FilterSize',5,'MinQuality',0.5);
disp(corners);
