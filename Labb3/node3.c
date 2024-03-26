#include <stdio.h>

#include "sim_engine.h"
#include "fixit.h"

extern int TRACE;
extern int YES;
extern int NO;

static int src3 = 3;

struct distance_table {
  int costs[4][4];
} dt3;

void printdt3();
/* Students to write the following two routines, and maybe some others */

void rtinit3() {
  /* TODO */
  infin(dt3.costs);
  dt3.costs[0][0] = 7;
  dt3.costs[1][1] = 999;
  dt3.costs[2][2] = 2;
  dt3.costs[3][3] = 0;
  //printdt3(&dt3);

  struct rtpkt initdt3;
  
  initdt3.sourceid = 3;
  initdt3.mincost[0] = 7;
  initdt3.mincost[1] = 999;
  initdt3.mincost[2] = 2;
  initdt3.mincost[3] = 0;
  initdt3.destid = 0;
  tolayer2(initdt3);
  initdt3.destid = 2;
  tolayer2(initdt3);
}


void rtupdate3(struct rtpkt *rcvdpkt) {
  /* TODO */
  int change = 0;
  int the_node = rcvdpkt->sourceid;
  for(int i = 0; i<4; i++)
  {
    if(dt3.costs[i][the_node] > dt3.costs[the_node][the_node] + rcvdpkt->mincost[i])
    {
      dt3.costs[i][the_node] = dt3.costs[the_node][the_node] + rcvdpkt->mincost[i];
      change = 1;
    }
  }
  if(change)
  {
    new_info(src3, dt3.costs);
  }
  printdt3(&dt3);
}

void printdt3(struct distance_table *dtptr) {
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
}







