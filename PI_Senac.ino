// Qual pino usar para ler o sensor? Pode usar qualquer pino!
#define FLOWSENSORPIN1 2

#define FLOWSENSORPIN2 3

// Conta quantos pulsos!
volatile uint16_t pulses = 0;
volatile uint16_t pulses2 = 0;
// Rastrear o estado do pino de pulso
volatile uint8_t lastflowpinstate;
volatile uint8_t lastflowpinstate2;
// Você pode tentar manter o tempo de quanto tempo é entre pulsos
volatile uint32_t lastflowratetimer = 0;
volatile uint32_t lastflowratetimer2 = 0;
// E usar isso para calcular uma taxa de fluxo
volatile float flowrate;
volatile float flowrate2;
// Interrupção é chamada uma vez por milissegundo, procura por pulsos do sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN1);
  
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
SIGNAL(TIMER1_COMPA_vect) {
  uint8_t y = digitalRead(FLOWSENSORPIN2);
  
  if (y == lastflowpinstate2) {
    lastflowratetimer2++;
    return; // nada mudou!
  }
  
  if (y == HIGH) {
    //low to high  transição!
    pulses2++;
  }
  lastflowpinstate2 = y;
  flowrate2 = 1000.0;
  flowrate2 /= lastflowratetimer2;  // Em hertz
  lastflowratetimer2 = 0;
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
void useInterrupt2(boolean w) {
  if (w) {
    // Timer0 já é usado para millis () - nós vamos apenas interromper em algum lugar
    // no meio e chamar a função "Compare A" acima
    OCR0A = 0xAF;
    TIMSK1 |= _BV(OCIE0A);
  } else {
    // Não chamar mais a função de interrupção COMPA
    TIMSK1 &= ~_BV(OCIE0A);
  }
}

void setup()
{
    Serial.begin (9600);
    Serial.print ("Sensor de fluxo OK!");

   pinMode(FLOWSENSORPIN1, INPUT);
   digitalWrite(FLOWSENSORPIN1, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN1);
   useInterrupt(true);
   pinMode(FLOWSENSORPIN2, INPUT);
   digitalWrite(FLOWSENSORPIN2, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN2);
   useInterrupt2(true);
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop()                     // executar uma e outra vez
{

   // Serial.print ("Freq: ");
   // Serial.println (flowrate);
   // Serial.print ("Pulses: ");
   // Serial.println (pulses, DEC);

    // Se for um sensor plástico utilizar o seguinte cálculo
    // Frequência do sensor (Hz) = 7.5 * Q (Liters/min)
    // Liters = Q * time elapsed (seconds) / 60 (seconds/minute)
    // Liters = (Frequency (Pulses/second) / 7.5) * time elapsed (seconds) / 60
    // Liters = Pulses / (7.5 * 60)
    float liters = pulses;
    liters /= 7.5;
    liters /= 60.0;

    float liters2 = pulses2;
    liters2 /= 7.5;
    liters2 /= 60.0;

    /*
        // Se for um sensor de bronze utilizar o seguinte cálculo
        float liters = pulses;
        liters /= 8.1;
        liters -= 6;
        liters /= 60.0;
    */
   // Serial.print (liters);
   // Serial.print (" Litros - Sensor 1  /  ");

   // Serial.print (liters2);
   // Serial.print (" Litros - Sensor 2  /  ");

    int percent = (liters - liters2) / liters2 * 100; //Calcula o percentual das leituras
    int lost = liters - liters2;

    // Condições para o funcionamento do ponteiro
    if (percent < 0){
    percent = 0;
    } else if (percent > 100){
    percent = 100;
    }
    percent = percent * 1.8;

    //Serial.write (percent); //Envia para a interface (Processing) o valor caculado entre os sensores
                           //que será um valor de 0 a 180

  Serial.print(percent);
  Serial.print(",");
  Serial.println(lost);

  delay (50); 
                
}
