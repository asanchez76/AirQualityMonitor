//Network related code goes in thifs file
#include "Arduino.h"
#include "network.h"
#include "secrets.h"
#include "display.h"
#include "hardwarepins.h"
#include <ThingSpeak.h>

// Wifi Settings - Update secrets.h with correct values
char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

WiFiEspClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void InitNetwork()
{
  // initialize serial for ESP module
  setEspBaudRate(ESP_BAUDRATE);
#ifdef DEBUG
  Serial.print("[NET]Searching for ESP8266...");
#endif
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG
    Serial.println("[NET]WiFi shield not present");
#endif
    // don't continue
    while (true);
  }
#ifdef DEBUG
  Serial.println("[NET]found it!");
#endif
}

void connectToNetwork()
{
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    LCDPrint("Connecting",SECRET_SSID);
#ifdef DEBUG
    Serial.print("[NET]Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
#endif
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
#ifdef DEBUG
      Serial.print(".");
#endif
      delay(5000);
    }
#ifdef DEBUG
    Serial.println("\nConnected.");
#endif
    LCDPrint("Connected.",SECRET_SSID);
    delay(1000);
    ThingSpeak.begin(client);  // Initialize ThingSpeak
  }
}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};
#ifdef DEBUG
  Serial.print("[NET]Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");
#endif

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);
  }
  Serial1.begin(baudrate);
}

void setIoTField(int id, float value)
{
	ThingSpeak.setField(id, value);
}

void setIoTStatus(String value)
{
	ThingSpeak.setStatus(value);
}

void writeIoTFields()
{
	//put a hold message on this display since button actions are not going to be available (except through interrupts
	LCDPrint("Uploading data.", "Please wait...");
	// write to the ThingSpeak channel
	int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
	if(x == 200){
#ifdef DEBUG
	Serial.println("[NET]Channel update successful.");
#endif
	}
		else{
#ifdef DEBUG
			Serial.println("[NET]Problem updating channel. HTTP error code " + String(x));
#endif
	}
	LCDClear();
}
