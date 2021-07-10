#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <LoRa.h>


const char* ssid = "imfaw";
const char* password = "11111111";

#define ss 15  // ตั้งค่า GOIO15
#define rst 16  // ตั้งค่า  GPIO16
#define dio0 4  // D2 ตั้งค่า GPIO4

const char * headerKeys[] = {"Set-Cookie"} ;
const size_t numberOfHeaders = 1;

String sessionid;
String login()
{
  String temp;
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    WiFiClient client;
    HTTPClient http;
 int httpCode = -1;
 while(httpCode != 200)
 {
    // Your Domain name with URL path or IP address with path
    http.begin(client, "http://185.211.58.230/api/login/");
   


    http.collectHeaders(headerKeys, numberOfHeaders);
    
    
  
    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST("{\"username\":\"pedram\",\"password\":\"pedram\"}");
    Serial.println(httpCode);
      if (httpCode > 0) {

      temp = http.header("Set-Cookie");
      Serial.println(temp);

      Serial.println("--------------------");

    } else {
      Serial.println("An error occurred sending the request");
    }

    http.end();
    return temp;

  }
  }

}
void setup () {

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.println("Connecting..");

  }

  Serial.println("Connected to WiFi Network");
  
    Serial.println("LoRa Receiver Callback");
 
  LoRa.setPins(ss, rst, dio0);
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
    // register the receive callback
  LoRa.onReceive(onReceive);
 
  // put the radio into receive mode
  LoRa.receive();
  
  sessionid = login();
}

void loop() {

}

void onReceive(int packetSize) {
  // received a packet
  Serial.println("Received packet '");
  string packet[5] = "";
 
  // read packet
  int j = 0;
  for (int i = 0; i < packetSize; i++) {
    char a = (char)LoRa.read();
    if (a != ',')
    packet[j] += a;
    else
    j++;
	  }
 

	  Serial.println(packet[0]);

	    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, "http://185.211.58.230/api/bulk_create_log/");




    http.addHeader("Content-Type", "application/json");
    http.addHeader("Cookie", sessionid);
    
    

    char req[150] ;
       
    sprintf(req,"{\"data\":[{\"emergency_alert\": %s,\"heartbeat\":%s,\"lat\":%s,\"lang\":%s}], \"band_id\":%s}",packet[3],packet[0],packet[1],packet[4]);
    Serial.println(req);


    int httpCode = http.POST(req);
    Serial.println("--------------------");
    Serial.println(httpCode);
    Serial.println(http.getString());
    Serial.println("--------------------");


    http.end();

  }

 


}
