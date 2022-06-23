function [media_desvio_w, media_desvio_wup, media_devio_wdown] = mediaPassos(data)

    total_passos_w = [];
    total_passos_wup = [];
    total_passos_wdown = [];
   
    for n=1:9
        %WALKING
        for i=1:size(data{1}{n},2)
            win = blackman(numel(data{1}{n}{i}));
            dft = abs(fftshift(fft(data{1}{n}{i}.*win))); 
            [pks,~] = findpeaks(dft);
            passos = numel(pks)./2;
            
            %divide por 2 
            total_passos_w = [total_passos_w passos];
        end
        
        %WALKING UPSTAIRS
        for i=1:size(data{2}{n},2)
            win = blackman(numel(data{2}{n}{i}));
            dft = abs(fftshift(fft(data{2}{n}{i}).*win)); 
            [pks,~] = findpeaks(dft);
            passos = numel(pks)./2;
            
            total_passos_wup = [total_passos_wup passos];
        end
        
        %WALKING DOWNSTAIRS
        for i=1:size(data{3}{n},2)
            win = blackman(numel(data{3}{n}{i}));
            dft = abs(fftshift(fft(data{3}{n}{i}.*win)));
            [pks,~] = findpeaks(dft);
            passos = numel(pks)./2;
            
            total_passos_wdown = [total_passos_wdown passos];
        end
    end
    % media (por minuto)
    media_w = mean(total_passos_w);
    media_wup = mean(total_passos_wup);
    media_wdown = mean(total_passos_wdown);
    
    % desvio padrao
    desvio_w = std(total_passos_w);
    desvio_wup = std(total_passos_wup);
    desvio_wdown = std(total_passos_wdown);
    
    media_desvio_w = ['MEDIA: ' num2str(media_w, '%.2f') ' | DESVIO: ' num2str(desvio_w, '%.2f')];
    media_desvio_wup = ['MEDIA: ' num2str(media_wup, '%.2f') ' | DESVIO: ' num2str(desvio_wup, '%.2f')];
    media_devio_wdown = ['MEDIA: ' num2str(media_wdown, '%.2f') ' | DESVIO: ' num2str(desvio_wdown, '%.2f')];
end