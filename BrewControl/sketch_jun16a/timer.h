byte get_timer(void);         /*Get Timer from Timer Pool */
void free_timer(byte);        /*Return Timer to Timer Pool */
void init_timer(byte,unsigned long);   /*Init timers timeout value  */
void timer_pool_init(void);   /*Init the pool when uC starts */
unsigned long check_timer(byte);       /*Return Timers value      */
void decrement_timers(void);  /*decrement all timers' values */


