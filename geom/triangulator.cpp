#include "triangulator.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>

/**
  Gray code functions adapted from http://en.wikipedia.org/wiki/Gray_code
  under the Creative Commons CC-BY-SA 3.0 license
  */
/*
        The purpose of this function is to convert an unsigned
        binary number to reflected binary Gray code.

        The operator >> is shift right. The operator ^ is exclusive or.
*/
int Triangulator::binaryToGray(int num)
{
        return (num >> 1) ^ num;
}

/*
        The purpose of this function is to convert a reflected binary
        Gray code number to a binary number.
*/
int Triangulator::grayToBinary(int num)
{
    unsigned int numBits = 8 * sizeof(num);
    unsigned int shift;
    for (shift = 1; shift < numBits; shift = 2 * shift)
    {
        num = num ^ (num >> shift);
    }
    return num;
}

std::vector<cv::Vec3d> Triangulator::computeGeometry(
        cv::Mat encoding,
        QCamera *camera,
        QProjector *projector,
        uint decimation)
{
    double px;
    uint x,y;
    cv::Vec3d P_up, P_fwd, M_hat, D, M, Cp;
    std::vector<cv::Vec3d> out;

    D = projector->getPosition() - camera->getPosition();
    P_up = projector->getUpVector();
    Cp = camera->getPosition();

    encoding.convertTo(encoding,CV_64F);

    for(x=0; x<(encoding.cols); x++){
        for(y=0; y<(encoding.rows); y++){
            px = encoding.at<double>(y,x);
            if(px<0) continue;
            P_fwd = projector->getPixelRay(px,0);
            M_hat = camera->getPixelRay(x,y);
            M = Triangulator::sumTo(M_hat,P_up,P_fwd,D);
            M = M+Cp;
            //if(M[1]>4 || M[2]<0) continue;

            out.push_back(M);

        }
    }
    std::cout << out.size() << " output points \n";
    return out;

}

Sheet* Triangulator::computeSheet(
        cv::Mat encoding,
        QCamera *camera,
        QProjector *projector,
        uint decimation)
{
    double px;
    uint x,y;
    cv::Vec3d P_up, P_fwd, M_hat, D, M, Cp;
    Sheet *sheet = new Sheet(cv::Size(encoding.cols,encoding.rows));

    D = projector->getPosition() - camera->getPosition();
    P_up = projector->getUpVector();
    Cp = camera->getPosition();

    std::cout << "Projector posn:\n " << cv::Mat(projector->getPosition())
              << "\nProjector rotn:\n " << projector->getOrientation()
              << "\nProjector fwd:\n " << projector->getFwdVector()
              << "\nCamera posn:\n " << cv::Mat(camera->getPosition())
              << "\nCamera rotn:\n " << camera->getOrientation()
              << "\nCamera fwd:\n " << camera->getFwdVector() << "\n";


    encoding.convertTo(encoding,CV_64F);

    for(x=0; x<(encoding.cols); x++){
        for(y=0; y<(encoding.rows); y++){
            px = encoding.at<double>(y,x);
            if(px<0) continue;
            P_fwd = projector->getPixelRay(px,0);
            M_hat = camera->getPixelRay(x,y);
            M = Triangulator::sumTo(M_hat,P_up,P_fwd,D)
                    +Cp;
            //if(M[1]>4 || M[2]<0) continue;

            sheet->setPoint(x,y,M);

        }
    }
    return sheet;

}

cv::Vec3d Triangulator::sumTo(const cv::Vec3d M_hat,
                              const cv::Vec3d P_up,
                              const cv::Vec3d P_fwd,
                              const cv::Vec3d D)
{
    // vector to measured point
    static cv::Vec3d M;

    // working matrix
    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);

    // solution matrix
    cv::Mat k;


    // set up working matrix
    /*A = cv::Matx33d(M_hat[0],   P_up[0],  P_fwd[0],
                    M_hat[1],   P_up[1],  P_fwd[1],
                    M_hat[2],   P_up[2],  P_fwd[2]);* /
    A.col(0) = cv::Mat(M_hat).reshape(1);
    A.col(1) = cv::Mat(P_up).reshape(1);
    A.col(2) = cv::Mat(P_fwd).reshape(1);*/
    for(uint i=0;i<3;i++){
        A.at<double>(i,0) = M_hat[i];
        A.at<double>(i,1) = P_up[i];
        A.at<double>(i,2) = P_fwd[i];
    }

    cv::Mat D_mat = cv::Mat(D).reshape(1);

    // invert the matrix
    A = A.inv();

    // obtain the solution
    k = A * D_mat;

    // scale the output
    M = k.at<double>(0,0) * M_hat;

    return M;
}

