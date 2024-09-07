#include <XantoI2C.h>
#include "XantoKT0803L.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


/* Variabel webserver */
const char* ssid = "IpinFM";  // Enter SSID here
const char* password = "";  //Enter Password here
ESP8266WebServer server(80);
unsigned long startMillis, currentMillis;


/*variabel radio*/
int frequency = 1080;
XantoKT0803L fm(D1, D2);

String SendHTML() {
  String mpage = "<!DOCTYPE html>\n";
  float ff = (float)frequency / 10;
  mpage += "<html>\n";
  mpage += "<body style=font-size:60px;>\n";
  mpage += "<form class='form-horizontal' action='/action_page' method='post'>\n";
  mpage += "<fieldset>\n";

  mpage += "<legend>Ipin FM \n";
  mpage += ff;
  mpage += " MHz</legend>\n";

  mpage += "<div class='form-group'>\n";
  mpage += "  <label class='col-md-4 control-label' for='frekuensi'>Frekuensi (720-1339)</label>  \n";
  mpage += " <div class='col-md-4'>\n";
  mpage += "  <input id='frekuensi' name='frekuensi' type='number' value='";
  mpage += frequency;
  mpage += "' class='form-control input-md' style=font-size:60px;>\n";

  mpage += "  </div>\n";
  mpage += "</div>\n";
  mpage += "<br>\n";

  mpage += "<div class='form-group'>\n";
  mpage += "  <div class='col-md-8'>\n";
  mpage += "    <button type='submit' value='kirim' id='button1id' name='button1id' class='btn btn-success' style=font-size:60px;>Kirim</button>\n";
  mpage += "    <button type='submit' value='matikan' id='button2id' name='button2id' class='btn btn-danger' style=font-size:60px;>Matikan</button>\n";
  mpage += "  </div>\n";
  mpage += "</div>\n";

  mpage += "</fieldset>\n";
  mpage += "</form>\n";
  mpage += "</body>\n";
  mpage += "</html>";
  return mpage;
}
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  server.send(200, "text/html", SendHTML()); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {
  frequency = server.arg("frekuensi").toInt();
  String kir = server.arg("button1id");
  String mat = server.arg("button2id");
  if (kir == "kirim") {
    fm.setFrequency((frequency));
    server.sendHeader("Location", "/", true);
    server.send(302, "text/html", SendHTML());
    //handleRoot(); //Send web page
  }
  if (mat == "matikan") {
    ESP.deepSleep(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  WiFi.softAP(ssid, password);
  delay(1000);     //Connect to your WiFi router
  //Serial.begin(9600);
  //-----------------------------sdsdsd
  fm.setFrequency(1080);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here
  server.onNotFound(handleRoot);

  server.begin();                  //Start server
  startMillis = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  currentMillis = millis();
  if (currentMillis - startMillis >= 60000) {
    if (WiFi.softAPgetStationNum() < 1) {
      ESP.deepSleep(0);
    }
  }
  delay(100);
}
