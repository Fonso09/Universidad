numStages = size(SOS, 1);
cmsis_coeffs = zeros(numStages, 5);

for i = 1:numStages
    % Aplicar la ganancia G(i) a los coeficientes bi y negar ai
    cmsis_coeffs(i, :) = [G(i) * SOS(i, 1), G(i) * SOS(i, 2), G(i) * SOS(i, 3), -SOS(i, 5), -SOS(i, 6)];
end

% Imprimir en formato CMSIS-DSP
fprintf('float iir_coeffs[%d][5] = {\n', numStages);
for i = 1:numStages
    fprintf('    { %.6ff, %.6ff, %.6ff, %.6ff, %.6ff },\n', cmsis_coeffs(i, :));
end
fprintf('};\n');
