clear;
clc;
close all;

im1 = double(imread("T1.jpg"));
im2 = double(imread("T2.jpg"));
im1_inverted = 255-im1;
dim = size(im2);

num_bins = ceil(256/10); % number of bins

tx_values = -10:10; % shift values 

% Correlation values & QMI values matrix for both the case hence dimensions
% are 2*length(tx_values). First row for the first case second row for the
% second case
correlation_coefficients = zeros(2, length(tx_values));
QMI = zeros(2,length(tx_values));

for i = 1:length(tx_values)
    tx = tx_values(i);

    shifted_im2 = imtranslate(im2, [tx, 0]);% this translate the image in x direction by tx pixels

    correlation_coefficients(1,i) = corr2(im1, shifted_im2);% Calulates the correlation coeff between the two matrices
end

figure;
plot(tx_values, correlation_coefficients(1,:));
xlabel('Shift (tx)');
ylabel('Correlation Coefficient (ρ)');
title('Correlation Coefficient vs. Shift');

for n = 1:length(tx_values)
    tx = tx_values(n);

    shifted_im2 = imtranslate(im2, [tx, 0]);

    % Joint Histogram of pixel intensities for the two images 
    joint_histogram = zeros(num_bins,num_bins);
    for i = 1:dim(1)
    
        for j = 1:dim(2)
            
            k = floor(im1(i,j)/10) + 1;
            l = floor(shifted_im2(i,j)/10) + 1;
            joint_histogram(k,l) = joint_histogram(k,l) + 1;
            
        end

    end

    total_data_points = dim(1) * dim(2);
    joint_histogram = joint_histogram / total_data_points;
    
    % Calculating marginal histogram from the joint histogram
    marginal_im1 = sum(joint_histogram,2);
    marginal_im2 = sum(joint_histogram,1);

    for i = 1:num_bins
    
        for j = 1:num_bins
            % Calculating QMI matrix
            QMI(1,n) = QMI(1,n) + (joint_histogram(i,j) - marginal_im1(i)*marginal_im2(j))^2;
 
        end

    end

end

figure;
plot(tx_values, QMI(1,:));
xlabel('Shift (tx)');
ylabel('QMI');
title('QMI vs. Shift');

% Second Case where second image in inverted image1 
for i = 1:length(tx_values)
    tx = tx_values(i);

    shifted_im1_inverted = imtranslate(im1_inverted, [tx, 0]);

    correlation_coefficients(2,i) = corr2(im1, shifted_im1_inverted);
end

figure;
plot(tx_values, correlation_coefficients(2,:));
xlabel('Shift (tx)');
ylabel('Correlation Coefficient (ρ)');
title('Correlation Coefficient vs. Shift (Inverted)');

for n = 1:length(tx_values)
    tx = tx_values(n);

    shifted_im1_inverted = imtranslate(im1_inverted, [tx, 0]);

    joint_histogram = zeros(num_bins,num_bins);
    for i = 1:dim(1)
    
        for j = 1:dim(2)
            
            k = floor(im1(i,j)/10) + 1;
            l = floor(shifted_im1_inverted(i,j)/10) + 1;
            joint_histogram(k,l) = joint_histogram(k,l) + 1;
            
        end

    end

    total_data_points = dim(1) * dim(2);
    joint_histogram = joint_histogram / total_data_points;
    
    marginal_im1 = sum(joint_histogram,2);
    marginal_im2 = sum(joint_histogram,1);

    for i = 1:num_bins
    
        for j = 1:num_bins
            
            QMI(2,n) = QMI(2,n) + (joint_histogram(i,j) - marginal_im1(i)*marginal_im2(j))^2;
 
        end

    end

end

figure;
plot(tx_values, QMI(2,:));
xlabel('Shift (tx)');
ylabel('QMI');
title('QMI vs. Shift (Inverted)');


