#include <stdio.h>

#include "sim_engine.h"
#include "fixit.h"

extern int TRACE;
extern int YES;
extern int NO;

static int src2 = 2;

struct distance_table {
  int costs[4][4];
} dt2;

void printdt2();
/* Students to write the following two routines, and maybe some others */

void rtinit2() {
  /* TODO */
  infin(dt2.costs);
  dt2.costs[0][0] = 3;
  dt2.costs[1][1] = 1;
  dt2.costs[2][2] = 0;
  dt2.costs[3][3] = 2;
  //printdt2(&dt2);

  struct rtpkt initdt2;
  
  initdt2.sourceid = 2;
  initdt2.mincost[0] = 3;
  initdt2.mincost[1] = 1;
  initdt2.mincost[2] = 0;
  initdt2.mincost[3] = 2;
  initdt2.destid = 0;
  tolayer2(initdt2);
  initdt2.destid = 1;
  tolayer2(initdt2);
  initdt2.destid = 3;
  tolayer2(initdt2);
}

void rtupdate2(struct rtpkt *rcvdpkt) {
  /* TODO */
  int change = 0;
  int the_node = rcvdpkt->sourceid;
  for(int i = 0; i<4; i++)
  {
    if(dt2.costs[i][the_node] > dt2.costs[the_node][the_node] + rcvdpkt->mincost[i])
    {
      dt2.costs[i][the_node] = dt2.costs[the_node][the_node] + rcvdpkt->mincost[i];
      change = 1;
    }
  }
  if(change)
  {
    new_info(src2, dt2.costs);
  }
  printdt2(&dt2);
}

void printdt2(struct distance_table *dtptr) {
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







