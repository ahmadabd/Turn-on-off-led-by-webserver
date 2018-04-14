#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "Bdorm";
const char *password = "YourPSKHere";

ESP8266WebServer server(80);

const int led = 13;

void handleLEDon() { 
  
  Serial.println("LED on page");
  digitalWrite(led, LOW); //LED is connected in reverse
  //server.send(200, "text/html", "LED is ON"); //Send ADC value only to client ajax request
}
 
void handleLEDoff() { 
 
  Serial.println("LED off page");
  digitalWrite(led, HIGH); //LED off
  //server.send(200, "text/html", "LED is OFF"); //Send ADC value only to client ajax request
}

void handleRoot() {
  digitalWrite ( led, HIGH);
  char temp[400];

  snprintf ( temp, 400,

"<html>"
  "<head>"
   "<meta http-equiv=\"refresh\" content=\"5\" />"
    "<title>ESP8266 Demo</title>"
    "<style>"
     "\"body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\""
    "</style>"
  "</head>"
  "<body>"
    "<h1><a href=\"ledOn\">On</a></h1><br>"
    "<h1><a href=\"ledOff\">Off</a></h1>"
  "</body>"
"</html>"
  );
  server.send (200, "text/html", temp);
  digitalWrite (led, 0);
}

void handleNotFound() {
  
  digitalWrite (led, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup( void ){
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  Serial.println ("" );

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if(MDNS.begin("esp8266")){
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLEDoff);
 
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  
  server.handleClient();
}

void drawGraph(){
  
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x+= 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
