//
// triangleBrush.cpp
//
// The implementation of Triangle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "triangleBrush.h"

extern float frand();

TriangleBrush::TriangleBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void TriangleBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
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

void TriangleBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
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
	int Ax, Ay, Bx, By, Cx, Cy;
	Ax = target.x - (0.5*size);
	Bx = target.x + (0.5*size);
	Cx = target.x;
	Ay = target.y - (0.5*size);
	By = target.y - (0.5*size);
	Cy = target.y + (0.5*size);

	float alpha = pDoc->getAlpha();
	glBegin( GL_POLYGON );
		SetColor(source, alpha);
		glVertex2i(Ax, Ay);
		glVertex2i(Bx, By);
		glVertex2i(Cx, Cy);

	glEnd();
}

void TriangleBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
	glDisable(GL_BLEND);
}

