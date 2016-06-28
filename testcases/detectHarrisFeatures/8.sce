i = imread('test2.jpg');
corners = detectHarrisFeatures(i,'SensitivityFactor',1.1);
disp(corners);

