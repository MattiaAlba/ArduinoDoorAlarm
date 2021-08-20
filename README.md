# ArduinoDoorAlarm
A very basic code for an alarm system for doors or windows based on Arduino, utilizing RFDI cards and a button.

For educational purpouses at beginner level.

Pinout:
RC522           UNO / MEGA            LCD                               Other

 SDA             D10   D53            A - VDD         +5V               Buzzer        10
 SCK             D13   D52            Vo              Trimmer           
 MOSI            D11   D51            RS              6                 alarm button 9
 MISO            D12   D50            E               7
 IRQ             N/A   N/A            D4              5
 GND             GND   GND            D5              4
 RST             D9    D49            D6              3
 3.3V            3.3V  3.3V           D7              2
                                      VSS - K - R/W   GND
