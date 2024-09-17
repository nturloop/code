  #include "WiFi.h"
#include "PubSubClient.h"
#include  "Wire.h"
#include <Adafruit_MCP3008.h>


/// WiFi 
const char *ssid = "Volta ouh"; // Entrez votre SSID WiFi  
const char *password = "Allo1111"; // Entrez votre mot de passe WiFi 
  
/// MQTT Broker 
const char *mqtt_broker = "192.168.0.145"; 
//const char *mqtt_broker ="mqtt.thingspeak.com"
//publish
const char *CourantINtopic = "ESP32/MPPT/CourantIN";
const char *CourantOUTtopic = "ESP32/MPPT/CourantOUT";
const char *TensionINtopic = "ESP32/MPPT/TensionIN";
const char *TensionOUTtopic = "ESP32/MPPT/TensionOUT";
const char *DutyCycleVAlueTopic = "ESP32/MPPT/DutyCyleValue";
//subscribe
const char *DutyCycleCTRTopic = "ESP32/MPPT/DutyCyleCTR";
const char *OCPINTopic = "ESP32/MPPT/OCPINT_State";
const char *BackFlowTopic ="ESP32/MPPT/BackFlow";
const char *PinSubTopic = "ESP32/MPPT/PinSub";
const char *CCProtTopic = "ESP32/MPPT/CCProt";
const char *IR2184Topic = "ESP32/MPPT/IR2184";

const char *mqtt_username = "AigDHBQpDzYqLgItKQYvFjo"; 
const char *mqtt_password = "C5UA2b7l+JudOXg9RkCAJau1"; 
const int mqtt_port = 1883; 

/// varible server et MQTT
WiFiClient espClient; 
PubSubClient client(espClient); 

/// Variable MCP3008
Adafruit_MCP3008 adc;

///assignation constante Pin de controle
const int backFlowPIN = 32;
const int OCPIN = 13;
const int PinSub = 21;
const int CCProtPin = 33;

///assignation constante Pin IR2184
const int PWMPIN = 27;
const int PINSD = 14;

///Constante IR2184
const int freq = 50000;
const int ledChannel = 0;
const int resolution = 8;
float dutyCycle = 50;

/// Assignation Pin analogique
const int TensionINPIN = 0;
const int CourantINPIN = 2;
//const int potPin = 4;
const int TensionOUTPIN = 1;
const int CourantOutPin = 3;

///VAriable analogique
//int potValue = 0;
float TensionIN = 0;
float TensionOut = 0;

int CourantIn = 0;
int courantOUT = 0;


//moyennage des donnée d'entrée
const int numlecture = 10;
int TensionTAB[numlecture];
int TensionOUTTAB[numlecture];
int CourantOUTTAB[numlecture];
int CourantINTAB[numlecture];
int lectureIndex = 0;          // the index of the current reading
int TotalTension = 0;              // the running total
int moyTension = 0; 

unsigned long previousMillis = 0;
unsigned long interval = 1500;

///initiation WIFI
void initWifi()
{
    WiFi.begin(ssid, password); 
        while (WiFi.status() != WL_CONNECTED) { 
             delay(500); 
             Serial.println("Connecting to WiFi.."); 
  } 
  Serial.println("Connected to the Wi-Fi network");       
}

///initation MQTT
void initMQTT()
{
    client.setServer(mqtt_broker, mqtt_port); 

 while (!client.connected()) { 
    String client_id = "esp32-client-"; 
    client_id += String(WiFi.macAddress()); 
    Serial.printf("The client %s connects to the public MQTT brokern", client_id.c_str()); 
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) { 
        client.subscribe(DutyCycleCTRTopic);
        client.subscribe(PinSubTopic);
        client.subscribe(OCPINTopic);
        client.subscribe(BackFlowTopic);
        client.subscribe(CCProtTopic);
        Serial.println("Public EMQX MQTT broker connected"); 
    } else { 
            Serial.print("failed with state "); 
            Serial.print(client.state()); 
            delay(2000); 
        } 
      
    } 
}


  void PublishMQTT()
  {
    if(!client.connected()){
      client.disconnect();
      String client_id = "esp32-client-"; 
      client_id += String(WiFi.macAddress()); 
      Serial.printf("The client %s connects to the public MQTT brokern", client_id.c_str()); 
      client.connect(client_id.c_str(), mqtt_username, mqtt_password);
        Serial.println("Public EMQX MQTT broker connected"); 
        } 
    
 // Publish et subscribe 
  client.publish(CourantINtopic, String(CourantIn).c_str()); 
  client.publish(CourantOUTtopic, String(courantOUT).c_str());
  client.publish(TensionINtopic, String(TensionIN).c_str());
  client.publish(TensionOUTtopic, String(TensionOut).c_str());
  client.publish(DutyCycleVAlueTopic, String(dutyCycle).c_str());
      Serial.println("publish");
  }

 
  float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

