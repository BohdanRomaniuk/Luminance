#include "config.h"

static DNSServer configDnsServer;
static WebServer configServer(80);
static bool isConfigServerStarted = false;
static byte configServerStatus = 0;
Configuration config;

const char configHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head><body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
</style>
<center>
<h3>WiFi settings</h3>
<form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="SSID">
    <input type="text" name="pass" placeholder="Pass">
    <input type="submit" value="Submit">
</form>
<h3>Switch WiFi mode</h3>
<form action="/ap" method="POST">
    <input type="submit" value="Access Point">
</form>
<form action="/local" method="POST">
    <input type="submit" value="Local Mode">
</form>
<form action="/exit" method="POST">
    <input type="submit" value="Exit Portal">
</form>
</center>
</body></html>)rawliteral";

void configStart() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  IPAddress apIP(CONFIG_IP);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(CONFIG_NAME);
  configDnsServer.start(53, "*", apIP);

  configServer.onNotFound([]() {
    configServer.send(200, "text/html", configHtml);
  });
  configServer.on("/connect", HTTP_POST, handleConnect);
  configServer.on("/ap", HTTP_POST, handleAP);
  configServer.on("/local", HTTP_POST, handleLocal);
  configServer.on("/exit", HTTP_POST, handleExit);
  configServer.begin();
  isConfigServerStarted = true;
  configServerStatus = 0;
}

void configStop() {
  WiFi.softAPdisconnect();
  configServer.stop();
  configDnsServer.stop();
  isConfigServerStarted = false;
}

bool configTick() {
  if (!isConfigServerStarted) {
    return 0;
  }
  configDnsServer.processNextRequest();
  configServer.handleClient();
  yield();

  if (configServerStatus) {
    configStop();
    return 1;
  }
}

void configRun(uint32_t prd) {
  uint32_t tmr = millis();
  configStart();
  while (!configTick()) {
    if (millis() - tmr > prd) {
      configServerStatus = 5;
      configStop();
      break;
    }
  }
}

byte getConfigStatus() {
  return configServerStatus;
}

void handleConnect() {
  strcpy(config.SSID, configServer.arg("ssid").c_str());
  strcpy(config.pass, configServer.arg("pass").c_str());
  config.mode = WIFI_STA;
  configServerStatus = 1;
}

void handleAP() {
  config.mode = WIFI_AP;
  configServerStatus = 2;
}

void handleLocal() {
  config.mode = WIFI_STA;
  configServerStatus = 3;
}

void handleExit() {
  configServerStatus = 4;
}