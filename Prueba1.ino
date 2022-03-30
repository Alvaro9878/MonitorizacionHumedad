#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads;
#include <Wire.h>

unsigned long sPreviousMillis = 0;
unsigned long Intervalo = 1000;

float soilMoisture = 0;


void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  Serial.println("Hello!"); 
  ads.setGain(GAIN_TWOTHIRDS);
  ads.begin();
  delay(100);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  
  
  if((currentMillis - sPreviousMillis) >= (Intervalo)){
    sPreviousMillis = currentMillis;
    
    soilMoistureGet();
  }
}

void soilMoistureGet(){

  const int ValorAire = 835;
  const int ValorAgua = 405;

  int adc0;
  adc0 = ads.readADC_SingleEnded(0);

  if(adc0 >= ValorAgua && adc0 <= ValorAire)
  {
    soilMoisture = map(adc0,ValorAire,ValorAgua,0,100);
  }
  else{
    if(adc0 > ValorAire)
      soilMoisture = 0;
    else if(adc0 < ValorAgua)
      soilMoisture = 100;
  }
  Serial.print("\nAIN0: "); Serial.println(adc0);
  Serial.print("HUMEDAD: "); Serial.println(soilMoisture);
}
  
