//----------------Программно аппаратный комплекс управления пешеходного перехода-------------------
//-------------------------------------------------------------------------------------------------
//--------------------------константы--------------------------------------------------------------
#define led_left 13                              //пин 1 канал стробоскопа
#define led_right 11                             //пин 2 канал стробоскопа
#define relay 7                                 //пин вкл. реле
#define light A0                                //пин дат. освещености
#define sensor 6                                //пин дат. движения
#define timer_run 55                            //таймер прохода пешехода (в секундах)
#define parametr_bright 800                     //параметр освещености 
#define freq_blink 50                   //частота моргания страбоскопа
#define freq_switch 350                 //частота переключения стробоскопов
//--------------------------обявление переменых----------------------------------------------------
bool status_led;                                 // определения пина стробоскопа
bool flag_sensor;                                // состояние дат. движения
unsigned long blink_time, color_time, last_time; // переменые времени
byte counter_run;                                // запуск таймера прохода печехода
//--------------------------настройка программы----------------------------------------------------
void setup() {
  Serial.begin(9600);                            
  pinMode(led_right,OUTPUT);                      // объявление 1 стробоскопа
  pinMode(led_left,OUTPUT);                      // объявление 2 стробоскопа
  pinMode(relay,OUTPUT);                         // объявление управления реле
  pinMode(light,INPUT);                          // объявление дат.освещености
  pinMode(sensor,INPUT);                         // объявление дат.движения
}
//-------------------------основной цикл программы-------------------------------------------------
void loop() {
 if(digitalRead(sensor)){                        // условие если есть движение
   flag_sensor=true;                             // состояние дат.движения активировано
   counter_run=timer_run;                        // запуск таймера прохода пешехода
 }
 if(flag_sensor){                                // условие если состояние дат. движения активно
   flasher(freq_blink ,freq_switch);             // запуск функции стробоскопа
   lighting (parametr_bright);                   // запуск функции 
 } 
}
//------------------------функция включения стробоскопов------------------------------------------
void flasher (int blinking , int switching ){            
if (millis()-blink_time>blinking){               // установка моргания стробоскопа            
  blink_time=millis();
  if(millis()-color_time>switching&&!digitalRead(status_led ? led_right : led_left )){ // меняем канал стробоскопа
    color_time=millis();
    status_led=!status_led;                      
  }
  digitalWrite(status_led ? led_right : led_left ,!digitalRead(status_led ? led_right : led_left )); 
 }
}
//------------------------функция включения освещения пешеходгного перехода-----------------------
void lighting (int bright ){
  if(analogRead(light)>bright)                   // условия проверки датчика освещености
     digitalWrite(relay,HIGH);                   // включения реле
   if(millis()-last_time>1000){                  // условия если прошла секнда
     last_time=millis();                         
     counter_run--;                              // уменьшаем таймер на одно значение
    }
  if(!counter_run){                               // условие если таймер равен нулю
     flag_sensor=false;                           // состояние дат. движение не активно
     counter_run=timer_run;                       // возвращаем значения таймера
     digitalWrite(relay,LOW);                     // выключаем реле
   } 
}
