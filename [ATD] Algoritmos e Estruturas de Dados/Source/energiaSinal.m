function [energia] = energiaSinal(data)
    win = blackman(numel(data));

    sinalEnergy = abs(data.*win).^2;
    energia = sum(sinalEnergy);
end
