//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalView.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	marker_x = 0;
	marker_y = 0;

}

void OriginalView::update_marker(int x, int y) 
{
	marker_x = x;
	marker_y = y;
	refresh();
}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucImage ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		ImpBrush::Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = m_pDoc->m_ucImage + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );

		glColor3f(0.9f, 0.05f, 0.1f);
		glBegin(GL_POLYGON);
			glVertex2f(marker_x - 4, marker_y);
			glVertex2f(marker_x, marker_y + 4);
			glVertex2f(marker_x + 4, marker_y);
			glVertex2f(marker_x, marker_y - 4);
		glEnd();

	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}


