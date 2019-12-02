#define segsTotal 1000
#define pinACS A5

#define sensibilidade 27.5  // 85mV/A (30A) 
#define offset 511
#define Ts 50

bool comeca = 0;
unsigned long tempoAnterior, tempoAtual=0;
unsigned int tempoTotal=0;

int   X[4]={0.0,0.0,0.0,0.0};
float Y[4]={0.0,0.0,0.0,0.0};
/*Coeficientes do filtro passa-baixas */
const float a[4]={1.0000, -0.5772, 0.4218, -0.0563};  //LPF Buttworth
const float b[4]={0.0985, 0.2956, 0.2956, 0.0985};    // fc = fs*2/5
void setup()
{
  pinMode(pinACS, INPUT); //Atribui como  entrada analog. a saída do ACS712
  Serial.begin(9600);     //Inicia comunicação analógica para envio de dados
}

void loop()
{
  
  while(1)//tempoTotal <= segsTotal*1000)
  {
    tempoAtual = millis();  //Obtém tempo atual [ms]
    int deltaT = tempoAtual-tempoAnterior;
  
    if(deltaT >= Ts)  //Força execução em intervalos regulares Ts
    {
      tempoAnterior = tempoAtual;
      
      X[3] = X[2];
      X[2] = X[1];
      X[1] = X[0];
      X[0] = analogRead(pinACS)-offset;

    //Implementação de filtragem em tempo discreto:  
      Y[3] = Y[2];
      Y[2] = Y[1];
      Y[1] = Y[0];
      Y[0] =          - a[1]*Y[1] - a[2]*Y[2] - a[3]*Y[3]
        +   b[0]*X[0] + b[1]*X[1] + b[2]*X[2] + b[3]*X[3];

      if(Y[0]>2.0)  comeca = 1; //Começa a enviar os valores quando
      if(comeca)                //a corrente for considerável
      {
        float corrente = Y[3]/sensibilidade;
        Serial.print(corrente); //Envia valor de corrente filtrado
        Serial.print(" ");
        Serial.println(tempoTotal/10); //Envia tempo atual
      }
    }
  }
}
