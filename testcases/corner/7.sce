i = imread('test1.jpg');
corners = corner(i,'MinQuality',1.1,'Method','MinimumEigenValue');
disp(corners);
