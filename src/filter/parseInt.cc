
bool
parseInt(const char* p, int& value)
{
  bool ok = true;
  int v = 0;
  while (*p)
  {
    if (! isdigit(*p))
    {
      ok = false;
      break;
    }
    v = 10*v + (*p - '0');
    ++p;
  }
  if (ok)
    value = v;
  return ok;
}



