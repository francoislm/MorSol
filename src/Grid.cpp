#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "Grid.h"

using namespace std;


Grid::Grid()
  :Direction_Decode_Str{
    {"N",Direction(0,-1)},
    {"NE",Direction(1,-1)},
    {"E",Direction(1,0)},
    {"SE",Direction(1,1)},
    {"S",Direction(0,1)},
    {"SW",Direction(-1,1)},
    {"W",Direction(-1,0)},
    {"NW",Direction(-1,-1)}
  },
  Direction_Decode_Int{
    Direction(0,-1),
    Direction(1,-1),
    Direction(1,0),
    Direction(1,1),
    Direction(0,1),
    Direction(-1,1),
    Direction(-1,0),
    Direction(-1,-1)
  },
  NbVoisins{{{0}}}
{
//
//  for ( auto dir_entry : Direction_Decode_Str )
//  {
//              cout  << dir_entry.first << "=<" << dir_entry.second.x << "," << dir_entry.second.y << ">" << endl;
//  }

  for(int x=0; x<MAX_GRID; x++)
      for (int y=0; y<MAX_GRID; y++)
          Cells[x][y]=EMPTY_CELL;
//  Min_X=27; Max_X=36;
//  Min_Y=27; Max_Y=36;
  Min_X=Max_X=27;
  Min_Y=Max_Y=27;

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
        if (!isspace(Grid_init[j].at(i)))
          AddPoint(Min_X+i, Min_Y+j,INIT_CELL);
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
#define XXX 34
for (int y=Min_Y-1; y<=Max_Y+1; y++) {for (int dd=0; dd<NB_DIRECTIONS; dd++) printf("(%d,%d)<%d> = %d\n", XXX, y, dd, NbVoisins[XXX][y][dd]); printf("\n");}
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
    cout << n << " voisins : " << nb_voisins[n] << endl;
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
  Direction dir=Direction_Decode_Str[sDir];
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

inline bool Grid::OutOfBounds(int x, int y)
{
  if (x<1 || y <1 || x>(MAX_GRID-1) || y>(MAX_GRID-1))
  {
    cerr << x << "," << y << " hors bornes." << endl;
    return true;
  }
  return false;
}

bool Grid::AddPoint(int x, int y, unsigned char TypeCell)
{
  if (OutOfBounds(x, y)) return false;

  if (Cells[x][y]!=EMPTY_CELL)
  {
    cerr << x << "," << y << " non vide, ajout Point impossible." << endl;
    return false;
  }

  if (TypeCell==EMPTY_CELL)
  {
    cerr << "Ajout vide impossible." << endl;
    return false;
  }

// Ajout du Point
  Cells[x][y]=TypeCell;

// completer NbVoisins pour les voisins
  int d_x, d_y, n, xx, yy;
  for (int d = 0, r_d = NB_DIRECTIONS / 2; d < NB_DIRECTIONS; d++, r_d = (r_d + 1) % NB_DIRECTIONS)
  { // r_d = direction opposée
    n = NbVoisins[x][y][r_d] + 1; // nb voisins à ajouter dans la direction d
    xx = x, yy = y;
    d_x = Direction_Decode_Int[d].x;
    d_y = Direction_Decode_Int[d].y;
    for (int i=NbVoisins[x][y][d] + 1; i>0; i--)
    {
      xx += d_x;
      yy += d_y;
      NbVoisins[xx][yy][r_d] += n;
    }
  }

// Augmentation des bornes d'encadrement ?
  if (x<Min_X) Min_X=x;
  if (x>Max_X) Max_X=x;
  if (y<Min_Y) Min_Y=y;
  if (y>Max_Y) Max_Y=y;
  return true;
}

bool Grid::RemovePoint(int x, int y)
{
  if (OutOfBounds(x, y)) return false;

  if (Cells[x][y]!=FILLED_CELL)
  {
    cerr << "(" << x << "," << y << ")=" << Cells[x][y] << " n'est supprimable." << endl;
    return false;
  }
  cout << "Suppression de " << x << "," << y << endl;
// Suppression du Point
  Cells[x][y]=EMPTY_CELL;

// completer NbVoisins pour les voisins
  int d_x, d_y, n, xx, yy;
  for (int d = 0, r_d = NB_DIRECTIONS / 2; d < NB_DIRECTIONS; d++, r_d = (r_d + 1) % NB_DIRECTIONS)
  { // r_d = direction opposée
    n = NbVoisins[x][y][r_d] + 1; // nb voisins à supprimer dans la direction d
    xx = x, yy = y;
    d_x = Direction_Decode_Int[d].x;
    d_y = Direction_Decode_Int[d].y;
    for (int i=NbVoisins[x][y][d] + 1; i>0; i--)
    {
      xx += d_x;
      yy += d_y;
      NbVoisins[xx][yy][r_d] -= n;
    }
  }

// Réduction des bornes d'encadrement ?
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


