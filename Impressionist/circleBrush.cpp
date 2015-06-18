//
// CircleBrush.cpp
//
// The implementation of Scattered Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circleBrush.h"

#define PI 3.1415
extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
}

void CircleBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
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

void CircleBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}
	int size = pDoc->getSize();
	if (dlg->getRand() == 1) {
		size = rand() % size;
	}
	
	int div = 36;
	float radius = size / 2;
	float x, y;
	float alpha = pDoc->getAlpha();
	glBegin(GL_POLYGON);
		SetColor(source, alpha);
		for (int j = 0; j < div; j++) {
			x = target.x + radius * cos(2 * PI * j / div);
			y = target.y + radius * sin(2 * PI * j / div);
			glVertex2f(x, y);
		}
	glEnd();
}

void CircleBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

