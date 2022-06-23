function [relacao] = sensibilidade(dft_x,dft_y,dft_z,threshold)
positive = 0;

%calculo da sensibilidade
total = numel(dft_x) + numel(dft_y) + numel(dft_z);
for n = 1:size(dft_x)
    if ((dft_x(n) < mean(dft_x)+threshold) && (dft_x(n) > mean(dft_x)-threshold))
        positive = positive + 1;
    end
end

for n = 1:size(dft_y)
    if ((dft_y(n) < mean(dft_y)+threshold) && (dft_y(n) > mean(dft_y)-threshold))
        positive = positive + 1;
    end
end

for n = 1:size(dft_z)
    if ((dft_z(n) < mean(dft_z)+threshold) && (dft_z(n) > mean(dft_z)-threshold))
        positive = positive + 1;
    end
end

relacao = positive / total;