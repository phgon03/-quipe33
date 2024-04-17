//SET UP SERVOS

#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo1;
Servo servo2;
Servo servo3;

//SET UP COMMUNICATION

//SoftwareSerial btSerial(12, 13); // RX | TX
SoftwareSerial btSerial =  SoftwareSerial(12, 13);

struct PacketData {
  int s1;
  int s2;
  int s3;
  int mot1;
  int mot2;
  byte pince1;
  byte pince2;
  byte park;
  byte vitesse;
  byte ranger;
};

PacketData data;

unsigned long now = 0;
unsigned long lastRecvTime = 0;


//ATTRIBUER DES VARIABLES AUX PINS

const int PinS1 = A0; //servomoteurs
const int PinS2 = A1;
const int PinS3 = A2;


#define in1 8 //Moteur gauche
#define in2 7
#define enA 9

#define in3 4 //Moteur droit
#define in4 5
#define enB 3

const int pin_pince2 = A5; //moteur de pince
const int pin_pince1 = A4;

int vitesse_max = 255;
int pwmOutput = 0;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//ATTRIBUER UN MODE AU PINS, SET UP LA RADIO ET LE DEBUG

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void setup() {
  servo1.attach(PinS1);
  servo2.attach(PinS2);
  servo3.attach(PinS3);

  pinMode(in1, OUTPUT); //Moteur gauche
  pinMode(in2, OUTPUT); 
  pinMode(enA, OUTPUT);

  pinMode(in3, OUTPUT); //Moteur droit
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(pin_pince1, OUTPUT);
  pinMode(pin_pince2, OUTPUT);  

  pinMode(12, INPUT);
  pinMode(13, OUTPUT);


  Serial.begin(9600);
  btSerial.begin(38400);

  pinMode(10,OUTPUT);

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//CODE PRINCIPAL

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void loop() {

  //Recherche des valeurs

  String dataString;
  if (btSerial.available()) {
    
    //Extraction des valeurs

    dataString = btSerial.readStringUntil('\n');
    sscanf(dataString.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &data.s1, &data.s2, &data.s3, &data.mot1, &data.mot2, &data.pince1, &data.pince2, &data.park, &data.vitesse, &data.ranger);

    //DEBUG
/*
    Serial.println("");Serial.println("");Serial.println("");
    Serial.println("REÇOIT");
    Serial.println("");

    Serial.print("Servo 1 : ");   Serial.println(data.s1);
    Serial.print("Servo 2 : ");   Serial.println(data.s2);
    Serial.print("Servo 3 : ");   Serial.println(data.s3);

    Serial.print("Moteur 1 : ");   Serial.println(data.mot1);
    Serial.print("Moteur 2 : ");   Serial.println(data.mot2);

    Serial.print("Pince 1 : ");   Serial.println(data.pince1);
    Serial.print("Pince 2 : ");   Serial.println(data.pince2);

    Serial.print("park : ");   Serial.println(data.park);
    Serial.print("vitesse : ");   Serial.println(data.vitesse);
    Serial.print("ranger : ");   Serial.println(data.ranger);

    */

      //Établir les conditions

    if (data.park) {
      data.mot1 = 1024/2;
      data.mot2 = 1024/2;
    }

    if (data.vitesse) {
      vitesse_max = 255*0.4;
    }
    else {
      vitesse_max = 230;
    }


    //Controle servomoteurs
  
    int angle_s1 = map(data.s1, 0, 1024, 0, 270);
    servo1.write(angle_s1);

    int angle_s2 = map(data.s2, 1024, 0, 44, 90);
    servo2.write(angle_s2);
    //Serial.println(angle_s2);
  
    int angle_s3 = map(data.s3, 0, 1024, 0, 180);
    servo3.write(angle_s3);

    //Controle moteurs
    avancer_reculer(in1, in2, enA, data.mot2); //Fonction pour contrôler la roue gauche
    avancer_reculer(in3, in4, enB, data.mot1); //Fonction pour contrôler la roue droite
    
    if (data.pince1==1){
      digitalWrite(pin_pince1, LOW);
      digitalWrite(pin_pince2, HIGH);
      //Serial.println("if");
    } 

    else if (data.pince2==1){
      digitalWrite(pin_pince1, HIGH);
      digitalWrite(pin_pince2, LOW);
      //Serial.println("else if");
      } 

    else{
      digitalWrite(pin_pince1, LOW);
      digitalWrite(pin_pince2, LOW);
      //Serial.println("else");
    }
    
    lastRecvTime = millis();

    }


    else {

      now = millis();

      if (now - lastRecvTime > 1000) {

      // Lorsque le signal a été perdu pendant au moins 1 seconde, on coupe les moteurs

        avancer_reculer(in1, in2, enA, 1024/2); //Fonction pour contrôler la roue gauche
        avancer_reculer(in3, in4, enB, 1024/2); //Fonction pour contrôler la roue droite
      }
    }





  delay(50);

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//FONCTIONS

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int avancer_reculer(int inx,  int iny, int enX, int potValue)

{
  

  if (potValue < 1023*0.45) // Si le potentiomètre est moins tourné que la moitié (avec une deadzone de 1/11)
  {
    pwmOutput = map(potValue, 0, 1023*0.45, vitesse_max , 40); // Map la valeur du potentiometre (limité par la manette) de 40 à 255 (vitesse initiale à 40)
    digitalWrite(inx, LOW);
    digitalWrite(iny, HIGH);
    analogWrite(enX, pwmOutput);
  
  }

  else if (potValue > 1023*0.55) // Si le potentiomètre est plus tourné que la moitié (avec une deadzone de 1/11)
  {
    pwmOutput = map(potValue, 1023*0.55, 1023, 40 , vitesse_max); // Map la valeur du potentiometre (limité par la manette) de 40 à 255 (vitesse initiale à 40)
    digitalWrite(inx, HIGH);
    digitalWrite(iny, LOW);
    analogWrite(enX, pwmOutput); 
    
  }

  else
  {
    pwmOutput = 0; //Arrêt au centre des joysticks
    digitalWrite(inx, LOW);
    digitalWrite(iny, LOW);
    analogWrite(enX, pwmOutput); 

    //Serial.print("Immobile    ");
   
  }
  //Serial.println(pwmOutput);




}