#include "FunctionalObj.h"
#include <cmath>

double defFunct(double per) {
    return 0;
}

FunctionalObj::FunctionalObj() : Line()
{
    fun = defFunct;
    pointBuff = new Point[res + 1];
}

FunctionalObj::FunctionalObj(Point p1, Point p2) : Line(p1, p2)
{
    fun = defFunct;
    pointBuff = new Point[res + 1];
}

FunctionalObj::FunctionalObj(Point p1, Point p2, double(*curveFun)(double)) : Line(p1, p2)
{
    fun = curveFun;
    pointBuff = new Point[res + 1];
}

FunctionalObj::FunctionalObj(double(*curveFun)(double)) : Line()
{
    fun = curveFun;
    pointBuff = new Point[res + 1];
}

FunctionalObj::~FunctionalObj()
{
    delete pointBuff;
}

void FunctionalObj::draw(Camera& cam)
{
    Vector scale = Vector(p2() - p1());
    Vector n = Line::normal().Normalize() * length;
    for (int i = 0; i <= res; i++) {
        pointBuff[i] = scale / res * i + n * fun(1.0 / res * i);
    }
    cam.drawUnsafe(pointBuff, res + 1, o, Color(0, 0, 0, 1));
}

bool FunctionalObj::intersect(Ray& r, Point* col, Vector* normal, double* per)
{
    Ray offsetRay = Ray(r);
    offsetRay.o = offsetRay.o - o;
    offsetRay.genLineFormula();

    int divs = pow(2, divisions);
    float stepSize = res / (float)divs;

    for (float i = 0; i < res; i += stepSize) {
        if (Line(pointBuff[(int)i], pointBuff[(int)(i + stepSize)]).intersect(offsetRay, col, normal, per)) {
            if (col || normal || per) {
                intDivide(i, i + stepSize, divs, offsetRay, col, normal, per);
            }
            return true;
        }
    }
    return false;
}

void FunctionalObj::intDivide(int min, int max, int size, Ray& r, Point* col, Vector* normal, double* per)
{
    int range = max - min;
    double stepSize = range / (float)size;
    Point tempPt;
    Line tempLine;
    double precOff = 0.0;
    if (range > size) {
        for (double i = min; i < max; i += stepSize) {
            tempLine = Line(pointBuff[(int)i], pointBuff[(int)(i + stepSize)]);
            if (tempLine.intersect(r, &tempPt, 0, 0)) {
                *col = tempPt + o;
                intDivide(i, i + stepSize, size, r, col, normal, per);
                return;
            }
        }
    }
    else {
        double tempPer;
        for (int i = min; i < max; i++) {
            tempLine = Line(pointBuff[(int)i], pointBuff[(int)(i + 1)]);
            if (tempLine.intersect(r, &tempPt, normal, per ? &tempPer : 0)) {
                *col = tempPt + o;
                if (per) *per = min / (double)res + (i + tempPer) * stepSize;
                return;
            }
        }
    }
    *normal = Vector();
}

Vector FunctionalObj::normal(const Point& at)
{
    bool useX = false;
    double yLen = p2().y - p1().y;
    double xLen = p2().x - p1().x;
    if (abs(xLen) > abs(yLen) * 10) useX = true;
    double off = useX ? at.x - p1().x : at.y - p1().y;
    double per = off / (useX ? xLen : yLen);

    Vector scale = Vector(p2() - p1());
    Vector n = Line::normal().Normalize() * length;

    int resInd = per * res;
    if (resInd >= res - 1) resInd = res - 2;

    return Vector(pointBuff[resInd + 1] - pointBuff[resInd]).Normalize().Normal();

    /*Vector posVec = Vector((o + scale * (per + 0.001) + n * fun(1.0 / (per + 0.001))) - (o + scale * per + n * fun(1.0 / per)));
    Vector retVec = posVec.Normalize().Normal();
    return retVec;*/
    //return Line::normal();
}
