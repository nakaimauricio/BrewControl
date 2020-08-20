
byte get_timer(void);         /*Get Timer from Timer Pool */
void free_timer(byte);        /*Return Timer to Timer Pool */
void init_timer(byte,unsigned long);   /*Init timers timeout value  */
void timer_pool_init(void);   /*Init the pool when uC starts */
unsigned long check_timer(byte);       /*Return Timers value      */
void decrement_timers(void);  /*decrement all timers' values */

/*
brief Timer pool used to keep information about available timers
   
   This timer pool is extensively used by most of the modules of the 
   OpenTCP project. All timers that are used are allocated from this
   pool. Maximum number of timers that can be used at any given time
   is defined by the #NumtimersS define.
*/ 
 
const int Numtimers = 20;      //quantidade de timer que podem ser utilizados
 
typedef struct  {
  unsigned long value;
  byte livre;
} type_timer_pool;

type_timer_pool timer_pool[Numtimers];

/*brief Initialize timer pool
  ingroup core_initializer
  author 
    li Jari Lahti (jari.lahti@violasystems.com)
  date 18.07.2001
  warning
    li This function <b>must</b> be invoked at startup before
    any other timer function is used.

  This function resets all timer counter to zero and initializes all 
  timers to available (free) state.
*/

void timer_pool_init (void)
{
  byte i;

  for( i=0; i < Numtimers; i++) {
    timer_pool[i].value = 0;
    timer_pool[i].livre = true;
 
  }
}

/*brief Obtain a timer from timer pool
*  \author 
*    \li Jari Lahti (jari.lahti@violasystems.com)
*  \date 18.07.2001
*  \return Handle to a free timer
*  \warning
*    \li Timers are considered to be critical resources, so if there is 
*    no available timer and get_timer is invoked, system will reset.
*
*  Invoke this function to obtain a free timer (it's handle that is) from
*  the timer pool.
*/

byte get_timer (void)
{
  byte i;
  byte first_match;
  
  for( i=0; i < Numtimers; i++) {
    if( timer_pool[i].livre == true ) {
      /* We found a free timer! */
      /* Mark is reserved     */
      
      timer_pool[i].livre = false;
      first_match = i;
      return first_match;   /* Return Handle  */
    }
  }
  /* Error Check  */
  for(;;);  
}

/*  ************************
//  ROTINA DE TIMERS RAPIDOS
    ************************
*/
#define NUMBER_QUICK  5

byte  timer_quick[NUMBER_QUICK];

byte get_timer_quick (void){
  byte i;
  
  for (i; i<NUMBER_QUICK; i++){
    if (timer_quick[i] == 0xff){
      timer_quick[i] = get_timer();
      return(timer_quick[i]);
    }
  }
}
/*  ***************************
//  LIBERAÇÃO DOS TIMER RAPIDOS
    ***************************
*/
void free_timer_quick (void){
  byte i;

  for (i; i<NUMBER_QUICK; i++){
    if (timer_quick[i] != 0xff){
      free_timer(timer_quick[i]);
      timer_quick[i] = 0xff;
    }
  }
}
  
/** \brief Release timer back to free timer pool
 *  \author 
 *    \li Jari Lahti (jari.lahti@violasystems.com)
 *  \date 18.07.2001
 *  \param nbr handle to timer beeing released
 *
 *  This function releases the timer who's handle is supplied as parameter.
 *  Use this when timer is not needed any more and other applications might
 *  use it.
 */
 
void free_timer (byte nbr)
{
  /* Make a simple check */
  if( nbr > (Numtimers-1) ) 
    return; 
  timer_pool[nbr].livre = true;
  timer_pool[nbr].value = 0;

}

/** \brief Initialize timer to a given time-out value
 *  \author 
 *    \li Jari Lahti (jari.lahti@violasystems.com)
 *  \date 18.07.2001
 *  \param nbr handle of timer who's value we're setting
 *  \param tout time-out value to set for this timer
 *
 *  Invoke this function to set timeout value for a timer with
 *  a given handle.
 *
 *  #TIMERTIC defines how quickly the timers' values are decremented so is
 *  it to initialize timers to correct timeouts.
 */
void init_timer ( byte nbr, unsigned long tout )
{
  /* Make a simple check */
  
  unsigned long val;
  
  if( nbr > (Numtimers-1) ) 
    return; 

  if( timer_pool[nbr].livre == true ) 
    return;
    
  /* All OK       */
  
  val = tout;
  
  /*OS_EnterCritical();*/
  
  /* Normalize seconds to timer tics  */
  
  timer_pool[nbr].value = val;

/*  OS_ExitCritical();*/

} 

/** \brief Return the value of a given timer
 *  \author 
 *    \li Jari Lahti (jari.lahti@violasystems.com)
 *  \date 18.07.2001
 *  \param nbr timer handle who's value is to be returned
 *  \return timer value
 *  \warning 
 *    \li Interrupts are not disabled when fetching the value, therefore
 *    returned value possibly has an error component +/- #TIMERTIC.
 *
 *  Function simply returns timer value of a given timer. No checks are
 *  made in order to make the function as fast as possible.
 */
unsigned long check_timer (byte nbr)
{
  return timer_pool[nbr].value;
}

/** \brief Decrement all timers' values by one
 *  \author 
 *    \li Vladan Jovanovic (vladan.jovanovic@violasystems.com)
 *  \date 18.07.2001
 *
 *  Invoke this function from timer interrupt to decrement timer counter values
 */
byte quantidade_timers;
unsigned long tempo_atual = 0;
unsigned long tempo_anterior = 0;
unsigned long tempo_decremento = 0;

void decrement_timers (void){
  byte i, quantidade_timers_temp;
  
  tempo_atual = millis();
  tempo_decremento = tempo_atual - tempo_anterior;
  tempo_anterior = tempo_atual;
//  Serial.print("tempo_decremento"); // send a byte with the value 45
//  Serial.println(tempo_decremento); // send a byte with the value 45

  /* Go Through Timers */
  while (tempo_decremento > 0){
    tempo_decremento--;
    quantidade_timers_temp = 0;
    for( i=0; i<Numtimers; i++ ) {
      if( (timer_pool[i].livre == false) && (timer_pool[i].value != 0)){
        quantidade_timers_temp++;
        timer_pool[i].value--;
      }
    }
  }
  quantidade_timers = quantidade_timers_temp;
//  Serial.print(" quantidade_timers "); // send a byte with the value 45
//  Serial.println(quantidade_timers); // send a byte with the value 45
}
