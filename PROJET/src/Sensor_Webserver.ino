/*********
  Projet objet connecté
  Aminata Diagne - Martin Boisse - Melaine SIAKO

  Groupe Sensaqua

  Code présent sur l'ESP côté capteurs
*********/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <MCP3008.h>
// Heure
#include <TimeLib.h>
#include <NtpClientLib.h>

//define pin connections to MCP3008
#define CS_PIN D8
#define CLOCK_PIN D5
#define MOSI_PIN D7
#define MISO_PIN D6

// Valeurs borne des paramètres
#define MIN_TEMP 23
#define MAX_TEMP 27

#define MIN_PH 6.3
#define MAX_PH 7.5

#define MIN_SALINITE 1.022
#define MAX_SALINITE 1.025

#define MIN_NITRATE 0
#define MAX_NITRATE 50

// Variable de temps
int time_add;
bool flag; 

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

//Sensor values
float val_temp;
float val_lum;
float val_nitrate;
float val_salinite;
float val_ph;

/*const char* ssid = "Livebox-S3X3";
const char* password = "grossebite";*/

const char* ssid = "AndroidAP";
const char* password = "zypy3306";

// Pour l'envoi de messages
const char* host = "maker.ifttt.com";
const int httpsPort = 443;

// Web Server on port 80
WiFiServer server(80);


// Connection vers ESP_LED 
const char* host_ESP_LED = "192.168.43.237";
WiFiClient client_ESP;
const int httpPort = 80;
int value;  //on/off les lumieres

/* ------------------------------ SETUP ------------------------------ */
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  Wire.begin(D3, D4);
  Wire.setClock(100000);
  // Connecting to WiFi network
  Serial.println();
  Serial.print("En train de se connecter à ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("ESP est connecté");
  
  // Starting the web server
  server.begin();
  Serial.println("Le server web fonctionne. En attente de l'adresse IP de l'ESP...");
  delay(10000);
  
  // Affichage de l'adresse IP
  Serial.println(WiFi.localIP());

  //Mise à 0 du temps
  time_add = 0;
  flag = true;

  // Get l'heure pour gérer l'allumage/exctinction des lumieres
  NTP.begin("pool.ntp.org", 1, true);
  NTP.setInterval(1000);

  Serial.print("Got NTP time: ");
  Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));

}

/* ----------------------------------------------------------------------- */

/* ----------------------------- ALLUMAGE LED ---------------------------- */

/*
  Obtention de l'heure et envoie de la commande d'allumage des LED
*/
String heure;
bool jour = false;
bool deja_diminue = false; 
 
void setLED() {

   // Get de l'heure
  heure = NTP.getTimeDateString(NTP.getLastNTPSync());

  // Jour / Nuit
  if(heure[0] == '1'&& heure[1] == '4'&& heure[3] == '1' && heure[4] == '4' && jour ==false)
  {  OnLed();
    jour = true; 
  } 
  else if(heure[0] == '2' && heure[1] == '0'&& heure[3] == '0' && heure[4] == '0' && jour ==true)
  {
    jour = false;
    OffLed();
  }  

  Serial.print("Lumiere:");
  Serial.println(val_lum);
  
  if (val_lum < 850 && deja_diminue == true)
  {
    value = HIGH; 
    modulation();
    deja_diminue = false;  
  }
  else if (val_lum>=850 && deja_diminue == false)  //plein soleil
  { 
    value = LOW;
    modulation();
    deja_diminue = true;
  }    
}

// Ajustement en fonction de la luminosité
void modulation ()
{
    if (!client_ESP.connect(host_ESP_LED, httpPort)) 
    {
    Serial.println("connection failed");
    return;
    }

  Serial.print("connecting to ");
  Serial.println(host_ESP_LED);

  String url = "/MOD?type=";
  url += value; 
//  url += "&pourcentage=40";

  // This will send the request to the server
  client_ESP.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host_ESP_LED + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client_ESP.available() == 0) 
  {
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client_ESP.stop();
      return;  
    }
  }
   
 // Read all the lines of the reply from server and print them to Serial
  while(client_ESP.available())
  {
    String line = client_ESP.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}
 


