main()
{
 start:
  next(); next(); next(); next(); next();
  next(); next(); next(); next();
  loop(); goto start;
}

next()
{
}

loop()
{
  static int count=10000000;

  if (count<=0)
    exit(0);
  count--;
}
