//
// scatteredCircleBrush.h
//
// The header file for Scattered Circle Brush. 
//

#ifndef SCATTEREDCIRCLEBRUSH_H
#define SCATTEREDCIRCLEBRUSH_H

#include "impBrush.h"

class ScatteredCircleBrush : public ImpBrush
{
public:
	ScatteredCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
