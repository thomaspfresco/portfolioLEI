% Variaveis para completar o nome dos diferentes ficheiros de dados para input
user = {'09', '10', '11', '12'};
exp = {'17', '18', '19', '20', '21', '22', '23', '24', '25'};

atividades = {'W', 'W-UP', 'W-DOWN', 'SIT', 'STAND', 'LAY', 'STAND-SIT', 'SIT-STAND', 'SIT-LAY', 'LAY-SIT', 'STAND-LAY', 'LAY STAND'};

% Cada sample de aceleração foi captado a uma frequência de 50Hz, segundo o site.
Fs = 50;

% Sensores usados
sensors = {'ACC X', 'ACC Y', 'ACC Z'};

% Variaveis que vão guardar a informação lida dos ficheiros de input

%Movimentos Dinâmicos
w_x = {{},{},{},{},{},{},{},{},{}};
w_y = {{},{},{},{},{},{},{},{},{}};
w_z = {{},{},{},{},{},{},{},{},{}};
 
w_up_x = {{},{},{},{},{},{},{},{},{}};
w_up_y = {{},{},{},{},{},{},{},{},{}};
w_up_z = {{},{},{},{},{},{},{},{},{}};
 
w_down_x = {{},{},{},{},{},{},{},{},{}};
w_down_y = {{},{},{},{},{},{},{},{},{}};
w_down_z = {{},{},{},{},{},{},{},{},{}};

%Movimentos Estáticos
sit_x = {{},{},{},{},{},{},{},{},{}};
sit_y = {{},{},{},{},{},{},{},{},{}};
sit_z = {{},{},{},{},{},{},{},{},{}};
 
stand_x = {{},{},{},{},{},{},{},{},{}};
stand_y = {{},{},{},{},{},{},{},{},{}};
stand_z = {{},{},{},{},{},{},{},{},{}};
 
lay_x = {{},{},{},{},{},{},{},{},{}};
lay_y = {{},{},{},{},{},{},{},{},{}};
lay_z = {{},{},{},{},{},{},{},{},{}};
 
%Movimentos Transição
stand_sit_x = {{},{},{},{},{},{},{},{},{}};
stand_sit_y = {{},{},{},{},{},{},{},{},{}};
stand_sit_z = {{},{},{},{},{},{},{},{},{}};
 
sit_stand_x = {{},{},{},{},{},{},{},{},{}};
sit_stand_y = {{},{},{},{},{},{},{},{},{}};
sit_stand_z = {{},{},{},{},{},{},{},{},{}};
 
sit_lay_x = {{},{},{},{},{},{},{},{},{}};
sit_lay_y = {{},{},{},{},{},{},{},{},{}};
sit_lay_z = {{},{},{},{},{},{},{},{},{}};
 
lay_sit_x = {{},{},{},{},{},{},{},{},{}};
lay_sit_y = {{},{},{},{},{},{},{},{},{}};
lay_sit_z = {{},{},{},{},{},{},{},{},{}};
 
stand_lay_x = {{},{},{},{},{},{},{},{},{}};
stand_lay_y = {{},{},{},{},{},{},{},{},{}};
stand_lay_z = {{},{},{},{},{},{},{},{},{}};
 
lay_stand_x = {{},{},{},{},{},{},{},{},{}};
lay_stand_y = {{},{},{},{},{},{},{},{},{}};
lay_stand_z = {{},{},{},{},{},{},{},{},{}};

% Importar a informação "labels.txt"
labels = importLabels([pwd, '/raw_data/','labels.txt']);
%disp(labels);

