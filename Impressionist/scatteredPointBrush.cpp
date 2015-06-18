//
// triangleBrush.cpp
//
// The implementation of Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
}

void ScatteredPointBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
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

void ScatteredPointBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
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
	int x, y;
	glPointSize(1);
	float alpha = pDoc->getAlpha();
	glBegin(GL_POINTS);
		SetColor(source, alpha);
		for (int i = 0; i < 10 * size; i++){
			x = target.x - size / 2 + rand() % size;
			y = target.y - size / 2 + rand() % size;
			ImpBrush::Point tmp(x, y);
			SetColor(tmp, alpha);
			glVertex2f(x, y);
		}
	glEnd();
}

void ScatteredPointBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

