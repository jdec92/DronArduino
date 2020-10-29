# Dron automatizado con Arduino
Este proyecto trata el diseño y construcción de un Dron tipo Quadcopter usando el microcontrolador Arduino, es decir, un Dron de 4 hélices. Esta introducción está dividida en dos bloques: 
 *  El primer bloque se centra en los conceptos generales que se necesitan saber, el diseño estructural, componentes y el diagrama eléctrico. 
 *  El segundo bloque se define el algoritmo implementado y los diferentes modos de vuelo.
 
Para una explicación más detallada se adjunta la memoría del proyecto en el siguiente enlace. 
[Memoría del Proyecto](https://github.com/jdec92/assets/blob/master/dron_arduino/TFG_JDEC.pdf)

## Primer Bloque
### Ejes de navegación
Para definir la posición del Dron se utiliza los 3 ángulos de navegación definidos como **Pitch, Roll** y **Yaw** 

![Ángulos de Navegación](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/navegation.png)

![Ángulos de Navegación Ilustrado](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/angle_dron.png)

Inclinando el Dron sobre estos ejes haremos que se desplace en la dirección de rotación.
Rotar en el eje **Pitch** hará a **avanzar o retroceder** 
Rotar en el eje **Roll** hará que se incline a la **izquierda o derecha**
Rotar sobre **Yaw** hará que gire sobre su propio eje vertical.
Además de estos tres movimientos, el Dron también será capaz de **subir o bajar**, ganando o perdiendo altura debido al **Throttle** (velocidad de los motores).

### Tipo Estructura
El Dron que se va a construir es de tipo ‘x’. Como vemos en la siguiente imagen la configuración que seleccionemos implica que para realizar un mismo movimiento los motores implicados tiene un sentido de giro diferente.

![Giro de motores](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/turning_engines.png)

En la configuración tipo ‘x’, para poder **avanzar** es necesario acelerar los dos motores traseros (3,4) y desacelerar los dos delanteros (1,2), es decir, rotar en el eje **Pitch**
Para inclinar hacía la **derecha** se acelera los motores de la izquierda (2,3) y reduce los motores de la derecha (1,4), es decir, rotar en el eje **Roll**
Para girar el Dron **sobre su eje vertical hacia la derecha**, se acelera los motores que giran en sentido horario (2,4) y reduce los motores que giran en sentido antihorario (1,3), es decir, rotar en el eje **Yaw**.
Para ascender el Dron, se acelera todos los motores (1,2,3,4), es decir, aumentamos el **Throttle**

### Componentes Hardware
 * Frame 450
 * Tira Led RGBW
 * Bluetooth HC-06
 * Resistencia 270KOhm y 180KOhm
 * Bateria Lipo
 * 4 motores Brushless
 * 4 ESC
 * 4 Helices
 * Placa de Distribución de Potencia
 * Arduino Uno
 * MPU6050
 * 5 Mosfet IRFZ44N
 * Pixy2
 * Cargador Imax B6
### Diagrama Eléctrico
El esquema eléctrico se ha diseñado con Proteus, en el diagrama no se incorpora la cámara pixy2, ya que esta va conectada únicamente por el puerto ISCP.
En el diagrama eléctrico se observa que hay conexiones indicada con etiquetas:
 * **Ground Arduino**, pertenece al puerto GND de la placa Arduino Uno.
 * **5V Arduino**, pertenece al puerto de salida de +5v de la placa Arduino
 * **12v Batería**, es una conexión a la salida positiva de la batería LIPO
 * **Ground Batería**, es una conexión al negativo de la batería LIPO.
 
![Diagrama Electrico](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/electric_diagram.jpg)

## Segundo Bloque
Antes de nada vamos a defenir el algoritmo PID, ya que si no comprendemos esto no podremos comprender los modos de vuelos.

### Algoritmo PID

El PID (controlador proporcional, integral y derivativo) es un mecanismo de control simultáneo por realimentación. Este calcula la desviación o error entre un valor medido y un valor deseado.
El algoritmo del control PID consta de tres parámetros distintos: el proporcional, el integral, y el derivativo. El valor proporcional depende del error actual, el integral depende de los errores pasados y el derivativo es una predicción de los errores futuros. La suma de estas tres acciones es usada para ajustar el proceso por medio de un elemento de control.
     Ajustando estas tres variables en el algoritmo de control del PID, el controlador puede proveer una acción de control adaptada a los requerimientos del proceso en específico.

![Algoritmo PID](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/pid.png)

El objetivo de un PID es conseguir un error entre la consigna de velocidad y la velocidad real de 0º/s (metros, grados… según la aplicación), es decir, que la velocidad de rotación real sea igual a la consigna que llega desde el Bluetooth o Pixy2. 
Lo primero que hace esta estructura de control es comparar la referencia de velocidad angular que nos llega desde el Bluetooth, con la lectura que recibimos del sensor MPU6050. Haciendo la resta de estas dos señales se consigue el valor de desviación o error del eje.
Tras acelerar los motores, el Dron empezará a rotar en el sentido que hayamos indicado hasta alcanzar la velocidad deseada, momento en el que el error bajará a 0º/s y habrá finalizado la operación.
Este error lo recibe el PID y genera una salida en función de los parámetros Kp (proporcional), Ki (integral) y Kd (derivativo) que hayamos establecido. 
Para establecer los valores se debe saber a qué se refiere cada uno:
 * La **acción proporcional P(t)**, amplia o reduce el error en función de una ganancia proporcional. 
    **Kp** es la ganancia proporcional que establecemos.
    
 * La **acción integral I(t)**, acumula el error a lo largo del tiempo. Se coge el error actual y lo se multiplica por el término Ki, pero en cada nuevo ciclo de control sumamos el valor obtenido en el ciclo anterior. De esta forma conseguimos que el error en régimen permanente sea de 0.

 * La **acción derivativa D(t)**, evalúa el incremento del error en función del tiempo, ponderado mediante la ganancia derivativa Kd. Sirve para suavizar la respuesta del control.

La expresión final de la acción de control es la suma de cada componente.

### Modos de Vuelos
 * ####     Modo Acrobático

En este modo de control solo se utiliza las lecturas de velocidad angular calculadas a partir de los datos obtenidos del sensor MPU6050. Si por ejemplo uno de los ejes da una vuelta completa en un segundo, la velocidad será de 360º/s.
     Si el eje Pitch del Dron rotara por cualquier razón, porque uno de los motores tiene más potencia, porque hay viento… el control tendrá que contrarrestar esta desviación actuando sobre los motores correspondientes. Pero el Dron no volverá a su posición inicial 0, simplemente compensara la rotación hasta detener el Dron. Esto es debido a que únicamente se utiliza como referencia la velocidad de rotación de los ejes. 

![Diagrama Modo Acrobático](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/pid_a.jpg)

 * ####     Modo Estable

Este modo necesita de dos PID en cascada por cada eje a controlar, además de lecturas de velocidad de rotación y aceleración del sensor MPU6050 a partir de los cuales calcular el ángulo de inclinación de cada eje en grados (º). 
La ventaja de este modo de vuelo es que el Dron es completamente estable. Al contrario que en el modo acrobático, cuando los valores de Pitch, Roll o Yaw vuelve a su posición de 0º de inclinación, el Dron volverá automáticamente a su posición de 0º. La estrategia de control es parecida a la figura mostrada para el modo acrobático, solo que utilizando un PID más en los ejes pitch y roll. 
El primer PID toma la lectura de inclinación (º) calculada a partir de las lecturas del sensor MPU6050 y la compara con la que se manda por él Bluetooth. Si hay una desviación de inclinación, este primer PID genera una referencia de velocidad para el siguiente lazo, acelerando los correspondientes motores y contrarrestando la inclinación. El segundo PID es como en el modo acrobatico.

![Diagrama Modo Estable](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/pid_e.jpg)

 * ####     Modo Aterrizaje y Despegue

Estos modos utiliza el mismo controlador PID que el modo estable, la única variación es la entrada del PID. La entrada ahora será para los ejes **Pitch, Roll y Yaw** de 0º y para el valor **Throttle** ira:
 * (Modo Despegue) aumentando en cada ciclo hasta llegar a valor definido como limite
 * (Modo Aterrizaje) disminuye en cada ciclo hasta llegar parar completamente los motores.

_NOTA: En este modo desactivamos el control del Dron a traves de Bluetooth, lo unico que podemos es cambiar el modo de vuelo, no podemos modificar el valor de **Pitch, Roll, Yaw o Throttle**_

![Diagrama Modo Aterrizaje y Despegue](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/pid_da.jpg)

 * ####     Modo Automatico

Este modo utiliza el mismo controlador PID que el modo estable, la única variación es la entrada del PID ahora es con el controlador de la cámara Pixy2, en vez del Bluetooth.
Este modo esta pensado para el seguimiento automatico de circuitos indicado con trayectorías o para el seguimientos de objetos de forma automatica.

![Diagrama Modo Pixy2](https://raw.githubusercontent.com/jdec92/assets/master/dron_arduino/pid_p.jpg)

_NOTA: En este modo desactivamos el control del Dron a traves de Bluetooth, lo unico que podemos es cambiar el modo de vuelo, no podemos modificar el valor de **Pitch, Roll, Yaw o Throttle**_
