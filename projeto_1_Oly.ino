#include <EEPROM.h>
#include <Crescer.h>

const int man_hidro = 5;
const int man_aquec = 6;
const int man_filtro = 7;
const int ilum1 = 8;
const int ilum = 9;
const int mt_aquec = 10;
const int mt_filt = 11; 
const int mt_hid = 12;

int hora_inicio_filtro;
int hora_final_filtro;
String hora_inicial;
String hora_final;
int i = 1;
int x,y;
boolean app_filtro = false;
boolean app_aquec = false;
boolean app_hidro = false;

Tempora tempFiltro;
boolean filtroTimer = false;
int seg = 0, minut = 0, hora = 21;

void setup()
{
    Serial.begin(9600); 
                       
    pinMode(mt_hid, OUTPUT);
    digitalWrite(mt_hid, HIGH);
    pinMode(mt_filt, OUTPUT);
    digitalWrite(mt_filt, HIGH);
    pinMode(mt_aquec, OUTPUT);
    digitalWrite(mt_aquec, HIGH);
    pinMode(ilum, OUTPUT);
    digitalWrite(ilum, HIGH);
    pinMode(ilum1, OUTPUT);
    digitalWrite(ilum1, HIGH);

    pinMode(man_filtro, INPUT_PULLUP);
    pinMode(man_hidro, INPUT_PULLUP);
    pinMode(man_aquec, INPUT_PULLUP);

    hora_inicio_filtro = EEPROM.read(0);
    hora_final_filtro = EEPROM.read(1);
    
    tempFiltro.defiSP(1000);

}

String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  while(Serial.available() > 0) {
    caractere = Serial.read();
   
    if (caractere != '\n')
      conteudo.concat(caractere);
      
    delay(10);
  }

  return conteudo;
}

void setHorFiltro(String data){

  while(data[i] != '-')
  {
      hora_inicial += data[i];
      i++;
  }

  i++;
  
  while(data[i] != 'x')
  {
      hora_final += data[i];
      i++;
  }

  hora_inicio_filtro = hora_inicial.toInt();
  hora_final_filtro = hora_final.toInt();
  
  Serial.println(hora_inicio_filtro);
  Serial.println(hora_final_filtro);

  EEPROM.write(0, hora_inicio_filtro);
  EEPROM.write(1, hora_final_filtro);
 
  hora_inicial = "";
  hora_final = "";
  i = 1;
}

void loop()
{
   if(Serial.available() > 0) {
      String data = leStringSerial();

          if(data[0] == 'x')
            setHorFiltro(data);
            
          if(data == "filton")   
            app_filtro = true;   
          else if(data == "filtoff")
            app_filtro = false;
    
          if(data == "aquecon")              
            app_aquec = true;
          else if(data == "aquecoff")        
            app_aquec = false;

          if(data == "hidron")              
            app_hidro = true; 
          else if(data == "hidroff")        
            app_hidro = false;
    
          if(data == "ilumon")              
            digitalWrite(ilum, LOW);   
          else if(data == "ilumoff")        
            digitalWrite(ilum, HIGH);
    
          if(data == "ilum1on")              
            digitalWrite(ilum1, HIGH);   
          else if(data == "ilum1off")        
            digitalWrite(ilum1, LOW);
      }

       int in_7 = digitalRead(man_filtro);
       int in_6 = digitalRead(man_aquec);
       int in_5 = digitalRead(man_hidro);

       if(in_7 == LOW || app_filtro == true)
          digitalWrite(mt_filt, LOW);
       if(in_7 == HIGH && app_filtro == false)
          digitalWrite(mt_filt, HIGH);

       if(in_6 == LOW || app_aquec == true)
          digitalWrite(mt_aquec, LOW);
       if(in_6 == HIGH && app_aquec == false)
          digitalWrite(mt_aquec, HIGH);

       if(in_5 == LOW || app_hidro == true)
          digitalWrite(mt_hid, LOW);
       if(in_5 == HIGH && app_hidro == false)
          digitalWrite(mt_hid, HIGH);
 

        
        if(hora_inicio_filtro == hora)
            digitalWrite(mt_filt, LOW);
           
        if(hora_final_filtro == hora)
            digitalWrite(mt_filt, HIGH);
            

        filtroTimer = tempFiltro.Saida(1);
        if(filtroTimer == true)
        {
            if(seg < 59)
            seg++;

            else if (minut < 59)
            {
              seg = 0;
              minut++;
            }

            else if(hora < 23)
            {
               seg = 0;
               minut = 0;
               hora++;
            }

            else{
              hora = 0;
              minut = 0;
              seg = 0;
            }


            /*Serial.print(hora);
            Serial.print(":");
            Serial.print(minut);
            Serial.print(":");
            Serial.print(seg);
            Serial.print("\n");
            Serial.print(hora_inicio_filtro);
            Serial.print("\n"); 
            Serial.print(hora_final_filtro);
            Serial.print("\n");*/
            
            EEPROM.write(2, hora);
            EEPROM.write(3, minut);
            EEPROM.write(4, seg);
            
            filtroTimer = tempFiltro.Saida(0);
  
        }
   
}
 
