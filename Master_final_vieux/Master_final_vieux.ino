//YouTube: Aslam Hossain, https://www.youtube.com/channel/UCdaQfJEKvpoAPIzQNwFGlHw
//Facebook Page: https://www.facebook.com/aslamhossain3852
//For Any Help, Feel Free to knock me through what's App: 01877543825


#include<SoftwareSerial.h>

SoftwareSerial btSerial(10,11); // RX | TX

struct PacketData 
{
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

int value;

PacketData data;

void setup()
{
  btSerial.begin(38400); 
  Serial.begin(9600);  

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//CODE PRINCIPAL

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void loop()
{

  //Prise des données
  
  data.s1       = analogRead(A0);
  data.s2       = analogRead(A1);
  data.s3       = analogRead(A2);
  data.mot1     = map(analogRead(A3), 348, 614, 0, 1023);
  data.mot2     = map(analogRead(A4), 354, 625, 0, 1023);
  data.pince1   = digitalRead(2);
  data.pince2   = digitalRead(3);
  data.park     = digitalRead(4);
  data.vitesse  = digitalRead(5);
  data.ranger   = digitalRead(6);





  //Envoi des données


  String dataString;
  dataString = dataString 
               + data.s1 + "," 
               + data.s2 + ","  
               + data.s3 + ","  
               + data.mot1 + ","
               + data.mot2 + ","
               + data.pince1 + "," 
               + data.pince2 + ","  
               + data.park + ","  
               + data.vitesse + ","
               + data.ranger + "\n";
  
  btSerial.print(dataString);
  Serial.print(dataString);

  //DEBUG

  Serial.println("");Serial.println("");Serial.println("");
  Serial.println("REÇOIT");
  Serial.println("");

  Serial.print("Calibration mot1: "), Serial.print(analogRead(A3)); Serial.print("     Calibration mot2: "), Serial.println(analogRead(A4));

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

  delay(50);
}
