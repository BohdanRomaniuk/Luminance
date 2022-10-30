#include "captive_portal.h"

static DNSServer cpDnsService;
static WebServer cpServer(80);

const char contentPage[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head><body>
<style type="text/css">
    input[type="text"] {margin-bottom:8px;font-size:20px;}
    input[type="submit"] {width:180px; height:60px;margin-bottom:8px;font-size:20px;}
</style>
<center>
<h3>WiFi налаштування</h3>
<form action="/connect" method="POST">
    <input type="text" name="ssid" placeholder="Назва Wifi">
    <input type="password" name="password" placeholder="Пароль">
    <input type="submit" value="Підключитися">
</form>
<h3>Інші режими роботи</h3>
<form action="/acesspoint" method="POST">
    <input type="submit" value="Точка доступу">
</form>
</center>
</body></html>)rawliteral";

static bool isCaptivePortalStarted = false;
static byte captivePortalStatus = 0;
CaptivePortalCfg captivePortalConfig;

void handleWifiConnect() {
  strcpy(captivePortalConfig.SSID, cpServer.arg("ssid").c_str());
  strcpy(captivePortalConfig.password, cpServer.arg("password").c_str());
  captivePortalConfig.mode = WIFI_STA;
  captivePortalStatus = WIFI_MODE;
}

void handleAccessPointConnect() {
  captivePortalConfig.mode = WIFI_AP;
  captivePortalStatus = ACCESS_POINT_MODE;
}

void startCaptivePortal() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  IPAddress captivePortalIp(CAPTIVE_PORTAL_IP);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(captivePortalIp, captivePortalIp, subnet);
  WiFi.softAP(CAPTIVE_PORTAL_NAME);
  cpDnsService.start(53, "*", captivePortalIp);

  cpServer.onNotFound([]() {
    cpServer.send(200, "text/html", contentPage);
  });
  cpServer.on("/connect", HTTP_POST, handleWifiConnect);
  cpServer.on("/acesspoint", HTTP_POST, handleAccessPointConnect);
  cpServer.begin();
  isCaptivePortalStarted = true;
  captivePortalStatus = INIT_MODE;
}

void stopCaptivePortal() {
  WiFi.softAPdisconnect();
  cpServer.stop();
  cpDnsService.stop();
  isCaptivePortalStarted = false;
}

bool isCaptivePortalConfigured() {
  if (isCaptivePortalStarted) {
    cpDnsService.processNextRequest();
    cpServer.handleClient();
    yield();
    if (captivePortalStatus) {
      stopCaptivePortal();
      return true;
    }
  }
  return false;
}

bool isAccessPoint() {
  return captivePortalConfig.mode == WIFI_AP || (captivePortalConfig.mode == WIFI_STA && captivePortalConfig.SSID[0] == '\0');
}

byte getCaptivePortalStatus() {
  return captivePortalStatus;
}