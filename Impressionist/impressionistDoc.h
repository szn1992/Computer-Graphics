// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "imageio.h"

const int x_kernel[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
const int y_kernel[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(const char *iname);			// called by the UI to load image
	int		saveImage(const char *iname, const char * type, int quality);// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	void	paintIt();
	void applyFilter( const unsigned char* sourceBuffer, //The filter kernel
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset );

	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setStrokeDirection(int type);
	int		getSize();						// get the UI size
	float	getAlpha();
	int		getDirection();
	float   getGradient(int x, int y);
	void	setSize(int size);				// set the UI size
	char*	getImageName();					// get the current image name
	

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Images for original image and painting.
	unsigned char*	m_ucImage;
	unsigned char*	m_ucPainting;
	float**			m_fgradient;
	int				m_nNormalizeChecker;

	// Used by the filtering code so that we can
	// preview filters before applying.
	unsigned char*	m_ucPreviewBackup;



	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	int					m_pCurrentDirection;
	int					m_pCurrentBrushType;
	// Size of the brush.
	int m_nSize;							

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const ImpBrush::Point p );



private:
	char			m_imageName[256];
};

extern void MessageBox(char *message);

#endif
