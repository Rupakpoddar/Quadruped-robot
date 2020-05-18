/* This program lets you exchange data between a web browser and ESP8266
   board connected to a local Wi-Fi network or by using point-to-point
   protocol(PPP). With this example, you can control the built-in LED of
   the ESP8266 board by connecting to the local IP address with any web
   browser on the same network.                                       */

#include <ESP8266WiFi.h>
 
const char* ssid = "Change_this";      //Your SSID
const char* password = "Change_this"; //Your password
 
int ledPin = 16;  //GPIO pin D0 of ESP8266 NodeMCU
WiFiServer server(80);  //Service Port for http
 
void setup() {
  Serial.begin(115200);
  delay(1000);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
   
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  //Match the request. Set ledPin according to the request.
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
  }
   
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //Do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");

  //For more info on button style: https://www.w3schools.com/css/tryit.asp?filename=trycss_buttons_color
  client.println(".button{");
  client.println("background-color: #4CAF50; /* Green */");
  client.println("width: 48%;");
  client.println("border: none;");
  client.println("color: white;");
  client.println("padding: 50px 40px;");
  client.println("text-align: center;");
  client.println("text-decoration: none;");
  client.println("display: inline-block;");
  client.println("font-size: 50px;");
  client.println("margin: 4px 2px;");
  client.println("cursor: pointer;");
  client.println("border-radius: 12px;");
  client.println("}");
  
  client.println(".button2 {background-color: #f44336;} /* Red */ ");

  client.println("</style>");
  client.println("</head>");
  client.println("<body bgcolor=black>");
  
  client.println("<button class=\"button\" onclick=\"window.location.href='/LED=OFF';\">ON</button>");
  client.println("<br></br>");
  client.println("<button class=\"button button2\" onclick=\"window.location.href='/LED=ON';\">OFF</button>");
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
