function [] = plotWindows(data, i)
    
    N = numel(data{i});
    
     black_win = blackman(N);
     hamming_win = hamming(N);
     hanning_win = hann(N);
    
     n = 0:N/size(black_win,1):N-1;
     
     figure;
     hold on
     plot(n,hamming_win,'DisplayName','HAMMING')
     plot(n,hanning_win,'DisplayName','HANN')
     plot(n,black_win,'DisplayName','BLACKMAN')
     title('Janelas')
     legend()
     xlabel('Samples');
     ylabel('Amplitude');
     hold off
end