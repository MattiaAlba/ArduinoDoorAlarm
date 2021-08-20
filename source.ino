/*RC522           UNO / MEGA            LCD                               Other

  SDA             D10   D53             A - VDD         +5V               Buzzer        10
  SCK             D13   D52             Vo              Trimmer           Panic Button  21
  MOSI            D11   D51             RS              6                 alarm button 9
  MISO            D12   D50             E               7
  IRQ             N/A   N/A             D4              5
  GND             GND   GND             D5              4
  RST             D9    D49             D6              3
  3.3V            3.3V  3.3V            D7              2
                                        VSS - K - R/W   GND
*/

//The three codes predefined (dad, son and mother) are placeholder for demostrating purpouses.
//You have to scan your own cards and hardcode them

#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal.h>
#define SDA_DIO 53
#define RESET_DIO 49
#define delayRead 1000    //Default delay for reading a card

RFID RC522(SDA_DIO, RESET_DIO);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int red_led = 26;
int green_led = 22;
int yellow_led = 24;
int bt = 9;

String dad = "21F212B24";        
String son = "9747B25CD";        
String mother = "A6E36985A9";    

void setup() {
  Serial.begin(9600);
  Serial.println("System startup");
  lcd.begin(16, 2); //Initialize LCD 16x2
  SPI.begin();
  RC522.init();

  //Defining hardware
  pinMode (red_led, OUTPUT);
  pinMode (green_led, OUTPUT);
  pinMode (yellow_led, OUTPUT);
  pinMode (bt, INPUT);
  digitalWrite (red_led, LOW);
  digitalWrite (green_led, LOW);
  digitalWrite (yellow_led, LOW);
  attachInterrupt(2, Interrupt, RISING);
  Serial.println("System ready!");
  Serial.println();
  ftext();
}

void loop() {
  boolean btstate = digitalRead(bt);
  byte i;
  //RFID reading
  if (RC522.isCard())                 //Check card presence
  {
    digitalWrite(yellow_led, LOW);    
    RC522.readCardSerial();           //Read card code
    String readCode = "";
    Serial.print("ID code: ");
    for (i = 0; i <= 4; i++)          //Concatenate read code into readCode Strings (repeat 4 times)
    {
      readCode += String (RC522.serNum[i], HEX);
      readCode.toUpperCase();
    }
    if (checkCode(readCode));        //Compare card code with predefined codes
    
    delay(delayRead);
    Serial.println();
    Serial.println();
  }
  else      //If card is not present
  {
    digitalWrite(yellow_led, HIGH);
    if (btstate == 0) {   //If card is not present and door has been open, activate the alarm
      Serial.println("alarm");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("alarm");
      alarm();
    }
  }
  noTone(10);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void ftext() {  //Print "Ready to scan" message
  lcd.clear();
  printText("", "Ready to scan...");
}

printText(String top, String bottom, String private){ //This function takes one string for the TOP row of the LCD,
  lcd.clear();                                        //one for the bottom row, and a third one for text
  lcd.setCursor(0,0);                                 //that must be displayed only on the serial monitor
  lcd.print(top.toUpperCase());   
  Serial.println(top);
  
  lcd.setCursor(0, 1);
  lcd.print(bottom.toUpperCase());

  Serial.println(top + " " + bottom);
  Serial.println(private);
}

int sound1() {              //Correct code sound
  tone (10, 1850, 400);
}

int sound2() {             //Wrong code sound
  tone(10, 800, 1000);
}

int alarm() {            //alarm 
  int counter = 0;
  for (counter == 0; counter < 10; counter++) {      //Play alarm for 4 seconds
    tone(10, 1800);
    delay(200);
    tone(10, 1000);
    delay(200);
  }
  ftext();  
}

void Interrupt() {                    //Disable alarm Interrupt (Outgoing)
  printText("Alarm", "deactivated");
  delay(600000);
  printText("Alarm", "activated");
  delay(300000);
  Serial.println();
  ftext();
}

void disablealarm() {                 //Disable alarm Interrupt (Incoming)
  printText("Alarm", "deactivated");
  delay(6000);
  printText("Alarm", "activated");
  delay(4000);
  digitalWrite(green_led, LOW);
  ftext();
}

boolean checkCode(String readCode) {        //Codes comparison
  if (readCode.equals(dad)) {         //confronto padre
    printText("ID: Father", "", readCode);
    sound1();
    digitalWrite(green_led, HIGH);
    delay(1400);
    disableallarm();
    return true;
  }
  if (readCode.equals(son)) {         //confronto figlio
    printText("ID: Son", "", readCode);
    sound1();
    digitalWrite(green_led, HIGH);
    delay(1400);
    disableallarm();
    return true;
  }
  if (readCode.equals(mother)) {         //confronto madre
    printText("ID: Mother", "", readCode);
    sound1();
    digitalWrite(green_led, HIGH);
    delay(1400);
    disableallarm();
    return true;
  }
  else {                             //nessun riscontro
    printText("ID: Unknown", "", readCode);
    sound2();
    digitalWrite(red_led, HIGH);
    delay(1200);
    digitalWrite(red_led, LOW);
    printText("Unknown ID", "Please try again", "");
    ftext();
    return false;
  }
}