% Ler ficheiros de input
for i=1:9
    e=i;
    if i == 1 || i == 2 
        u=1;
    elseif i == 3 || i == 4 || i == 5
        u=2;
    elseif i == 6 || i == 7
        u=3;
    else
        u=4;
    end
    
    % construir o nome do ficheiro de dados
    file = [pwd, '/raw_data/',sprintf('acc_exp%s_user%s.txt', exp{e}, user{u})];
    
    % Importar info para a variavel data
    data = importData(file);
            
    %selecionar da info das labels apenas a do ficheiro considerado na iteração 
    indices_labels = intersect(find(labels(:, 1) == str2double(exp{e})), find(labels(:, 2) == str2double(user{u})));
    %disp(size(indices_labels));
    % Calculo do vetor de tempo da data
    t = (0: size(data, 1) - 1)./Fs;
    
    % Calculo do tamanho de colunas da data (saber quantos sensores foram utilizados)
    [~, n_plots] = size(data);
    
    % Plot dos de cada sinal (info do ficheiro)
    
    %plotData(n_plots, atividades, data, sensors, t, labels, indices_labels, acc_file);
    
    %guardar informação separada por tipo 
    %de atividade para cada experiência (para uso posterior)
    for j=1:size(indices_labels,1) 
        
       switch labels((indices_labels(j,1)),3) %Switch do ID da atividade
            case 1 %WALKING
                atividade = w_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                w_x{i} = atividade;
                
                atividade = w_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                w_y{i} = atividade;
 
                atividade = w_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                w_z{i} = atividade;
                
            case 2 %WALKING_UPSTAIRS
                atividade = w_up_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                w_up_x{i} = atividade;
 
                atividade = w_up_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                w_up_y{i} = atividade;
 
                atividade = w_up_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                w_up_z{i} = atividade;
 
            case 3 %WALKING_DOWNSTAIRS
                atividade = w_down_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                w_down_x{i} = atividade;
 
                atividade = w_down_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                w_down_y{i} = atividade;
 
                atividade = w_down_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                w_down_z{i} = atividade;
 
            case 4 %SITTING
                atividade = sit_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                sit_x{i} = atividade;
 
                atividade = sit_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                sit_y{i} = atividade;
 
                atividade = sit_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                sit_z{i} = atividade;
 
            case 5 %STANDING
                atividade = stand_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                stand_x{i} = atividade;
 
                atividade = stand_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                stand_y{i} = atividade;
 
                atividade = stand_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                stand_z{i} = atividade;
 
            case 6 %LAYING
                atividade = lay_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                lay_x{i} = atividade;
 
                atividade = lay_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                lay_y{i} = atividade;
 
                atividade = lay_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                lay_z{i} = atividade;
 
            case 7 %STAND_TO_SIT
                atividade = stand_sit_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                stand_sit_x{i} = atividade;
 
                atividade = stand_sit_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                stand_sit_y{i} = atividade;
 
                atividade = stand_sit_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                stand_sit_z{i} = atividade;
 
            case 8 %SIT_TO_STAND
                atividade = sit_stand_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                sit_stand_x{i} = atividade;
 
                atividade = sit_stand_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                sit_stand_y{i} = atividade;
 
                atividade = sit_stand_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                sit_stand_z{i} = atividade;
 
            case 9 %SIT_TO_LIE
                atividade = sit_lay_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                sit_lay_x{i} = atividade;
 
                atividade = sit_lay_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                sit_lay_y{i} = atividade;
 
                atividade = sit_lay_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                sit_lay_z{i} = atividade;
 
            case 10 %LIE_TO_SIT
                atividade = lay_sit_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                lay_sit_x{i} = atividade;
 
                atividade = lay_sit_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                lay_sit_y{i} = atividade;
 
                atividade = lay_sit_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                lay_sit_z{i} = atividade;
 
            case 11 %STAND_TO_LIE
                atividade = stand_lay_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                stand_lay_x{i} = atividade;
 
                atividade = stand_lay_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                stand_lay_y{i} = atividade;
 
                atividade = stand_lay_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                stand_lay_z{i} = atividade;
 
            case 12 %LIE_TO_STAND
                atividade = lay_stand_x{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),1);
                lay_stand_x{i} = atividade;
 
                atividade = lay_stand_y{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),2);
                lay_stand_y{i} = atividade;
 
                atividade = lay_stand_z{i};
                atividade{size(atividade,2)+1} = data(labels(indices_labels(j,1),4):labels(indices_labels(j,1),5),3);
                lay_stand_z{i} = atividade;
            otherwise
        end
    end    
end

