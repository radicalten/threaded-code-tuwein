typedef void (*Inst)();

Inst *ip;
extern Inst prog[];

#define NEXT ((*ip++)())

void next()
{
}

void loop()
{
  static int count=10000000;

  if (count<=0)
    exit(0);
  count--;
  ip=prog;
}

Inst prog[] = {next,next,next,next,next,next,next,next,next,loop};

main()
{
  ip=prog;
  for (;;)
    NEXT;
}
