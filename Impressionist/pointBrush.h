//
// pointBrush.h
//
// The header file for Point Brush. 
//

#ifndef POINTBRUSH_H
#define POINTBRUSH_H

#include "impBrush.h"

class PointBrush : public ImpBrush
{
public:
	PointBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
