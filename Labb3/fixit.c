#include <stdio.h>

#include "sim_engine.h"



void infin(int d_table[4][4]){
    for(int i = 0; i<4; i++)
    {
        for(int j = 0; j<4; j++)
        {
            d_table[i][j] = 999;
        }
    }
}

void new_info(int src, int d_table[4][4]){
    struct rtpkt packet;
    packet.sourceid = src;
    for(int i = 0; i<4; i++)
    {
        packet.mincost[i] = 999;
    }
    packet.destid = 0;

    for(int i = 0; i<4; i++)
    {
        for(int j = 0; j<4; j++)
        {
            if(d_table[i][j] < packet.mincost[i])
                packet.mincost[i] = d_table[i][j];
        }
    }
    for(int i = 0; i<4; i++)
    {
        if(i != src && d_table[i][i] != 999)
        {
            packet.destid = i;
            tolayer2(packet);
        }
    }
}