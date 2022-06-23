function [max_mag] = plotSinal(data, i,titulo1,titulo2)
     fs = 50;
     Ts = 1/fs;
     
     N = numel(data{i});
     t = linspace(0,Ts.*(N-1)/60,N);

     figure;
     plot(t,data{i});
     xlim([0 Ts*(N-1)/60])
     title(titulo1);
     xlabel('t[s]');
     ylabel('Amplitude');
     hold on
     trend = data{i} - detrend(data{i},'constant');
     plot(t,trend)
     hold off
     
     X = abs(fftshift(fft(detrend(data{i}))));
     N = numel(X); % Periodo Fundamental
     
     if (mod(N,2)==0)
         f = -fs/2:fs/N:fs/2-fs/N;
     else
         f = -fs/2+fs/(2*N):fs/N:fs/2-fs/(2*N);
     end

     figure;
     plot(f,X);
     title(titulo2);
     xlabel('F(Hz)');
     ylabel('Magnitude |X|');
      
     max_mag = findpeaks(X);
     max_mag = max(max_mag);
end