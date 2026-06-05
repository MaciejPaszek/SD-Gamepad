%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function int32 = Int16ToInt32(int16)
    
    % Wagi dla int16
    int16weights = zeros(1, 16);

    for i = 1:15
        int16weights(i) = 2^i
    end

    % int16Wag = (2 * ones(1, 16)) .^ fliplr(0:15);
    % int16Wag(1) = -int16Wag(1);
    % 
    % % Wagi dla int32
    % int32Wag = (2 * ones(1, 32)) .^ fliplr(0:31);
    % int32Wag(1) = -int32Wag(1);
    % 
    % decInt16 = 

end