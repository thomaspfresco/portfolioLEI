function labels = importLabels(filename)

% variáveis auxiliares
startRow = 1;
endRow = inf;

formatSpec = '%f%f%f%f%f%[^\n\r]';

% abrir o ficheiro de texto
fileID = fopen(filename,'r');

% ler info do ficheiro de acordo com o tipo de formatação definido em 'formatSpec' (por coluna)
data = textscan(fileID, formatSpec, endRow-startRow, 'Delimiter', " ", 'MultipleDelimsAsOne', true, 'TextType', 'string', 'HeaderLines', startRow-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');

% fechar ficheiro
fclose(fileID);

% array de output
labels = [data{1:end-1}];