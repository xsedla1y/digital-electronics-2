# Lab 7: ONDŘEJ SEDLÁK

### Arduino Uno pinout

1. In the picture of the Arduino Uno board, mark the pins that can be used for the following functions/operations:
   | **Pin type** | **Pin number / name** |
   | :-: | :-: |
   | **PWM generators from Timer0, Timer1, Timer2** | **3, 5, 6, 9, 10, 11** |
   | analog channels for **ADC** | **A0, A1, A2, A3, A4, A5** |
   | **UART** pins | **TX => (1), RX <= (0)** |
   | **I2C** pins | **SCL, SDA** |
   | **SPI** pins | **10, 11, 12, 13** |
   | external interrupt pins **INT0, INT1** | **2, 3** |
   
   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/07-i2c/07-photos/ArduinoUNO_pinout.png)

### I2C communication

2. Draw a timing diagram of I2C signals when calling function `rtc_read_years()`. Let this function reads one byte-value from RTC DS3231 address `06h` (see RTC datasheet) in the range `00` to `99`. Specify when the SDA line is controlled by the Master device and when by the Slave device. Draw the whole request/receive process, from Start to Stop condition. The image can be drawn on a computer (by [WaveDrom](https://wavedrom.com/) for example) or by hand. Name all parts of timing diagram.

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/07-i2c/07-photos/lab7_flowdiag.png)

### Meteo station

Consider an application for temperature and humidity measurements. Use sensor DHT12, real time clock DS3231, LCD, and one LED. Every minute, the temperature, humidity, and time is requested from Slave devices and values are displayed on LCD screen. When the temperature is above the threshold, turn on the LED.

3. Draw a flowchart of `TIMER1_OVF_vect` (which overflows every 1&nbsp;sec) for such Meteo station. The image can be drawn on a computer or by hand. Use clear description of individual algorithm steps.

   ![your figure](https://github.com/xsedla1y/digital-electronics-2/blob/main/07-i2c/07-photos/lab7_diagram.png)
