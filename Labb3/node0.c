#include <stdio.h>

#include "sim_engine.h"
#include "fixit.h"

extern int TRACE;
extern int YES;
extern int NO;

static int src0 = 0;

struct distance_table {
  int costs[4][4];
} dt0;

void printdt0();
/* Students to write the following two routines, and maybe some others */

void rtinit0() {
  infin(dt0.costs);   //Fyll alla platser med 999
  /*Fyll i längderna utefter de noder som noden når*/
  dt0.costs[0][0] = 0;
  dt0.costs[1][1] = 1;
  dt0.costs[2][2] = 3;
  dt0.costs[3][3] = 7;
  //printdt0(&dt0);

  struct rtpkt initdt0;
  
  /*Packa in datan och sicka den till närliggande noder*/
  initdt0.sourceid = 0;
  initdt0.mincost[0] = 0;
  initdt0.mincost[1] = 1;
  initdt0.mincost[2] = 3;
  initdt0.mincost[3] = 7;
  initdt0.destid = 1;
  tolayer2(initdt0);
  initdt0.destid = 2;
  tolayer2(initdt0);
  initdt0.destid = 3;
  tolayer2(initdt0);
}

void rtupdate0(struct rtpkt *rcvdpkt) {
  int change = 0;   //Init att det ej hänt någon förändring
  int the_node = rcvdpkt->sourceid;   //Hämta src-ID från noden som man tagit emot data från
  for(int i = 0; i<4; i++)  //Undersök om det finns kortare vägar
  {
    if(dt0.costs[i][the_node] > dt0.costs[the_node][the_node] + rcvdpkt->mincost[i])
    {
      dt0.costs[i][the_node] = dt0.costs[the_node][the_node] + rcvdpkt->mincost[i];
      change = 1;   //En ändring har hänt
    }
  }
  if(change)
  {
    new_info(src0, dt0.costs);  //Packa ny info och skicka vidare
  }
  printdt0(&dt0);
}

void printdt0(struct distance_table *dtptr) {
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

void linkhandler0(int linkid, int newcost) {
  /* DON'T CHANGE */
}

