#include  <ESP8266WiFi.h>
#include  <ESP8266WiFiMulti.h>  
#include  <ESP8266mDNS.h>
#include  <ESP8266WebServer.h>


ESP8266WiFiMulti  wifiMulti;          //  Create  an  instance  of  the ESP8266WiFiMulti  class,  called  'wifiMulti'
ESP8266WebServer  server(80);       //  Create  a webserver object  that  listens for HTTP  request on  port  80

/********Prototypes***************/
void handleNotFound();
void allumage(void);
void extinction(void);
void modularite(void);

/**********************************/

String addr_client = "192.168.43.10"; // pour repondre a la requete d'un seul IP.
String addr_test ; 
String type,dat ; 


/*********************************/


void  setup(void){
    Serial.begin(115200);                 //  Start the Serial  communication to  send  messages  to  the computer
    wifiMulti.addAP("AndroidAP", "zypy3306");      //  add Wi-Fi networks  you want  to  connect to
    //Serial.println("Connecting  ...");
    int i = 0;
    while (wifiMulti.run()  !=  WL_CONNECTED) { //  Wait  for the Wi-Fi to  connect:  scan  for Wi-Fi networks, and connect to  the strongest of  the networks  above
        delay(250);
       // Serial.print('.');
    }
   // Serial.println('\n');
    //Serial.print("Connected to  ");
   // Serial.println(WiFi.SSID());                            //  Tell  us  what  network we're connected to
    //Serial.print("IP  address:\t");
    //Serial.println(WiFi.localIP());                     //  Send  the IP  address of  the ESP8266 to  the computer
    if  (MDNS.begin("esp8266")) {                           //  Start the mDNS  responder for esp8266.local
      //  Serial.println("mDNS  responder started");
    } else  {
        //Serial.println("Error setting up  MDNS  responder!");
    }

  
    server.on("/ON",  HTTP_GET, allumage); 
    server.on("/OFF",  HTTP_GET, extinction); 
    server.on("/MOD",  HTTP_GET,modularite); 
    server.onNotFound(handleNotFound);                //  When  a client  requests  an  unknown URI (i.e. something other than  "/"), call  function "handleNotFound"
    server.begin();                                                     //  Actually  start the server
    
   // Serial.println("HTTP  server  started");
}

/*****************************************/
void  loop(void){
    server.handleClient();                                        //  Listen  for HTTP  requests  from  clients
}
/*****************************************/

void  handleNotFound(){
    server.send(404,  "text/plain", "404: Not found");  //  Send  HTTP  status  404 (Not  Found)  when  there's no  handler for the URI in  the request
}
/*****************************************/
void allumage(void){
addr_test = server.client().remoteIP().toString(); 
  if(addr_test == addr_client){
    Serial.print('A');
    server.send(200);
  }
}
/*****************************************/
void extinction(void){

 addr_test = server.client().remoteIP().toString(); 
 if(addr_test == addr_client){
    Serial.print('E'); 
    server.send(200);
  }
}
/*****************************************/
void modularite(void){
  
  
  addr_test = server.client().remoteIP().toString();   
  if(addr_test == addr_client){
    type = server.arg(0);
        
    dat = 'M';
    dat +=';'; 
    dat +=type; 
  
    Serial.print(dat);
    server.send(200);
 }
  
}


