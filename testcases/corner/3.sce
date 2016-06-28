i = imread('test3.jpg');
corners = corner(i,'FilterSize',7,'Method','MinimumEigenValue');
disp(corners);
