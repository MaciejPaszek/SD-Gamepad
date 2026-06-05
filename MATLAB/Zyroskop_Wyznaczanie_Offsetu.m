%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop - Wyznaczanie Offsetu
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [gAvg] = Zyroskop_Wyznaczanie_Offsetu(t, gVal, filename)

    % Maksymalna wartość pomiaru
    gMin = min(gVal);

    % Minimalna wartość pomiaru
    gMax = max(gVal);

    % Offset - wartość średnia pomiaru statycznego
    gAvg = floor(mean(gVal));

    % Nazwy osi
    axisNames  = ["X", "Y", "Z"];

    % Kolory osi
    axisColors = ["r", "g", "b"]; 
    
    % Obraz
    figure("Name", filename);

    for i = 1:3
        
        % Etykiety tekstowe
        gLabel    = sprintf("g%s", axisNames(i));
        gMaxLabel = sprintf("max(g%s) = %d", axisNames(i), gMax(i));
        gAvgLabel = sprintf("avg(g%s) = %d", axisNames(i), gAvg(i));
        gMinLabel = sprintf("min(g%s) = %d", axisNames(i), gMin(i));
        
        % Linie
        lineSpecDash    = sprintf("%s--", axisColors(i));
        lineSpecDashDot = sprintf("%s-.", axisColors(i));

        % Wykres dla pojedynczej osi
        subplot(3, 1, i);
        plot(t, gVal(:, i), axisColors(i));
        yline(gMax(i), lineSpecDash, gMaxLabel);
        yline(gAvg(i), lineSpecDashDot, gAvgLabel);
        yline(gMin(i), lineSpecDash, gMinLabel);
        xlabel("t, s");
        ylabel(gLabel);
        ylim([gMin(i) - 1, gMax(i) + 1]);
        grid("on");
        legend(gLabel, ...
            gMaxLabel, ...
            gAvgLabel, ...
            gMinLabel);

    end
end