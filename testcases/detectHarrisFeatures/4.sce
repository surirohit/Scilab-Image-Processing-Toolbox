i = imread('test1.jpg');
corners = detectHarrisFeatures(i,'SensitivityFactor',0.08);
disp(corners);
