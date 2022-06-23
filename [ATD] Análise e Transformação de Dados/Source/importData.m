function accexpXXuserYY = importData(filename)

% variáveis auxiliares
startRow = 1;
endRow = inf;

formatSpec = '%f%f%f%[^\n\r]';

% Abrir o ficheiro de texto.
fileID = fopen(filename,'r');

% ler info do ficheiro de acordo com o tipo de formatação definido em 'formatSpec' (por coluna)
dataArray = textscan(fileID, formatSpec, endRow-startRow, 'Delimiter', " ", 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN, 'HeaderLines', startRow-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');

% fechar ficheiro
fclose(fileID);

% array de output
accexpXXuserYY = [dataArray{1:end-1}];