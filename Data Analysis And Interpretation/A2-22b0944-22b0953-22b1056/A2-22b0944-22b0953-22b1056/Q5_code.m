clear; 
clc; 
close all;
% DIstribution of probabilities
probabilities = [0.05, 0.4, 0.15, 0.3, 0.1];
values = [1, 2, 3, 4, 5];

% Values of N
Ns = [5, 10, 20, 50, 100, 200, 500, 1000, 5000, 10000];
nsamp = 10000;
X_bar =  zeros(length(Ns), nsamp);

% Preallocate MADs array
MADs = zeros(length(Ns), 1);

% Create a figure for histograms
figure;

for i = 1:length(Ns)
    N = Ns(i);
 
   
    % Generate random samples for each value of N
    for j = 1:nsamp
        data = randsample(values, N, true, probabilities);
        X_bar(i,j) = mean(data);
    end

    % Plot histogram
    subplot(5, 2, i);
    histogram(X_bar(i, :), 50);
    title(['N = ' num2str(N)]);

end

for i = 1: length(Ns)
    N = Ns(i);

   
    mu = mean(X_bar(i, :));
    sigma = std(X_bar(i, :));

    [f, x] = ecdf(X_bar(i, :));

    gaussian_cdf = normcdf(x, mu, sigma);

    MADs(i) = max(abs(f-gaussian_cdf));
   
    figure;
    plot(x, f, 'b');
    hold on;
    plot(x, gaussian_cdf, 'r');
    title(['Empirical CDF vs Gaussian CDF (N = ' num2str(N) ')']);
    legend('Empirical CDF', 'Gaussian CDF');
    hold off;
end

figure;
plot(Ns, MADs, 'o-');
xlabel('N');
ylabel('Maximum Absolute Difference');
title('Maximum Absolute Difference between Empirical CDF and Gaussian CDF');
