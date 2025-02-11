#include <SPI.h>
#include <Ethernet.h>

const int bellPin = 2;

// I had issues with ethernet card reset pin
// So I've added a manual reset pin to reset the ethernet card on setup()
const int ethernetResetPin = 3;

byte mac[] = { 0x14, 0x5E, 0x12, 0x10, 0xD5, 0xE2 };

IPAddress ip(10,1,1,111);

EthernetServer server(80);

String readString;

bool shouldPlayOnce;
bool shouldPlayTwice;
bool shouldPlayThrice;

void setup() {
    pinMode(bellPin, OUTPUT);
    pinMode(ethernetResetPin, OUTPUT);
    digitalWrite(ethernetResetPin, LOW); 
    delay(100);
    digitalWrite(ethernetResetPin, HIGH); 
    delay(100);

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
                  if (checkAPICommand(readString, "playonce")) {
                      shouldPlayOnce = true;
                  } else if (checkAPICommand(readString, "playtwice")) {
                      shouldPlayTwice = true;
                  } else if (checkAPICommand(readString, "playthrice")) {
                      shouldPlayThrice = true;
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
     if (shouldPlayOnce) {
        shouldPlayOnce = false;
        digitalWrite(bellPin, HIGH); 
        delay(50);
        digitalWrite(bellPin, LOW);
        delay(100);
     } else if (shouldPlayTwice) {
      shouldPlayTwice = false;
        digitalWrite(bellPin, HIGH); 
        delay(30);
        digitalWrite(bellPin, LOW);
        delay(500);
        digitalWrite(bellPin, HIGH); 
        delay(80);
        digitalWrite(bellPin, LOW);
        delay(100);
     } else if (shouldPlayThrice) {
        shouldPlayThrice = false;
        digitalWrite(bellPin, HIGH); 
        delay(50);
        digitalWrite(bellPin, LOW);
        delay(200);
        digitalWrite(bellPin, HIGH); 
        delay(50);
        digitalWrite(bellPin, LOW);
        delay(200);
        digitalWrite(bellPin, HIGH); 
        delay(50);
        digitalWrite(bellPin, LOW);
        delay(100);
     }
}

// MARK : - Helper Functions

bool checkAPICommand(String URLString, String command){
    return (URLString.indexOf(command) > 0);
}

String valToAPI(String valName, String val){
    return "<" + valName + ">" + val + "</" + valName + ">";
}
