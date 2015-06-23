#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "Grid.h"

using namespace std;


Grid::Grid()
{
  DirDecode={
    {"N",Direction(0,-1)},
    {"S",Direction(0,1)},
    {"E",Direction(1,0)},
    {"W",Direction(-1,0)},
    {"NE",Direction(1,-1)},
    {"NW",Direction(-1,-1)},
    {"SE",Direction(1,1)},
    {"SW",Direction(-1,1)}
  };

//  for ( auto dir_entry : DirDecode )
//  {
//              cout  << dir_entry.first << "=<" << dir_entry.second.x << "," << dir_entry.second.y << ">" << endl;
//  }

  for(int x=0; x<MAX_GRID; x++)
      for (int y=0; y<MAX_GRID; y++)
          Cells[x][y]=EMPTY_CELL;
  Min_X=27; Max_X=36;
  Min_Y=27; Max_Y=36;

  #define SIZEY_GRID_INIT 10
  string Grid_init[SIZEY_GRID_INIT] = {
      "   XXXX   ",
      "   X  X   ",
      "   X  X   ",
      "XXXX  XXXX",
      "X        X",
      "X        X",
      "XXXX  XXXX",
      "   X  X   ",
      "   X  X   ",
      "   XXXX   "};

  for(int j=0; j<SIZEY_GRID_INIT; j++)
  {
      int l=Grid_init[j].size();
      for(int i=0; i<l; i++)
      {
        if (isspace(Grid_init[j].at(i)))
          Cells[Min_X+i][Min_Y+j]=EMPTY_CELL;
        else
          Cells[Min_X+i][Min_Y+j]=INIT_CELL;
      }
  }
}

Grid::~Grid()
{
    //dtor
}

void Grid::Print()
{
  //cout << "Origine=(" << Min_X << "," << Min_Y << ")" << endl;
  for (int y=Min_Y; y<=Max_Y; y++)
  {
    for(int x=Min_X; x<=Max_X; x++)
          cout << Cells[x][y];
      cout <<  endl;
  }
}

void Grid::Stats()
{
  int nb_voisins[8]={0};
  int n;

  for (int y=Min_Y; y<=Max_Y; y++)
  {
    for(int x=Min_X; x<=Max_X; x++)
    {
      if (Cells[x][y]!=EMPTY_CELL)
      {
        n = Cell_0_1(Cells[x-1][y-1])
          + Cell_0_1(Cells[x-1][y])
          + Cell_0_1(Cells[x-1][y+1])
          + Cell_0_1(Cells[x][y-1])
          + Cell_0_1(Cells[x][y+1])
          + Cell_0_1(Cells[x+1][y-1])
          + Cell_0_1(Cells[x+1][y])
          + Cell_0_1(Cells[x+1][y+1]);
        nb_voisins[n]++;
//        cout << x << "," << y << " : " << n << " " << nb_voisins[n] << endl;
      }
    }
  }
  cout << endl << "Stats :" << endl;
  for(n=0; n<=7; n++)
  {
    cout << n << " -> " << nb_voisins[n] << " voisins" << endl;
  }
}

//inline int char Grid::operator() (const int x, const int y) const
//{
//  return Cells[x][y];
//}
//
//inline int char& Grid::operator() (const int x, const int y)
//{
//  return Cells[x][y];
//}

