#include <Arduino.h>
#include <ESP32Encoder.h>

// Definizione pin per TB6612FNG
#define PWMA 25
#define AIN1 26
#define AIN2 27
#define STBY 14

// Pin Encoder
#define ENCODER_C1 19
#define ENCODER_C2 18

// Configurazione PWM
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// Oggetto encoder
ESP32Encoder encoder;

// PPR encoder JGA25-371: 12 PPR in quadratura x4
const int PPR = 12 * 4;

// RPM no-load dichiarato dal datasheet
const float RPM_NO_LOAD_DICHIARATO = 100.0;

// Variabili globali
float rapportoCalcolato = 0;
bool calibrazioneCompletata = false;

// Funzioni controllo motore
void motorAvanti(int velocita) {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CHANNEL, velocita);
}

void motorStop() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CHANNEL, 0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n╔════════════════════════════════════════════════════╗");
  Serial.println("║  CALCOLO AUTOMATICO RAPPORTO DI RIDUZIONE         ║");
  Serial.println("║  Motore JGA25-371 con Encoder                     ║");
  Serial.println("╚════════════════════════════════════════════════════╝\n");

  // Configurazione PWM
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMA, PWM_CHANNEL);

  // Configurazione pin motore
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Configurazione encoder
  ESP32Encoder::useInternalWeakPullResistors = UP;
  encoder.attachFullQuad(ENCODER_C1, ENCODER_C2);
  encoder.clearCount();

  motorStop();

  Serial.println("✓ Sistema inizializzato");
  Serial.println("✓ RPM no-load dichiarato: " + String(RPM_NO_LOAD_DICHIARATO) + " RPM\n");

  // Test encoder
  Serial.println("→ Test encoder in corso...");
  Serial.println("  (Ruota manualmente l'albero se vuoi verificare)");
  delay(2000);

  long testCount = abs(encoder.getCount());
  if (testCount > 0) {
    Serial.println("✓ Encoder funzionante (rilevati " + String(testCount) + " impulsi)\n");
  } else {
    Serial.println("✓ Encoder pronto (nessun movimento rilevato)\n");
  }

  encoder.clearCount();

  // Avvio calibrazione automatica
  Serial.println("╔════════════════════════════════════════════════════╗");
  Serial.println("║  AVVIO CALIBRAZIONE AUTOMATICA                    ║");
  Serial.println("╚════════════════════════════════════════════════════╝\n");
  Serial.println("⚠️  ASSICURATI CHE IL MOTORE SIA IN NO-LOAD");
  Serial.println("    (nessun carico, ruota libero)\n");
  Serial.println("Avvio tra 5 secondi...\n");

  for(int i = 5; i > 0; i--) {
    Serial.println("  " + String(i) + "...");
    delay(1000);
  }

  Serial.println("\n→ Motore in funzione per 10 secondi A VELOCITÀ MASSIMA...\n");

  // Avvia il motore a velocità MASSIMA per calibrazione corretta
  encoder.clearCount();
  unsigned long tempoInizio = millis();
  motorAvanti(255);  // VELOCITÀ MASSIMA per no-load a 100 RPM

  // Raccolta dati per 10 secondi
  int campioni = 0;
  float sommaRPM = 0;
  unsigned long ultimaMisura = millis();
  long impulsiPrecedenti = 0;

  while (millis() - tempoInizio < 10000) {
    // Calcola RPM ogni 200ms
    if (millis() - ultimaMisura >= 200) {
      long impulsiAttuali = encoder.getCount();
      long deltaImpulsi = impulsiAttuali - impulsiPrecedenti;
      float deltaTempoMinuti = 200.0 / 60000.0;

      float rpmMotore = (deltaImpulsi / (float)PPR) / deltaTempoMinuti;

      // Scarta i primi campioni (accelerazione)
      if (millis() - tempoInizio > 2000 && abs(rpmMotore) > 100) {
        sommaRPM += abs(rpmMotore);
        campioni++;

        Serial.print("  Campione #");
        Serial.print(campioni);
        Serial.print(": RPM Motore = ");
        Serial.println(abs(rpmMotore), 0);
      }

      impulsiPrecedenti = impulsiAttuali;
      ultimaMisura = millis();
    }
    delay(10);
  }

  motorStop();

  // Calcolo rapporto
  if (campioni > 0) {
    float rpmMotoreMedio = sommaRPM / campioni;
    rapportoCalcolato = rpmMotoreMedio / RPM_NO_LOAD_DICHIARATO;
    calibrazioneCompletata = true;

    Serial.println("\n╔════════════════════════════════════════════════════╗");
    Serial.println("║  CALIBRAZIONE COMPLETATA                          ║");
    Serial.println("╚════════════════════════════════════════════════════╝\n");

    Serial.println("📊 RISULTATI:");
    Serial.println("  • Campioni raccolti: " + String(campioni));
    Serial.println("  • RPM motore medio: " + String(rpmMotoreMedio, 1) + " RPM");
    Serial.println("  • RPM uscita atteso: " + String(RPM_NO_LOAD_DICHIARATO, 1) + " RPM");
    Serial.println("  • Rapporto calcolato: 1:" + String(rapportoCalcolato, 1));

    // Arrotonda al valore standard più vicino
    int rapportiStandard[] = {19, 34, 36, 45, 90, 150, 210, 380};
    int rapportoVicino = rapportiStandard[0];
    float differenzaMinima = abs(rapportoCalcolato - rapportoVicino);

    for(int i = 1; i < 8; i++) {
      float diff = abs(rapportoCalcolato - rapportiStandard[i]);
      if(diff < differenzaMinima) {
        differenzaMinima = diff;
        rapportoVicino = rapportiStandard[i];
      }
    }

    Serial.println("  • Rapporto standard più vicino: 1:" + String(rapportoVicino));
    Serial.println("\n✅ USA QUESTO VALORE NEL TUO CODICE:");
    Serial.println("   const float RAPPORTO_RIDUZIONE = " + String(rapportoCalcolato, 1) + ";\n");

  } else {
    Serial.println("\n❌ ERRORE: Nessun dato valido raccolto");
    Serial.println("   Verifica che l'encoder sia collegato correttamente\n");
  }

  Serial.println("Passaggio alla modalità test tra 3 secondi...\n");
  delay(3000);
}

