%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Czyszczenie przestrzeni roboczej, czyszczenie konsoli, zamykanie okien 
clear all;
close all;
clc;

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

% Okres próbkowania
h = 0.01;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Badane zakresy pomiarowe
DPS = [ 250;
        500;
       1000;
       2000];

for i = 1:4

    % Nazwa pliku
    csvFilename = sprintf("Żyroskop Statyczny %d DPS.csv", DPS(i));

    % Odczyt danych z pliku CSV
    DataTable = readtable(csvFilename, opts);
    
    % Konwersja tekstu na liczbę
    t   = double(string(DataTable.t)) * h;
    gX  = double(string(DataTable.gX));
    gY  = double(string(DataTable.gY));
    gZ  = double(string(DataTable.gZ));
    
    % Macierz danych pomiarowych z trzech osi
    gVal = [gX, gY, gZ];
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Wyznaczanie Offsetu
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    gOffset = Zyroskop_Wyznaczanie_Offsetu(t, gVal, ...
        sprintf("Wyznaczanie Offsetu %d DPS.png", DPS(i)))
    
    % Stosowanie offsetu
    gVal = gVal - gOffset;
    
    gOffset = Zyroskop_Wyznaczanie_Offsetu(t, gVal, ...
        sprintf("Zastosowanie Offsetu %d DPS.png", DPS(i)));

end