cv::Vec3d Triangulator::intersectRayPlane(const cv::Vec3d planeOrigin,
                                          const cv::Vec3d planeRot,
                                          const cv::Vec3d rayOrigin,
                                          const cv::Vec3d rayDirection)
{
    cv::Vec3d x(1,0,0);
    cv::Vec3d z(0,0,1);
    cv::Mat matNorm;
    cv::Vec3d planeNorm;
    cv::Vec3d P_up, P_right;
    cv::Mat rotn;
    cv::Vec3d D,M;
    // calculate the normal
    cv::Rodrigues(planeRot,rotn);
    matNorm = rotn * cv::Mat(z);
    matNorm.reshape(1);
    planeNorm = cv::Vec3d(matNorm);
    // create a pair of vectors in the plane
    P_up = x.cross(planeNorm);
    P_right = P_up.cross(planeNorm);
    // calculate the difference vector
    D = rayOrigin - planeOrigin;

    M = sumTo(rayDirection,P_up,P_right,D);
    M = M + planeOrigin;

    return M;
}

bool Triangulator::inTri(const cv::Vec3d P, const cv::Vec3d A, const cv::Vec3d B, const cv::Vec3d C)
{
    // Compute vectors
    cv::Vec3d v0 = C - A;
    cv::Vec3d v1 = B - A;
    cv::Vec3d v2 = P - A;

    // Compute dot products
    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);

    // Compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

cv::Vec3d Triangulator::getPlane(std::vector<cv::Vec3d> pts)
{
    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);
    cv::Mat b = cv::Mat::zeros(3,1,CV_64F);
    uint i;
    cv::Vec3d pt;
    for(i=0;i<pts.size();i++){
        pt = pts[i];
        A.at<double>(0,0) += pt[2] * pt[2];
        A.at<double>(1,1) += pt[1] * pt[1];
        A.at<double>(1,0) += pt[2] * pt[1];
        A.at<double>(2,0) += pt[2];
        A.at<double>(2,1) += pt[1];

        b.at<double>(0) += pt[0] * pt[2];
        b.at<double>(1) += pt[0] * pt[1];
        b.at<double>(2) += pt[0];
    }
    A.at<double>(0,1) = A.at<double>(1,0);
    A.at<double>(0,2) = A.at<double>(2,0);
    A.at<double>(1,2) = A.at<double>(2,1);
    A.at<double>(2,2) = (double) pts.size();

    cv::Mat m = A.inv() * b;
    return cv::Vec3d(m.at<double>(2),m.at<double>(1),m.at<double>(0));
}

void Triangulator::cropToStandard(cv::Mat encoding,
                                  CalibrationStandard *standard,
                                  std::vector<cv::Point2f> imagePts)
{
    int u,v;
    cv::Point2f _A = imagePts.at(standard->getPointA());
    cv::Point2f _B = imagePts.at(standard->getPointB());
    cv::Point2f _C = imagePts.at(standard->getPointC());
    cv::Point2f _D = imagePts.at(standard->getPointD());
    cv::Vec3d A(_A.x,_A.y,0);
    cv::Vec3d B(_B.x,_B.y,0);
    cv::Vec3d C(_C.x,_C.y,0);
    cv::Vec3d D(_D.x,_D.y,0);
    cv::Vec3d P;
    bool inRect;
    for(u=0;u<encoding.cols;u++){
        for(v=0;v<encoding.rows;v++){
            P = cv::Vec3d(u,v,0);
            inRect = inTri(P,A,B,C) || inTri(P,B,C,D);
            if(!inRect){
                encoding.at<float>(v,u) = -1;
            }
        }
    }
}



cv::Vec3d Triangulator::getCentroid(std::vector<cv::Vec3d> pts)
{
    double x,y,z;
    uint i;
    for(i=0;i<pts.size();i++){
        x+=pts[i][0];
        y+=pts[i][1];
        z+=pts[i][2];
    }
    return cv::Vec3d(x/i,y/i,z/i);
}

