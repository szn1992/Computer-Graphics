//
// impBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "impBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;
int			ImpBrush::c_nDirectionCount = 0;
int*		ImpBrush::c_pDirections = NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const ImpBrush::Point source, float alpha)
{
	ImpressionistDoc* pDoc = GetDocument();


	GLubyte color[3];

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	float r_factor = 1 - pDoc->m_pUI->getR();
	float g_factor = 1 - pDoc->m_pUI->getG();
	float b_factor = 1 - pDoc->m_pUI->getB();
	float r = 1.0 * color[0] * (1 + r_factor) * (1 - g_factor) * (1 - b_factor);
	float g = 1.0 * color[1] * (1 + g_factor) * (1 - r_factor) * (1 - b_factor);
	float b = 1.0 * color[2] * (1 + b_factor) * (1 - r_factor) * (1 - g_factor);
	glColor4f( r / 255.0, g / 255.0, b / 255.0, alpha );

}
