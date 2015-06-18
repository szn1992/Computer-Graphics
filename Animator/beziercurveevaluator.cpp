#include "beziercurveevaluator.h"
#include <cassert>
#include "math.h"
using namespace Animator;

int BEnChoosei(int n, int i)
{
	int r = 1;
	if(i > n)
		return 0;
	for(int d = 1; d <= i; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										 std::vector<Point>& ptvEvaluatedCurvePts, 
										 const float& fAniLength, 
										 const bool& bWrap,
										 const float default_val) const
{	
	s_iSegCount = 200;
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	float newStartInd = 0;
	
	while(iCtrlPtCount - newStartInd > 3){
		for(float n=0; n < s_iSegCount; n++){
			float u = ((float)n)/((float)s_iSegCount - 1);
			float x=0.0;
			float y=0.0;
			for(int i=0; i < 4;i++){
				float factor = pow(u,i) * pow((1-u),3-i) * BEnChoosei(3,i);
				x += factor*(ptvCtrlPts[i+newStartInd].x);
				y += factor*(ptvCtrlPts[i+newStartInd].y);
			}
			ptvEvaluatedCurvePts.push_back(Point(x,y));
		}
		newStartInd+=3;
	}

	float y1 = ptvCtrlPts[0].y, y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if (iCtrlPtCount - newStartInd == 3) {

			std::vector<Point> temp;
			temp.clear();
			temp.push_back(ptvCtrlPts[newStartInd]);
			temp.push_back(ptvCtrlPts[newStartInd + 1]);
			temp.push_back(ptvCtrlPts[newStartInd + 2]);
			temp.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));

			for (int i = 0; i < s_iSegCount; i++) {
				float u = ((float)i) / ((float)s_iSegCount - 1);
				float x = 0.0;
				float y = 0.0;
				for (int j = 0; j < 4; j++) {
					float factor = BEnChoosei(3,j) * pow(u, j) * pow((1 - u), 3 - j);
					x += factor * (temp[j].x);
					y += factor * (temp[j].y);
				}
				if (x > fAniLength)
					x -= fAniLength;
				ptvEvaluatedCurvePts.push_back(Point(x, y));
			}
		}
		else {
			while (iCtrlPtCount - newStartInd>0){
				ptvEvaluatedCurvePts.push_back(ptvCtrlPts[newStartInd]);
				newStartInd++;
			}
			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				y1 = ptvCtrlPts[iCtrlPtCount - 1].y + (ptvCtrlPts[0].y - ptvCtrlPts[iCtrlPtCount - 1].y) *
					(fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			}
			ptvEvaluatedCurvePts.push_back(Point(0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y1));
		}
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		while (iCtrlPtCount - newStartInd>0){
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[newStartInd]);
			newStartInd++;
		}
		ptvEvaluatedCurvePts.push_back(Point(0, y1));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
	}

}
