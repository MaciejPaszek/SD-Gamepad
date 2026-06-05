%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Czyszczenie przestrzeni roboczej, czyszczenie konsoli, zamykanie okien 
clear all;
close all;
clc;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Opcje
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Zakres próbek do kalibracji
offsetBegin = 1;
offsetEnd = 100;
offsetRange = offsetBegin:offsetEnd;

% Szerokość okna przesuwnego
windowSize = 10;

% Okres próbkowania
h = 0.1;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Wczytywanie Danych Pomiarowych
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Opcje importu danych tekstowych
opts = delimitedTextImportOptions;

% Pierwsza linia pliku zawiera nagłówek
opts.VariableNamesLine = 1;     

% Od drugiej lini pliku zaczynają się dane
opts.DataLines = 2;        

% Wartości w pliku są oddzielone średnikami
opts.Delimiter = ";";           

% Nazwa pliku
%csvFilename = "Żyroskop Pomiary Statyczne.csv";
csvFilename = "Pomiary Ruch.csv";

% Odczyt danych z pliku CSV
DataTable = readtable(csvFilename, opts);

% Konwersja tekstu na liczbę
t   = double(string(DataTable.t)) * h;
gX  = double(string(DataTable.gX));
gY  = double(string(DataTable.gY));
gZ  = double(string(DataTable.gZ));
vX  = double(string(DataTable.vX));
vY  = double(string(DataTable.vY));
vZ  = double(string(DataTable.vZ));


% Macierz danych pomiarowych z trzech osi
gVal = [gX, gY, gZ];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Wyznaczanie Offsetu
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

gOffset = Zyroskop_Wyznaczanie_Offsetu(t(offsetRange), gVal(offsetRange, :), ...
    "Wyznaczanie Offsetu.png");

% Stosowanie offsetu
gVal = gVal - gOffset;

gOffset = Zyroskop_Wyznaczanie_Offsetu(t(offsetRange), gVal(offsetRange, :), ...
    "Zastosowanie Offsetu.png");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Okno Przesuwne
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

gWindow = Zyroskop_Okno_Przesuwne(t, gVal, 3, "Okno Przesuwne");

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Całkowanie
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

gIntegral = Zyroskop_Calkowanie(t, gVal, "Całkowanie");