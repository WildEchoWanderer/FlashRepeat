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
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, HIGH);
    delay(1000); // 1 Sekunde warten
    digitalWrite(blueLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);

    
}

void loop() {

   
}