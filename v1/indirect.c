#define NEXT goto ***ip++
main()
{
  static void *nextp=&&next;
  static void *loopp=&&loop;
  static void  **prog[] = {&nextp,&nextp,&nextp,&nextp,&nextp,&nextp,&nextp,&nextp,&nextp,&loopp};
  void ***ip=prog;
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