void OnLed()  {

  if (!client_ESP.connect(host_ESP_LED, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("connecting to ");
  Serial.println(host_ESP_LED);

  String url = "/ON";

  // This will send the request to the server
  client_ESP.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host_ESP_LED + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client_ESP.available() == 0) 
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client Timeout !");
      client_ESP.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client_ESP.available())
  {
    String line = client_ESP.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}

void OffLed() {

  if (!client_ESP.connect(host_ESP_LED, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("connecting to ");
  Serial.println(host_ESP_LED);

  String url = "/OFF";

  // This will send the request to the server
  client_ESP.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host_ESP_LED + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client_ESP.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client_ESP.stop();
      return;
    }
  }
 
  // Read all the lines of the reply from server and print them to Serial
  while(client_ESP.available())
  {
    String line = client_ESP.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}

/* ----------------------------------------------------------------------- */

/* ------------------------------ GET PARAM ------------------------------ */

/*
  Obtention des paramêtres grâce au MCP3008
*/

void getParam() {

  val_lum = adc.readADC(0); // Lit le channel 0 du MCP3008 (pin 1)
  val_temp = adc.readADC(2) * (3.3*10/1023);
  //val_ph = adc.readADC(5);
  val_ph = 6.5;
  //val_salinite = adc.readADC(3);
  val_salinite = 1.02;
  //val_nitrate = adc.readADC(4);  
  val_nitrate = 25;
}

/* ----------------------------------------------------------------------- */

/* ----------------------------- SEND MESSAGE ---------------------------- */

/*
  Envoie du message d'alerte
*/

void sendMessage()  {

  if(flag == true)  {
    Serial.print("-------------- Envoi du message d'alerte --------------");
    WiFiClientSecure client;
    Serial.print("Connection à ");
    Serial.println(host);
    if (!client.connect(host, httpsPort)) {
      Serial.println("Connection echouée");
      return;
    }

    // Trouvé sur le site IFTTT
    String url = "https://maker.ifttt.com/trigger/ESP/with/key/pBW4-1AX_w1PC5L4qaeKxEieBuVZHE74Dg7hpafatQs______CACACA";

    Serial.print("Appel de l'url suivant: ");
    Serial.println(url);
    // Envoi de la requête
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "User-Agent: BuildFailureDetectorESP8266\r\n" +
                "Connection: close\r\n\r\n");
    Serial.println("Requête envoyée");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("requête reçue");
        break;
      }
    } 
    String line = client.readStringUntil('\n');
    Serial.println("La réponse a été:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("Fermeture de la connection");
  
    // Mise en attente du prochain SMS
    flag = false;
  }

}

/* ----------------------------------------------------------------------- */

/* --------------------------------- LOOP -------------------------------- */
WiFiClient client ;
void loop() {
   /* ----------------- Contrôle des paramêtres ----------------- */

  getParam();
  delay(5000);
  setLED();

 

  if(val_temp > MAX_TEMP || val_temp < MIN_TEMP)  
    sendMessage();
  // if(val_ph > MAX_PH || val_ph < MIN_PH)
  //   sendMessage();
  // if(val_salinite > MAX_SALINITE || val_salinite < MIN_SALINITE)
  //   sendMessage();
  // if(val_nitrate > MAX_NITRATE || val_nitrate < MIN_NITRATE)
  //   sendMessage();  

  /* ------------------------------------------------------------ */

  /* ----------------- Limiter le nombre de SMS ----------------- */

  time_add++;
  if(time_add > 8000) {
    flag = true;
    time_add = 0;
  }

  /* ------------------------------------------------------------ */

  /* ------------ Côté Serveur WEB - écoute client -------------- */

  // En ecoute de nouveaux clients
  client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {

            /* ----------------- Affichage de la page Web ----------------- */

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // Web Page intro
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><META HTTP-EQUIV=\"refresh\" CONTENT=\"15\">");
            client.println("<style>");
            client.println("body  {");
            client.println("background: url(\"/img/background.png\");");
            client.println("background-repeat: no-repeat;");
            client.println("background-size: 53%;}");
            client.println("h4    {");
            client.println("font-family: verdana;");
            client.println("color: #FF4000;}");
            client.println("h3    {");
            client.println("font-family: verdana;");
            client.println("font-weight: bold;");
            client.println("color: teal;}");
            client.println("</style>");
            client.println("</head>");
            // Body
            client.println("<body>");
            client.println("<table border=\"2\" width=\"405\" cellpadding=\"10\">");
            client.println("<tbody>");
            client.println("<tr>");
            client.println("<td>");
            client.println("<h4>Temperature min: ");
            client.println(MIN_TEMP);
            client.println("&deg;C</h4>");
            client.println("<h3>Temperature actuelle: ");
            //Affichage temperature
            client.println(val_temp);
            Serial.print(val_temp);
            client.println("&deg;C</h3>");
            client.println("<h4>Temperature max: ");
            client.println(MAX_TEMP);
            client.println("&deg;C</h4></td></tr>");
            client.println("<tr><td>");
            client.println("<h4>Ph min: ");
            client.println(MIN_PH);
            client.println("</h4>");
            client.println("<h3>Ph actuel: ");
            //Affichage PH
            client.println(val_ph);
            client.println("</h3>");
            client.println("<h4>Ph max: ");
            client.println(MAX_PH);
            client.println("</h4></td></tr>");
            client.println("<tr><td>");
            client.println("<h4>Salinit&eacute; min: ");
            client.println(MIN_SALINITE);
            client.println("</h4>");
            client.println("<h3>Salinit&eacute; actuelle: ");
            //Affichage salinite
            client.println(val_salinite);
            client.println("</h3>");
            client.println("<h4>Salinit&eacute; max: ");
            client.println(MAX_SALINITE);
            client.println("</h4></td></tr>");
            client.println("<tr><td>");
            client.println("<h4>Taux de nitrate min: ");
            client.println(MIN_NITRATE);
            client.println("mg/Litre</h4>");
            client.println("<h3>Taux de nitrate actuel: ");
            //Affichage nitrate
            client.println(val_nitrate);
            client.println("</h3>");
            client.println("<h4>Taux de nitrate max: ");
            client.println(MAX_NITRATE);
            client.println("mg/Litre</h4></td></tr>");
            // Fin Page
            client.println("</tbody></table></body></html>");

            /* ------------------------------------------------------------ */

            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    //delay(1);
    //client.stop();
    //Serial.println("Client disconnected.");
  }

  /* ------------------------------------------------------------ */

} 

/* ----------------------------------------------------------------------- */
