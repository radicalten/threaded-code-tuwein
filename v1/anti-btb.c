/* a version of direct.c with only 10% BTB hit rate (for a
   straightforward BTB) */

#define NEXT goto **ip++
main()
{
  static void  *prog[] = {&&next1,&&next1,&&next2,&&next2,&&next3,&&next3,&&next2,&&next1,&&next3,&&loop,&&next4,&&next5};
  void **ip=prog;
  int    count = 10000000;

 next1:
  NEXT;
 next4:
  NEXT;
 next2:
  NEXT;
 next5:
  NEXT;
 next3:
  NEXT;
 loop:
  if (count>0) {
    count--;
    ip=prog;
    NEXT;
  }
}
