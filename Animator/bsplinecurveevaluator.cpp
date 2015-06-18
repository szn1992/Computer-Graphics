#include "bsplinecurveevaluator.h"
#include <cassert>
using namespace Animator;

int BnChoosei(int n, int i)
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

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										  std::vector<Point>& ptvEvaluatedCurvePts, 
										  const float& fAniLength, 
										  const bool& bWrap,
										  const float default_val) const
{	

	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	if(iCtrlPtCount< 3){
		ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());

		float x = 0.0;
		float y1;

		if (bWrap) {
			// if wrapping is on, interpolate the y value at xmin and
			// xmax so that the slopes of the lines adjacent to the
			// wraparound are equal.

			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
					ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			}
			else
				y1 = ptvCtrlPts[0].y;
			ptvEvaluatedCurvePts.push_back(Point(x, y1));
		}

		/// set the endpoint based on the wrap flag.
		float y2;
		x = fAniLength;
		if (bWrap) {
			y2 = y1;
			ptvEvaluatedCurvePts.push_back(Point(x, y2));
		}
	}
	else{
		std::vector<Point> newPtvCtrlPts;
		std::vector<Point> trimmedPtvCtrlPts;
		
		trimmedPtvCtrlPts.clear();
		if (!bWrap) {
			trimmedPtvCtrlPts.push_back(ptvCtrlPts[0]);
			trimmedPtvCtrlPts.push_back(ptvCtrlPts[0]);
		} else {
			trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 3].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 3].y));
			trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y));
			trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		}

		for(int i=0;i<iCtrlPtCount;i++){
			trimmedPtvCtrlPts.push_back(ptvCtrlPts[i]);
		}
		if(iCtrlPtCount>1){
			if (!bWrap) {
				trimmedPtvCtrlPts.push_back(ptvCtrlPts[iCtrlPtCount-1]);
				trimmedPtvCtrlPts.push_back(ptvCtrlPts[iCtrlPtCount-1]);
			} else {
				trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
				trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y));
				trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[2].x + fAniLength, ptvCtrlPts[2].y));			
			}
		}
		for(int i = 0; i < trimmedPtvCtrlPts.size()-3; i++){
			Point v0 = Point(1.0/6*trimmedPtvCtrlPts[i].x+2.0/3*(trimmedPtvCtrlPts[i+1].x)+1.0/6*trimmedPtvCtrlPts[i+2].x,
				1.0/6*trimmedPtvCtrlPts[i].y+2.0/3*trimmedPtvCtrlPts[i+1].y+1.0/6*trimmedPtvCtrlPts[i+2].y);
			Point v1 = Point(2.0/3*trimmedPtvCtrlPts[i+1].x+1.0/3*(trimmedPtvCtrlPts[i+2].x),
				2.0/3*trimmedPtvCtrlPts[i+1].y+1.0/3*(trimmedPtvCtrlPts[i+2].y));
			Point v2 = Point(1.0/3*trimmedPtvCtrlPts[i+1].x+2.0/3*(trimmedPtvCtrlPts[i+2].x),
				1.0/3*trimmedPtvCtrlPts[i+1].y+2.0/3*(trimmedPtvCtrlPts[i+2].y));
			Point v3 = Point(1.0/6*trimmedPtvCtrlPts[i+1].x+2.0/3*(trimmedPtvCtrlPts[i+2].x)+1.0/6*trimmedPtvCtrlPts[i+3].x,
				1.0/6*trimmedPtvCtrlPts[i+1].y+2.0/3*trimmedPtvCtrlPts[i+2].y+1.0/6*trimmedPtvCtrlPts[i+3].y);

			newPtvCtrlPts.clear();
			newPtvCtrlPts.push_back(v0);
			newPtvCtrlPts.push_back(v1);
			newPtvCtrlPts.push_back(v2);
			newPtvCtrlPts.push_back(v3);

			for(float n=0; n < s_iSegCount; n++){
				float u = ((float)n)/((float)s_iSegCount-1);
				float x=0.0;
				float y=0.0;
				for(int i=0; i < 4;i++){
					float factor = pow(u,i) * pow((1-u),3-i) * BnChoosei(3,i);
					x += factor*(newPtvCtrlPts[i].x);
					y += factor*(newPtvCtrlPts[i].y);
				}
				ptvEvaluatedCurvePts.push_back(Point(x,y));
			}
		}
	}
	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}
}
