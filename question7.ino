#include <Wire.h>
#include "rgb_lcd.h"
#include <RTClib.h>
#include "DHT.h"

int X;
int val_T, val_H;

// déclaration d'une horloge
RTC_DS1307 RTC; // Class RTC_DS1307

// déclaration d'un LCD
rgb_lcd lcd; // Class LCD

// déclaration d'un capteur de température de type DHT22 sur l'entrée analogique A0
DHT dht(A0, DHT22);

void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    dht.begin();
    
    RTC.begin(); // Démarrage de la librairie RTClib.h 
    RTC.adjust(DateTime(2016, 9, 28, 9, 42, 0)); // Initialisation de l'horloge
}

void loop()
{
    for (X = 0; X < 5; X++)
    {
        lcd.clear();
        DateTime now = RTC.now(); // récupération permanente de l'heure
        lcd.setCursor(2, 0);
        if (now.day() < 10) { lcd.print("0"); } 
        lcd.print(now.day());
        lcd.print(":");
        if (now.month() < 10) { lcd.print("0"); }
        lcd.print(now.month());
        lcd.print(":");
        lcd.print(now.year());
        lcd.setCursor(2, 1);
        if (now.hour() < 10) { lcd.print("0"); }
        lcd.print(now.hour());
        lcd.print(":");
        if (now.minute() < 10) { lcd.print("0"); }
        lcd.print(now.minute());
        lcd.print(":");
        if (now.second() < 10) { lcd.print("0"); }
        lcd.print(now.second());
        delay(1000);
    }

    lcd.clear();
    
    // Lecture et affichage de la température et de l'humidité pendant 5 secondes
    for (X = 0; X < 5; X++)
    {
        float temperature = dht.readTemperature(); // Lecture de la température
        float humidity = dht.readHumidity(); // Lecture de l'humidité

        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature);
        lcd.print(" C");

        lcd.setCursor(0, 1);
        lcd.print("Hum: ");
        lcd.print(humidity);
        lcd.print(" %");

        delay(1000); // Attente d'une seconde entre chaque affichage
    }
    delay(5000); // Pause de 5 secondes avant de revenir à l'affichage de l'heure
}
