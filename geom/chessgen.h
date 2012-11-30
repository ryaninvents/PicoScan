#ifndef FR_CHESSGEN_H
#define FR_CHESSGEN_H

#include <vector>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

namespace fr {

class ChessGen
{
public:
    static vector<Point3f> getBoard(Size size, double scale, bool asym);
};

} // namespace fr

#endif // FR_CHESSGEN_H