% % 3.1 3.2 3.3 3.6 (janelas: blackman, hamming, hann)
% atividade_x = w_x;
% atividade_y = w_y;
% atividade_z = w_z;
% 
% for n=1:1    
%         e=n;
%         if n == 1 || n == 2 
%             u=1;
%         elseif n == 3 || n == 4 || n == 5
%             u=2;
%         elseif n == 6 || n == 7
%             u=3;
%         else
%             u=4;
%         end
% 
%         file = sprintf('acc-exp%s-user%s.txt', exp{e}, user{u});
%         
%         %:1 --> plot apenas a primeira ocorrencia da atividade
%         %:size(atividade_x{n},2) --> plot para cada ocorrencia 
%         max_mag_x = [];
%         for i=1:1
%             title = ['Sinal Original - walking X ',num2str(i, '%d'),' - ', file];
%             plotSinal(atividade_x{n}, i, title)
%           
%             title = ['|DFT| - walking X ',num2str(i, '%d'),' (blackman) - ', file];
%             max_mag = plotDFT(atividade_x{n}, i, title, 'blackman');
%             max_mag_x = [max_mag_x max_mag];
%             
%             title = ['|DFT| - walking X ',num2str(i, '%d'),' (hamming) - ', file];
%             max_mag = plotDFT(atividade_x{n}, i, title, 'hamming');
%             max_mag_x = [max_mag_x max_mag];
%             
%             title = ['|DFT| - walking X ',num2str(i, '%d'),' (hann) - ', file];
%             max_mag = plotDFT(atividade_x{n}, i, title, 'hann');
%             max_mag_x = [max_mag_x max_mag];
% 
%         end
% 
%         %:1 --> plot apenas a primeira ocorrencia da atividade
%         %:size(atividade_y{n},2) --> plot para cada ocorrencia 
%         max_mag_y = [];
%         for i=1:1
%             title = ['Sinal Original - walking Y ',num2str(i, '%d'),' - ', file];
%             plotSinal(atividade_y{n}, i, title)
%           
%             title = ['|DFT| - walking Y ',num2str(i, '%d'),' (blackman) - ', file];
%             max_mag = plotDFT(atividade_y{n}, i, title, 'blackman');
%             max_mag_y = [max_mag_y max_mag];
%             
%             title = ['|DFT| - walking Y ',num2str(i, '%d'),' (hamming) - ', file];
%             max_mag = plotDFT(atividade_y{n}, i, title, 'hamming');
%             max_mag_y = [max_mag_y max_mag];
%             
%             title = ['|DFT| - walking Y ',num2str(i, '%d'),' (hann) - ', file];
%             max_mag = plotDFT(atividade_y{n}, i, title, 'hann');
%             max_mag_y = [max_mag_y max_mag];
% 
%         end
%         
%         %:1 --> plot apenas a primeira ocorrencia da atividade
%         %:size(atividade_z{n},2) --> plot para cada ocorrencia 
%         max_mag_z = [];
%         for i=1:1
%             title = ['Sinal Original - walking Z ',num2str(i, '%d'),' - ', file];
%             plotSinal(atividade_z{n}, i, title)
%           
%             title = ['|DFT| - walking Z ',num2str(i, '%d'),' (blackman) - ', file];
%             max_mag = plotDFT(atividade_z{n}, i, title, 'blackman');
%             max_mag_z = [max_mag_z max_mag];
%             
%             title = ['|DFT| - walking Z ',num2str(i, '%d'),' (hamming) - ', file];
%             max_mag = plotDFT(atividade_z{n}, i, title, 'hamming');
%             max_mag_z = [max_mag_z max_mag];
%             
%             title = ['|DFT| - walking Z ',num2str(i, '%d'),' (hann) - ', acc_file];
%             max_mag = plotDFT(atividade_z{n}, i, title, 'hann');
%             max_mag_z = [max_mag_z max_mag];
% 
%         end
% 
%         aux = vertcat(max_mag_x,max_mag_y);
%         aux = vertcat(aux,max_mag_z);
%         
%         T = array2table(aux,'VariableNames',{'blackman','hamming','hann'},'RowNames',{'eixo X','eixo Y','eixo Z'});
%         disp(T);       
% end

% 3.4
% dinamicas_x = {w_x, w_up_x, w_down_x};
% dinamicas_y = {w_y, w_up_y, w_down_y};
% dinamicas_z = {w_z, w_up_z, w_down_z};
% 
% [media_desvio_wx, media_desvio_wupx, media_desvio_wdownx] = mediaPassos(dinamicas_x);
% [media_desvio_wy, media_desvio_wupy,  media_desvio_wdowny] = mediaPassos(dinamicas_y);
% [media_desvio_wz, media_desvio_wupz,  media_desvio_wdownz] = mediaPassos(dinamicas_z);
% 
% dinamicas = {'walking'; 'walking up'; 'walking down'};
% eixo_X = {media_desvio_wx; media_desvio_wupx; media_desvio_wdownx};
% eixo_Y = {media_desvio_wy; media_desvio_wupy; media_desvio_wdowny};
% eixo_Z = {media_desvio_wz; media_desvio_wupz; media_desvio_wdownz};
% 
% T = table(dinamicas, eixo_X, eixo_Y, eixo_Z);
% disp(T)

