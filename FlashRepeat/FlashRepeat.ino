/*
Dieses skript steuert ein Simon game für einen Arduino.
Es ermöglicht die Auswahl von verschiedenen Spielmodi durch das gleichzeitige Drücken von Tasten.
Die Tasten sind wie folgt zugeordnet:
- Blau & Rot: Normales Spiel
- Gelb & Grün: Schwerer Modus
- Blau & Gelb: N-Back Modus (2 Back) (noch nicht implementiert)
- Rot & Grün: N-Back Modus (3 Back) (noch nicht implementiert)

*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

#define MAX_SEQUENCE_LENGTH 100  // Maximale Länge für das Sequenz-Array
#define MIN_LEVEL 9  // Minimale Sequenzlänge

// Pin-Definitionen für Tasten und LEDs
#define blueButton 13
#define redButton 12
#define yellowButton 11
#define greenButton 10
#define blueLED 9
#define redLED 8
#define yellowLED 7
#define greenLED 6
#define buzzerPin 3
#define NOTE_G4  392
#define NOTE_E4  330
#define NOTE_C4  262
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Tonzuordnung für Farben
#define NOTE_BLU 196   // Blau: G3
#define NOTE_GRN 262   // Grün: C4
#define NOTE_YEL 330   // Gelb: E4
#define NOTE_RED 523   // Rot: C5

// Spiel-Variablen 
int spielSequenz[MAX_SEQUENCE_LENGTH];
int aktuellesLevel = 0;
int buttonPins[] = {blueButton, redButton, greenButton, yellowButton};
int ledPins[] = {blueLED, redLED, greenLED, yellowLED };
int buzzer = buzzerPin;
int gameModeSelect = 0;  // wählt den Spielmodus

// Variablen für die Sequenzlänge
int maxLevel = 20; // Standardwert für die maximale Sequenzlänge

// Display initialisieren (Adafruit-Library, Adresse 0x20, 16x2)
Adafruit_LiquidCrystal lcd(0); // Wenn keine Jumper auf A0-A2 gelötet sind

// Hilfsfunktionen für das Spiel

// Funktionsprototypen
void ledMitTon(int farbe, int dauer = 500);

// Zeigt die aktuelle Sequenz an, indem die LEDs nacheinander leuchten
void zeigeSequenz() {
  for(int i = 0; i < aktuellesLevel; i++) {
    ledMitTon(spielSequenz[i]);
  }
}

// Wartet auf die Eingabe des Spielers und gibt den Index der gedrückten Taste zurück
int warteAufEingabe() {
  while(true) {
    for(int i = 0; i < 4; i++) {
      if(digitalRead(buttonPins[i]) == HIGH) {
        ledMitTon(i, 200);
        return i;
      }
    }
  }
}

// Funktion, die aufgerufen wird, wenn der Spieler das Spiel verliert
void spielVerloren() {
  // Sofort Display aktualisieren
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Verloren!");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(aktuellesLevel-1);
  
  // Verloren-Melodie und LED-Animation gleichzeitig
  unsigned long noteStart = millis();
  int currentNote = 0;
  int notes[] = {NOTE_G4, NOTE_E4, NOTE_C4, NOTE_B4, NOTE_G4, NOTE_E4};
  int noteDurations[] = {300, 300, 300, 150, 150, 500};
  int totalNotes = 6;
  
  // Drei Durchläufe für LED-Blinken
  for(int i = 0; i < 3; i++) {
    // LEDs an
    for(int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    
    // Während LED an ist, spiele Töne
    if(currentNote < totalNotes) {
      tone(buzzer, notes[currentNote], noteDurations[currentNote]);
      currentNote++;
    }
    
    delay(200);
    
    // LEDs aus
    for(int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(200);
  }
  noTone(buzzer);
  for(int i=0; i<16; i++) {
    lcd.setCursor(i,1);
    lcd.print("-");
    delay(30);
  }
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(aktuellesLevel-1);
  delay(1500);
  delay(1000);
  gameModeSelect = 0; // Zurück zum Auswahlmodus
}

// Funktion die aufgerufen wird wenn der Spieler gewonnen hat
void spielGewonnen() {
  // Erfolgs-Melodie abspielen
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
  
  Serial.println("Gewonnen! Maximale Sequenz erreicht! Glückwunsch!");
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gewonnen!");
  for(int i=0; i<16; i++) {
    lcd.setCursor(i,1);
    lcd.print("*");
    delay(30);
  }
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(aktuellesLevel);
  delay(1500);
  delay(1000);
  gameModeSelect = 0; // Zurück zum Auswahlmodus
}

// Hilfsfunktion: LED mit Ton
void ledMitTon(int farbe, int dauer = 500) {
  int note = 0;
  switch(farbe) {
    case 0: note = NOTE_BLU; break; // Blau
    case 1: note = NOTE_RED; break; // Rot
    case 2: note = NOTE_YEL; break; // Gelb
    case 3: note = NOTE_GRN; break; // Grün
  }
  digitalWrite(ledPins[farbe], HIGH);
  tone(buzzer, note, dauer);
  delay(dauer);
  digitalWrite(ledPins[farbe], LOW);
  noTone(buzzer);
  delay(100);
}

void setup() {
   Serial.begin(9600);
   Serial.println("FlashRepeat Ready!");
   
   // Display initialisieren
   if (!lcd.begin(16, 2)) {
      Serial.println("LCD init failed! Check wiring & address.");
      while(1);
   }
   Serial.println("LCD initialized successfully.");
   
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

   // Display Startbildschirm
   lcd.clear();
   lcd.print("FlashRepeat!");
   lcd.setCursor(0, 1);
   lcd.print("Bereit...");

   // Legt den Anfangszustand der LEDs fest (Selbsttest)
   digitalWrite(blueLED, HIGH);
   delay(100);
   digitalWrite(redLED, HIGH);
   delay(100);
   digitalWrite(greenLED, HIGH);
   delay(100);
   digitalWrite(yellowLED, HIGH);
   delay(500);
   digitalWrite(blueLED, LOW);
   delay(100);
   digitalWrite(redLED, LOW);
   delay(100);
   digitalWrite(greenLED, LOW);
   delay(100);
   digitalWrite(yellowLED, LOW);
   delay(100);
   
   // Startmelodie
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
   
   delay(1000);
}

void loop() {
   // Zeige Startbildschirm wenn kein Spielmodus aktiv ist
   if (gameModeSelect == 0) {
      static unsigned long lastUpdate = 0;
      if (millis() - lastUpdate >= 3000) { // Alle 3 Sekunden aktualisieren
         lcd.clear();
         lcd.print("FlashRepeat!");
         lcd.setCursor(0,1);
         lcd.print("Waehle Modus");
         lastUpdate = millis();
      }
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
         lcd.clear();
         lcd.print("Normal Mode");
         lcd.setCursor(0, 1);
         lcd.print("Selected!");
         delay(2000);
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
         lcd.clear();
         lcd.print("Hard Mode");
         lcd.setCursor(0, 1);
         lcd.print("Selected!");
         delay(2000);
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
         lcd.clear();
         lcd.print("2-Back Mode");
         lcd.setCursor(0, 1);
         lcd.print("Selected!");
         delay(2000);
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
         lcd.clear();
         lcd.print("3-Back Mode");
         lcd.setCursor(0, 1);
         lcd.print("Selected!");
         delay(2000);
         digitalWrite(redLED, LOW);
         digitalWrite(greenLED, LOW);
      }
   }
   
   // Konfiguration der Sequenzlänge NACH der Moduswahl, aber VOR dem Spielstart
   if (gameModeSelect > 0) {
      boolean configuring = true;
      unsigned long lastInputTime = millis();
      int lastLevelDisplay = maxLevel;
      lcd.clear();
      lcd.print("Max. Level: ");
      lcd.print(maxLevel);
      lcd.setCursor(0,1);
      lcd.print("B10 G1 R-1 Y-10");
      
      while(configuring) {
        bool changed = false;
        if(digitalRead(blueButton) == HIGH) {
          maxLevel = min(maxLevel + 10, MAX_SEQUENCE_LENGTH);
          lastInputTime = millis();
          changed = true;
          delay(200);
        }
        if(digitalRead(yellowButton) == HIGH) {
          maxLevel = max(maxLevel - 10, MIN_LEVEL);
          lastInputTime = millis();
          changed = true;
          delay(200);
        }
        if(digitalRead(redButton) == HIGH) {
          maxLevel = min(maxLevel + 1, MAX_SEQUENCE_LENGTH);
          lastInputTime = millis();
          changed = true;
          delay(200);
        }
        if(digitalRead(greenButton) == HIGH) {
          maxLevel = max(maxLevel - 1, MIN_LEVEL);
          lastInputTime = millis();
          changed = true;
          delay(200);
        }
        // Anzeige nur aktualisieren, wenn sich der Wert geändert hat
        if (changed && maxLevel != lastLevelDisplay) {
          lcd.setCursor(11,0);
          lcd.print("    "); // Lösche alte Zahl
          lcd.setCursor(11,0);
          lcd.print(maxLevel);
          lastLevelDisplay = maxLevel;
        }
        // Prüfe ob 5 Sekunden ohne Eingabe vergangen sind
        if (millis() - lastInputTime > 5000) {
          configuring = false;
        }
      }
      // Bestätigung anzeigen
      lcd.clear();
      lcd.print("Laenge: ");
      lcd.print(maxLevel);
      delay(1000);
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
          
          // Display auf Score-Anzeige umstellen
          lcd.clear();
          lcd.print("Level: ");
          lcd.print(aktuellesLevel);
          
          // Sequenz anzeigen
          delay(1000);
          zeigeSequenz();
          
          // Spielereingabe abwarten und überprüfen
          for(int i = 0; i < aktuellesLevel; i++) {
            int spielerEingabe = warteAufEingabe();
            
            // Prüfen ob Eingabe korrekt ist
            if(spielerEingabe != spielSequenz[i]) {
              spielVerloren();
              return;  // Zurück zur Modusauswahl
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
          
          // Nach jedem richtigen Durchgang Punktestand aktualisieren
          lcd.setCursor(0,1);
          lcd.print("Score: ");
          lcd.print(aktuellesLevel);
          lcd.print("   "); // Leerzeichen zum Löschen
          
          // Prüfen ob maximales Level erreicht wurde
          if(aktuellesLevel >= maxLevel) {
            spielGewonnen();
            return;
          }
        }
      }
      
      //Starte Modus 2 Schwerer Modus (schnellere Anzeigedauer der Sequenz)
      if (gameModeSelect == 2) {
        Serial.println("Starting Hard Game Mode");
        
        // Sequenz zurücksetzen
        aktuellesLevel = 0;
        randomSeed(millis());  // Zufallsgenerator initialisieren
        
        while(true) {
          // Neue Sequenz hinzufügen
          spielSequenz[aktuellesLevel] = random(4);  // 0-3 für die vier Farben
          aktuellesLevel++;
          
          // Sequenz anzeigen (schneller)
          delay(200);
          zeigeSequenz();
          
          // Spielereingabe abwarten und überprüfen
          for(int i = 0; i < aktuellesLevel; i++) {
            int spielerEingabe = warteAufEingabe();
            
            // Prüfen ob Eingabe korrekt ist
            if(spielerEingabe != spielSequenz[i]) {
              spielVerloren();
              return;  // Zurück zur Modusauswahl
            }
            delay(100); // Kürzere Wartezeit für schwereren Modus
          }
          
          // Spieler hat es geschafft, kurzes Erfolgssignal
          for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], HIGH);
          }
          delay(200);
          for(int i = 0; i < 4; i++) {
            digitalWrite(ledPins[i], LOW);
          }
          
          // Nach jedem richtigen Durchgang Punktestand aktualisieren
          lcd.setCursor(0,1);
          lcd.print("Score: ");
          lcd.print(aktuellesLevel);
          lcd.print("   "); // Leerzeichen zum Löschen
          
          // Prüfen ob maximales Level erreicht wurde
          if(aktuellesLevel >= maxLevel) {
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