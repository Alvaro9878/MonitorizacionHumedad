/* 

 Envía un JSON con la información de los sensores a la pasarella ESP-NOW -> MQTT que está escuchando

*/
 
//Librerias
#include <ESP8266WiFi.h> 
#include <espnow.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads;
#include <Wire.h>


// GPIOs
#define LEDCheck 14
#define Sensor 12
#define LEDSalida 15
#define Sensor_2 13


//canal que se va a utilizar para el ESP-NOW
#define WIFI_CHANNEL 0
//2 segundos que va a estar enviando la placa información de que está viva
#define ALIVE_SECS 2  // segundos
//2 segundos máximo para dar TIME_OUT e irse a dormir
#define SEND_TIMEOUT 2000

//Claves que necesitamos para encriptar la comunicación por ESP-NOW
uint8_t kok[16]= {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t key[16]= {0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44, 0x33, 0x44};

//MAC de la ESP remota que recibe los mensajes de esta ESP mediante ESP-NOW. 08:3A:F2:A8:11:75

uint8_t macPasarela[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};
//MAC de la ESP que está en la pasarela y se encarga del ESP-NOW
uint8_t macDHT11[] = {0x3E, 0x33, 0x33, 0x33, 0x33, 0x34};
//Variable dónde se especifica el tiempo que se deja entre actualización y actualización
int frecuenciaActualizacion=30000;
//char para construir el mensaje que se va a enviar por ESP-NOW
char mensaje[512];
//Variables para controlar el tiempo.
unsigned long waitMs=0;
//Variable que se utilizará para controlar en tiempo cada envío de información.
int countTime = 250;
int countAliveTime = 0;
//Valor actual del LED de salida
int currentLEDValue = 1;
//Valor que llega por ESP-NOW del LED de salida
int espNowLEDValue = 0;


unsigned adc0,adc1,adc2,adc3;

volatile bool enviado = false;

void setup() {
	
	Serial.begin(115200); 
	Serial.println();
	
	//Inicializamos los dispositivos a través de GPIO
	pinMode(LEDCheck, OUTPUT);  
 	digitalWrite(LEDCheck, HIGH);
  	pinMode(Sensor, OUTPUT);  
  	digitalWrite(Sensor, HIGH);
  	pinMode(Sensor_2, OUTPUT);  
  	digitalWrite(Sensor_2, HIGH);

	
	//Put WiFi in AP mode.
	WiFi.mode(WIFI_AP);
  //Establecemos la MAC para esta ESP
  wifi_set_opmode(STATIONAP_MODE);
	wifi_set_macaddr(SOFTAP_IF, &macDHT11[0]);
	Serial.print("MAC: "); Serial.println(WiFi.softAPmacAddress());

  //Inicializamos ESP-NOW
	if (esp_now_init() != 0) {
		Serial.println("*** Fallo al iniciar el ESP-NOW");
	  ESP.restart();
	}

	//Especificamos las claves para enviar la información de manera encriptada.
  esp_now_set_kok(kok, 16);
  
	//Especificamos el rol del ESP-NOW (0=OCIOSO, 1=MAESTRO, 2=ESCLAVO y 3=MAESTRO+ESCLAVO)
	esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
	//Emparejamos con el otro ESP
	esp_now_add_peer(macPasarela, ESP_NOW_ROLE_CONTROLLER, WIFI_CHANNEL, key, 0);
	//Especificamos las claves para enviar la información de manera encriptada.
  esp_now_set_peer_key(macPasarela, key, 16);

  //Comprueba si un paquete de datos enviado ha sido recibido correctamente por un par
	esp_now_register_send_cb([](uint8_t* mac, uint8_t sendStatus) {
		Serial.printf("Mensaje enviado, estado (0=OK, 1=ERROR) = %d\n", sendStatus);
  
    ESP.deepSleep(10*60*1e+6);
    delay(100);
	});
  
	//Si llega un mensaje por ESP-NOW a esta ESP, cambiamos el valor del LED.
	esp_now_register_recv_cb(OnRecv);
 ads.setGain(GAIN_TWOTHIRDS);
 ads.setDataRate(7);
 ads.begin();
 Serial.println("setup done");
}

void loop() {
     
  
  //Un condicional para controlar cada cuanto tiempo se envía información del sensor.
  if(countTime<millis() && !enviado){

    //Obtenemos los datos del ADS1015 y apagamos los dispositivos
    

    soilMoistureGet();   
    digitalWrite(LEDCheck, LOW); 
    digitalWrite(Sensor, LOW);
    digitalWrite(Sensor_2, LOW);    
    //Creamos y mostramos el JSON que se va a enviar.

    
    sprintf(mensaje,"orchard/espnow/datos|{\"hum1\":%d,\"hum2\":%d,\"hum3\":%d,\"hum4\":%d}", adc0, adc1, adc2, adc3);
    Serial.printf("Mensaje: %s\n",mensaje);
    
    //Enviamos el JSON por ESP_NOW
    esp_now_send(macPasarela, (uint8_t *) mensaje, strlen(mensaje)+1);
    enviado = true;
    //Apagamos el LED

  }
  
}

// callback function executed when data is received
void OnRecv(uint8_t *mac_addr,  uint8_t *data, uint8_t data_len) {

  Serial.printf("\r\nReceived\t%d Bytes\t%d", data_len, data[0]);

}

void soilMoistureGet(){



  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

 
  Serial.print("\nAIN0: "); Serial.println(adc0);
  Serial.print("\nHUMEDAD0: "); Serial.println(soilMoisture);
  Serial.print("\nAIN1: "); Serial.println(adc1);
  Serial.print("\nHUMEDAD1: "); Serial.println(soilMoisture_1);
  Serial.print("\nAIN2: "); Serial.println(adc2);
  Serial.print("\nHUMEDAD2: "); Serial.println(soilMoisture_2);
  Serial.print("\nAIN3: "); Serial.println(adc3);
  Serial.print("\nHUMEDAD3: "); Serial.println(soilMoisture_3);
}
  
