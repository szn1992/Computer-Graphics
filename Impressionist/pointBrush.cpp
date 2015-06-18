//
// pointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointBrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
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

void PointBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
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
	glPointSize(size);
	float alpha = pDoc->getAlpha();
	glBegin( GL_POINTS );
		SetColor(source, alpha);
		glVertex2d( target.x, target.y );

	glEnd();
}

void PointBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
	// do nothing so far
	glDisable(GL_BLEND);
}

