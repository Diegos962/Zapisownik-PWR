#include "../include/poboczneDopasowywanie.h"

int levenshtein_distance(const QString &s1, const QString &s2)
{
  int s1len = s1.size();
  int s2len = s2.size();
  auto column_start = (decltype(s1len))1;
  auto column = new decltype(s1len)[s1len + 1];
  std::iota(column + column_start, column + s1len + 1, column_start);
	
  for (auto x = column_start; x <= s2len; x++)
    {
      column[0] = x;
      auto last_diagonal = x - column_start;
      for (auto y = column_start; y <= s1len; y++)
	{
	  auto old_diagonal = column[y];
	  auto possibilities =
	    {
	      column[y] + 1,
	      column[y - 1] + 1,
	      last_diagonal + (s1[y - 1] == s2[x - 1]? 0 : 1)
	    };
	  column[y] = std::min(possibilities);
	  last_diagonal = old_diagonal;
	}
    }
  auto result = column[s1len];
  delete[] column;
  return result;
}

bool deleteWord(QString &line, const QString &word)
{
  int pos1 = 0;
  int wSize = word.size();
  pos1 = line.indexOf(word, 0);
  if(pos1 == -1)
    return false;
  do
    {
      if(pos1 == 0)
	{
	  if(line[wSize] == ' ')
	    {
	      line.remove(pos1, wSize);
	      line = line.simplified();
	      return true;
	    }
	}
      else if(pos1 == line.size() - wSize)
	{
	  if(line[line.size()-wSize-1] == ' ')
	    {
	      line.remove(pos1, wSize);
	      line = line.simplified();
	      return true;
	    }
	}
      else if(pos1 != -1)
	{
	  if(line[pos1-1] == ' ' && line[wSize+pos1] == ' ')
	    {
	      line.remove(pos1, wSize);
	      line = line.simplified();
	      return true;
	    }
	}
      pos1 = line.indexOf(word, pos1+wSize);
    }
  while(pos1 != -1);
  return false;
}

void usun_przedrostki(QString &line)
{
  QString przedrostki[21] = {"dr", "dr.", "inż.", "inż", "prof.", "prof", "mgr", "mgr.", "hab.", "hab",
			     "doc.", "doc", "nadzw.", "nadzw", "arch.", "arch", "pwr", "pwr.", "zw.", "zw", "¤"};
  for(int i = 0; i < 21; i++)
    if(deleteWord(line, przedrostki[i]))
      i = 0;
  line.remove(',');
}

void dopasuj(Kurs &a, QList<Prowadzacy> prow)
{
  float max = 0, lev = 0, len = 0;
  float percentage = 0, percentage2 = 0;
  QString nazwa, tmpx, tmp, temp;
  QVector<QPair<QString, QString>> replacements
  {{"ą", "a"}, {"ć", "c"}, {"ę", "e"}, {"ł", "l"}, {"ń", "n"}, {"ó", "o"}, {"ś", "s"}, {"ź", "z"}, {"ż", "z"}};
  if(a.lista().size() != 0)
    if(a.lista().at(0).size() != 0)
      return;
  QList<Prowadzacy> aaaa;
  Prowadzacy xyz;
      
  for(int i = 0; i < a.prowadzacy().size(); i++)
    {
      a.lista().push_back(aaaa);
      a.dane().push_back(xyz);
      max = 45;
      nazwa = a.prowadzacy().at(i);
      nazwa = nazwa.toLower();
      usun_przedrostki(nazwa);
      for (auto const &r : replacements)
	nazwa.replace(r.first, r.second);
      for(auto &b: prow)
	{
	  tmpx = b.nazwa();
	  tmpx = tmpx.toLower();
	  usun_przedrostki(tmpx);
	  for (auto const &r : replacements)
	    tmpx.replace(r.first, r.second);
	  
	  tmp = tmpx.section(" ", 1, -1) + " " + tmpx.section(" ", 0, 0);
	  lev = levenshtein_distance(nazwa, tmp);
	  len = qMax(nazwa.size(), tmp.size());
	  percentage = (len - lev)/len * 100;
	  
	  temp = tmpx.section(" ", 0, 0) + " " + tmpx.section(" ", 1, -1);
	  lev = levenshtein_distance(nazwa, temp);
	  percentage2 = (len - lev)/len * 100;

	  percentage = qMax(percentage, percentage2);
	  if(percentage >= max)
	    {
	      b.setMatch(percentage);
	      a.lista()[i].push_back(b);
	    }
	}
    }
  for(int i = 0; i < a.lista().size(); i++)
   {
      qSort(a.lista()[i].begin(), a.lista()[i].end(), [](const Prowadzacy &s1, const Prowadzacy &s2) {return s1.match() > s2.match();});
      if (a.lista()[i].size() > 0)
	{
	  float var = a.lista()[i].at(0).match();
	  if(var > 95)
	    a.dane()[i] = a.lista()[i].at(0);
	  while(a.lista()[i].size() > 102-var)
	    a.lista()[i].removeLast();
	}
    }
}
