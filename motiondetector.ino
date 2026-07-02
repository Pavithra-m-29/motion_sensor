#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define PIR_PIN D5
#define LED D6

const char* ssid = "pavi";
const char* password = "veeralak";

ESP8266WebServer server(80);

String webpage()
{
  int motionStatus = digitalRead(PIR_PIN);

  String alertMsg;
  String alertColor;
  String icon;

  if(motionStatus == HIGH)   // Motion Detected
  {
    digitalWrite(LED, HIGH);

    alertMsg = "MOTION DETECTED";
    alertColor = "#ff1744";
    icon = "🚶";
  }
  else
  {
    digitalWrite(LED, LOW);

    alertMsg = "NO MOTION DETECTED";
    alertColor = "#00c853";
    icon = "🛡️";
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta http-equiv="refresh" content="2">

<title>Smart PIR Motion Detection System</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Arial, sans-serif;
}

body{
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background:linear-gradient(
135deg,
#141e30,
#243b55
);
}

.container{
text-align:center;
width:100%;
}

.title{
font-size:38px;
font-weight:bold;
color:white;
margin-bottom:25px;
text-shadow:2px 2px 10px rgba(0,0,0,0.4);
}

.card{
width:420px;
margin:auto;
background:white;
padding:35px;
border-radius:25px;
box-shadow:0 10px 30px rgba(0,0,0,0.3);
}

.card h2{
color:#333;
margin-bottom:20px;
}

.icon{
font-size:70px;
margin-bottom:15px;
}

.alert{
margin-top:15px;
padding:18px;
border-radius:15px;
font-size:24px;
font-weight:bold;
color:white;
}

.footer{
margin-top:25px;
color:white;
font-size:18px;
}

.statusText{
font-size:20px;
color:#555;
margin-bottom:10px;
}

</style>

</head>

<body>

<div class="container">

<div class="title">
🚶 SMART PIR MOTION DETECTION
</div>

<div class="card">

<div class="icon">
)rawliteral";

  html += icon;

  html += R"rawliteral(
</div>

<h2>PIR Sensor Status</h2>

<div class="statusText">
Live Motion Monitoring
</div>

<div class="alert" style="background:
)rawliteral";

  html += alertColor;

  html += R"rawliteral(
;">
)rawliteral";

  html += alertMsg;

  html += R"rawliteral(
</div>

</div>

<div class="footer">
ESP8266 Web Server Dashboard
</div>

</div>

</body>
</html>
)rawliteral";

  return html;
}

void handleRoot()
{
  server.send(200, "text/html", webpage());
}

void setup()
{
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED, OUTPUT);

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int timeout = 0;

  while(WiFi.status() != WL_CONNECTED && timeout < 30)
  {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi Connected Successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.begin();

    Serial.println("Web Server Started");
  }
  else
  {
    Serial.println();
    Serial.println("WiFi Connection Failed!");
  }
}

void loop()
{
  server.handleClient();
}