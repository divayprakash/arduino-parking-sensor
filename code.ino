#include <TimerOne.h>                                 
 
#define trigPin 4                                     
#define echoPin 2                                     
#define onLED 12                                      
#define echo_int 0                                    
 
#define TIMER_US 50                                    
#define TICK_COUNTS 4000                              
 
volatile long echo_start = 0;                          
volatile long echo_end = 0;                           
volatile long echo_duration = 0;                      
volatile int trigger_time_count = 0;                  
volatile long range_flasher_counter = 0;              
 
void setup() 
{
  pinMode(trigPin, OUTPUT);                           
  pinMode(echoPin, INPUT);                          
  pinMode(onLED, OUTPUT);                        
  Timer1.initialize(TIMER_US);                        
  Timer1.attachInterrupt( timerIsr );                  
  attachInterrupt(echo_int, echo_interrupt, CHANGE);  
  Serial.begin (9600);                                
}
 
void loop()
{
    Serial.println(echo_duration / 58);              
    delay(100);                                       
}
 
void timerIsr()
{
    trigger_pulse();                                 
    distance_flasher();                              
}
 
void trigger_pulse()
{
      static volatile int state = 0;                 

      if (!(--trigger_time_count))                   
      {                                              
         trigger_time_count = TICK_COUNTS;           
         state = 1;                                  
      }
 
      switch(state)                                  
      {
        case 0:                                      
            break;
 
        case 1:                                      
           digitalWrite(trigPin, HIGH);              
           state = 2;                                
           break;
 
        case 2:                                      
        default:      
           digitalWrite(trigPin, LOW);               
           state = 0;                                
           break;
     }
}
 
void echo_interrupt()
{
  switch (digitalRead(echoPin))                     
  {
    case HIGH:                                      
      echo_end = 0;                                 
      echo_start = micros();                        
      break;
 
    case LOW:                                       
      echo_end = micros();                          
      echo_duration = echo_end - echo_start;        
      break;
  }
}
 
void distance_flasher()
{
      if (--range_flasher_counter <= 0)                
      {                                                
         if (echo_duration < 25000)                    
         {
           range_flasher_counter = echo_duration * 2;  
         }
         else
         {
           range_flasher_counter = 25000;              
         }
 
         digitalWrite( onLED, digitalRead( onLED ) ^ 1 );   
      }
}