#include <iostream>
#include <Grid.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h>

#define ARGS_OPT "f:"
// -f filename = to import a file of points

using namespace std;

Grid myGrid;

int main(int argc, char *argv[])
{
//  SetConsoleOutputCP(1252);
//  SetConsoleCP(1252);

  // pour caler la sortie de la console en CP850
  setlocale(LC_ALL, "");

  bool fFlag = false;
  string fFilename;
  //int index;
  int c;

  opterr = 0;
  while ((c = getopt (argc, argv, ARGS_OPT)) != -1)
      switch (c)
      {
      case 'f':
        fFlag = true;
        fFilename = optarg;
        break;
      case '?':
        if (optopt == 'i')
          cerr << "Option -"  << optopt << " requires an argument." << endl;
        else if (isprint (optopt))
          cerr << "Unknown option -"  << optopt << endl;
        else
          cerr << "Unknown option character \\x"  << hex << optopt << endl;
        return 1;
      default:
        abort();
      }

//  for (index = optind; index < argc; index++)
//    cerr << "Non-option argument " << argv[index] << endl;
  if (optind != argc) {
    cerr << "Non-option argument." << endl;
    return 1;
  }

  myGrid.Print();
  //myGrid.Stats();

  system("pause");

  if (fFlag) {
      myGrid.ImportPoints(fFilename);
  }

  cout << endl;
  myGrid.Print();
  myGrid.Stats();

  // to wait
//  cin;
  return 0;
}
