# Actividad 2 - Wokwi
En esta actividad se ha utilizado el dispositivo programado en la Actividad 1, el cual simula una boya marina, y se ha ampliado su funcionalidad. En concreto, se ha añadido un algoritmo de control de la temperatura interior para mantenerla a una determinada temperatura de consigna con un margen de +/- 3ºC.
El dispositivo mejorado puede verse en este enlace al [proyecto Wokwi](https://wokwi.com/projects/432228479492520961)

## Elementos añadidos al dispositivo respecto a la Actividad 1
Al objeto de implementar de la nueva funcionalidad, se han añadido los siguientes elmentos al prototipo:
- Un display de 4 dígitos para mostrar la temperatura interior con dos cifras decimales.
- Un LED de 8x8 puntos tipo MAX7219 que simula la activación del sistema de calefacción.
- Un motor servo con una palanca que simula la activación del sistema de refrigeración.

## Funcionamiento del algoritmo de control
Dado que el sensor de temperatura DHT22 disponible en Wokwi tiene que manipularse manualmente, se opta por distinguir entre una temperatura exterior (tempExterior), la del ambiente, que es medida por el sensor, y una temperatura interior (tempInterior) del recinto donde se alojan las baterías. De manera natural, la temperatura interior tendería a aproximarse a la temperatura exterior. Esto se simula incrementando o decrementando la temperatura interior según la temperatura exterior y lo hace a una velocidad de 0.5ºC cada 500ms. Sin intervención de sistema de climatización, la temperatura interior tiende a igualarse a la exterior, lo cual se simula con este código:
```
 if (tempInterior > temperature){
    tempInterior = tempInterior - 0.5;
  } else {
    tempInterior = tempInterior + 0.5;
  }
```
Para el funcionamiento del sistema de climatización, se establece una temperatura de consigna (tempConsigna) de 25ºC y un margen (tempMargin) de +/- 3ºC. Además, el sistema tiene dos modos de funcionamiento, similar a los equipos de climatización reales, un modo calefacción (en invierno) y un modo refrigeración (en verano). Por ejemplo, una bomba de calor de aerotermia puede funcionar en ambos modos pero necesita una válvula 4 vías para cambiar el ciclo que hace el refrigerante según el caso ([cómo funciona una bomba de calor](https://learn.apolloheatpumps.com/es/como-funciona-una-bomba-de-calor/))

