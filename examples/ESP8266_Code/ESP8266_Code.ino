//////////////////////////////////////////////////////////
//  _____        _                    _____      _
// |  __ \      (_)                  / ____|    (_)
// | |  | |_   _ _ _ __   ___ ______| |     ___  _ _ __
// | |  | | | | | | '_ \ / _ \______| |    / _ \| | '_ \ 
// | |__| | |_| | | | | | (_) |     | |___| (_) | | | | |
// |_____/ \__,_|_|_| |_|\___/       \_____\___/|_|_| |_|
//  Code for ESP8266 boards - V2.4.5
//  © Duino-Coin Community 2019-2021
//  Distributed under MIT License
//////////////////////////////////////////////////////////
//  https://github.com/revoxhere/duino-coin - GitHub
//  https://duinocoin.com - Official Website
//////////////////////////////////////////////////////////
//  If you don't know what to do, visit official website
//  and navigate to Getting Started page. Happy mining!
//////////////////////////////////////////////////////////

#include <ESP8266WiFi.h> // Include WiFi library
#include <ESP8266mDNS.h> // OTA libraries
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DuinoCoin.h>

namespace
{
const char *ssid = "WiFi SSID";         // Change this to your WiFi SSID
const char *password = "WiFi Pass";     // Change this to your WiFi password
const char *ducouser = "DUCO Username"; // Change this to your Duino-Coin username
const char *rigIdentifier = "None";     // Change this if you want a custom miner name

const char *host = "51.15.127.80"; // Static server IP
const int port = 2811;
unsigned int Shares = 0; // Share variable

String clientBuffer = "";

WiFiClient client;

#define LED_BUILTIN 2

#define BLINK_SHARE_FOUND 1
#define BLINK_SETUP_COMPLETE 2
#define BLINK_CLIENT_CONNECT 3
#define BLINK_RESET_DEVICE 5

#define END_TOKEN  '\n'
#define SEP_TOKEN  ','

void SetupWifi()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA);        // Setup ESP in client mode
  WiFi.begin(ssid, password); // Connect to wifi

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.println("Local IP address: " + WiFi.localIP().toString());
}

void SetupOTA()
{
  ArduinoOTA.onStart([]() { // Prepare OTA stuff
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void blink(uint8_t count)
{
  for (int x = 0; x < count; ++x)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
  }
  yield(); // register updates can take several cpu cycles;
}

void RestartESP(String msg)
{
  Serial.println(msg);
  Serial.println("Resetting ESP...");
  blink(BLINK_RESET_DEVICE);
  ESP.reset();
}

void handleSystemEvents(void)
{
  ArduinoOTA.handle();
  yield();
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void waitForClientData(void)
{
  clientBuffer = "";

  while (client.connected())
  {
    if (client.available())
    {
      clientBuffer = client.readStringUntil(END_TOKEN);
      if (clientBuffer.length() == 1 && clientBuffer[0] == END_TOKEN)
        clientBuffer = "???\n"; // NOTE: Should never happen...

      break;
    }
    handleSystemEvents();
  }
}

void ConnectToServer()
{
  if (client.connected())
    return;

  Serial.println("\nConnecting to Duino-Coin server...");
  if (!client.connect(host, port))
    RestartESP("Connection failed.");

  waitForClientData();
  blink(BLINK_CLIENT_CONNECT); // Blink 3 times - indicate sucessfull connection with the server
}

bool max_micros_elapsed(unsigned long current, unsigned long max_elapsed)
{
  static unsigned long _start = 0;

  if ((current - _start) > max_elapsed)
  {
    _start = current;
    return true;
  }

  return false;
}
} // namespace

void setup()
{
  Serial.begin(115200); // Start serial connection
  Serial.println("\nDuino-Coin ESP8266 Miner v2.4.5");

  SetupWifi();
  SetupOTA();

  blink(BLINK_SETUP_COMPLETE); // Blink 2 times - indicate sucessfull connection with wifi network
}

void loop()
{
  ArduinoOTA.handle(); // Enable OTA handler
  ConnectToServer();

  Serial.println("Asking for a new job for user: " + String(ducouser));

  client.print("JOB," + String(ducouser) + ",ESP8266"); // Ask for new job
  String hash = getValue(clientBuffer, SEP_TOKEN, 0);
  String job = getValue(clientBuffer, SEP_TOKEN, 1);
  unsigned int diff = getValue(clientBuffer, SEP_TOKEN, 2).toInt();

  Serial.println("Job received: " + hash + " " + job + " " + String(diff));

  float StartTime = micros(); // Start time measurement
  unsigned int iJob = Ducos1a.work(hash, job, diff);

  if (iJob > 0)
  {
    unsigned long ElapsedTime = micros() - StartTime;  // Calculate elapsed time
    float ElapsedTimeSeconds = ElapsedTime * .000001f; // Convert to seconds
    float HashRate = iJob / ElapsedTimeSeconds;
    client.print(String(iJob) + "," + String(HashRate) + ",ESP8266 Miner v2.4.5" + "," + String(rigIdentifier)); // Send result to server
    waitForClientData();
    Shares++;
    Serial.println(clientBuffer + " share #" + String(Shares) + " (" + String(iJob) + ")" + " Hashrate: " + String(HashRate) + " Free RAM: " + String(ESP.getFreeHeap()));
    blink(BLINK_SHARE_FOUND);
  }
}