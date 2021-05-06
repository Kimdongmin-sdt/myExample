#include "mbed.h"
 
DigitalOut led(LED1);
InterruptIn btn(BUTTON1);
 
EventQueue queue;
 
void do_something_outside_irq() {
  // this does not run in the ISR
  printf("Toggle LED!\r\n");
}
 
void do_something_in_irq() {
  // this runs in the ISR
  led = !led;
 
  // then defer the printf call to the other thread
  queue.call(&do_something_outside_irq);
}
 
int main() {
  Thread eventThread;
  //eventThread.start(callback(&queue, &EventQueue::dispatch_forever));
 
  btn.fall(&do_something_in_irq);
  printf("test33\n");
 
  queue.dispatch_forever();
  printf("test\n");
  while (1) {
      ThisThread::sleep_for(2000);
  }
  
}