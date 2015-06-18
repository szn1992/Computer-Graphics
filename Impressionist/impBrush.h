#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>
#include <math.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES = 1,
	BRUSH_CIRCLES = 2,
	BRUSH_SCATTERED_POINTS = 3,
	BRUSH_SCATTERED_LINES = 4,
	BRUSH_SCATTERED_CIRCLES = 5,
	BRUSH_TRIANGLE = 6,
	BRUSH_SCATTERED_CIRCLES_EDIT = 7,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum
{
	LDIRECTION_SLIDER_RIGHT = 0,
	LDIRECTION_GRADIENT = 1,
	LDIRECTION_BRUSH_DIRECTION = 2,
	NUM_LDIRECTION_TYPE
};

class ImpressionistDoc; // Pre-declaring class



class ImpBrush 
{
protected:
	ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:

	typedef struct Point 
	{
	public:
		Point() {};
		Point(int xx, int yy) { x = xx; y = yy; };

		int x, y;
	} Point;

	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target ) = 0;
	virtual void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target ) = 0;
	virtual void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const ImpBrush::Point source, float alpha );

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	char* BrushName( void );
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.
	static int			c_nDirectionCount;
	static int*			c_pDirections;

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;
};

#endif

