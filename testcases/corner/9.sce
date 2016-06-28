i = imread('test3.jpg');
corners = corner(i,'SensitivityFactor',0.08,'MinQuality',0.5);
disp(corners);
