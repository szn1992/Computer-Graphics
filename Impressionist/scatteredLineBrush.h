//
// scatteredLineBrush.h
//
// The header file for Scattered Line Brush. 
//

#ifndef SCATTEREDLINEBRUSH_H
#define SCATTEREDLINEBRUSH_H

#include "impBrush.h"

class ScatteredLineBrush : public ImpBrush
{
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
