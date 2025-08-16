clear; clc;

x = -3:0.02:3;
y = 6.5*sin(2.1*x + (pi/3));
z=y;

size = 6/0.02 + 1;% +1 b/c there is one more data point at x=-3

% Part 1: f = 30%

permutations = randperm(size);
frac = round(0.3 * size);% No. of points to be corrupted
corrupted = permutations(1 : frac);%points to be corrupted
corrVals = 100 + 20 * rand(frac,1);

% creating corruted values
for i=1:frac
    z(corrupted(i)) = y(corrupted(i)) + corrVals(i);
end

% applying moving median filtering
ymedian_30 = zeros(1, size);
for i=1:8
    ymedian_30(i) = median(z(1:i+8));% near the ends the neighbourhood is smaller
end

for i=9:size-8
    ymedian_30(i) = median(z(i-8:i+8));
end

for i=size-7:size
    ymedian_30(i) = median(z(i-8:size));% near the ends the neighbourhood is smaller
end

% applying moving average filtering
ymean_30 = zeros(1, size);
for i=1:8
    ymean_30(i) = mean(z(1:i+8));
end

for i=9:size-8
    ymean_30(i) = mean(z(i-8:i+8));
end

for i=size-7:size
    ymean_30(i) = mean(z(i-8:size));
end

% applying moving quartile filtering
yquartile_30 = zeros(1, size);
for i=1:8
    yquartile_30(i) = quantile(z(1:i+8),0.25);
end

for i=9:size-8
    yquartile_30(i) = quantile(z(i-8:i+8),0.25);
end

for i=size-7:size
    yquartile_30(i) = quantile(z(i-8:size),0.25);
end

%Figrue
figure; % Creating a new figure window

plot(x, y, 'color', [0, 0.4470, 0.7410], 'LineStyle', '-', 'LineWidth', 2, 'DisplayName', 'Original Wave');
hold on;
plot(x, z, 'color', [0.6350, 0.0780, 0.1840], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Corrupted Wave');
plot(x, ymedian_30, 'color', 'black', 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Median');
plot(x, ymean_30, 'color', [0.8500, 0.3250, 0.0980], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Mean');
plot(x, yquartile_30, 'color', [0.4660, 0.6740, 0.1880], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Quartile');

xlabel('x');
ylabel('y');
title('Plot for f = 30%');
legend('Location', 'best');
grid on;
hold off;

% Mean squared errors for each filtering
disp("For f = 30%:");

err_median = (sum((ymedian_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for median filtering: ' num2str(err_median)]);

err_mean = (sum((ymean_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for mean filtering: ' num2str(err_mean)]);

err_quartile = (sum((yquartile_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for quartile filtering: ' num2str(err_quartile)]);

%==========================================================================================================

% Part 2: f = 60%

permutations = randperm(size);
frac = round(0.6 * size);% No. of points to be corrupted
corrupted = permutations(1 : frac);%points to be corrupted
corrVals = 100 + 20 * rand(frac,1);

% creating corruted values
for i=1:frac
    z(corrupted(i)) = y(corrupted(i)) + corrVals(i);
end

% applying moving median filtering
ymedian_60 = zeros(1, size);
for i=1:8
    ymedian_60(i) = median(z(1:i+8));% near the ends the neighbourhood is smaller
end

for i=9:size-8
    ymedian_60(i) = median(z(i-8:i+8));
end

for i=size-7:size
    ymedian_60(i) = median(z(i-8:size));% near the ends the neighbourhood is smaller
end

% applying moving average filtering
ymean_60 = zeros(1, size);
for i=1:8
    ymean_30(i) = mean(z(1:i+8));
end

for i=9:size-8
    ymean_60(i) = mean(z(i-8:i+8));
end

for i=size-7:size
    ymean_60(i) = mean(z(i-8:size));
end

% applying moving quartile filtering
yquartile_60 = zeros(1, size);
for i=1:8
    yquartile_60(i) = quantile(z(1:i+8),0.25);
end

for i=9:size-8
    yquartile_60(i) = quantile(z(i-8:i+8),0.25);
end

for i=size-7:size
    yquartile_60(i) = quantile(z(i-8:size),0.25);
end

%Figrue

figure; % Creating a new figure window

plot(x, y, 'color', [0, 0.4470, 0.7410], 'LineStyle', '-', 'LineWidth', 2, 'DisplayName', 'Original Wave');
hold on;
plot(x, z, 'color', [0.6350, 0.0780, 0.1840], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Corrupted Wave');
plot(x, ymedian_60, 'color', 'black', 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Median');
plot(x, ymean_60, 'color', [0.8500, 0.3250, 0.0980], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Mean');
plot(x, yquartile_60, 'color', [0.4660, 0.6740, 0.1880], 'LineStyle', '-', 'LineWidth', 1.5, 'DisplayName', 'Quartile');

xlabel('x');
ylabel('y');
title('Plot for f = 60%');
legend('Location', 'best');
grid on;
hold off;

% Mean squared errors for each filtering
disp("For f = 60%:");

err_median = (sum((ymedian_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for median filtering: ' num2str(err_median)]);

err_mean = (sum((ymean_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for mean filtering: ' num2str(err_mean)]);

err_quartile = (sum((yquartile_30-y).^2))/(sum(y.^2));
disp(['Mean squared error for quartile filtering: ' num2str(err_quartile)]);