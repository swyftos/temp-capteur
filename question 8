#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <SD.h>
#include <RTClib.h>
#include <DHT.h>

// Déclaration de l'écran tactile et de l'afficheur TFT
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Déclaration du capteur DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Déclaration de l'horloge RTC
RTC_DS1307 RTC;

// Calibration du tactile
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

void bouton()
{
  tft.fillScreen(ILI9341_RED);

  const char* labels[] = {"DATE/HEURE", "TEMPERATURE", "HUMIDITE", "PRESSION", "AUTRE..."};
  
  for (int i = 0; i < 5; i++) {
    tft.fillRect(40, 20 + (i * 60), 160, 40, ILI9341_WHITE);
    tft.setCursor(60, 33 + (i * 60));
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setTextWrap(0);
    tft.print(labels[i]);
  }
}

// Affichage de la date et l'heure
void heure_date()
{
  DateTime now = RTC.now();

  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(3);

  tft.setCursor(50, 100);
  if (now.day() < 10) tft.print("0");
  tft.print(now.day());
  tft.print("/");
  if (now.month() < 10) tft.print("0");
  tft.print(now.month());
  tft.print("/");
  tft.print(now.year());

  tft.setCursor(60, 150);
  if (now.hour() < 10) tft.print("0");
  tft.print(now.hour());
  tft.print(":");
  if (now.minute() < 10) tft.print("0");
  tft.print(now.minute());
  tft.print(":");
  if (now.second() < 10) tft.print("0");
  tft.print(now.second());
}

// Affichage de la température
void afficher_temperature()
{
  float temperature = dht.readTemperature();
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);

  if (isnan(temperature)) {
    tft.print("Erreur de lecture");
  } else {
    tft.print("Temperature: ");
    tft.print(temperature);
    tft.print(" C");
  }
}

// Affichage de l'humidité
void afficher_humidite()
{
  float humidite = dht.readHumidity();
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);

  if (isnan(humidite)) {
    tft.print("Erreur de lecture");
  } else {
    tft.print("Humidite: ");
    tft.print(humidite);
    tft.print(" %");
  }
}

// Fonction pression (placeholder)
void afficher_pression()
{
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Pression: Donnees non dispo.");
}

// Fonction autre (placeholder)
void afficher_autre()
{
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Autre: Fonction a definir.");
}

void setup()
{
  Serial.begin(9600);
  tft.begin();
  RTC.begin();
  dht.begin();

  if (!ts.begin()) {
    Serial.println("Unable to start touchscreen.");
  } else {
    Serial.println("Touchscreen started.");
  }

  // Réglage automatique de la date et l'heure
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

  bouton();
}

void loop()
{
  if (!ts.bufferEmpty())
  {
    TS_Point p = ts.getPoint();
    int x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if ((x > 40) && (x < 200))
    {
      if ((y > 20) && (y < 60)) // DATE/HEURE
      {
        if (ts.touched())
        {
          heure_date();
          delay(2000);
          bouton();
        }
      }
      else if ((y > 80) && (y < 120)) // TEMPERATURE
      {
        if (ts.touched())
        {
          afficher_temperature();
          delay(2000);
          bouton();
        }
      }
      else if ((y > 140) && (y < 180)) // HUMIDITE
      {
        if (ts.touched())
        {
          afficher_humidite();
          delay(2000);
          bouton();
        }
      }
      else if ((y > 200) && (y < 240)) // PRESSION
      {
        if (ts.touched())
        {
          afficher_pression();
          delay(2000);
          bouton();
        }
      }
      else if ((y > 260) && (y < 300)) // AUTRE...
      {
        if (ts.touched())
        {
          afficher_autre();
          delay(2000);
          bouton();
        }
      }
    }
  }
}



