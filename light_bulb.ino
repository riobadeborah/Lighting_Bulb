#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x48, 0xD3 };
IPAddress ip(169, 254, 28, 168);
EthernetServer server(80);
int led = 8;
String readString; // Add this line to declare readString

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  Ethernet.begin(mac, ip);
  server.begin();

  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println("LED Controller Test 1.0");
}

void loop() {
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("New client connected");
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Append the received character to readString
        readString += c;

        if (c == '\n') {
          // Check for the end of the HTTP request
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println();

          if (readString.indexOf("?lighton") > 0) {
            digitalWrite(led, HIGH);
            Serial.println("LED On");
          } else if (readString.indexOf("?lightoff") > 0) {
            digitalWrite(led, LOW);
            Serial.println("LED Off");
          }

          delay(1);
          client.stop();
          Serial.println("Client disconnected");
          readString = ""; // Clear readString for the next request
        }
      }
    }
  }
}
