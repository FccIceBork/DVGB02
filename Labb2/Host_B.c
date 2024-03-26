#include "Sim_Engine.h"
#include "Beta.h"

static int seq;


/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
  if(corupted(&packet))   //Är checksumman fel sätt skicka tillbaka fel ack nummer
  {
    packet.acknum = 0;
    tolayer3(B, packet);
  }
  else
  {
    packet.acknum = 1;
    packet.checksum = checksum(&packet);
    if(packet.seqnum == seq)    //Anländer packetet i rätt ordning packa upp paketet och skicka tillbaka rätt ack nummer
    {
      tolayer5(B, packet.payload);
      seq++;
      if(seq > 1)     //Ser till att seq ej går över 1
        seq = 0;
    }
    tolayer3(B, packet);
  }
}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
  printf("\033[1;35mInitializing B, init!\033[0;37m\n");
  seq = 0;
}