% % 3.5
% figure;
% hold on
% grid on
% 
% for i=1:9
%     for j=1:size(w_x{i},2)
%         black_win = blackman(numel(w_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(w_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'o','MarkerEdgeColor',[1 0 0],'DisplayName','WALKING');
%         
%     end
%     
%     for j=1:size(w_up_x{i},2)
%         black_win = blackman(numel(w_up_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(w_up_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_up_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_up_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'o','MarkerEdgeColor',[0 1 0],'DisplayName','WALKING_UPSTAIRS');
%         
%     end
%     
%     for j=1:size(w_down_x{i},2)
%         black_win = blackman(numel(w_down_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(w_down_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_down_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(w_down_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'o','MarkerEdgeColor',[0 0 1],'DisplayName','WALKING_DOWNSTAIRS');
%         
%     end
%     
%     for j=1:size(sit_x{i},2)
%         black_win = blackman(numel(sit_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(sit_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), '+','MarkerEdgeColor',[0 1 1],'DisplayName','SITTING');
%     end
%     
%     for j=1:size(stand_x{i},2)
%         black_win = blackman(numel(stand_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(stand_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), '+','MarkerEdgeColor',[1 0 1],'DisplayName','STANDING');
%     end
%     
%     for j=1:size(lay_x{i},2)
%         black_win = blackman(numel(lay_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(lay_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), '+','MarkerEdgeColor',[1 1 0],'DisplayName','LAYING');
%     end
%     
%     for j=1:size(stand_sit_x{i},2)
%         black_win = blackman(numel(stand2sit_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(stand2sit_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand2sit_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand2sit_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd', 'MarkerEdgeColor',[0 0.4470 0.7410],'DisplayName','STAND_TO_SIT');
%     end
%     
%     for j=1:size(sit2stand_x{i},2)
%         black_win = blackman(numel(sit2stand_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(sit2stand_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit2stand_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit2stand_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd','MarkerEdgeColor',[0.8500 0.3250 0.0980],'DisplayName','SIT_TO_STAND');
%     end
%     
%     for j=1:size(sit2lay_x{i},2)
%         black_win = blackman(numel(sit2lay_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(sit2lay_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit2lay_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(sit2lay_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd', 'MarkerEdgeColor',[0.9290 0.6940 0.1250],'DisplayName','SIT_TO_LIE');
%     end
%     
%     for j=1:size(lay2sit_x{i},2)
%         black_win = blackman(numel(lay2sit_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(lay2sit_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay2sit_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay2sit_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd','MarkerEdgeColor',[0.4940 0.1840 0.5560],'DisplayName','LIE_TO_SIT');
%     end
%     
%     for j=1:size(stand2lay_x{i},2)
%         black_win = blackman(numel(stand2lay_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(stand2lay_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand2lay_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(stand2lay_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd', 'MarkerEdgeColor',[0.4660 0.6740 0.1880],'DisplayName','STAND_TO_LIE');
%     end
%     
%     for j=1:size(lay2stand_x{i},2)
%         black_win = blackman(numel(lay2stand_x{i}{j}));
%         dft = abs(fftshift(fft(detrend(lay2stand_x{i}{j}).*black_win)));
%         [dft_x, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay2stand_y{i}{j}).*black_win)));
%         [dft_y, ~] = findpeaks(dft);
%         dft = abs(fftshift(fft(detrend(lay2stand_z{i}{j}).*black_win)));
%         [dft_z, ~] = findpeaks(dft);
%         plot3(dft_x(1), dft_y(1), dft_z(1), 'd', 'MarkerEdgeColor',[0.3010 0.7450 0.9330],'DisplayName','LIE_TO_STAND');
%     end
%     
% end
% hold off
% view(20,15)
%     
% 4
% Importar data
data = importData([pwd,'/raw_data/','acc_exp17_user09.txt']);

% Short-time Fourier Transform 
eixo_z = data(:,3);
plotSTFT(eixo_z);

