%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop - Wyznaczanie Offsetu
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [v] = Zyroskop_Calkowanie(t, gVal, filename)

    % Długość wektora danych
    N = size(gVal, 1);
    
    % Inicjalizacja wektora wyjściowego
    v = zeros(N, 3);
    
    % Pierwszy pomiar
    v(1, :) = gVal(1, :);

    % Okno przesuwne
    for i = 2:N
    
        v(i) = v(i-1) + gVal(i);
    
    end
    
    % Nazwy osi
    axisNames  = ["X", "Y", "Z"];

    % Kolory osi
    axisColors = ["r", "g", "b"]; 
    
    % Obraz
    figure("Name", filename);

    

    for i = 1:3
        
        % Etykiety tekstowe
        vLabel     = sprintf("v%s", axisNames(i));
        vZeroLabel = sprintf("v%s = 0", axisNames(i));

        % Linie
        lineSpecDashDot = sprintf("%s-.", axisColors(i));

        % Wykres dla pojedynczej osi
        subplot(3, 1, i);
        plot(t, v(:, i), axisColors(i));
        yline(0, lineSpecDashDot, vZeroLabel);
        xlabel("t, s");
        ylabel(vLabel);
        grid("on");
        legend(vLabel, ...
            vZeroLabel);

    end
end