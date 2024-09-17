function MAJAffichage(affichageh,DataAnalogueActuel,temps)

addpoints(affichageh.TensionEntree,datenum(temps),DataAnalogueActuel.TensionEntree)
addpoints(affichageh.courant,datenum(temps),DataAnalogueActuel.Courant)
addpoints(affichageh.TensionSortie,datenum(temps),DataAnalogueActuel.TensionSortie)
addpoints(affichageh.CourantSortie,datenum(temps),DataAnalogueActuel.CourantOUT)
addpoints(affichageh.dutyCycle,datenum(temps),DataAnalogueActuel.DutyCycleValue)

%{

ax.XLim = datenum([temps-seconds(15) temps]);
datetick('x','keeplimits');
%}
end
