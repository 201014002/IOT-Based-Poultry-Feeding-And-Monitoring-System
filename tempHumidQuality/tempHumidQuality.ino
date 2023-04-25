#include <dht.h> // Including library for dht
#include <ESP8266WiFi.h>
// #include <pthread.h>

String api_key = "MUK9ERO9I2EVVC24"; // Enter your Write API key from ThingSpeak
const char *ssid = "";            // replace with your wifi ssid and wpa2 key
const char *pass = "";
const char *server = "api.thingspeak.com";

#define DHTPIN 0 // pin where the dht11 is connected, D3 in ESP8266
dht DHT;

WiFiClient client;

// int flag[2];
// int turn;
// // const int MAX = 1e9;
// int ans = 0;

// void lock_init()
// {
//     // Initialize lock by resetting the desire of
//     // both the threads to acquire the locks.
//     // And, giving turn to one of them.
//     flag[0] = flag[1] = 0;
//     turn = 0;
// }

// // Executed before entering critical section
// void lock(int self)
// {
//     // Set flag[self] = 1 saying you want to acquire lock
//     flag[self] = 1;

//     // But, first give the other thread the chance to
//     // acquire lock
//     turn = 1 - self;

//     // Wait until the other thread looses the desire
//     // to acquire lock or it is your turn to get the lock.
//     while (flag[1 - self] == 1 && turn == 1 - self)
//         ;
// }

// // Executed after leaving critical section
// void unlock(int self)
// {
//     // You do not desire to acquire lock in future.
//     // This will allow the other thread to acquire
//     // the lock.
//     flag[self] = 0;
// }

void AQ(double *);
void T(double *);
void H(double *);

void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode(2, OUTPUT);
    digitalWrite(2, 0);
    Serial.println("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    pinMode(5, OUTPUT);
}

void loop()
{

    double a = NULL;
    double h = NULL;
    double t = NULL;
    // a = analogRead(A0);
    AQ(&a); // this is the air quality
    H(&h);  // this is the humidity
    T(&t);  // this is the temperature
    dataSend(a, h, t);
    ledAlert(a);

}

void dataSend(double A, double h, double t)
{
    if (isnan(A) || isnan(h) || isnan(t))
    {

        Serial.println("Failed to read from the sensors!");
    }
    else if (client.connect(server, 80))
    {
        String postStr = api_key;
        postStr += "&field1=";
        postStr += String(A / 1023 * 100);
        postStr += "&field2=";
        postStr += h;
        postStr += "&field3=";
        postStr += t;
        postStr += "\r\n";
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        Serial.print("Gas Level: ");
        Serial.print(A / 1023 * 100);
        Serial.print(", Temperature: ");
        Serial.print(t);
        Serial.print(" degrees Celcius. ");
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.println("%. Send to Thingspeak.");
    }
    delay(500);
    client.stop();
    Serial.println("Waiting...");
}

void AQ(double *a)
{
    *a = analogRead(A0);
    // a = data;
}

void H(double *h)
{
    int chk = DHT.read11(DHTPIN);
    //  printf(" %d ",&DHT.humidity);
    *h = DHT.humidity;
}

void T(double *t)
{
    int chk = DHT.read11(DHTPIN);
    //  printf(" %d ", &DHT.temperature);
    *t = DHT.temperature;
}


void ledAlert(double a){

    double air = (a / 1023 * 100);
    // thingspeak needs minimum 15 sec delay between updates.
    if (air >= 40.00 && air <= 100.00)
    {
        // digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH

        // Wait for a second
        // digitalWrite(5, HIGH);
        // delay(1000);
        digitalWrite(5, LOW);
        delay(3000);
        digitalWrite(5, HIGH);
        delay(3000);
    }
    else if (air >= 101.00 && air <= 150.00)
    {
        digitalWrite(5, LOW);
        delay(2500);
        digitalWrite(5, HIGH);
        delay(2500);
    }
    else if (air >= 151.00 && air <= 200.00)
    {
        digitalWrite(5, LOW);
        delay(1500);
        digitalWrite(5, HIGH);
        delay(1500);
    }
    else if (air >= 201.00 && air <= 300.00)
    {
        digitalWrite(5, LOW);
        delay(500);
        digitalWrite(5, HIGH);
        delay(500);
    }
    else if (air >= 301.00 && air <= 500.00)
    {
        digitalWrite(5, LOW);
        // delay(2500);
    }
    else
    {
        // digitalWrite(BUILTIN_LED, LOW);
        // digitalWrite(5, LOW);
        // delay(1000);
        digitalWrite(5, HIGH);
        delay(1000);


    }
}
