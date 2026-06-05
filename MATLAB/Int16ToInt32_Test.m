%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Wagi dla int16
wagiInt16 = Wagi(16)
wagiInt32 = Wagi(32)

liczba = zeros(1,16);
liczba(14) = 1;
liczba(15) = 1;
liczba(16) = 1;

liczba * wagiInt16'

liczba = ones(1,16);
liczba(14) = 0;
liczba(15) = 0;

liczba * wagiInt16'


function w = Wagi(bit)
    
    w = zeros(1, bit);
    
    for i = 1:bit
        if(i == 1)
            w(i) = -2^(bit - i);
        else
            w(i) =  2^(bit - i);
        end
    end

end

