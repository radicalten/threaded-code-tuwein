#define NEXT goto **ip++
main()
{
  static void  *prog[] = {&&next,&&next,&&next,&&next,&&next,&&next,&&next,&&next,&&next,&&loop};
  void **ip=prog;
  int    count = 10000000;

 next:
  NEXT;
 loop:
  if (count>0) {
    count--;
    ip=prog;
    NEXT;
  }
}
