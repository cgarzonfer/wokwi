# Actividad 2
En esta actividad se ha utilizado el dispositivo programado en la Actividad 1, el cual simula una boya marina, y se ha ampliado su funcionalidad. En concreto, se ha añadido un algoritmo de control de la temperatura interior para mantenerla a una determinada temperatura de consigna con un margen de +/- 3ºC.
El dispositivo mejorado puede verse en este enlace al [proyecto Wokwi](https://wokwi.com/projects/432228479492520961)

## Elementos añadidos al dispositivo
Al objeto de implementar de la nueva funcionalidad, se han añadido los siguientes elmentos al prototipo:
- Un display de 4 dígitos para mostrar la temperatura interior con dos cifras decimales.
- Un LED de 8x8 puntos tipo MAX7219 que simula la activación del sistema de calefacción.
- Un motor servo con una palanca que simula la activación del sistema de refrigeración.
Estos nuevos elementos se ubican bajo la pantalla LCD, como se aprecia en la siguiente imagen.
![imagen](/.../assets/myAvatar.png)


## Funcionamiento del algoritmo de control
Dado que el sensor de temperatura DHT22 disponible en Wokwi tiene que manipularse manualmente, se opta por distinguir entre una temperatura exterior (temperature), la del ambiente, que es medida por el sensor, y una temperatura interior (tempInterior) del recinto donde se alojan las baterías. De manera natural, la temperatura interior tiende a aproximarse a la temperatura exterior y lo hace a una velocidad de 0.5ºC cada 500ms.
