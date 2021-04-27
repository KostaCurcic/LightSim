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
        pointBuff[i] = o + scale / res * i + n * fun(1.0 / res * i);
    }
    cam.drawUnsafe(pointBuff, res + 1, 0, 0, 0, 1);
}

Ray FunctionalObj::intersect(Ray& r)
{
    r.genLineFormula();

    int divs = pow(2, divisions);
    float stepSize = res / (float)divs;
    for (float i = 0; i < res; i += stepSize) {
        if (Line(pointBuff[(int)i], pointBuff[(int)(i + stepSize)]).intersect(r).o.isValid()) {
            return intDivide(i, i + stepSize, divs, r);
        }
    }
    return Ray();
}

Ray FunctionalObj::intDivide(int min, int max, int size, Ray& r)
{
    int range = max - min;
    Ray retRay;
    if (range > size) {
        float stepSize = range / (float)size;
        for (float i = min; i < max; i += stepSize) {
            if (Line(pointBuff[(int)i], pointBuff[(int)(i + stepSize)]).intersect(r).o.isValid()) {
                retRay = intDivide(i, i + stepSize, size, r);
                if (retRay.o.isValid()) return retRay;
                else return Line(pointBuff[(int)i], pointBuff[(int)(i + stepSize)]).intersect(r);
            }
        }
    }
    else {
        Ray ret;
        for (int i = min; i < max; i++) {
            ret = Line(pointBuff[(int)i], pointBuff[(int)(i + 1)]).intersect(r);
            if (ret.o.isValid()) {
                return ret;
            }
        }
        return Line(pointBuff[min], pointBuff[max]).intersect(r);
    }
    return Ray();
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
