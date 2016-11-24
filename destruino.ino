#include <Servo.h>


#define pinSensorFrente 0
#define pinSensorTras 1
#define pinBotaoInicio 3
#define pinServoDireito 6
#define pinServoEsquerdo 5
#define pinPIR 7
#define pinTrig 12 // verde
#define pinEcho 13 // azul

#define CM 1
#define INC 0

#define PRETO 600
#define BRANCO 100
#define printValuesEnabled 1

#define DIR_FRENTE 'w'
#define DIR_TORETTO 't'
#define DIR_TRAS 's'
#define DIR_ROSCA 'r'
#define DIR_ESQUERDA 'a'
#define DIR_DIREITA 'd'
#define DIR_PARAR 'q'


Servo servoD;
Servo servoE;

boolean iniciado = false;
boolean sumoIniciado = false;
int sensorT = 0;
int sensorF = 0;
long duration, distanciaCm, distanciaInc;
int PIR;
boolean direcaoParaOndeIr = false;

void setup() {
  Serial.begin(9600);
  
  servoD.attach(pinServoDireito);
  servoE.attach(pinServoEsquerdo);
  servoD.write(90);
  servoE.write(90);

  pinMode(pinBotaoInicio, INPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(pinPIR, INPUT);

  delay(3000);
}

void iniciaSumo() 
{
  delay(2000);
  Serial.println("Iniciando sumo...");
  writeServo(80, 80);
  delay(1000);
    
  direcao(DIR_FRENTE);
  delay(2000);
  direcao(DIR_PARAR);
  delay(1000);
  writeServo(100, 100);
  Serial.println("Terminado inicio de sumo...");

  iniciado = false;
  sumoIniciado = true;
}

void loop() 
{
  verificaBotaoInicio();
  
  if (iniciado) 
  {
    iniciaSumo();
  } 
  else if (sumoIniciado) 
  {
    lerSensorLinha();
    lerUltrasom();
    lerPIR();
  
    if (sensorT > PRETO)
    {
      direcao(DIR_FRENTE);
      delay(1000);
    } 
    else if (sensorF > PRETO) 
    {
      direcao(DIR_TRAS);
      delay(1000);
      direcao(DIR_DIREITA);
      delay(6000);
    }
    else if (distanciaCm < 30) 
    {
      direcao(DIR_TORETTO);
      
    } 
    else if (PIR > 0) 
    {
      direcao(DIR_ROSCA);
    } 
    else 
    {
      direcaoParaOndeIr = !direcaoParaOndeIr;
      if (direcaoParaOndeIr)
      {
        direcao(DIR_DIREITA);
      }
      else
      {
        direcao(DIR_ESQUERDA);
      }

      delay(100);
    }
  } 
  else
  {
    direcao(DIR_PARAR);
  }
  
  printValues();
}

void verificaBotaoInicio()
{
  int valBotao = digitalRead(pinBotaoInicio);
  if (valBotao == HIGH) {
    iniciado = false;
 
    digitalWrite(13, LOW);
  } else {
    iniciado = true;
    digitalWrite(13, HIGH); 
  }
}

void lerSensorLinha() 
{
  sensorT = analogRead(pinSensorTras);
  sensorF = analogRead(pinSensorFrente);  
}

int lerUltrasom() 
{
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);

  digitalWrite(pinTrig, LOW);

  int distancia = (ultrasonic_ranging(CM));

  return distancia;
}

void lerPIR() 
{
  PIR = digitalRead(pinPIR);
}

void direcao(char dir)
{

  switch(dir) 
  {
    case DIR_FRENTE: // para frente      
      writeServo(80, 100);
      Serial.println("Frente");
      break;

    case DIR_TORETTO: // para frente      
      writeServo(0, 180);
      Serial.println("TORETTO!!");
      break;
      
    case DIR_TRAS: // para trás
      writeServo(100, 80);
      Serial.println("Tras");
      break;

    case DIR_ROSCA: // para trás
      writeServo(180, 180);
      Serial.println("Tras");
      break;

    case DIR_DIREITA: // direita
      writeServo(90, 100);
      Serial.println("Direita");
      break;

    case DIR_ESQUERDA: // esquerda
      writeServo(80, 90);
      Serial.println("Esquerda");
      break;

    case DIR_PARAR:
      writeServo(90, 90);
      Serial.println("Parar");
      break;
  }
}

void writeServo(int D, int E)
{
  servoD.write(D);
  servoE.write(E);
}

void printValues()
{
  if (printValuesEnabled > 0) 
  {
    Serial.print("F ");
    Serial.print(sensorF);
    Serial.print("  ");
    Serial.print("T ");
    Serial.print(sensorT);
    Serial.print("  PIR ");
    Serial.print(PIR);
    Serial.print("  Dist  ");
    Serial.println(distanciaCm);
  }
}

long ultrasonic_timing() 
{
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  duration = pulseIn(pinEcho,HIGH);
  return duration;
}

long ultrasonic_ranging(int sys)
{
  ultrasonic_timing();
  
  distanciaCm = duration / 29 / 2 ;
  distanciaInc = duration / 74 / 2;
  if (sys)
  return distanciaCm;
  else
  return distanciaInc;
}