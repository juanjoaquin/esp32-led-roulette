const int PINES[] = {13, 12, 14, 27, 26};
const int CANTIDAD = 5;
const int PIN_BOTON = 0;   // boton BOOT de la placa

void apagarTodos() {
  for (int i = 0; i < CANTIDAD; i++) digitalWrite(PINES[i], LOW);
}

void encenderSolo(int indice) {
  apagarTodos();
  digitalWrite(PINES[indice], HIGH);
}

void girarRuleta() {
  int ganador = random(0, CANTIDAD);

  // Pasos totales: varias vueltas completas + lo necesario para caer en el ganador
  int vueltas = random(3, 6);
  int pasos = vueltas * CANTIDAD + ganador;

  Serial.println("Girando...");

  int posicion = 0;
  for (int paso = 0; paso <= pasos; paso++) {
    posicion = paso % CANTIDAD;
    encenderSolo(posicion);

    // Cuanto mas cerca del final, mas lento (frena poco a poco)
    float avance = (float)paso / pasos;
    int espera = 40 + (int)(avance * avance * 400);
    delay(espera);
  }

  // El ganador titila
  Serial.printf("Salio el LED numero %d!\n", ganador + 1);
  for (int i = 0; i < 6; i++) {
    digitalWrite(PINES[ganador], !digitalRead(PINES[ganador]));
    delay(250);
  }
  encenderSolo(ganador);  // queda encendido el ganador
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < CANTIDAD; i++) {
    pinMode(PINES[i], OUTPUT);
  }
  pinMode(PIN_BOTON, INPUT_PULLUP);

  randomSeed(esp_random());
  apagarTodos();

  Serial.println("\n=== Ruleta de LEDs ===");
  Serial.println("Presiona el boton BOOT para girar.");
}

void loop() {
  if (digitalRead(PIN_BOTON) == LOW) {   // presionado
    girarRuleta();
    delay(300);                          // evita lanzamientos dobles
  }
}