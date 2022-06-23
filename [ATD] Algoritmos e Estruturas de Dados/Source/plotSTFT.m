function [] = plotSTFT(data)
fs = 50; %Frequencia utilizada na experiencia
Ts = 1/fs; % Periodo fundamental
N = numel(data);
t = N*Ts; % Tempo total da atividade do utilizador

intervalos = [0.1 0.05 0.01 0.005 0.001 0.007];

for n = 1:6
    Tframe = intervalos(n)*t;
    
    %Sobreposicao de 50% de modo a melhorar a continuidade temporal e minimizar
    %a atenuacao da janelas nas epocas afastadas do centro
    Toverlap = Tframe/2;
    
    Nframe = round(Tframe*fs);
    
    win = blackman(Nframe);
    
    Noverlap = round(Toverlap*fs);
    espetro = []; % Vetor para guardar o espetro do sinal
    
    f = linspace(-fs/2,fs/2,Nframe); % Calculo da frequencia
    x =  find(f>=0); % Tratamento do sinal apenas em valores positivos
    
    % Calculo do espetro
    for i = 1:Nframe-Noverlap:N-Nframe
        x_frame = data(i:i+Nframe-1).*win;
        m_X_frame = abs(fftshift(fft(x_frame)));
        espetro = horzcat(espetro,m_X_frame(x)); %horzcat faz a concatenação de arrays horizontalmente
    end
    figure();
    imagesc(20*log10(espetro))
    
    set(gca,'YDir','normal')
    xlabel('t [s]')
    ylabel('f [Hz]')
    title(['STFT | Tamanho janela: ',num2str(intervalos(n), '%.3f x t | overlap 50 por cento')])
    colorbar
end
end