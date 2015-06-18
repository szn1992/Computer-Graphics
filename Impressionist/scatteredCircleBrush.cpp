//
// scatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredCircleBrush.h"

#define PI 3.1415
extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
}

void ScatteredCircleBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize( (float)size );

	BrushMove( source, target );
}

void ScatteredCircleBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	int size = pDoc->getSize();
	if (dlg->getRand() == 1) {
		size = rand() % size + 1;
	}
	int div = 36;
	float radius = size / 2;
	float x, y;
	float center_x, center_y;

	float alpha = pDoc->getAlpha();
	for (int i = 0; i < 8; i++){
		center_x = target.x - size / 2 + rand() % size;
		center_y = target.y - size / 2 + rand() % size;
		glBegin(GL_POLYGON);
			ImpBrush::Point tmp(center_x, center_y);
			SetColor(tmp, alpha);
			for (int j = 0; j < div; j++){
				x = center_x + radius * cos(2 * PI * j / div);
				y = center_y + radius * sin(2 * PI * j / div);
				glVertex2f(x, y);
			}
		glEnd();
	}
}

void ScatteredCircleBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

