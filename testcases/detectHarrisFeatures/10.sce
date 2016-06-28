i = imread('test2.jpg');
corners = detectHarrisFeatures(i,'SensitivityFactor',0.1,'SensitivityFactor',0.7);
disp(corners);
