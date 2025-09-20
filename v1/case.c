#define NEXT break
#define I_NEXT 0
#define I_LOOP 1

main()
{
  static int prog[] = {I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_NEXT,I_LOOP};
  int *ip=prog;
  int    count = 10000000;

  for (;;) {
    switch (*ip++) {
    case I_NEXT:
      NEXT;
    case I_LOOP:
      if (count>0) {
	count--;
	ip=prog;
	NEXT;
	/* the rest is to get gcc to make a realistic switch statement */
      }
      else
	return;
    case 12:
    case 8:
    case 4:
    case 5:
      count=25;
      NEXT;
    case 10:
    case 2:
    case -1:
      count--;
      NEXT;
    }
  }
}
