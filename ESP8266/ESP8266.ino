#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// WiFi credentials
const char* ssid = "Redmi A1";
const char* password = "maecy123";

// ThingSpeak channel details
unsigned long channelId = 2054224;
const char* apiKey = "BNR3SM6WI0Z25UQ1";

// Initialize WiFi client and ThingSpeak client
WiFiClient client;

String receivedValue;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Con nected to WiFi!");

  // Initialize ThingSpeak client
  ThingSpeak.begin(client);
}

void loop() {

  if (Serial.available()) {
      receivedValue = Serial.readStringUntil('$');

      Serial.println(receivedValue);

      int index = receivedValue.indexOf('%');
      String temperature = receivedValue.substring(0, index);
      
      String removeTemperature = removeWord(receivedValue, temperature);
      String removeFirstCharacter = removeTemperature.substring(1);
      int statusInsideIndex = removeFirstCharacter.indexOf('%');
      String statusInside = removeFirstCharacter.substring(0, statusInsideIndex);
      
      String removeStatusInside = removeWord(removeFirstCharacter, statusInside);
      String cm = removeStatusInside.substring(1);
      
      String all = temperature + ", " + statusInside + ", " + cm;

      Serial.println(temperature);
      Serial.println(statusInside);
      Serial.println(cm);
      Serial.println(all);


       // set the fields with the values
      ThingSpeak.setField(1, temperature);
      ThingSpeak.setField(2, statusInside);
      ThingSpeak.setField(3, cm);
      
      // write to the ThingSpeak channel
      int response = ThingSpeak.writeFields(channelId, apiKey);
  
      if(response == 200){
        Serial.println("Channel update successful.");
      }
      else{
        Serial.println("Problem updating channel. HTTP error code " + String(response));
      }

     delay(15000);
     Serial1.println("1");

      // Wait 15 seconds before sending the next set of values
  }
 
}


String removeWord(String str, String word) {
  int index = str.indexOf(word);
  if (index == -1) return str;
  int len = word.length();
  return str.substring(0, index) + str.substring(index+len);
}
