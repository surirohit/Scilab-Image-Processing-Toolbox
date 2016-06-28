i = imread('test2.jpg');
corners = corner(i,'ROI',[10 10 100 100],'Method','MinimumEigenValue');
disp(corners);

