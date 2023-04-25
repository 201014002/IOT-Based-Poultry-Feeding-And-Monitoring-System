#include <ESP8266WiFi.h>
 
String apiKey = "16NOEI55OGTPOOFT";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "KHAN";  // name of your wifi
const char *pass =  "01061958";  // password of your wifi
const char* server = "api.thingspeak.com";
 
int pir_sensor =  D1; 
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       pinMode(pir_sensor,INPUT);
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
  
int pir = digitalRead(pir_sensor);


   if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
    {  
      
       String postStr = apiKey;
       postStr +="&field1=";
       postStr += String(pir);
       postStr += "\r\n\r\n";

       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);

       Serial.print("motion : ");
       Serial.print(pir);
       Serial.println(" , Send to Thingspeak.");
    }
   client.stop();
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
