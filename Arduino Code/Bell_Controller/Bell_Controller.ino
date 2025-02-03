
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h> 

Servo bellServo;
const int bellServoPin = 5;
const int bellServoON = 180; 
const int bellServoOFF = 40; // Zero position

byte mac[] = { 0x14, 0x5E, 0x12, 0x10, 0xD5, 0xE2 };

IPAddress ip(192,168,3,111);

EthernetServer server(80);

String readString;

bool shouldPlay;

void setup() {
    bellServo.attach(bellServoPin);
    bellServo.write(bellServoOFF);
    delay(500);
    bellServo.detach();

    Ethernet.begin(mac, ip);
    server.begin();
}

void loop() {
  listenForClients();
  playIfNeeded();
}

void listenForClients() {
    readString = "";
    EthernetClient client = server.available();
    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (readString.length() < 100) {
                    readString += c; 
                } 
        
                if (c == '\n' && currentLineIsBlank) {
                  if (checkAPICommand(readString, "play")) {// lights off
                      shouldPlay = true;
                  }

                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: close");
                  client.println();
                  client.println("<!DOCTYPE HTML>");
                  client.println("<html>");
                  client.println("ok");
                  client.println("</html>");

                  break;
              }
              if (c == '\n') {
                  currentLineIsBlank = true;
              } else if (c != '\r') {
                  currentLineIsBlank = false;
              }
          }
      }
      delay(1);
      client.stop();
    }
}

void playIfNeeded() {
     if (shouldPlay) {
        shouldPlay = false;
        bellServo.attach(bellServoPin);
        delay(10); 
        bellServo.write(bellServoON); 
        delay(100);  
        bellServo.write(bellServoOFF); 
        delay(500);  
        bellServo.write(bellServoON); 
        delay(100);  
        bellServo.write(bellServoOFF); 
        delay(100);  
        bellServo.detach();
     }
}

// MARK : - Helper Functions

bool checkAPICommand(String URLString, String command){
    return (URLString.indexOf(command) > 0);
}

String valToAPI(String valName, String val){
    return "<" + valName + ">" + val + "</" + valName + ">";
}
