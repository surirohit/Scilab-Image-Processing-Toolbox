i = imread('test2.jpg');
corners = corner(i,'MinQuality',0.5);
disp(corners);
