/* Description: This sketch is uploaded to my younger sister's esp8266 board to control a set of addressable led strips
 *              in her room.
 *              Using this sketch, the esp8266 board sets up a web server to which we will connect using a Flutter mobile
 *              application. The application will send out HTTP requests and the board will process the request and respond.
 *              
 * resource: https://randomnerdtutorials.com/esp8266-web-server/
 */

#include <ESP8266WiFi.h>

#define INTERVAL 1000

// Loading led strip-related libraries
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

const int LED_PIN = D4;
const int FLAG_LED = D5;
#define NUM_LEDS 300
#define BRIGHTNESS 64
#define VOLTS 5
#define MAX_MA 3000
CRGB leds[NUM_LEDS];
int count = 0;
int _delay = 10;
int UPDATES_PER_SECOND = 400;
int red = 0;
int green = 0;
int blue = 255;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
String led_mode = "off";
String prev_mode = "off";

// network credentials
const char* ssid     = "egon24";
const char* password = "4432egon";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

int curr_time = 0;
int prev_time = 0;

void setup() {
  Serial.begin(115200);
  connectToNetwork();

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    curr_time = millis();
    prev_time = curr_time;
    while (client.connected() && curr_time - prev_time <= INTERVAL) { // loop while the client's connected
      curr_time = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            Serial.println("Header: " + header);

            if(header.indexOf("GET /favicon.ico HTTP/1.1") == -1){
              // check if request for solid rgb came in
              if(header.indexOf("rgb") > 0){
                Serial.println("In rgb");
                String temp = header.substring(header.indexOf("rgb/")+4);
  
                // parse out the red, green, blue values from the message
                red = temp.substring(0,temp.indexOf("/")).toInt();
                temp = temp.substring(temp.indexOf("/")+1);
            
                green = temp.substring(0,temp.indexOf("/")).toInt();
                temp = temp.substring(temp.indexOf("/")+1);
            
                blue = temp.toInt();
  
                Serial.print("Received: ");
                Serial.print(red);
                Serial.print(",");
                Serial.print(green);
                Serial.print(",");
                Serial.println(blue);
                led_mode = "rgb";
              }
              else{
                String temp = header.substring(header.indexOf("GET /")+5);
                temp = temp.substring(0,temp.indexOf(" HTTP/1.1"));
                Serial.println("Temp: " + temp);
                
                led_mode = temp;
              }
              Serial.print("Current mode: " + led_mode);
            }
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  // deal with led strip 
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  if(led_mode == "off"){
    // prevent a function call to off() if it remains off
    if(prev_mode != "off"){
      off();
      prev_mode = "off";
    }
  }
  else if(led_mode == "rgb"){
    if(prev_mode == "off"){
      startIndex = 0;
      prev_mode = "rgb";
    }
    setRGB();
  }
//  else if(led_mode == "study"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "study";
//    }
//    UPDATES_PER_SECOND = 200;
//    currentPalette = StudyColors_p;
//    use_palette(startIndex);
//  }
//  else if(led_mode == "party"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "party";
//    }
//    UPDATES_PER_SECOND = 800;
//    currentPalette = RainbowColors_p;
//    use_palette(startIndex);
//  }
//  else if(led_mode == "twinkle_christmas"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "twinkle_christmas";
//    }
//    UPDATES_PER_SECOND = 400;
//    currentPalette = RedGreenWhite_p;
//    twinkle();
//  }
//  else if(led_mode == "twinkle_blue"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "twinkle_blue";
//    }
//    UPDATES_PER_SECOND = 400;
//    currentPalette = Ice_p;
//    twinkle();
//  }
//  else if(led_mode == "twinkle_green"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "twinkle_green";
//    }
//    UPDATES_PER_SECOND = 400;
//    currentPalette = Holly_p;
//    twinkle();
//  }
//  else if(led_mode == "snow"){
//    if(prev_mode == "off"){
//      startIndex = 0;
//      prev_mode = "snow";
//    }
//    UPDATES_PER_SECOND = 400;
//    currentPalette = Snow_p;
//    twinkle();
//  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  
} // end loop

void connectToNetwork(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// LED effects ---------------------------------------------
void off(){
  FastLED.clear();
}

void setRGB(){
  for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(red,green,blue);
  }
  FastLED.show();
}
