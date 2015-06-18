//
// LineBrush.cpp
//
// The implementation of Scattered Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "lineBrush.h"

#define PI 3.1415
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize((float)size);

	BrushMove(source, target);
}

void LineBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	if (dlg->getRand() == 1) {
		size = rand() % size;
	}
	int angle;
	if (pDoc->getDirection() == 0) {
		angle = dlg->getAngle();
	}
	else if (pDoc->getDirection() == 1) {
		angle = pDoc->getGradient(target.x, target.y);
	}
	else {
		double xDiff = target.x - (dlg->m_paintView->currentPoint.x);
		double yDiff = target.y - (dlg->m_paintView->currentPoint.y);
		dlg->m_paintView->currentPoint = target;
		if (xDiff == 0) 
			angle = 90;
		else
			angle = atan2(yDiff, xDiff) * 180 / PI;
	}
	int width = dlg->getWidth();

	float Ax, Ay, Bx, By;

	glLineWidth(width);
	float alpha = pDoc->getAlpha();
	glBegin(GL_LINES);
		SetColor(source, alpha);
		Ax = target.x + size * cos(PI * angle / 180);
		Ay = target.y + size * sin(PI * angle / 180);
		Bx = target.x - size * cos(PI * angle / 180);
		By = target.y - size * sin(PI * angle / 180);

		glVertex2f(Ax, Ay);
		glVertex2f(Bx, By);
	glEnd();
}

void LineBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

