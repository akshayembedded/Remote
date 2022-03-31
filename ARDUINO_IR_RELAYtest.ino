#include <IRremote.h>

#define Boton0 0x1FEE01F //0
#define Boton1 0x1FE50AF  //1
#define Boton2 0x1FEB04F //5
//#define Boton3 0xFF7A85

const int trigPin1 = 2;
const int echoPin1 = 3;
unsigned long t=0,d =5000;           
long duration1;
int distanceleft;                          
int SensorIR = 11;                      //sentencia int: declaramos/nombramos el pin 11 como "SensorIR"
int Rele = 8;                           //sentencia int: declaramos/nombramos el pin 8 como "Rele"
                                     
IRrecv irrecv(SensorIR);                //Asociamos el receptor o SensorIR

decode_results codigo ;                 //Declaramos/nombramos los resultados (de los codigos emitidos)
                                        // como "codigo"
void setup()
{
  Serial.begin(9600);                   //Activamos el "monitor serie"
  // En caso de que el controlador de interrupción se bloquee en la configuración, dará una pista
  // al usuario de lo que esta sucediendo.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn();                  //Se inicia/activa el receptor
  Serial.println("Enabled IRin");

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode (Rele, OUTPUT) ;               //Declaramos el pin 8 o "Rele" como salida
}

void loop() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceleft = duration1 * 0.034 / 2;
  if (distanceleft<=15)
  {
   digitalWrite(Rele,LOW);
   Serial.println("Humen presence");
   delay(100);
   t=0;
   }
   else
   {
  if (irrecv.decode(&codigo)) {          //Decodifica o "lee" el codigo emitido por el mando/control remoto
    Serial.println(codigo.value, HEX);   //Imprime el codigo en valor hexadecimal y lo muestra en el "monitor serie"

    if (codigo.value == Boton1){   //1      //Orden condicional: si el codigo recibido corresponde al boton 2
      digitalWrite(Rele, HIGH) ;
    
        t=0;}        //Acción: entonces el Rele se activa (en este caso se apaga el punto de luz) 
      delay(50);                         
    if (codigo.value == Boton0){ //0        //Orden condicional: si el codigo recibido corresponde al boton 1
      digitalWrite(Rele, LOW);
       
        t=0;}          //Acción: entonces el Rele se desactiva (en este caso se enciende el punto de luz)
      delay(50);
    if (codigo.value == Boton2){//5
      digitalWrite(Rele, HIGH);
     t=millis();
       
    }
   
  } if(t!=0)
    {
    if((millis()-t)>d)
    {  Serial.println("5 off");
      digitalWrite(Rele, LOW);
      t=0;
    }
    }
    irrecv.resume();                     // Recibe el siguiente valor
  delay(100);
}
}
