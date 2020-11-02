clear all
close all
clc

format short
canal_serie = serial('COM4','BaudRate',115200,'Terminator', 'CR/LF');
fopen(canal_serie);
xlabel('Puntos'); ylabel('Datos');
grid on; hold on; grid minor;
prop = line(nan,nan,'Color','b','LineWidth',1);
prop1 = line(nan,nan,'Color','r','LineWidth',1);
prop2 = line(nan,nan,'Color','g','LineWidth',1);
legend('Eje x','Eje Y','Eje z');  %% Podeis poner el mombre que querais. Las señales llegan en el mismo orden que las pongamos en Arduino
dato = fscanf(canal_serie, '%f %f %f ',[3,1]); 

i = 1;
figure(1);
while 1 

    dato = fscanf(canal_serie, '%f %f %f ',[3,1]);
    y(i) = dato(1,:);  %% POTENCIA
    x(i) = dato(2,:);  %% PITCH
    z(i) = dato(3,:);  %% ROLL    

    if i<1000      
    set(prop,'YData', y(1:i),'XData',(1:i));
    set(prop1,'YData', x(1:i),'XData',(1:i));
    set(prop2,'YData', z(1:i),'XData',(1:i));    
    end
    
    if i>1000      
    set(prop,'YData', y(i-1000:i),'XData',(i-1000:i));
    set(prop1,'YData', x(i-1000:i),'XData',(i-1000:i));
    set(prop2,'YData', z(i-1000:i),'XData',(i-1000:i));    
    end
    
    xlim([i-400 i])
    drawnow;
    i = i+1; 
end