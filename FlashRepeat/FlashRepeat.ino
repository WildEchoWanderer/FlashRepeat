/*
Dieses skript steuert ein Simon game für einen Arduino.
Es ermöglicht die Auswahl von verschiedenen Spielmodi durch das gleichzeitige Drücken von Tasten.
Die Tasten sind wie folgt zugeordnet:
- Blau & Rot: Normales Spiel
- Gelb & Grün: Schwerer Modus
- Blau & Gelb: N-Back Modus (2 Back)
- Rot & Grün: N-Back Modus (3 Back)

Testbrancheintrag- merge test

Main Branch Test
*/

#include <Arduino.h>

// Pin-Definitionen für Tasten und LEDs
#define blueButton 13
#define redButton 12
#define yellowButton 11
#define greenButton 10
#define blueLED 7
#define redLED 6
#define yellowLED 5
#define greenLED 4

// Spiel-Variablen 
#define MAX_LEVEL 100
int spielSequenz[MAX_LEVEL];
int aktuellesLevel = 0;
int buttonPins[] = {blueButton, redButton, yellowButton, greenButton};
int ledPins[] = {blueLED, redLED, yellowLED, greenLED};
int gameModeSelect = 0;  // wählt den Spielmodus

// Hilfsfunktionen für das Spiel

// Zeigt die aktuelle Sequenz an, indem die LEDs nacheinander leuchten
void zeigeSequenz() {
  for(int i = 0; i < aktuellesLevel; i++) {
    digitalWrite(ledPins[spielSequenz[i]], HIGH);
    delay(500);
    digitalWrite(ledPins[spielSequenz[i]], LOW);
    delay(250);
  }
}

// Wartet auf die Eingabe des Spielers und gibt den Index der gedrückten Taste zurück
int warteAufEingabe() {
  while(true) {
    for(int i = 0; i < 4; i++) {
      if(digitalRead(buttonPins[i]) == HIGH) {
        digitalWrite(ledPins[i], HIGH);
        delay(200);
        digitalWrite(ledPins[i], LOW);
        return i;
      }
    }
  }
}

// Funktion, die aufgerufen wird, wenn der Spieler das Spiel verliert
void spielVerloren() {
  // Alle LEDs dreimal blinken lassen
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(200);
    for(int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(200);
  }
  Serial.print("Spiel vorbei! Erreichte Stufe: ");
  Serial.println(aktuellesLevel - 1);
}

void setup() {
   // Richtet die digitalen E/A-Pins ein
   pinMode(blueButton, INPUT);
   pinMode(redButton, INPUT);
   pinMode(yellowButton, INPUT);
   pinMode(greenButton, INPUT);
   pinMode(blueLED, OUTPUT);
   pinMode(redLED, OUTPUT);
   pinMode(yellowLED, OUTPUT);
   pinMode(greenLED, OUTPUT);

   // Legt den Anfangszustand der LEDs fest (Selbsttest)
   digitalWrite(blueLED, HIGH);
   delay(100);
   digitalWrite(redLED, HIGH);
   delay(100);
   digitalWrite(yellowLED, HIGH);
   delay(100);
   digitalWrite(greenLED, HIGH);
   delay(500); // 0.5 Sekunde warten
   digitalWrite(blueLED, LOW);
   delay(100);
   digitalWrite(redLED, LOW);
   delay(100);
   digitalWrite(yellowLED, LOW);
   delay(100);
   digitalWrite(greenLED, LOW);

   // Serial Monitor initialisieren
   Serial.begin(9600);
   Serial.println("Simon Game Ready!");
   Serial.println("Press a button to start the game.");
   // Spiel bereit für Modusauswahl

}

