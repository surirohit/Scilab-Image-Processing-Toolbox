i = imread('test2.jpg');
corners = detectHarrisFeatures(i,'ROI',[10 10 100 100]);
disp(corners);

