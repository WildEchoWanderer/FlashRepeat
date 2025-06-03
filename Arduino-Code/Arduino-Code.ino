/*
Dieses skript steuert ein Simon game.
*/

#define blueButton 13
#define redButton 12
#define yellowButton 11
#define greenButton 10
#define blueLED 7
#define redLED 6
#define yellowLED 5
#define greenLED 4

//Variablen erstellen
int gameModeSelect = 0;  // wählt den spielmodus


void setup() {
   // Richtet die digitalen E/A-Pins ein
   pinMode(blueButton, INPUT);
   pinMode(redButton, INPUT);
   pinMode(yellowButton, INPUT)
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
   delay(1000); // 1 Sekunde warten
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
      }
   }
   
   //Spielstart gemäss dem gewählten Modus
   if (gameModeSelect > 0) {
      Serial.println("Starting Game...");
      // Starte Modus 1 Normal Game Mode
      if (gameModeSelect == 1) {
         Serial.println("Starting Normal Game Mode");
         // Hier kann der Code für den normalen Spielmodus eingefügt werden
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