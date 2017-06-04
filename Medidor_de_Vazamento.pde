import processing.serial.*; //Importa a biblioteca para abrir uma comunicação Serial
Serial myPort; //Instância a biblioteca para a comuniação Serial

int linefeed = 10; // nova linha ASCII = 10

//Cria uma instância para cada imagem da interface
PImage fundo; //Background
PImage ponteiro; //Ponteiro       
//=======================

int percent = 0; //Esta variável conterá a leitura da pecentagem
int litros = 0; //Esta variável conterá a leitura de litros perdidos
int ajuste_y=170; //Variável para ajusta o eixo x na tela (Centralizar)
                 //Se não precisar de ajuste deixe com valor 0

void setup()
{
    
  String portName = Serial.list()[0]; //Lista as portas COM (Serial) encontradas
                                      //Pega a primeira porta (Posição 0 em "Serial.list()[0]" e
                                      //armazena na variável portName
                                      //Se você tiver mais de uma porta COM, identifique qual a do Arduino
  
  myPort =  new Serial(this, Serial.list()[0], 9600); //Abre uma comunicação Serial com baud rate de 9600
  
  myPort.bufferUntil(linefeed);
  
  size(800, 600); //Define o tamanho da tela

  //Carrega as imagens e armazena elas em suas respectivas instâncias
  fundo = loadImage("fundo.jpg");
  ponteiro = loadImage("ponteiro.png");
  //================================================================
  
}

void serialEvent (Serial myPort) 
{
  // Lê o buffer serial como string
  String myString = myPort.readString();
 
  // Se tivermos outros bytes linefeed
  if (myString != null) 
  {
    // trim crap
    myString = trim(myString);
    // Dividir string em commas 
    // e converter seções em números inteiros. 
    String sensors[] = split(myString, ',');
    if(sensors.length>1)
    {
      percent = int(sensors[0]); 
      litros = int(sensors[1]); 
      print(percent);
      print(',');
      println(litros);
    }
  }
}

void draw()
{
  background(fundo); //Atualiza a imagem de fundo (backgroud) da interface

  translate(width/2, height/2+ajuste_y); //Posiciona o ponto 0 da interface no centro da tela
                                         //Com ajuste de 14 pixels em y (height)
  //********************************************************  
  // Texto e contagem em litros
  
  fill(0); //Define a cor (R, G, B)  
  textSize(20);
  text("Desperdício:", 75, 70);
  text("litros", 250, 70);  
  
  fill(255, 0, 0); //Define a cor (R, G, B) 
  textSize(20);
  text(litros, 210, 70); 

  //********************************************************
  //float c = radians(valor_percent-90); //Pega o valor lido referendo ao sensor
                                   
  rotate(radians(percent-90)); //Pega o valor lido referendo ao sensor,
                                 //Subtrai 90 desse valor, converte para radianos e
                                 //Executa a função de rotação da tela
                                 
  image(ponteiro, -(ponteiro.width/2), -ponteiro.height+(7)); //Posiciona o centro do furo do ponteiro
                                                              //Na posição central do gráfico na imagem de fundo
  
}
