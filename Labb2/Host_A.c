#include "Sim_Engine.h"
#include "Beta.h"


static int current_pos;     /*vilket paket*/
static int pos_next_packet; /*vilket pakets som ska skickas*/
static int seq;             /*sekvensnummer, 0 eller 1*/
static int current_seq;     /*nuvarande packets checksum*/
static bool connected;      /*uppkopplad i en coversation elle ej*/

static struct pkt myPackets[BUFF];


/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) {
  if(seq > 1)   //För att hantera sekvensen så att den aldrig går över 1
    seq = 0;
  memcpy(myPackets[current_pos].payload, message.data, sizeof(message.data));   //För över data från message till payload
  myPackets[current_pos].seqnum = seq++;
  myPackets[current_pos].acknum = 0;
  myPackets[current_pos].checksum = checksum(&myPackets[current_pos]);      //Genererat en checksum utefter seck och ack
  current_pos++;
  if(!connected)    //Starta en timer och skicka iväg första packetet
  {
    starttimer(A, INCREMENT_TIME);
    tolayer3(A, myPackets[++pos_next_packet]);
    current_seq = myPackets[pos_next_packet].seqnum;
    connected = 1;
  }
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
  stoptimer(A);
  if(corupted(&packet))   //Om checksumman är fel starta om timmern och sickar om packetet
  {
    starttimer(A, INCREMENT_TIME);
    tolayer3(A, myPackets[pos_next_packet]);
    return;
  }
  if(packet.acknum == 0 || packet.seqnum != current_seq)    //Om ack eller seq är fel starta om timern och sicka packetet igen
  {
    starttimer(A, INCREMENT_TIME);
    tolayer3(A, myPackets[pos_next_packet]);
  }
  else
  {
    if((current_pos-1) > pos_next_packet)   //Finns det ett packet som är redo att skicka skicka det
    {
      starttimer(A, INCREMENT_TIME);
      tolayer3(A, myPackets[++pos_next_packet]);
      current_seq = myPackets[pos_next_packet].seqnum;
    }
    else
    {
      connected = 0;
    }
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  starttimer(A, INCREMENT_TIME);
  tolayer3(A, myPackets[pos_next_packet]);
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  printf("\033[1;35mInitializing A, init!\033[0;37m\n");
  current_pos = 0;
  pos_next_packet = -1;
  seq = 0;
  current_seq = 0;
  connected = 0;
}
