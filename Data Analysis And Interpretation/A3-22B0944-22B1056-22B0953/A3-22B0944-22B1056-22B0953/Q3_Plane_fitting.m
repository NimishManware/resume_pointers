clear; clc; close all;

points = dlmread("XYZ.txt",',');
n = size(points, 1);

X = points(:, 1); 
Y = points(:, 2); 
Z = points(:, 3); 

Sx2 = dot(X,X);
Sy2 = dot(Y,Y);
Sxy = dot(X,Y);
Sxz = dot(X,Z);
Syz = dot(Y,Z);
Sx = sum(X);
Sy = sum(Y);
Sz = sum(Z);

A = [Sx2 Sxy Sx; Sxy Sy2 Sy; Sx Sy n];
B = [Sxz Syz Sz];

V = A\B(:);

Noise = Z - V(1) * X - V(2) * Y - V(3);
Variance = var(Noise);

fprintf('The required equation of plane: z = %d*x + %d*y + %d\n',V(1),V(2),V(3));
fprintf('Variance of Noise: %d',Variance);
