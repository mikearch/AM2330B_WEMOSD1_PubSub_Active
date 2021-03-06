/* AM2330B Temperature and humiity sensor with WeMosD1 R2
Red = 3v3 or use 5v with logic level shifter
Black = gnd
White = SCL (D1)
Yellow = SDA (D2)

consider pull-down resistors on SCL and SDA

*/

//Libraries
#include <Wire.h>
#include <AM2320.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Initializes AM2320B sensor instance
AM2320 th;


// Timers auxiliary variables
unsigned long Tnow = millis();
unsigned long lastMeasure = 0;
unsigned long publishInterval = 120000; //node red publish every 2 mins


// Change the credentials below, so your ESP8266 connects to your router

//Ljubljana
//const char* ssid = "335C8E";
//const char* password = "EVW32C0N00099679";

//Radovica
const char* ssid = "DWR-921-2BF8"; //radovica server
const char* password = "CY3qR8KT"; //radovica server

//Radovica D-link
//const char* ssid = "HUAWEI-E5776-89CD"; //radovica server
//const char* password = "QFYTA2E2"; //radovica server

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker

//Ljubljana
//const char* mqtt_server = "192.168.0.24";

//Radovica-mobil wifi small
//const char* mqtt_server = "192.168.0.101";

//Radovica-mobil wifi small D-link router
const char* mqtt_server = "192.168.0.50";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;  //change this for each ESP8266
PubSubClient client(espClient);  //change variable as line above
//Radovica Bar Outside
//WiFiClient espBar1Client;  //change this for each ESP8266
//PubSubClient client(espBar1Client);  //change variable as line above

long lastMsg = 0;
char msg[50];
int value = 0;

//Variables

float hum;  //Stores humidity value
float temp; //Stores temperature value

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/desk, you check if the message is either 1 or 0. Turns the Desk outlet according to the message
  if(topic=="home/office/esp1/desk"){
      Serial.print("Changing Desk light to ");
      if(messageTemp == "1"){

        Serial.print("On");
      }
      else if(messageTemp == "0"){

        Serial.print("Off");
      }
  }
  if(topic=="home/office/esp1/workbench"){
      Serial.print("Changing Workbench light to ");
      if(messageTemp == "1"){

        Serial.print("On");
      }
      else if(messageTemp == "0"){

        Serial.print("Off");
      }
  }
  Serial.println();
}
// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {  //change per instructions above
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("home/office/esp1/desk");
      client.subscribe("home/office/esp1/workbench");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  setup_wifi();
  Wire.begin();    //For AM2320B sensor
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{

 if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
    delay(2000);
   getTemp();
  Tnow = millis();
    if (Tnow - lastMeasure > publishInterval) {
    lastMeasure = Tnow;
    Serial.println("publish");
    pubTemp();
    delay(500); //Delay 2 sec.
}
}
