#include "Sim_Engine.h"
#include "Beta.h"


int checksum(struct pkt *p)     //Generera n checksum
{
    int sum = 0;
    for(int i = 0; i < sizeof(p->payload); i++)
    {
        sum += (int)(p->payload[i]);
    }
    return sum + p->acknum + p->seqnum;
}

bool corupted(struct pkt *p)    //Kolla om checksum stämmer
{
    if(p->checksum == checksum(p))
        return 0;
    else
        return 1;
}