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
#define buzzerPin 8
#define NOTE_G4  392
#define NOTE_E4  330
#define NOTE_C4  262
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523


// Spiel-Variablen 
#define MAX_LEVEL 100
int spielSequenz[MAX_LEVEL];
int aktuellesLevel = 0;
int buttonPins[] = {blueButton, redButton, yellowButton, greenButton};
int ledPins[] = {blueLED, redLED, yellowLED, greenLED};
int buzzer = buzzerPin;
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
  // Verloren-Melodie abspielen (absteigende Melodie mit Trauer-Effekt)
  tone(buzzer, NOTE_G4, 300);  // Startet hoch
  delay(300);
  tone(buzzer, NOTE_E4, 300);  // Geht runter
  delay(300);
  tone(buzzer, NOTE_C4, 300);  // Noch tiefer
  delay(300);
  tone(buzzer, NOTE_B4, 150);  // Kurzer hoher Ton
  delay(150);
  tone(buzzer, NOTE_G4, 150);  // Zurück nach unten
  delay(150);
  tone(buzzer, NOTE_E4, 500);  // Langer Schlusston
  delay(500);
  noTone(buzzer);
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

// Funktion die aufgerufen wird wenn der Spieler gewonnen hat
void spielGewonnen() {
  // Erfolgs-Melodie abspielen (aufsteigende Melodie)
  tone(buzzerPin, NOTE_C4, 150);
  delay(150);
  tone(buzzerPin, NOTE_E4, 150);
  delay(150);
  tone(buzzerPin, NOTE_G4, 150);
  delay(150);
  tone(buzzerPin, NOTE_C5, 400);
  delay(400);
  noTone(buzzerPin);
  
  // Lauflicht 5 mal vor und zurück
  for(int durchlauf = 0; durchlauf < 5; durchlauf++) {
    // Vorwärts
    for(int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], HIGH);
      delay(100);
      digitalWrite(ledPins[i], LOW);
    }
    // Rückwärts
    for(int i = 2; i >= 0; i--) {
      digitalWrite(ledPins[i], HIGH);
      delay(100);
      digitalWrite(ledPins[i], LOW);
    }
  }
  
  Serial.println("Gewonnen! Maximale Sequenz erreicht!");
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
   pinMode(buzzerPin, OUTPUT);

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
   delay(100);
   tone(buzzerPin, NOTE_C4, 200);
   delay(200);
   tone(buzzerPin, NOTE_E4, 200);
   delay(200);
   tone(buzzerPin, NOTE_G4, 200);
   delay(200);
   tone(buzzerPin, NOTE_E4, 200);
   delay(200);
   tone(buzzerPin, NOTE_C4, 200);
   delay(200);
   noTone(buzzerPin);



   // Serial Monitor initialisieren
   Serial.begin(9600);
   Serial.println("Simon Game Ready!");
   Serial.println("Press two buttons to start the game.");
   // Spiel bereit für Modusauswahl

}

void loop() {
   // Prüfen ob die blaue Taste für 5 Sekunden gehalten wird. Geht in den Gewonnen-Modus.
   if (digitalRead(blueButton) == HIGH) {
      digitalWrite(blueLED, HIGH);  // Visuelle Rückmeldung
      delay(5000);  // 3 Sekunden warten
      if (digitalRead(blueButton) == HIGH) {  // Prüfen ob Taste noch gedrückt ist
         digitalWrite(blueLED, LOW);
         // Bestätigungsblinken mit allen LEDs
         for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], HIGH);
         }
         delay(500);
         for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], LOW);
         }
         spielGewonnen(); // Gewonnen-Sequenz abspielen
         while(digitalRead(blueButton) == HIGH) {  // Warten bis Taste losgelassen wird
            delay(50);
         }
         return;
      }
      digitalWrite(blueLED, LOW);
   }

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
            spielGewonnen();
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
      digitalWrite(buzzerPin, LOW);
      gameModeSelect = 0; // Zurücksetzen der Moduswahl
   }

}