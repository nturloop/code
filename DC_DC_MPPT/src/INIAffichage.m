function [affichage] = INIAffichage(fig)

h = animatedline(fig.UIAxes,"Color",'b');
h2 = animatedline(fig.UIAxes,"Color",'r');

%{

ax = gca;
ax.YGrid = 'on';
ax.YLim = [0 25];
%}
h3 = animatedline(fig.UIAxes2,"Color",'b');
h4 = animatedline(fig.UIAxes2,"Color",'r');
%{

ax1 = gca;
ax1.YGrid = 'on';
ax1.YLim = [0 20];
%}

h5 = animatedline(fig.UIAxes3,"Color",'b');
%{

ax1 = gca;
ax1.YGrid = 'on';
ax1.YLim = [0 30];
%}
affichage = struct("TensionEntree",h,"courant",h2,"TensionSortie",h3,"CourantSortie",h4,"dutyCycle",h5);

%{

h6 = animatedline("Color",'b');
ax1 = gca;
ax1.YGrid = 'on';
ax1.YLim = [0 30];



B = uicontrol('Style', 'PushButton');
B.String = {'Menu'};
B.Callback = @exit_loop;
B.Position = [10 10 50 20]

C = uicontrol('Style', 'PushButton');
C.String = {'Activer'};
C.Callback = @Activer;
C.Position = [10 30 50 20]


drawnow
%}

end