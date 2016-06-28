i = imread('test2.jpg');
corners = corner(i,'SensitivityFactor',1.1);
disp(corners);

