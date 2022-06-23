function [] = plotData(n_plots, atividades, data, sensors, t, labels, indices_labels, acc_file)

figure;

for i=1:n_plots
    
    subplot(n_plots, 1, i); plot(t./60, data(:, i), 'k--')
    axis([0, t(end)./60 min(data(:, i)) max(data(:, i))])
    xlabel('Time (min)');
    ylabel(sensors{i}); %lista com sensores
    if i == 1
        title(acc_file)
    end
    hold on
    for j = 1 : numel(indices_labels)
        plot(t(labels(indices_labels(j), 4):labels(indices_labels(j), 5))./60, data(labels(indices_labels(j), 4):labels(indices_labels(j), 5), i));
        if mod(j, 2) == 1
            ypos = min(data(:, i)) - (0.2 * min(data(:, i)));
        else
            ypos = max(data(:, i)) - (0.2 * min(data(:, i)));
        end
        text(t(labels(indices_labels(j), 4))/60, ypos, atividades{labels(indices_labels(j), 3)});
    end
end
end