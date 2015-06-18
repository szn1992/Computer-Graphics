//
// triangleBrush.h
//
// The header file for Triangle Brush. 
//

#ifndef TRIANGLEBRUSH_H
#define TRIANGLEBRUSH_H

#include "impBrush.h"

class TriangleBrush : public ImpBrush
{
public:
	TriangleBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
