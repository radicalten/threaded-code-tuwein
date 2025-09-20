/* a version of direct.c with only 10% BTB hit rate (for a
   straightforward BTB) */

#define NEXT goto **ip++
main()
{
  static void  *prog[] = {&&next1,&&next2,&&next3,&&next4,&&next5,&&next6,&&next7,&&next8,&&next9,&&loop};
  void **ip=prog;
  int    count = 10000000;
  NEXT;

 next9:
  NEXT;
 next8:
  NEXT;
 next7:
  NEXT;
 next6:
  NEXT;
 next5:
  NEXT;
 next4:
  NEXT;
 next3:
  NEXT;
 next2:
  NEXT;
 next1:
  NEXT;
 loop:
  if (count>0) {
    count--;
    ip=prog;
    NEXT;
  }
}
