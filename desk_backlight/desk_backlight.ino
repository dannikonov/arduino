#include <SPI.h>
#include <EthernetENC.h>

#define R_PIN 2  //pin red LED
#define G_PIN 3  //pin blue LED
#define B_PIN 4  //pin green LED

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 111);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

bool connectionStatus = false;
void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);  // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


  Serial.begin(9600);  // открыть порт для связи
  pinMode(5, OUTPUT);

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}


int timer = 0;
int val;

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a new line
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();

          int n = 3;
          client.print("{");
          for (int analogChannel = 1; analogChannel < n; analogChannel++) {

            client.print("\"channel_");
            client.print(analogChannel);
            client.print("\":");
            client.print(analogRead(analogChannel));


            if (analogChannel != n - 1) {
              client.print(",");
            }
          }
          client.print("}");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

  delay(1);






  // analogWrite(R_PIN, timer % 255);
  // analogWrite(G_PIN, 255 - timer % 255);
  // analogWrite(B_PIN, 128 - timer % 255);


  timer += 1;

  if (timer < 500) {
    digitalWrite(5, HIGH);  // turn the LED on (HIGH is the voltage level)
  }

  if (500 < timer && timer < 1000) {
    digitalWrite(5, LOW);  // turn the LED off by making the voltage LOW
  }

  if (timer > 1000) {
    timer = 0;
  }
}
