void parseCMD(uint8_t reqCode)
{
  switch(reqCode)
  {
    case 0x80:
    {
      runRTES=true;
      break;
    }
  }
}
