i = imread('test3.jpg');
corners = detectHarrisFeatures(i,'SensitivityFactor',0.08,'MinQuality',0.5);
disp(corners);