void loop() {
  if (!calibrazioneCompletata) {
    delay(1000);
    return;
  }

  // Modalità test con il rapporto calcolato
  static unsigned long ultimoStampa = 0;
  static unsigned long ultimoCambio = 0;
  static int fase = 0;
  static long impulsiPrecedenti = 0;
  static unsigned long tempoPrecedente = millis();

  // Calcola RPM in tempo reale
  if (millis() - tempoPrecedente >= 100) {
    long impulsiAttuali = encoder.getCount();
    long deltaImpulsi = impulsiAttuali - impulsiPrecedenti;
    float deltaTempoMinuti = (millis() - tempoPrecedente) / 60000.0;

    float rpmMotore = (deltaImpulsi / (float)PPR) / deltaTempoMinuti;
    float rpmUscita = rpmMotore / rapportoCalcolato;

    impulsiPrecedenti = impulsiAttuali;
    tempoPrecedente = millis();

    // Stampa ogni 200ms
    if (millis() - ultimoStampa >= 200) {
      Serial.print("Pos: ");
      Serial.print(impulsiAttuali);
      Serial.print(" | RPM Motore: ");
      Serial.print(abs(rpmMotore), 0);
      Serial.print(" | RPM Uscita: ");
      Serial.println(abs(rpmUscita), 1);
      ultimoStampa = millis();
    }
  }

  // Ciclo di test
  if (millis() - ultimoCambio >= 3000) {
    switch(fase) {
      case 0:
        Serial.println("\n>>> Motore AVANTI - velocità media");
        motorAvanti(150);
        break;
      case 1:
        Serial.println("\n>>> Motore STOP");
        motorStop();
        break;
      case 2:
        Serial.println("\n>>> Motore AVANTI - velocità alta");
        motorAvanti(200);
        break;
      case 3:
        Serial.println("\n>>> Motore STOP");
        motorStop();
        encoder.clearCount();
        break;
    }

    fase = (fase + 1) % 4;
    ultimoCambio = millis();
  }
}