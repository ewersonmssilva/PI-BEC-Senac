import processing.serial.*; //Importa a biblioteca para abrir uma comunicação Serial
Serial myPort; //Instância a biblioteca para a comuniação Serial

//Cria uma instância para cada imagem da interface
PImage fundo; //Background
PImage ponteiro; //Ponteiro
       
//=======================

int valor_recebido; //Cria uma variável para armazenas o valor recebido pela serial
int valor_percent; //Cria uma variável para armazenar o valor do percentual

int ajuste_y=14; //Variável para ajusta o eixo x na tela (Centralizar)
                 //Se não precisar de ajuste deixe com valor 0

void setup()
{
    
  String portName = Serial.list()[0]; //Lista as portas COM (Serial) encontradas
                                      //Pega a primeira porta (Posição 0 em "Serial.list()[0]" e
                                      //armazena na variável portName
                                      //Se você tiver mais de uma porta COM, identifique qual a do Arduino
  
  myPort = new Serial(this, portName, 9600); //Abre uma comunicação Serial com baud rate de 9600
  
  size(800, 600); //Define o tamanho da tela

  //Carrega as imagens e armazena elas em suas respectivas instâncias
  fundo = loadImage("fundo.jpg");
  ponteiro = loadImage("ponteiro.png");
  //================================================================
  
}

void draw()
{
  
  if (myPort.available() > 0) //Se algo for recebido pela serial
  {
    valor_recebido = myPort.read(); //Armazena o que foi lido dentro da variável valor recebido
    
    if (valor_recebido<181) valor_percent = valor_recebido; //Se o valor recebido for menor que 181,
                                                        //armazena esse valor dentro da variável valor_percent
  }

  background(fundo); //Atualiza a imagem de fundo (backgroud) da interface

  translate(width/2, height/2+ajuste_y); //Posiciona o ponto 0 da interface no centro da tela
                                         //Com ajuste de 14 pixels em y (height)


  //********************************************************
  //float c = radians(valor_percent-90); //Pega o valor lido referendo ao sensor
                                   
  rotate(radians(valor_percent-90)); //Pega o valor lido referendo ao sensor,
                                 //Subtrai 90 desse valor, converte para radianos e
                                 //Executa a função de rotação da tela
                                 
  image(ponteiro, -(ponteiro.width/2), -ponteiro.height+(7)); //Posiciona o centro do furo do ponteiro
                                                              //Na posição central do gráfico na imagem de fundo
  
}
