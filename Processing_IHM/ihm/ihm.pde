import org.gicentre.utils.stat.*;    // For chart classes.
import processing.serial.*; 
 
 
Serial myPort;    // The serial port
PFont myFont;     // The display font
String inString;  // Input string from serial port
int lf = 10;      // ASCII linefeed 

float[] X_axis;

float[] RH;
float[] T;
float[] Lux;
float[] Pres;
float[] temp;

int NB_VAL_MAX = 200;

// Displays a simple line chart representing a time series.
 
XYChart lineChart;
XYChart lineChart2;
XYChart lineChart3;
XYChart lineChart4;
 
// Loads data into the chart and customises its appearance.
void setup()
{
  //size(1000,800);// Taille de fenetre x,y
  size(1000,700);
  textFont(createFont("Arial",10),10);
 
  X_axis = new float [NB_VAL_MAX];
  RH = new float [NB_VAL_MAX];
  T = new float [NB_VAL_MAX];
  Lux = new float [NB_VAL_MAX];
  Pres = new float [NB_VAL_MAX];
  temp = new float [NB_VAL_MAX];
    
 for (int i=0; i< NB_VAL_MAX; i++){
  X_axis[i] = i;
 }
 
 
 
 
  // Both x and y data set here.  
  lineChart = new XYChart(this);
  lineChart.setData(X_axis,RH);
                                  
 lineChart2 = new XYChart(this);                    
 lineChart2.setData(X_axis,T);
                                  
 lineChart3 = new XYChart(this);   
 lineChart3.setData(X_axis,Lux);
                                  
 lineChart4 = new XYChart(this);
 lineChart4.setData(X_axis,Pres);
   
  // Axis formatting and labels.
  lineChart.showXAxis(true); 
  lineChart.showYAxis(true); 
  lineChart.setMinY(0);
     
  lineChart.setYFormat("###,###");  // Monetary value in $US
 // lineChart.setXFormat("0000");      // Year
  
  lineChart2.showXAxis(true); 
  lineChart2.showYAxis(true); 
  lineChart2.setMinY(0);
  
  lineChart2.setYFormat("###,###");  // Monetary value in $US
  //lineChart2.setXFormat("0000");      // Year
  
  lineChart3.showXAxis(true); 
  lineChart3.showYAxis(true); 
  lineChart3.setMinY(0);
     
  lineChart3.setYFormat("###,###");  // Monetary value in $US
  //lineChart3.setXFormat("0000");      // Year
  
  lineChart4.showXAxis(true); 
  lineChart4.showYAxis(true); 
  lineChart4.setMinY(0);
     
  lineChart4.setYFormat("###,###");  // Monetary value in $US
  //lineChart4.setXFormat("0000");      // Year
   
   
  // Symbol colours
  lineChart.setPointColour(color(180,50,50,100));
  lineChart.setPointSize(5);
  lineChart.setLineWidth(2);
  
    // Symbol colours
  lineChart2.setPointColour(color(180,50,50,100));
  lineChart2.setPointSize(5);
  lineChart2.setLineWidth(2);
  
    // Symbol colours
  lineChart3.setPointColour(color(180,50,50,100));
  lineChart3.setPointSize(5);
  lineChart3.setLineWidth(2);
  
    // Symbol colours
  lineChart4.setPointColour(color(180,50,50,100));
  lineChart4.setPointSize(5);
  lineChart4.setLineWidth(2);
  
   //myPort = new Serial(this, Serial.list()[0], 9600);      // Prendra le premier port com dispo
   myPort = new Serial(this, "COM7", 9600);      // Prendra le premier port com dispo
  // TODO : trouver un formalisme dans l'envoi des données pour les traiter ici, ensuite.
  // Dans notre cas : "RH/T/LUX/PRES/TEMP\n"
  
  
}
 
// Draws the chart and a title.
void draw() //Equivalent loop() arduino
{
  
  // MAJ des données à afficher
 lineChart.setData(X_axis,RH);
 lineChart2.setData(X_axis,T);
 lineChart3.setData(X_axis,Lux);
 lineChart4.setData(X_axis,Pres);

  
  
  background(255);    // 
  textSize(9);
  
  //Décalage des graph en Y
  /*lineChart.draw(15,15,470,170);    // coordx, coody, taillex, taille y
  lineChart2.draw(15,200,470,170);
  lineChart3.draw(15,385,470,170);
  lineChart4.draw(15,570,470,170);*/
  lineChart.draw(15,15,900,160);    // coordx, coody, taillex, taille y
  lineChart2.draw(15,175,900,160);
  lineChart3.draw(15,335,900,160);
  lineChart4.draw(15,495,900,160);
        
   
  // Draw a title over the top of the chart.
  fill(120);                                          
  textSize(20);                                        // Define following text function size
  text("Hygrométrie (en %)", 70,30);  //text("test", positionx, positiony)
  textSize(11);
  text("DHT11", 70,45);
     
  textSize(20);                                        // Define following text function size
  text("Température (en °C)", 70,190);  //text("test", positionx, positiony)
  textSize(11);
  text("MPL115A2", 70,205);
  
  textSize(20);                                        // Define following text function size
  text("Luminance (en lux)", 70,350);  //text("test", positionx, positiony)
  textSize(11);
  text("TEMT6000", 70,365);
  
  textSize(20);                                        // Define following text function size
  text("Pression (en kPa)", 70,510);  //text("test", positionx, positiony)
  textSize(11);
  text("MPL115A2", 70,525);
}

void serialEvent(Serial p) {       //ISR lorsque réception de datas sur le port série
  String inString = p.readStringUntil('\n'); // Lis les données du "paquet" de données reçu et s'arrête à \n
  if(inString != null){
   String[] data_received = split(inString, '/');
   
   // Récupérer données dans data_received [0], [1] ...
   try {
     //Fonction permettant de décaler les données reçues dans le tableau
     // Fonction System.arraycopy(RH (((Tableau d'entrée))),0 (((indice de début))), RH (((Tableau de dest))),
     //                            1 (((indice de dest))), NB_VAL_MAX - 1 (((Nombre de valeurs à copier))) )
     System.arraycopy(RH, 0, RH, 1, NB_VAL_MAX - 1);
     System.arraycopy(T, 0, T, 1, NB_VAL_MAX - 1);
     System.arraycopy(Lux, 0, Lux, 1, NB_VAL_MAX - 1);
     System.arraycopy(Pres, 0,Pres, 1, NB_VAL_MAX - 1);
     
     //Affichage pour le debugger (faculatatif)
     print("Hygro:");
     print(data_received[0]);
     print(" Temperature:");
     print(data_received[1]);
     print(" Luminance:");
     print(data_received[2]);
     print(" Pression:");
     print(data_received[3]);
     
    //RH[0] = data_received[0];//Float.parseFloat(str);
    if (Float.parseFloat(data_received[0]) >= 1000){
       data_received[0]=null;
    }
    RH[0] = Float.parseFloat(data_received[0]);
    //print(RH[0]);
    T[0] = Float.parseFloat(data_received[1]);
    Lux[0] = Float.parseFloat(data_received[2]);
    Pres[0] = Float.parseFloat(data_received[3]);
    }catch(RuntimeException e) {
    e.printStackTrace();
    } 
  }
} 
