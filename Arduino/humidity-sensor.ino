#include <SimpleDHT.h>

#include <ESP8266WiFi.h>//esta para conectarlo a una red wifi
#include <ESP8266WebServer.h>//esta para que el node funcione como un servidor de la red wifi
#include "DHTesp.h" // esta la ocupas para leer los datos del sensor, este tiene que ser de 3 pines

#include <string.h> // Librerias estándares de c++ ojalá no rompan todo
#include <sstream>

//si cuentas con un sensensor DHT 11 de 4 pines revisa la siguiente liga
//https://www.prometec.net/dht11-web-server-esp8266/


//Libreria para 3 pines
#define DHTpin 14 //el pin 14 en para el node es el 5D

//Put your SSID & Password/
const char* ssid = "Vito";  // Enter SSID here
const char* password = "123456789";  //Enter Password here

ESP8266WebServer server(80);//creamos el servidor en el puerto 30. Pd: en el 80 no?

//Si quieres bórralo
String SendHTML(float Temperaturestat,float Humiditystat);
void handle_OnConnect();// 
void handle_NotFound();

               
DHTesp dht; //inicializamos el sensor 

float Temperature;//inicializamos la temperatura
float Humidity;// y la humedad
 
void setup() {
  Serial.begin(115200);//esto no lo toques
  delay(100);
  
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
     

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  
  server.handleClient(); //no me lei la documentacion de esto pero es importante
  
}

void handle_OnConnect() {
  Temperature = dht.getTemperature(); // Gets the values of the temperature
  Humidity = dht.getHumidity(); // Gets the values of the humidity
  if(Humidity!=2147483647&&Temperature!=2147483647){
  Serial.println(Temperature);
  Serial.println(Humidity);
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
  }
  else{
  Serial.println("0");
  Serial.println("0");
  server.send(200, "text/html", SendHTML(0,0));  
  }
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

//esta es la pagina que vas a cargar en el nodeMCU, supongo que hay mejor forma de hacer esto pero pues por mientras esta chido
String SendHTML(float Temperaturestat,float Humiditystat){
  // Stringstream es parte de la libreria estandar de c++, si no funciona vuelvan a concatenar con el +=
  std::stringstream ptr;
  // Esta es la cosa que concatena los valores con sus llaves para poder accesarlas en formato JSON 
  // Si el pendejo de Alex no borra esto díganle que es bien JOTO
  ptr << "{\"temperatura\":\"" << (int)Temperaturestat << "\", \"humedad\":\"" << (int)Humiditystat << "\"}";
  // regresamos una string normal de c la cual tiene una conversion implícita a la string de arduino
  return ptr.str().c_str();
}