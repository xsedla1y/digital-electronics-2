# Project 1 - Analog joy-stick, rotary encoder, LCD and LEDs.

Application of analog joy-stick (2 ADC channels, 1 push button), rotary encoder, and Digilent PmodCLP LCD module.


## Členové týmu

* Josef Komár 230271 (Zodpovědný za: program enkodéru, řízení LED a jejich blikání, video )
* Ondřej Sedlák 230312 (Zodpovědný za: program pro joystick, celková kompletace programu a odlazení, github dokumentace)

## Popis hardwaru

V našem projektu byla použita deska Arduino UNO, která je založená na AVR čipu ATMEGA328P. Dále jsou použity externí součástky, jako je např. analogový joystick, rotary enkodér, LCD displej keysight a 7 různých LED diod.

![arduino uno pinout](images/arduino_pinout.png)

### Rotary encoder

A rotary encoder is a type of position sensor which is used for determining the angular position of a rotating shaft. It generates an electrical signal, either analog or digital, according to the rotational movement.

![rotary encoder](images/encoder.jpg)

### Analog joy-stick

Analog joystick produces two voltages; one corresponding to position with respect to X-axis and another corresponding to the position with respect to Y-axis. The voltages produced depend on the position of the joystick.
To interface the Analog Joystick with Arduino Uno, we need to use ADC on the microcontroller of the Arduino UNO board.

![analog joy-stick](images/joystick.jpg)



## Popis softwaru

Při psaní kódu jsme využili hned několik knihoven vytvořených v PC cvičeních. Např. timer, gpio a lcd.

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## Zdroje

1. https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/
2. https://docs.arduino.cc/hardware/uno-rev3
3. https://docs.arduino.cc/static/581ef7a3be69646a3f9eb4f6e2575548/A000066-datasheet.pdf
4. https://www.electronicwings.com/arduino/analog-joystick-interfacing-with-arduino-uno
