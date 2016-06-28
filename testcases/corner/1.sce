i = imread('test1.jpg');
corners = corner(i,'Method','MinimumEigenValue');
disp(corners);
