#include "../include/poboczneTerminy.h"

QString DecodeTermin(const QString &ter)
{
  QStringList data = ter.split(";", QString::SkipEmptyParts);
  QString ret = "";
  for(QString str: data)
    {
      switch(str[1].digitValue())
	{
	case 0:
	  ret += "pn";
	  break;
	case 1:
	  ret += "wt";
	  break;
	case 2:
	  ret += "sr";
	  break;
	case 3:
	  ret += "cz";
	  break;
	case 4:
	  ret += "pt";
	  break;
	}
      switch(str[0].digitValue())
	{
	case 0:
	  ret += "/TN";
	  break;
	case 1:
	  ret += "/TP";
	  break;
	case 2:
	  ret += "";
	  break;
	}
      ret += "  ";
      ret += str.mid(3, 2);
      ret += ":";
      ret += str.mid(5, 2);
      ret += " - ";
      ret += str.mid(7, 2);
      ret += ":";
      ret += str.mid(9, 2);
      ret += "; ";
    }
  return ret;
}


QString godziny(const QString &text, const int &start)
{
  if(text.size() < 11)
    return "";
  int pos1 = text.indexOf('-', start);
  if(text.size()-pos1 < 5)
    return "";
  QString godz1 = "", godz2 = "";
  if(pos1 > 4)
    {
      int pos2;
      godz1 = text.mid(pos1-5, 5);
      godz2 = text.mid(pos1+1, 5);
      if( (pos2 = godz1.indexOf(':')) > 0)
	godz1.remove(pos2, 1);
      else
	return "";
      if( (pos2 = godz2.indexOf(':')) > 0)
	godz2.remove(pos2, 1);
      else
	return "";
      return godz1+godz2;
    }
  return "";
}

QString terminy(QString text, QString &ret)
{
  int pos1 = 0;
  QString kod;
  do
    {
      pos1 = text.indexOf(',');
      kod = "";
      if(pos1 > 0)
	text.remove(pos1, text.size()-pos1);
      if( (pos1 = text.indexOf("pn")) >= 0)
	kod += "0";
      else if( (pos1 = text.indexOf("wt")) >= 0)
	kod += "1";
      else if( (pos1 = text.indexOf("sr")) >= 0 || (pos1 = text.indexOf("śr")) >= 0)
	kod += "2";
      else if( (pos1 = text.indexOf("cz")) >= 0)
	kod += "3";
      else if( (pos1 = text.indexOf("pt")) >= 0)
	kod += "4";
      else
	return "";
      if(text.indexOf("TN") >= pos1 && text.indexOf("TN") < pos1+4)
	kod.insert(0, "0");
      else if(text.indexOf("TP") >= pos1 && text.indexOf("TP") < pos1+4)
	kod.insert(0, "1");
      else
	kod.insert(0, "2");
      if(pos1 >= 0)
	{
	  int pos2 = text.indexOf('(');
	  int pos3 = text.indexOf(')', pos2);
	  if(pos3-pos2 == 3 && pos1-pos2 == 1)
	    kod += "1";
	  else
	    kod += "0";
	  text.remove(pos1, 2);
	}
      QString hours = godziny(text, pos1);
      if(hours.size() == 0)
	return "";
      ret += (kod + hours + ';');
    }
  while(pos1 >= 0);
  return "";
}

bool sprawdzTerminy(const QString &text)
{
  if(text.size() != 12)
    return false;
  if(text.at(0).digitValue() < 0 || text.at(0).digitValue() > 2)
    return false;
  if(text.at(1).digitValue() < 0 || text.at(1).digitValue() > 4)
    return false;
  if(text.at(2).digitValue() < 0 || text.at(2).digitValue() > 1)
    return false;
  for(int i = 3; i < 11; i++)
    if(text.at(i).digitValue() < 0 || text.at(i).digitValue() > 9)
      return false;
  if(text.at(11) != ';')
    return false;
  return true;
}
