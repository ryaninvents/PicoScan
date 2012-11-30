#include "chessgen.h"

namespace fr {

vector<Point3f> ChessGen::getBoard(Size size, double scale, bool asym)
{
    int x,y;
    double _x, _y;
    static vector<Point3f> pts;
    for(x=0;x<size.height;x++){
        for(y=0;y<size.width;y++){
            _x = x;
            _y = y;
            if(asym){
                if(x%2 == 1) _y += 0.5;
                _x /= 2;
            }
            _x *= scale;
            _y *= scale;
            pts.push_back(Point3d(_y,_x,0));
        }
    }
    return pts;
}


} // namespace fr
