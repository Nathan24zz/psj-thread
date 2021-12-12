#include <Wire.h>
#include <ESP8266WiFi.h>

#define TAMPIL_SERIAL true
#define PIN_SENSOR A0

int DSPIN = D5;

int sensorValue = 0;
float tdsValue = 0;
float voltage = 0;

void setup() {

  Serial.begin(9600);
}

void temperature(){
  double waterTemp = TempRead();
  waterTemp = waterTemp*0.0625;

  Serial.print(waterTemp,2); Serial.print(F("C "));
}

void tds(){
  sensorValue = analogRead(PIN_SENSOR);
  voltage = sensorValue * 5 / 1024.0; // Mengkonversi nilai analog ke voltage
  
  // Mengkonversi nilai voltage ke nilai TDS
  tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5; 

  Serial.print(tdsValue); Serial.println(F("ppm"));
}

void loop() {

  temperature();
  tds();
  Serial.print(F(""));

  delay(1000);
}

boolean DS18B20_Init()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(DSPIN, LOW);
  delayMicroseconds(750);//480-960
  digitalWrite(DSPIN, HIGH);
  pinMode(DSPIN, INPUT);
  int t = 0;
  while(digitalRead(DSPIN))
  {
    t++;
    if(t > 60) return false;
    delayMicroseconds(1);
  }
  t = 480 - t;
  pinMode(DSPIN, OUTPUT);
  delayMicroseconds(t);
  digitalWrite(DSPIN, HIGH);
  return true;
}
 
void DS18B20_Write(byte data)
{
  pinMode(DSPIN, OUTPUT);
  for(int i=0; i<8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(10);
    if(data & 1) digitalWrite(DSPIN, HIGH);
    else digitalWrite(DSPIN, LOW);
    data >>= 1;
    delayMicroseconds(50);
    digitalWrite(DSPIN, HIGH);
  }
}
 
byte DS18B20_Read()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(2);
  byte data = 0;
  for(int i=0; i<8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);
    delayMicroseconds(5);
    data >>= 1;
    if(digitalRead(DSPIN)) data |= 0x80;
    delayMicroseconds(55);
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
  }
  return data;
}
 
int TempRead()
{
  if(!DS18B20_Init()) return 0;
   DS18B20_Write (0xCC); // Send skip ROM command
   DS18B20_Write (0x44); // Send reading start conversion command
  if(!DS18B20_Init()) return 0;
   DS18B20_Write (0xCC); // Send skip ROM command
   DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
   int waterTemp = DS18B20_Read (); // Low byte
   waterTemp |= DS18B20_Read () << 8; // High byte
  return waterTemp;
}
