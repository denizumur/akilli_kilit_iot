#define doo  1000// do komutu ile karismasin diye doo yazdim (kalin do)
#define re   1122
#define mi   1260
#define fa   1335
#define sol  1498
#define la   1681
#define si   1887
#define dom  2000// do komutu ile karismasin diye dom yazdim (ince do))

int melodi[] =     {re,mi,fa,sol,fa,sol,sol,sol, fa, sol,sol,sol ,fa, mi, re,mi,fa,sol,fa,sol,sol,sol, fa, sol,sol,sol ,fa, mi,   re, mi ,fa,sol,mi,fa,re,mi,doo,doo,fa,mi,re,re,re, mi ,fa,sol,mi,fa,re,mi,doo,doo,fa,mi,re,re };
int notasuresi[] = {4,  4,  4,  4, 4,  4,4,  3 ,4,4,4,4,4,2,4,  4,  4,  4, 4,  4,4,  3 ,4,4,4,4,4,2       ,4,4,4,4,4,4,4,4,4,4,4,4,2,2,    4,4,4,4,4,4,4,4,4,4,4,4,2,2};// her notanin süresini array yaptim
int tempo = 1200;


#define BLYNK_TEMPLATE_ID "TMPL6nlYKG6GK"
#define BLYNK_TEMPLATE_NAME "esp8266"
#define BLYNK_AUTH_TOKEN "VuFRWcELzZWAZurCuY9hODwRrMV5iTJV"

#define button D1
#define buzzer D2

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <FirebaseESP8266.h>




#include <time.h> // Zaman bilgisi almak için

const char* ntpServer = "pool.ntp.org"; // NTP sunucusu
const long gmtOffset_sec = 10800;      // GMT+3 için offset (Türkiye için)
const int daylightOffset_sec = 0;     // Yaz saati uygulanmıyorsa 0

String currentTime; 
String currentDate; 



// Firebase konfigürasyonu
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Firebase nesnesi
FirebaseData firebaseData;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "c";  // WiFi SSID
char pass[] = "124578de";   // WiFi Şifresi
Servo myServo;
int buton=0;
int totalBellCount = 0;
int sayac=0;
// Sanal pin oluşturma
BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
   myServo.write(buton);
  if(buton == 0){
    buton = 180;
  }
  else if(buton == 180){
    buton = 0;
  }
 
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");


    firebaseConfig.host = "kameralikilit-default-rtdb.firebaseio.com"; // Realtime Database URL
  firebaseConfig.api_key = "AIzaSyAC8swshrmegCe79j93bQ7CghHd53QCS2c";      
  firebaseAuth.user.email = "melihdede00@gmail.com"; // Firebase kullanıcı e-postası
  firebaseAuth.user.password = "123456";   
 Firebase.begin(&firebaseConfig, &firebaseAuth);
 
  // Firebase'den mevcut zil sayısını al
  if (Firebase.getInt(firebaseData, "/totalBellCount")) {
    if (firebaseData.dataType() == "int") {
      totalBellCount = firebaseData.intData();
    }
  } else {
    Serial.println("Firebase read failed: " + firebaseData.errorReason());
  }




   configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Zaman senkronize ediliyor...");
  delay(2000);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Zaman alınamadı!");
    return;
  }
  //Serial.println(&timeinfo, "Zaman: %Y-%m-%d %H:%M:%S");





  pinMode(button,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(D0,OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud",80);
  myServo.attach(D0);
  
}

void loop() {
  Blynk.run();
  
  if (digitalRead(button) == 1) {
     for (int Nota = 0; Nota <10; Nota++) 
    {
      int sure = tempo/notasuresi[Nota];
     tone(D2, melodi[Nota],sure);// 8 numarali bacaktan notalarin frekanslarini nota uzunluklarina göre gönder
     delay(sure*1.2);// notalar arasinda biraz beklesin. Eger 1 ile çarparsaniz notalar birbirine bitisik olur
    }


    totalBellCount++;
    Firebase.setInt(firebaseData, "/totalBellCount", totalBellCount);

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStr[20];
      char dateStr[11];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
      strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &timeinfo);
      currentTime = Strin…