float MoyenneAnalogue(int TableauValeur[],int DIMtableau, int indexAnalogue, int ValeurLu)
{
  unsigned int index;
  long total;

  total = 0;
  for(index=0; index < DIMtableau;index++)
    {total+=TableauValeur[index];}

 total = total - TableauValeur[indexAnalogue];
 TableauValeur[indexAnalogue] = ValeurLu;
 total = total + ValeurLu;


return total/DIMtableau;
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

 if (String(topic) == String(DutyCycleCTRTopic)) {
    dutyCycle = constrain(String(messageTemp).toInt(),0,250);
    Serial.println("DutyCyle controle a : "+messageTemp);}

  if ((String(topic) == String(OCPINTopic))){
    if(String(messageTemp).toInt() != digitalRead(OCPIN)){
    Serial.println("OCPIN toggle");
    digitalWrite(OCPIN, !digitalRead(OCPIN));}}
  
  if ((String(topic) == String(BackFlowTopic))){
    if(String(messageTemp).toInt() != digitalRead(backFlowPIN)){
    Serial.println("backflowPIN toggle");
    digitalWrite(backFlowPIN, !digitalRead(backFlowPIN));}}

  if ((String(topic) == String(PinSubTopic))){
    if(String(messageTemp).toInt() != digitalRead(PinSub)){
      Serial.println("PinSub toggle");
      digitalWrite(PinSub, !digitalRead(PinSub));}}
  
  if ((String(topic) == String(CCProtTopic))){
    if(String(messageTemp).toInt() != digitalRead(CCProtPin)){
      Serial.println("CCProtPin toggle");
      digitalWrite(CCProtPin, !digitalRead(CCProtPin));}}

      
  if ((String(topic) == String(CCProtTopic))){
    if(String(messageTemp).toInt() != digitalRead(PINSD)){
      Serial.println("IR2184 shutdown toggle");
      digitalWrite(PINSD, !digitalRead(PINSD));}}
  
}

void setup() { 
 //Mise de la vitesse de transmission à 115200; 
    Serial.begin(9600); 
 // Connecting to a Wi-Fi network 
    initWifi();

  //connexion au broker MQTT  
    initMQTT();
    client.setCallback(callback);
//initiation Pin PWM
  ledcAttach(PWMPIN,freq, resolution);

  // Initiation Pin IR2184
  pinMode(PINSD, OUTPUT);
  digitalWrite(PINSD, LOW);

//setup des pin de controle
 pinMode(backFlowPIN, OUTPUT);
  pinMode(OCPIN, OUTPUT);
  pinMode(PinSub, OUTPUT);
  pinMode(CCProtPin, OUTPUT);
  digitalWrite(backFlowPIN,LOW);
  digitalWrite(OCPIN,LOW);
  digitalWrite(PinSub,LOW);
  digitalWrite(CCProtPin,LOW);


adc.begin();
  Serial.println("Setup du esp32");
  delay(1000);

  
  } 

void loop() { 
  //Serial.println(ESP.getFreeHeap());
  client.loop(); 
//Lecture TEnsion d'entrée
  int TensionRAW = adc.readADC(TensionINPIN);
  float TensionMOY = MoyenneAnalogue(TensionTAB,sizeof(TensionTAB) / sizeof(TensionTAB[0]),lectureIndex,TensionRAW); // filtre sur les 10 derniere data 
  float TensionMAP = TensionMOY/1024*3.3; //mapping sur le 3.3volt
  TensionIN = (TensionMAP*(220000+5100)/5100);

 //Lecture Tension Sortie
  int TensionSortie = adc.readADC(TensionOUTPIN);
  float TensionOUTMOY = MoyenneAnalogue(TensionOUTTAB,sizeof(TensionOUTTAB) / sizeof(TensionOUTTAB[0]),lectureIndex,TensionSortie);
  float TensionOUTMAP = TensionOUTMOY/1024*3.3;
  TensionOut = (TensionOUTMAP*(23500+1000)/1000);

//Lecture Courant d'entrée
  int CourantRaw = adc.readADC(CourantINPIN);
  float CourantInMoy = MoyenneAnalogue(CourantINTAB,sizeof(CourantINTAB) / sizeof(CourantINTAB[0]),lectureIndex,CourantRaw);
  CourantIn = CourantInMoy/1023*3.3*((1000+3300)/3300);

//Lecture Courant de Sortie
int courantOutRaw = adc.readADC(CourantOutPin);
float courantOUTMOY = MoyenneAnalogue(CourantOUTTAB,sizeof(CourantOUTTAB) / sizeof(CourantOUTTAB[0]),lectureIndex,courantOutRaw);
float CourantOUTMAP = (courantOUTMOY/1024*3.3);
float courantINter = CourantOUTMAP * (4878+8960)/8960;
courantOUT = (CourantOUTMAP-1.6)/0.185;



  //potValue = adc.readADC(potPin);
  //dutyCycle = mapfloat(potValue,0,1023,0,250);

  ledcWrite(PWMPIN, dutyCycle);

 lectureIndex = lectureIndex + 1;
  if(lectureIndex >= numlecture) {
    lectureIndex = 0;
  }

 
 Serial.print("rapport : ");Serial.println(dutyCycle);
  Serial.print("TensionEntree : "); Serial.println(TensionIN);
  Serial.print("TensionSortie : "); Serial.println(TensionOut);
  Serial.print("CourantEntree : "); Serial.println(CourantIn);
  Serial.print("COURANT SORTIE  : "); Serial.println(courantOUT);
  
  
  PublishMQTT();

  delay(500);
  
	//if (Serial.available()) {
	//	String temp = Serial.readString();
   // temp.trim();
		//Serial.println(temp);
    //if(temp.equals("OCPIN")){
      
    unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
    
  } 

 

