i = imread('test2.jpg');
corners = corner(i,'SensitivityFactor',0.1,'SensitivityFactor',0.7);
disp(corners);
