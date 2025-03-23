/**
1306 SDA - D21
1306 SCL - D22
*/

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "settings.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SSD1306_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t Task1;
TaskHandle_t Task2;

IPAddress ip(192, 168, 1, 111);
WiFiServer server(80);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

IPAddress dns(192, 168, 1, 1);

int i = 0;
int delayTime = 1;
uint task2delayTime = 500;

void Task1code(void* pvParameters) {
}


void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    // digitalWrite(19, HIGH);  // turn the LED on (HIGH is the voltage level)
    // delay(500);              // wait for a second
    // digitalWrite(19, LOW);   // turn the LED off by making the voltage LOW
    // delay(500);              // wait for a second

    display.clearDisplay();
    display.drawRect(0, 0, display.width() - 1, 16, WHITE);
    display.drawRect(0, 16, display.width() - 1, 48, WHITE);
    display.setCursor(3, 4);
    display.print(i / 1000);


    char buffer[8];
    sprintf(buffer, "34: %d", analogRead(34));
    display.setCursor(3, 20);
    display.print(buffer);

    sprintf(buffer, "35: %d", analogRead(35));
    display.setCursor(3, 28);
    display.println(buffer);

    display.drawRect(64, 16, display.width() - 1, 48, WHITE);
    display.drawPixel(65, 18, WHITE);
    display.drawPixel(66, 18, WHITE);
    display.drawPixel(67, 19, WHITE);

    display.display();

    delay(task2delayTime);
    i += task2delayTime;
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);

  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(19, OUTPUT);

  Serial.println("Started");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  } else {
    Serial.println("wrong");
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();


  if (!WiFi.config(ip, gateway, subnet, dns, dns)) {
    Serial.println("Failed to configure");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  server.begin();


  // xTaskCreatePinnedToCore(
  //   Task1code, /* Task function. */
  //   "Task1",   /* name of task. */
  //   10000,     /* Stack size of task */
  //   NULL,      /* parameter of the task */
  //   1,         /* priority of the task */
  //   &Task1,    /* Task handle to keep track of created task */
  //   0);        /* pin task to core 0 */
  // delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
  delay(500);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      char c = client.read();
      if (c == '\n' && currentLineIsBlank) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();

        String webPage = "";

        for (int analogChannel = 34; analogChannel <= 35; analogChannel++) {
          webPage += "arduino{channel=\"";
          webPage += analogChannel;
          webPage += "\"} ";
          webPage += analogRead(analogChannel);
          if (analogChannel != 35) {
            webPage += "\n";
          }
        }

        client.print(webPage);
        break;
      }

      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }

    delay(1);

    client.stop();
    Serial.println("client disconnected");
  }

  delay(1);
}


// https://wiki.amperka.ru/products:esp32-wroom-wifi-devkit-v1