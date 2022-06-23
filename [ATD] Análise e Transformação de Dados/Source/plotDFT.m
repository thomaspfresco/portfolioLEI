function [max_mag] = plotDFT(data, i, text, window)
     fs = 50;

     if window == "hamming"
        win = hamming(numel(data{i}));
     elseif window == "blackman"
        win = blackman(numel(data{i}));
     elseif window == "hann"
        win = hann(numel(data{i}));
     end
     
     X = abs(fftshift(fft(detrend(data{i}).*win)));
     N = numel(X);
     
     if (mod(N,2)==0)
         f = -fs/2:fs/N:fs/2-fs/N;
     else
         f = -fs/2+fs/(2*N):fs/N:fs/2-fs/(2*N);
     end
     
     figure;
     plot(f,X);
     title(text);
     xlabel('F(Hz)');
     ylabel('Magnitude |X|');
     
     max_mag = findpeaks(X);
     max_mag = max(max_mag);
end