void Triangulator::computePhase(std::vector<cv::Mat> fringes,
                                   cv::Mat output,
                                   double scale)
{
    uint m = fringes.size();
    double alpha,I;
    uint i,x,y;

    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);
    cv::Mat B = cv::Mat::zeros(3,1,CV_64F);
    cv::Mat X;
    alpha = 2*M_PI/m;

    A.at<double>(0,0) = m;
    for(i=0;i<m;i++){
        A.at<double>(1,0) += cos(alpha*i);
        A.at<double>(2,0) += sin(alpha*i);
        A.at<double>(1,1) += cos(alpha*i)*cos(alpha*i);
        A.at<double>(2,1) += cos(alpha*i)*sin(alpha*i);
        A.at<double>(2,2) += sin(alpha*i)*sin(alpha*i);
    }
    A.at<double>(0,1) = A.at<double>(1,0);
    A.at<double>(0,2) = A.at<double>(2,0);
    A.at<double>(1,2) = A.at<double>(2,1);

    A = A.inv();

    for(x=0;x<output.cols;x++){
       for(y=0;y<output.rows;y++){
           B = cv::Mat::zeros(3,1,CV_64F);
           for(i=0;i<m;i++){
               I = (double)fringes[i].at<unsigned char>(y,x);
               B.at<double>(0) += I;
               B.at<double>(1) += I*cos(alpha*i);
               B.at<double>(2) += I*sin(alpha*i);
           }
           X = A*B;
           output.at<double>(y,x) = atan2(X.at<double>(2),
                                          X.at<double>(1));
       }
    }
    output /= scale;
}

cv::Mat Triangulator::computeBinary(
        std::vector<cv::Mat> frames,
        uint maskThreshold,
        uint drop)
{
    // current bit
    uint n;

    // for looping through the images
    uint x, y;

    // working bit value
    int bitVal;

    // highest bit
    uint nmax = frames.size()/2;

    // temporary holders
    cv::Mat img, inv;

    // quality mask
    cv::Mat qmask;

    // encoding
    cv::Mat encoding;

    // generate the quality mask
    frames.at(2 *(nmax-1)).convertTo(img,CV_32S);
    frames.at(2*nmax-1).convertTo(inv,CV_32S);
    qmask = img-inv;

    // initialize encoding
    encoding = cv::Mat::zeros(frames.at(0).rows,
                              frames.at(0).cols,
                              CV_32S);

    // collect the bits
    for(n=0;n<nmax;n++){

        // gather our images
        frames.at(2 * n).convertTo(img,CV_32S);
        frames.at(2*n+1).convertTo(inv,CV_32S);

        // difference
        img = inv - img;

        // collect the bit
        for(x=0; x<img.cols; x++){
            for(y=0; y<img.rows; y++){
                // if the difference is positive
                if(img.at<int>(y,x)>0){
                    // add the bit
                    encoding.at<int>(y,x) += 1<<n;
                }
            }
        }
    }

    // decode and apply mask
    for(x=0;x<encoding.cols;x++){
        for(y=0;y<encoding.rows;y++){
            if(abs(qmask.at<int>(y,x)) > maskThreshold){
                bitVal = grayToBinary(encoding.at<int>(y,x));
                bitVal = (bitVal >> drop) << drop;
                encoding.at<int>(y,x) = bitVal;
            }else{
                encoding.at<int>(y,x) = -1;
            }
        }
    }

    return encoding;
}

#define HUE_FORMAT CV_8U

cv::Mat Triangulator::maphsv(cv::Mat img, double scale)
{
    std::vector<cv::Mat> hsv;
    hsv.resize(3);
    img.convertTo(img,CV_32S);
    hsv[0] = img*180/scale;
    hsv[1] = cv::Mat::ones(img.rows,img.cols,HUE_FORMAT)*255;
    hsv[2] = cv::Mat::ones(img.rows,img.cols,HUE_FORMAT)*255;
//    cv::threshold(image,hsv[2],0,255,CV_THRESH_BINARY);
    for(uint x=0; x<img.cols; x++){
        for(uint y=0; y<img.rows; y++){
            hsv[2].at<char>(y,x) = img.at<int>(y,x)>0?255:0;
        }
    }
    hsv[0].convertTo(hsv[0],HUE_FORMAT);
    hsv[2].convertTo(hsv[2],HUE_FORMAT);

    cv::Mat out;
    cv::merge(hsv,out);
    cv::cvtColor(out,out,CV_HSV2BGR);
    return out;
}

