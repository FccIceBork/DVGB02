#include <stdio.h>

#include "sim_engine.h"
#include "fixit.h"

extern int TRACE;
extern int YES;
extern int NO;

static int src1 = 1;

struct distance_table
{
  int costs[4][4];
} dt1;

void printdt1();
/* Students to write the following two routines, and maybe some others */

void rtinit1()
{
  /* TODO */
  infin(dt1.costs);
  dt1.costs[0][0] = 1;
  dt1.costs[1][1] = 0;
  dt1.costs[2][2] = 1;
  dt1.costs[3][3] = 999;
  //printdt1(&dt1);

  struct rtpkt initdt1;

  initdt1.sourceid = 1;
  initdt1.mincost[0] = 1;
  initdt1.mincost[1] = 0;
  initdt1.mincost[2] = 1;
  initdt1.mincost[3] = 999;
  initdt1.destid = 0;
  tolayer2(initdt1);
  initdt1.destid = 2;
  tolayer2(initdt1);
}

void rtupdate1(struct rtpkt *rcvdpkt)
{
  /* TODO */
  int change = 0;
  int the_node = rcvdpkt->sourceid;
  for (int i = 0; i < 4; i++)
  {
    if (dt1.costs[i][the_node] > dt1.costs[the_node][the_node] + rcvdpkt->mincost[i])
    {
      dt1.costs[i][the_node] = dt1.costs[the_node][the_node] + rcvdpkt->mincost[i];
      change = 1;
    }
  }
  if (change)
  {
    new_info(src1, dt1.costs);
  }
  printdt1(&dt1);
}

void printdt1(struct distance_table *dtptr)
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n", dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n", dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n", dtptr->costs[3][0], dtptr->costs[3][2]);
}

void linkhandler1(int linkid, int newcost)
{
  /* DON'T CHANGE */
}
