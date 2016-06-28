i = imread('test1.jpg');
corners = corner(i,'SensitivityFactor',0.08);
disp(corners);