void loop() {
   // Auswahl des Spielmodus mit dem gleichzeiten drücken von Tasten für 50ms. Die Wahl wird bestätigt mit dem Leuchten der korrespondierenden LED.
   // Blau & Rot = Normales Spiel
   if (digitalRead(blueButton) == HIGH && digitalRead(redButton) == HIGH) {
      delay(50); // Entprellung
      if (digitalRead(blueButton) == HIGH && digitalRead(redButton) == HIGH) {
         delay(50); // Entprellung
         gameModeSelect = 1; // Normales Spiel
         digitalWrite(blueLED, HIGH);
         digitalWrite(redLED, HIGH);
         Serial.println("Normal Game Mode Selected");
         delay(2000); // 2 Sekunde warten, um die Auswahl zu sehen
         digitalWrite(blueLED, LOW);
         digitalWrite(redLED, LOW);
      }
   }
   // Gelb & Grün = Schwerer Modus
   if (digitalRead(yellowButton) == HIGH && digitalRead(greenButton) == HIGH) {
      delay(50); // Entprellung
      if (digitalRead(yellowButton) == HIGH && digitalRead(greenButton) == HIGH) {
         delay(50); // Entprellung
         gameModeSelect = 2; // Schwerer Modus
         digitalWrite(yellowLED, HIGH);
         digitalWrite(greenLED, HIGH);
         Serial.println("Hard Game Mode Selected");
         delay(2000); // 2 Sekunde warten, um die Auswahl zu sehen
         digitalWrite(yellowLED, LOW);
         digitalWrite(greenLED, LOW);
      }
   }
   // Blau & Gelb = N-Back Modus (2 Back)
   if (digitalRead(blueButton) == HIGH && digitalRead(yellowButton) == HIGH) {
      delay(50); // Entprellung
      if (digitalRead(blueButton) == HIGH && digitalRead(yellowButton) == HIGH) {
         delay(50); // Entprellung
         gameModeSelect = 3; // N-Back Modus
         digitalWrite(blueLED, HIGH);
         digitalWrite(yellowLED, HIGH);
         Serial.println("N-Back Game Mode Selected");
         delay(2000); // 2 Sekunde warten, um die Auswahl zu sehen
         digitalWrite(blueLED, LOW);
         digitalWrite(yellowLED, LOW);
      }
   }
   // Rot & Grün = N-Back Modus (3 Back)
   if (digitalRead(redButton) == HIGH && digitalRead(greenButton) == HIGH) {
      delay(50); // Entprellung
      if (digitalRead(redButton) == HIGH && digitalRead(greenButton) == HIGH) {
         delay(50); // Entprellung
         gameModeSelect = 4; // N-Back Modus
         digitalWrite(redLED, HIGH);
         digitalWrite(greenLED, HIGH);
         Serial.println("N-Back Game Mode Selected");
         delay(2000); // 2 Sekunde warten, um die Auswahl zu sehen
         digitalWrite(redLED, LOW);
         digitalWrite(greenLED, LOW);
      }
   }
   
   //Spielstart gemäss dem gewählten Modus
   if (gameModeSelect > 0) {
      Serial.println("Starting Game...");
      // Starte Modus 1 Normal Game Mode
      if (gameModeSelect == 1) {
        Serial.println("Starting Normal Game Mode");
        
        // Sequenz zurücksetzen
        aktuellesLevel = 0;
        randomSeed(millis());  // Zufallsgenerator initialisieren
        
        while(true) {
          // Neue Sequenz hinzufügen
          spielSequenz[aktuellesLevel] = random(4);  // 0-3 für die vier Farben
          aktuellesLevel++;
          
          // Sequenz anzeigen
          delay(1000);
          zeigeSequenz();
          
          // Spielereingabe abwarten und überprüfen
          for(int i = 0; i < aktuellesLevel; i++) {
            int spielerEingabe = warteAufEingabe();
            
            // Prüfen ob Eingabe korrekt ist
            if(spielerEingabe != spielSequenz[i]) {
              spielVerloren();
              return;  // Zurück zur Hauptschleife
            }
            delay(200);
          }
          
          // Spieler hat es geschafft, kurzes Erfolgssignal
          for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], HIGH);
          }
          delay(250);
          for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], LOW);
          }
          
          // Prüfen ob maximales Level erreicht wurde
          if(aktuellesLevel >= MAX_LEVEL) {
            Serial.println("Gewonnen! Maximale Sequenz erreicht!");
            return;
          }
        }
      }




      // Hier kann der Code für den gewählten Spielmodus eingefügt werden
      // Zum Beispiel: startGame(gameModeSelect);
      // Nach dem Starten des Spiels, die LEDs zurücksetzen
      digitalWrite(blueLED, LOW);
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
      gameModeSelect = 0; // Zurücksetzen der Moduswahl
   }

}