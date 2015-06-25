#ifndef GRID_H
#define GRID_H
#include <map>

#define MAX_GRID 64
#define EMPTY_CELL '.'
#define INIT_CELL 'O'
#define FILLED_CELL 'X'

#define NB_DIRECTIONS 8

#define Cell_0_1(c) ((c==EMPTY_CELL)?0:1)


class Grid
{
    struct Point
    {
      short int x,y;
      Point (){x=0;y=0;};
      Point (short int _x, short int _y) {x=_x; y=_y;}
    };
    typedef Point Direction;

    public:
        Grid();
        virtual ~Grid();
//        int char& operator() (const int x, const int y);
//        int char  operator() (const int x, const int y) const;
        void Print();
        void Stats();
        bool ImportPoints(const std::string &Filename);
        bool AddPointInLine(int x, int y, const std::string &direction);
        bool AddPoint(int x, int y, unsigned char TypeCell=FILLED_CELL);
        bool RemovePoint(int x, int y);
        std::map <std::string, Direction> DirDecode;

    protected:
    private:
        unsigned char Cells[MAX_GRID][MAX_GRID];
        short int NbVoisins[MAX_GRID][MAX_GRID][NB_DIRECTIONS];
        short int Min_X, Max_X, Min_Y, Max_Y;
        inline int Cell_0or1 (unsigned char);
};

#endif // GRID_H
