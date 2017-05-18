// Qual pino usar para ler o sensor? Pode usar qualquer pino!
#define FLOWSENSORPIN 2

#define FLOWSENSORPIN2 3

// Conta quantos pulsos!
volatile uint16_t pulses = 0;
// Rastrear o estado do pino de pulso
volatile uint8_t lastflowpinstate;
// Você pode tentar manter o tempo de quanto tempo é entre pulsos
volatile uint32_t lastflowratetimer = 0;
// E usar isso para calcular uma taxa de fluxo
volatile float flowrate;
// Interrupção é chamada uma vez por milissegundo, procura por pulsos do sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nada mudou!
  }
  
  if (x == HIGH) {
    //low to high  transição!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // Em hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 já é usado para millis () - nós vamos apenas interromper em algum lugar
    // no meio e chamar a função "Compare A" acima
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // Não chamar mais a função de interrupção COMPA
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup() {
   Serial.begin(9600);
   Serial.print("Flow sensor test!");
      
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);
}

void loop()                     // executar uma e outra vez
{ 
 
  Serial.print("Freq: "); Serial.println(flowrate);
  Serial.print("Pulses: "); Serial.println(pulses, DEC);
  
  // Se for um sensor plástico utilizar o seguinte cálculo
  // Frequência do sensor (Hz) = 7.5 * Q (Liters/min)
  // Liters = Q * time elapsed (seconds) / 60 (seconds/minute)
  // Liters = (Frequency (Pulses/second) / 7.5) * time elapsed (seconds) / 60
  // Liters = Pulses / (7.5 * 60)
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;

/*
  // Se for um sensor de bronze utilizar o seguinte cálculo
  float liters = pulses;
  liters /= 8.1;
  liters -= 6;
  liters /= 60.0;
*/
  Serial.print(liters); Serial.println(" Liters");
  
  delay(100);
}
