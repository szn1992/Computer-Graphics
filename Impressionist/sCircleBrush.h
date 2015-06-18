//
// sCircleBrush.h
//
// The header file for version 2 Scattered Circle Brush. 
//

#ifndef SCIRCLEBRUSH_H
#define SCIRCLEBRUSH_H

#include "impBrush.h"

class SCircleBrush : public ImpBrush
{
public:
	SCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
