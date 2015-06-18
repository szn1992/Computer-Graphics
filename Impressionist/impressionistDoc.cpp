// 
// impressionistDoc.cpp
//
// It basically maintain the Image for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "lineBrush.h"
#include "circleBrush.h"
#include "scatteredPointBrush.h"
#include "scatteredLineBrush.h"
#include "scatteredCircleBrush.h"
#include "sCircleBrush.h"
#include "triangleBrush.h"

#define PI 3.1415
#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	
	m_nWidth		= -1;
	m_ucImage		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;
	m_fgradient		= NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_TRIANGLE]
		= new TriangleBrush(this, "Triangle");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES_EDIT]
		= new SCircleBrush(this, "S Circles");

	ImpBrush::c_nDirectionCount = NUM_LDIRECTION_TYPE;
	ImpBrush::c_pDirections = new int[ImpBrush::c_nDirectionCount];
	ImpBrush::c_pDirections[0] = 0;
	ImpBrush::c_pDirections[1] = 1;
	ImpBrush::c_pDirections[2] = 2;

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];
	m_pCurrentDirection = ImpBrush::c_pDirections[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
	m_pCurrentBrushType = type;
	m_pUI->f_activation(type);
}

void ImpressionistDoc::setStrokeDirection(int type)
{
	m_pCurrentDirection = ImpBrush::c_pDirections[type];
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

int ImpressionistDoc::getDirection()
{
	return m_pCurrentDirection;
}

float ImpressionistDoc::getGradient(int x, int y)
{
	if (x >= 1 && x < m_nWidth &&
		y >= 1 && y < m_nHeight)
		return m_fgradient[y][x];
	else
		return 0.0;
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=load(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load image file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;
	m_nNormalizeChecker = 1;

	// release old storage
	delete [] m_ucImage;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	m_ucImage		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	float** tmp_matrix = new float*[height];
	m_fgradient = new float*[height];
	for (int i = 0; i < height; i++) {
		tmp_matrix[i] = new float[width];
		m_fgradient[i] = new float[width];
		for (int j = 0; j < width; j ++) {
			tmp_matrix[i][j] = (data[3 * (i * width + j)] + data[3 * (i * width + j) + 1] + data[3 * (i * width + j) + 2]) / 3.0;
		}
	}
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			float x_sum = 0.0;
			float y_sum = 0.0;
			// calculate the gx and gy for each pixel
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					x_sum += x_kernel[m][n] * tmp_matrix[i - 1 + m][j - 1 + n];
					y_sum += y_kernel[m][n] * tmp_matrix[i - 1 + m][j - 1 + n];
				}
			}
			if (x_sum == 0.0 && y_sum == 0.0)
				m_fgradient[i][j] = 0.0;
			else
				m_fgradient[i][j] = atan2(x_sum, y_sum) * 180 / PI;
		}
	}
	for (int i = 0; i < height; i++) {
		delete[] tmp_matrix[i];
	}
	delete[] tmp_matrix;

	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(const char *iname, const char * type, int quality) 
{

	save(iname, m_ucPainting, m_nPaintWidth, m_nPaintHeight, type, quality);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

void ImpressionistDoc::paintIt()
{	
	m_pUI->m_paintView->drawAll();
}


// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	// This needs to be implemented for image filtering to work.
	if (m_nNormalizeChecker == 1) {
		divisor = 0;
		for (int x = 0; x < knlWidth; x++) {
			for (int y = 0; y < knlHeight; y++) {
				divisor += filterKernel[y * knlWidth + x];
			}
		}
	} else if (divisor == 0) {
		divisor = 1;
	}
	
	double r,g,b;	// r,g,b of one pixel in original image
	double sum_r, sum_g, sum_b; // sums of r,g,b
	double des_r, des_g, des_b; // r,g,b of one pixel in destBuffer

	// clear destBuffer first
	for (int i = 0; i < srcBufferWidth; i++) {
		for (int j = 0; j < srcBufferHeight; j++) {
			destBuffer[3 * (j * srcBufferWidth + i) + 0] = 0;
			destBuffer[3 * (j * srcBufferWidth + i) + 1] = 0;
			destBuffer[3 * (j * srcBufferWidth + i) + 2] = 0;
		}
	}

	int fixed_r, fixed_c;	// for boundary cases
	// filter each pixel in the original image through the kernel
	for (int i = 0; i < srcBufferWidth; i++) {
		for (int j = 0; j < srcBufferHeight; j++) {
			sum_r = sum_g = sum_b = 0; 	// clear the sums of r,g,b

			for (int x = 0; x < knlWidth; x++) {
				for (int y = 0; y < knlHeight; y++) {
					fixed_r = j + y - 2;
					fixed_c = i + x - 2;
					// when meet boundary cases, fill the same value in the out-edge
					// with the closesd edge
					if (fixed_r < 0)
						fixed_r = 0;
					if (fixed_c < 0)
						fixed_c = 0;
					if (fixed_r >= srcBufferHeight)
						fixed_r = srcBufferHeight - 1;
					if (fixed_c >= srcBufferWidth)
						fixed_c = srcBufferWidth - 1;

					r = sourceBuffer[3 * (fixed_r * srcBufferWidth + fixed_c) + 0];
					g = sourceBuffer[3 * (fixed_r * srcBufferWidth + fixed_c) + 1];
					b = sourceBuffer[3 * (fixed_r * srcBufferWidth + fixed_c) + 2];

					sum_r += filterKernel[y * knlWidth + x] * r;
					sum_g += filterKernel[y * knlWidth + x] * g;
					sum_b += filterKernel[y * knlWidth + x] * b;
				}
			}

			// calculate the results r,g,b
			des_r = sum_r / divisor + offset;
			des_g = sum_g / divisor + offset;
			des_b = sum_b / divisor + offset;

			// if the results r,g,b are out of bound, fix them
			if (des_r > 255)
				des_r = 255;
			else if (des_r < 0)
				des_r = 0;
			if (des_g > 255)
				des_g = 255;
			else if (des_g < 0)
				des_g = 0;
			if (des_b > 255)
				des_b = 255;
			else if (des_b < 0)
				des_b = 0;

			// assign the values of results to the pixel in destBuffer
			destBuffer[3 * (j * srcBufferWidth + i) + 0] = (unsigned char)des_r;
			destBuffer[3 * (j * srcBufferWidth + i) + 1] = (unsigned char)des_g;
			destBuffer[3 * (j * srcBufferWidth + i) + 2] = (unsigned char)des_b;
		}
	}
}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucImage + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const ImpBrush::Point p )
{
	return GetOriginalPixel( p.x, p.y );
}



