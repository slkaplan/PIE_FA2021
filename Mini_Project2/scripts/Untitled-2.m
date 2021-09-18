data = csvread("C:\Users\ieykamp\Olin\MatSci\Specimen_RawData_2.csv");
time = data(:, 1);
extension = data(:, 2);
load = data(:, 3);
thickness = 4e-3; % inches
width = 1.00; % inches
A = width * thickness;
initial_length = 5; % inches

stress = load ./ A;
strain = extension ./ initial_length;

figure(1);
clf;
hold on;

yield_extension = 20.69035;
yield_index = find(extension == yield_extension);


strength = cumsum(stress(1:end-1) .* diff(strain));

yyaxis left;
plot(strain(1:end-1), stress(1:end-1), 'b-');
plot(strain(yield_index), stress(yield_index), 'ks');
ylabel("Stress (N / m^2)");

yyaxis right;
plot(strain(1:end-1), strength);
plot(strain(yield_index), strength(yield_index), 'ks');
ylabel("Strength (Nm)");

xlabel("Strain (mm)");
title("Stress vs. Strain Plot for Tensile Test");