function [gWindow] = Zyroskop_Okno_Przesuwne(t, gVal, windowSize, filename)
    
    % Długość wektora danych
    N = size(gVal, 1);
    
    % Inicjalizacja wektora wyjściowego
    gWindow = zeros(N, 3);
    
    % Okno przesuwne
    for i = 1:N
        
        % Minimalny indeks, nie może być mniejszy od 1
        startIndex = max([1; i - windowSize]);

        gWindow(i, :) = floor(mean(gVal(startIndex:i, :)));

    end
    
    % Nazwy osi
    axisNames  = ["X", "Y", "Z"];

    % Kolory osi
    axisColors = ["r", "g", "b"]; 

    % Obraz
    figure("Name", filename);

    for i = 1:3

        % Etykiety tekstowe
        gValLabel    = sprintf("g%s", axisNames(i));
        gWindowLabel = sprintf("gWindow%s", axisNames(i));

        % Linie
        lineSpecDash    = sprintf("%s--", axisColors(i));

        % Wykres dla pojedynczej osi
        subplot(3, 1, i);
        plot(t, gVal(:, i), axisColors(i), ...
             t, gWindow(:, i), lineSpecDash);

        xlabel("t, s");
        ylabel(gValLabel);
        grid("on");
        legend(gValLabel, ...
            gWindowLabel);

    end

end