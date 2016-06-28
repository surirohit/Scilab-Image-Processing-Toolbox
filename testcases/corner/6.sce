i = imread('test3.jpg');
corners = corner(i,'FilterSize',6);
disp(corners);

