//Written by Rupak Poddar
//www.youtube.com/RupakPoddar
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define interval 100 //Change interval from here
#include "Quadruped_parameters.h"

const char* ssid = "Change_this";      //Your SSID
const char* password = "Change_this"; //Your password

WiFiServer server(80);  //Service Port for http

void sw(int k1 = k1_normal, int k2 = k2_normal, int k3 = k3_normal, int k4 = k4_normal, int l1 = l1_normal, int l2 = l2_normal, int l3 = l3_normal, int l4 = l4_normal) {
  pwm.setPWM(4, 0, k1);
  delay(interval);
  pwm.setPWM(5, 0, k2);
  delay(interval);
  pwm.setPWM(6, 0, k3);
  delay(interval);
  pwm.setPWM(7, 0, k4);
  delay(interval);
  pwm.setPWM(0, 0, l1);
  delay(interval);
  pwm.setPWM(1, 0, l2);
  delay(interval);
  pwm.setPWM(2, 0, l3);
  delay(interval);
  pwm.setPWM(3, 0, l4);
}

void setup() {
  pinMode(16, OUTPUT);  //LED indicator for Wi-Fi connection
  digitalWrite(16, LOW);
  Serial.begin(115200); //Baud rate
  pwm.begin();
  pwm.setPWMFreq(60); //Servo motors work at 60hz refresh rate
  delay(1000);

  //Connect to WiFi network
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

  digitalWrite(16, HIGH);
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  if (request.indexOf("/S") != -1) {
    sw(k1_sleep,k2_sleep,k3_sleep,k4_sleep,l1_sleep,l2_sleep,l3_sleep,l4_sleep);//sleep
  }
  if (request.indexOf("/W") != -1) {
    sw();//wake
  }
  if (request.indexOf("/F") != -1) {
    forward();
  }
  if (request.indexOf("/F3") != -1) {
    forward();
    forward();
    forward();
  }
  if (request.indexOf("/B") != -1) {
    backward();
  }
  if (request.indexOf("/B3") != -1) {
    backward();
    backward();
    backward();
  }
  if (request.indexOf("/L") != -1) {
    left();
  }
  if (request.indexOf("/L3") != -1) {
    left();
    left();
    left();
  }
  if (request.indexOf("/R") != -1) {
    right();
  }
  if (request.indexOf("/R3") != -1) {
    right();
    right();
    right();
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
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

  client.println("</style>");
  client.println("</head>");
  client.println("<body bgcolor=black>");

  client.println("<button class=\"button\" onclick=\"window.location.href='/F';\">Forward</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/F3';\">Forward (x4)</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/B';\">Backward</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/B3';\">Backward (x4)</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/L';\">Left</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/L3';\">Left (x4)</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/R';\">Right</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/R3';\">Right (x4)</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/S';\">Sleep</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/W';\">Wake</button>");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

/*----------Quadruped Functions----------*/

void forward()
{
  pwm.setPWM(4, 0, k1_up);
  delay(interval);
  pwm.setPWM(0, 0, l1_forward);
  delay(interval);
  pwm.setPWM(4, 0, k1_normal);
  delay(interval);

  pwm.setPWM(5, 0, k2_up);
  delay(interval);
  pwm.setPWM(1, 0, l2_forward);
  delay(interval);
  pwm.setPWM(5, 0, k2_normal);
  delay(interval);

  pwm.setPWM(6, 0, k3_up);
  delay(interval);
  pwm.setPWM(2, 0, l3_forward);
  delay(interval);
  pwm.setPWM(6, 0, k3_normal);
  delay(interval);

  pwm.setPWM(7, 0, k4_up);
  delay(interval);
  pwm.setPWM(3, 0, l4_forward);
  delay(interval);
  pwm.setPWM(7, 0, k4_normal);
  delay(interval);

  pwm.setPWM(0, 0, l1_normal);
  delay(1);
  pwm.setPWM(1, 0, l2_normal);
  delay(1);
  pwm.setPWM(2, 0, l3_normal);
  delay(1);
  pwm.setPWM(3, 0, l4_normal);
}

void backward()
{
  pwm.setPWM(7, 0, k4_up);
  delay(interval);
  pwm.setPWM(3, 0, l4_backward);
  delay(interval);
  pwm.setPWM(7, 0, k4_normal);
  delay(interval);

  pwm.setPWM(6, 0, k3_up);
  delay(interval);
  pwm.setPWM(2, 0, l3_backward);
  delay(interval);
  pwm.setPWM(6, 0, k3_normal);
  delay(interval);

  pwm.setPWM(5, 0, k2_up);
  delay(interval);
  pwm.setPWM(1, 0, l2_backward);
  delay(interval);
  pwm.setPWM(5, 0, k2_normal);
  delay(interval);

  pwm.setPWM(4, 0, k1_up);
  delay(interval);
  pwm.setPWM(0, 0, l1_backward);
  delay(interval);
  pwm.setPWM(4, 0, k1_normal);
  delay(interval);

  pwm.setPWM(3, 0, l4_normal);
  delay(1);
  pwm.setPWM(2, 0, l3_normal);
  delay(1);
  pwm.setPWM(1, 0, l2_normal);
  delay(1);
  pwm.setPWM(0, 0, l1_normal);
}

void left()
{
  pwm.setPWM(5, 0, k2_up);
  delay(interval);
  pwm.setPWM(1, 0, l2_forward);
  delay(interval);
  pwm.setPWM(5, 0, k2_normal);
  delay(interval);

  pwm.setPWM(4, 0, k1_up);
  delay(interval);
  pwm.setPWM(0, 0, l1_backward);
  delay(interval);
  pwm.setPWM(4, 0, k1_normal);
  delay(interval);

  pwm.setPWM(1, 0, l2_normal);
  delay(interval);
  pwm.setPWM(0, 0, l1_normal);
}

void right()
{
  pwm.setPWM(4, 0, k1_up);
  delay(interval);
  pwm.setPWM(0, 0, l1_forward);
  delay(interval);
  pwm.setPWM(4, 0, k1_normal);
  delay(interval);

  pwm.setPWM(5, 0, k2_up);
  delay(interval);
  pwm.setPWM(1, 0, l2_backward);
  delay(interval);
  pwm.setPWM(5, 0, k2_normal);
  delay(interval);

  pwm.setPWM(0, 0, l1_normal);
  delay(interval);
  pwm.setPWM(1, 0, l2_normal);
}
