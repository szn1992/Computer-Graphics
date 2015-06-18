#include "catmullcurveevaluator.h"
#include <cassert>
using namespace Animator;

int CnChoosei(int n, int i) {
	int r = 1;
	if(i > n)
		return 0;
	for(int d = 1; d <= i; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}

void CatmullCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										  std::vector<Point>& ptvEvaluatedCurvePts, 
										  const float& fAniLength, 
										  const bool& bWrap,
										  const float default_val) const
{	
	//s_iSegCount = 200;
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();
	
	if(iCtrlPtCount < 2){
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
		if(bWrap) {
			trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y));
			trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		} else {
			trimmedPtvCtrlPts.push_back(ptvCtrlPts[0]);
		}
		
		for(int i=0;i<iCtrlPtCount;i++){
			trimmedPtvCtrlPts.push_back(ptvCtrlPts[i]);
		}
			
		if(iCtrlPtCount>1){
			if (bWrap) {
				trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
				trimmedPtvCtrlPts.push_back(Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y));
			}else {
				trimmedPtvCtrlPts.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
			}
		}
		
		float maxX=-1.0;
		for(int i = 0; i<trimmedPtvCtrlPts.size() - 3;i++){
			Point p0 = trimmedPtvCtrlPts[i];
			Point p1 = trimmedPtvCtrlPts[i + 1]; 
			Point p2 = trimmedPtvCtrlPts[i + 2];
			Point p3 = trimmedPtvCtrlPts[i + 3];
			Point v2 = Point((p1.x + (1.0/6.0) * (p2.x - p0.x)), (p1.y + (1.0/6.0) * (p2.y - p0.y)));
			Point v3 = Point((p2.x - (1.0/6.0) * (p3.x - p1.x)), (p2.y - (1.0/6.0) * (p3.y - p1.y)));
						
			newPtvCtrlPts.clear();
			newPtvCtrlPts.push_back(p1);
			newPtvCtrlPts.push_back(v2);
			newPtvCtrlPts.push_back(v3);
			newPtvCtrlPts.push_back(p2);
			
			if (p3.x < 0.1) {
				ptvEvaluatedCurvePts.push_back(p3);
			}
			else {
				for (float n = 0; n < s_iSegCount; n++){
					float u = ((float)n) / ((float)s_iSegCount - 1);
					float x = 0.0;
					float y = 0.0;
					for (int j = 0; j < 4; j++){
						float factor = pow(u, j) * pow((1 - u), 3 - j) * CnChoosei(3, j);
						x += factor*(newPtvCtrlPts[j].x);
						y += factor*(newPtvCtrlPts[j].y);
					}
					if (x > maxX && x < p2.x) {
						maxX = x;
					}
					else if (maxX > x && maxX < p2.x){
						x = maxX;
					}
					if (x <= p2.x)
						ptvEvaluatedCurvePts.push_back(Point(x, y));
				}
			}
		}
	}
	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}
}
