clear; clc; close all;

% Drawing values from normal dist. with sigma = 4 from stadndar normal dist.
sigma = 4;
n = 1000;

Samples = 4 * randn(n, 1);

indices_T = randperm(n,500);

T = Samples(indices_T);

V = setdiff(Samples, T);

Bandwidths = [0.001, 0.1, 0.2, 0.9, 1, 2, 3, 5, 10, 20, 100];

LL = zeros(length(Bandwidths),1);

for i = 1 : length(Bandwidths)
    joint_likelihood = 1;
    
    for j = 1:length(V)
        likelihood = sum(exp(-(V(j) - T).^2 / (2 * Bandwidths(i)^2))) / (750 * Bandwidths(i) * sqrt(2 * pi));
        joint_likelihood = joint_likelihood * likelihood;
    end
    
    % Calculate the log of the joint likelihood
    LL(i) = log(joint_likelihood);
end

[best_LL, best_sigma_index] = max(LL);
best_sigma = Bandwidths(best_sigma_index);
fprintf('The sigma for best LL is : %d\n',best_sigma);

figure;
plot(log(Bandwidths), LL, 'o-');
xlabel('log(\sigma)');
ylabel('log of the joint likelihood (LL)');
title('LL vs log(\sigma)');

% Set of x ∈ [−8 : 0.1 : 8] 
X = linspace(-8, 8, 161);

Approx_PDF = pn(X, T, best_sigma);
True_PDF = normpdf(X, 0, sigma);

figure;
plot(X, Approx_PDF, 'b', 'LineWidth', 2);
hold on;  
plot(X, True_PDF, 'r', 'LineWidth', 2);
title('True PDF And Approx. PDF for best LL');
xlabel('x');
ylabel('Probability Density');
legend('Approximate PDF', 'True PDF');
hold off;

%Calculating D
D = zeros(length(Bandwidths),1);
for i = 1: length(Bandwidths)
    D(i) = sum((normpdf(V,0,sigma) - pn(V,T,Bandwidths(i))).^2);
end

[best_D, best_sigma_index_2] = min(D);
best_sigma_2 = Bandwidths(best_sigma_index_2);
fprintf('The sigma for best D is : %d\n and the best D value is: %d\n',best_sigma_2,best_D);
fprintf('The D value at the sigma which gave best LL value is: %d\n',D(best_sigma_index));


figure;
plot(log(Bandwidths), D, 'o-');
xlabel('log(\sigma)');
ylabel('D');
title('D vs log(\sigma)');

Approx_PDF_2 = pn(X, T, best_sigma_2);

figure;
plot(X, Approx_PDF_2, 'b', 'LineWidth', 2);
hold on;  
plot(X, True_PDF, 'r', 'LineWidth', 2);
title('True PDF And Approx. PDF for best D');
xlabel('x');
ylabel('Probability Density');
legend('Approximate PDF', 'True PDF');
hold off;

function result = pn(X, Y, sigma)
    result = zeros(length(X),1);
    
    for i = 1:length(Y)
        for j = 1:length(X)
        result(j) = result(j) + exp(-(X(j) - Y(i))^2 / (2 * sigma^2));
        end
    end

    result = result / (length(Y) * sigma * sqrt(2 * pi));
end