bool Grid::ImportPoints(const string &Filename)
{
  stringstream ss;
  int NumLigne=0;
  int x,y, O_x, O_y;
  char c_contents[3];
  string ligne;
  string dir;

  ifstream fichier(Filename);
  if (fichier.fail()) {
    cerr << "Fichier introuvable. " << Filename << endl;
    return false;
  }

  if (!getline(fichier,ligne))
  {
    cerr << "Fichier vide ! " << Filename << endl;
    return false;
  }
  if (sscanf(ligne.c_str(),"ORIGIN=%i,%i", &O_x, &O_y)!=2)
  {
    cerr << "'ORIGIN=' attendu " << endl;
    return false;
  }
  //cout << ligne << " : " << O_x << "," << O_y << endl;

  while (getline(fichier,ligne))
  {
    NumLigne++;
    if (sscanf(ligne.c_str(),"%i,%i,%2s", &x, &y, c_contents)!=3)
    {
      cerr << "Format incorrect ligne " << NumLigne << endl;
      return false;
    }
    dir=(string)c_contents;
    //cout << ligne << " : " << x+O_x << "," << y+O_y << dir << endl;
    if (!AddPointInLine(x+O_x, y+O_y, dir))
      return false;
    //Print();
  }

  fichier.close();
  return true;
}

bool Grid::AddPointInLine(int x0, int y0, const string &sDir)
{
  Direction dir=DirDecode[sDir];
  int nb_vides=0;
  int x=x0, y=y0;

  //cout << endl << "Start at " << x0 << "," << y0 << " - Dir : " << dir.x << "," << dir.y << endl;
  for (int i=0;i<=4;i++)
  {
    if (Cells[x][y] == EMPTY_CELL)
    {
      nb_vides++;
      x0=x; y0=y;
    }
    //cout << x << "," << y << " = " << Cells[x][y] << endl;
    x+=dir.x; y+=dir.y;
  }
  if (nb_vides !=1)
  {
    cerr << nb_vides << " vides. Ligne non complétée." << endl;
    return false;
  }
  return AddPoint(x0, y0);
}

bool Grid::AddPoint(int x, int y)
{
  if (x<1 || y <1 || x>(MAX_GRID-1) || y>(MAX_GRID-1))
  {
    cerr << x << "," << y << " hors bornes." << endl;
    return false;
  }
  //cout << "Ajout de " << x << "," << y << endl;
  Cells[x][y]=FILLED_CELL;

// augmentation des bornes d'encadrement ?
  if (x<Min_X) Min_X=x;
  if (x>Max_X) Max_X=x;
  if (y<Min_Y) Min_Y=y;
  if (y>Max_Y) Max_Y=y;
  return true;
}

bool Grid::RemovePoint(int x, int y)
{
  if (x<1 || y <1 || x>(MAX_GRID-1) || y>(MAX_GRID-1))
  {
    cerr << x << "," << y << " hors bornes." << endl;
    return false;
  }
  if (Cells[x][y]!=FILLED_CELL)
  {
    cerr << "(" << x << "," << y << ")=" << Cells[x][y] << " n'est supprimable." << endl;
    return false;
  }
  cout << "Suppression de " << x << "," << y << endl;
  Cells[x][y]=EMPTY_CELL;

// reduction des bornes d'encadrement ?
  int n;
  bool reduced;
  if (x==Min_X)
  {
    reduced=true;
    for (n=Min_Y; n<=Max_Y; n++)
      if (Cells[Min_X][n]!=EMPTY_CELL)
      {
        reduced=false;
        break;
      }
    if (reduced)
      Min_X++;
  }
  if (y==Min_Y)
  {
    reduced=true;
    for (n=Min_X; n<=Max_X; n++)
      if (Cells[n][Min_Y]!=EMPTY_CELL)
      {
        reduced=false;
        break;
      }
    if (reduced)
      Min_Y++;
  }
  if (x==Max_X)
  {
    reduced=true;
    for (n=Min_Y; n<=Max_Y; n++)
      if (Cells[Max_X][n]!=EMPTY_CELL)
      {
        reduced=false;
        break;
      }
    if (reduced)
      Max_X--;
  }
  if (x==Max_Y)
  {
    reduced=true;
    for (n=Min_X; n<=Max_X; n++)
      if (Cells[n][Max_Y]!=EMPTY_CELL)
      {
        reduced=false;
        break;
      }
    if (reduced)
      Max_Y--;
  }

  return true;
}
