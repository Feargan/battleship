X = [-0.2,
-0.18,
-0.16,
-0.14,
-0.12,
-0.1,
-0.08,
-0.06,
-0.04,
-0.02,
0,
0.02,
0.04,
0.06,
0.08,
0.1,
0.12,
0.14,
0.16,
0.18,
0.2,
0.22,
0.24,
0.26,
0.28,
0.3,
0.32,
0.34,
0.36,
0.38,
0.4];
Ymin = [0
0
0
0
0.0001
0.0005
0.0015
0.0043
0.01
0.0209
0.0382
0.0657
0.0982
0.1528
0.2503
0.3291
0.4041
0.5249
0.5954
0.7321
0.8012
0.8556
0.8975
0.9544
0.9685
0.9825
0.9903
0.9975
0.9985
0.9996
0.9998
];
Ymax = [0.0001
0.0002
0.0004
0.0012
0.004
0.0095
0.021
0.0391
0.0773
0.1299
0.1857
0.2662
0.3981
0.4961
0.5863
0.6832
0.8097
0.8586
0.9082
0.9407
0.9641
0.9903
0.9935
0.9968
0.9992
0.9998
1
1
1
1
1
];
fcn = @(b,x) normcdf(x, b(1), b(2));                    % Objective Function
NRCFmin = @(b) norm(Ymin - fcn(b,X));
NRCFmax = @(b) norm(Ymax - fcn(b,X));                     % Norm Residual Cost Function
Bmin = fminsearch(NRCFmin, [1; 1]);                          % Estimate Parameters
Bmax = fminsearch(NRCFmax, [1; 1]);
Xplot = linspace(min(X), max(X));
figure(1)
plot(X, Ymin, 'pg')
hold on
plot(X, Ymax, '*r')
plot(Xplot, fcn(Bmin,Xplot))
plot(Xplot, fcn(Bmax,Xplot))
title(sprintf('\\mu\_{min} = %.8f\n\\sigma\_{min} = %.8f\n\\mu\_{max} = %.8f\n\\sigma\_{max} = %.8f', Bmin, Bmax))
ylabel("Autofill chance");
xlabel("Indicator");
hold off
grid
