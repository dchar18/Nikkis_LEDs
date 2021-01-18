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

// used for the "twinkle" modes ---------------------
// Overall twinkle speed.
// 0 (VERY slow) to 8 (VERY fast).  
// 4, 5, and 6 are recommended, default is 4.
#define TWINKLE_SPEED 4

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).  
// Default is 5.
#define TWINKLE_DENSITY 6

// Background color for 'unlit' pixels
CRGB gBackgroundColor = CRGB::Black; 

// If AUTO_SELECT_BACKGROUND_COLOR is set to 1,
// then for any palette where the first two entries 
// are the same, a dimmed version of that color will
// automatically be used as the background color.
#define AUTO_SELECT_BACKGROUND_COLOR 0

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1

CRGBPalette16 targetPalette;
// --------------------------------------------------

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
  else if(led_mode == "party"){
    if(prev_mode == "off"){
      startIndex = 0;
      prev_mode = "party";
    }
    UPDATES_PER_SECOND = 800;
    currentPalette = RainbowColors_p;
    use_palette(startIndex);
  }
  else if(led_mode == "twinkle_christmas"){
    if(prev_mode == "off"){
      startIndex = 0;
      prev_mode = "twinkle_christmas";
    }
    UPDATES_PER_SECOND = 400;
    currentPalette = RedGreenWhite_p;
    twinkle();
  }
  else if(led_mode == "twinkle_blue"){
    if(prev_mode == "off"){
      startIndex = 0;
      prev_mode = "twinkle_blue";
    }
    UPDATES_PER_SECOND = 400;
    currentPalette = Ice_p;
    twinkle();
  }
  else if(led_mode == "twinkle_green"){
    if(prev_mode == "off"){
      startIndex = 0;
      prev_mode = "twinkle_green";
    }
    UPDATES_PER_SECOND = 400;
    currentPalette = Holly_p;
    twinkle();
  }
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

void use_palette(uint8_t colorIndex){
  uint8_t brightness = 255;
  for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      colorIndex += 3;
  }
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

// functions related to twinkle effects ----
void twinkle(){
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  // if AUTO_SELECT_BACKGROUND_COLOR == 1, and the first two colors of
  // the current palette are identical, then a deeply faded version of
  // that color is used for the background color
  CRGB bg;
  if( (AUTO_SELECT_BACKGROUND_COLOR == 1) && (currentPalette[0] == currentPalette[1] )){
    bg = currentPalette[0];
    uint8_t bglight = bg.getAverageLight();
    if( bglight > 64) {
      bg.nscale8_video( 16); // very bright, so scale to 1/16th
    } else if( bglight > 16) {
      bg.nscale8_video( 64); // not that bright, so scale to 1/4th
    } else {
      bg.nscale8_video( 86); // dim, scale to 1/3rd.
    }
  } 
  else {
    bg = gBackgroundColor; // just use the explicitly defined background color
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for( CRGB& pixel: leds) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, 
      // use the new color.
      pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      pixel = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      pixel = bg;
    }
  }
}

//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt) {
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( currentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } 
  else {
    c = CRGB::Black;
  }
  return c;
}

// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}
