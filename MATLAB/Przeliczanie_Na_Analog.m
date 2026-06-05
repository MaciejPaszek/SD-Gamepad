%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SD Gamepad - Żyroskop
% (c) Maciej Paszek, Paweł Suwart 2026
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Obraz
filename = "Przeliczanie Na Analog.png";
figure("Name", filename);

pMin = -90;
pMax =  90;

aMin =      0;
aMax =   4095;

p = [-105, pMin, pMax,  105];
a = [aMin, aMin, aMax, aMax];

plot(p, a, "r");

xline(pMin, "g--", sprintf("pMin = %d", pMin), ...
    "LabelVerticalAlignment", "bottom");
xline(pMax, "g--", sprintf("pMax = %d", pMax), ...
    "LabelVerticalAlignment", "bottom");
xline(0, "g--", sprintf("p = %d", 0), ...
    "LabelVerticalAlignment", "bottom");

yline(aMin, "b--", sprintf("aMin = %d", aMin), ...
    "LabelHorizontalAlignment", "left");
yline(aMax, "b--", sprintf("aMax = %d", aMax), ...
    "LabelHorizontalAlignment", "left");
yline(2048, "b--", sprintf("aMax = %d", 2048), ...
    "LabelHorizontalAlignment", "left");

xlabel("p, deg");
ylabel("a, -");

margin = (aMax - aMin) / 5;
xlim([-105, 105]);
ylim([aMin - margin, aMax + margin]);

grid("on");
set(gca, "FontSize", 14);
exportgraphics(gcf, filename);