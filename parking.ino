#include <TM1637Display.h> //Bibliothèque de l'afficheur 4 digits 
#include <Servo.h> //Bibliothèque de servo moteur
#include <SPI.h>
#include <MFRC522.h>
/**
 * Typical pin layout used:
 * ------------------------
 *             Reader/PCD   Uno/101       
 * Signal      Pin          Pin          
 * ------------------------------------
 * RST/Reset   RST          9          
 * SPI SS      SDA(SS)      10          
 * SPI MOSI    MOSI         11 / ICSP-4   
 * SPI MISO    MISO         12 / ICSP-1
 * SPI SCK     SCK          13 / ICSP-3 
 * 
 */


// déclaration
#define RST_PIN         9        
#define SS_PIN          10      
boolean o;
String  tg [2] = {"18212638126","132158111167"};
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
 const byte ledPin1 = A4; // pin de Led1
 const byte ledPin2 = A5; // pin de Led2
 const byte CLK = A0;  // entre Clock de l'afficheur
 const byte DIO = A1; // entre Dio de l'afficheur
 const byte buttonPin = 3 ;    // le pin de la pushbottom numéro 1
 const byte buttonPin2 = 2;    // le pin de la pushbottom numéro 2
 const byte buttonPin3 = 7;   // le pin de la pushbottom numéro 3
 const byte buttonPin4 = 8;     // le pin de la pushbottom numéro 4
 const byte bar1 = 4 ;        // le pin de la première servo moteur de l'entré
 const byte bar2 = 5;         // le pin de la deuxième servo moteur du sortie
 int buttonState = 0;         // variable pour lire l'état du pushbutton 1 
 int buttonState2 = 0;       // variable pour lire l'état du pushbutton 2
 int buttonState3 = 0;         // variable pour lire l'état  du pushbutton 3
 int buttonState4 = 0;         // variable pour lire l'état du pushbutton 4
 int o1 = 0;         // boolean pour combiné avec la 1er bottom et la 2ème bottom
 int o3 = 0;          // booléan pour combiné avec la 1er bottom et la 2ème bottom       
 Servo servo1,servo2; //declaration des servo
 TM1637Display display(CLK, DIO); // declaration pour l'affichage
 int nmbr = 10;    // le nombre max des voitures 
 boolean rf;
 int sensorValue = 0;
 int sensorPin = A2;    // select the input pin for the potentiometer
 int Light = A3;
 
  void setup() {
       pinMode(Light,OUTPUT);
       pinMode(ledPin1,OUTPUT);            // Led1 Sortie
       pinMode(ledPin2,OUTPUT);            //Led2 sortie
       pinMode(buttonPin, INPUT);         //pushbutton1 Entree
       pinMode(buttonPin2, INPUT);         //pushbutton2 Entree
       pinMode(buttonPin3, INPUT);         //pushbutton3 Entree
       pinMode(buttonPin4, INPUT);        //pushbutton4 Entre
       servo1.attach(bar1);               // attacher le servo avec  L'Entre
       servo2.attach(bar2);               // attacher le servo avec L'Entre
       servo1.write(0);                  //initialisation du servo avec l'angle 0  (bar fermé)
       servo2.write(0);                 //initialisation du servo avec l'angle 0  (bar fermé)
       display.setBrightness(0x0a);     //définir la luménisité de l'afficheur
       display.showNumberDec(nmbr);       //affichage du nombre total des places dans l'afficheur
      SPI.begin();        // Init SPI bus
      mfrc522.PCD_Init(); // Init MFRC522 card
      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
      }
      dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
      rf = false;
  } 

  
boolean compare(String s)
{
  boolean b=false;
  for(int i=0; i<2;i++)
  {
    if (s==tg[i])  {b=true;  break;}
    
  } return b;
}
  
String dump_byte_array(byte *buffer, byte bufferSize) {
  String s = "";
    for (byte i = 0; i < bufferSize; i++) {
        s=s+buffer[i];   
    }
    return s;
}
  
  void loop() {
  sensorValue = analogRead(sensorPin);
  if (sensorValue < 50)
  {digitalWrite(Light, HIGH);}
  else {  digitalWrite(Light, LOW);}

    
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
String s=dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
if (compare(s)) {rf= true;}
  else { rf = false;}
 }
      
          // entre 1
      buttonState = digitalRead(buttonPin); // verification du L'état du capture de l'entree 

  if ((buttonState == HIGH) && (nmbr > 0) && !o1 && rf) {   // Si y'on a encore de la place et la verification est positive 
    servo1.write(90);       // ouverture de la barre 
    delay(1500);            // attendre 15 sec 
    o1 = !o1;               // o1 variable qui nous aide a controlé le pushbutton
   //  buttonState = LOW;
   digitalWrite(ledPin1,HIGH) ;
   rf = false;
  }

  // sortie 1
  buttonState2 = digitalRead(buttonPin2);  // verification du l'état du capture de la sortie
  if ((buttonState2 == HIGH) && o1) {    // si la verification est positive 
      servo1.write(0);                  // ouverture de la barre 
    delay(1500);                       // attendre 15 sec 
      nmbr --;
    display.showNumberDec(nmbr);     // 
    o1=!o1;
    digitalWrite(ledPin1,LOW); 
 //    buttonState2 =LOW;
  }

  // sortie 2
   buttonState3 = digitalRead(buttonPin3);    // verification 
 
  if ((buttonState3 == HIGH) && (nmbr < 10) && !o3) {
   
    servo2.write(90);
    delay(1500);
    o3=!o3;
    digitalWrite(ledPin2,HIGH) ;
    // buttonState3  = LOW;
  }

  // entre 2
  buttonState4 = digitalRead(buttonPin4);

  if ((buttonState4 == HIGH) && o3) {
      servo2.write(0);
    delay(1500);
      nmbr ++;
    display.showNumberDec(nmbr);
    o3=!o3;
     digitalWrite(ledPin2,HIGH);
 //    buttonState2 =LOW;
  }
}







