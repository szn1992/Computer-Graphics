//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintView.h"
#include "impBrush.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static ImpBrush::Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	rl_flag = false;
}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

	if(!valid())
	{


		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		int	cx	= m_nWindowWidth = w();
		int	cy	= m_nWindowHeight = h();



		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );

		glViewport( 0, 0, cx, cy );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0, cx, 0, cy );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		// This translation is needed to make the image pixels line up with
		// the screen's pixels.  For details, visit:
		// http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
		glTranslatef( 0.375, 0.375, 0.0 );


		glClear( GL_COLOR_BUFFER_BIT );
	}

	ImpBrush::Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	if ( m_pDoc->m_ucPainting && isAnEvent) 
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		ImpBrush::Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		ImpBrush::Point target( coord.x, m_nWindowHeight - coord.y );

		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			RestoreContent();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			if (rl_flag)
			{
				RestoreContent();
				break;
			}
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );

			SaveCurrentBackContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			startPoint = target;
			break;
		case RIGHT_MOUSE_DRAG:
			RestoreContent();
			rl_flag = true;
			glLineWidth(1);
			glBegin(GL_LINES);
				glColor3f(0.8f, 0.05f, 0.1f);
				glVertex2d(startPoint.x, startPoint.y);
				glVertex2d(target.x, target.y);
			glEnd();
			break;
		case RIGHT_MOUSE_UP:
			RestoreContent();
			if (m_pDoc->m_pCurrentBrushType == 1 ||
				m_pDoc->m_pCurrentBrushType == 4)
			{
				if (target.x == startPoint.x && target.y == startPoint.y)
				{
					m_pDoc->m_pUI->setAngle(0);
					break;
				}
				else if (target.x == startPoint.x) {
					m_pDoc->m_pUI->setAngle(90);
					break;
				}
				m_pDoc->m_pUI->setSize(int(sqrt(pow((target.x - startPoint.x), 2.0f) + pow((target.y - startPoint.y), 2.0f))));
				m_pDoc->m_pUI->setAngle(180 - abs((atan2(float(target.y - startPoint.y), (target.x - startPoint.x)))) * 180 / M_PI);
			}
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA
}

void PaintView::drawAll()
{
	this->make_current();
#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

	if (!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable(GL_DEPTH_TEST);

		ortho();

		int	cx = m_nWindowWidth = w();
		int	cy = m_nWindowHeight = h();

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer(GL_FRONT);

		glViewport(0, 0, cx, cy);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, cx, 0, cy);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// This translation is needed to make the image pixels line up with
		// the screen's pixels.  For details, visit:
		// http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
		glTranslatef(0.375, 0.375, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	ImpBrush::Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y = 0;

	m_nWindowWidth = w();
	m_nWindowHeight = h();

	int drawWidth, drawHeight;
	drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
	drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if (startrow < 0) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting +
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth = drawWidth;
	m_nDrawHeight = drawHeight;

	m_nStartRow = startrow;
	m_nEndRow = startrow + drawHeight;
	m_nStartCol = scrollpos.x;
	m_nEndCol = m_nStartCol + drawWidth;

	int spacing = m_pDoc->m_pUI->getSpacing();
	for (int i = 0; i < m_nEndCol; i += spacing) {
		for (int j = 0; j < m_nEndRow; j += spacing) {
			ImpBrush::Point source(i + m_nStartCol, m_nEndRow - j);
			ImpBrush::Point target(i, m_nWindowHeight - j - 1);
			m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
			if (j + spacing >= m_nEndRow && j != m_nEndRow - 1)
				j = m_nEndRow - 1 - spacing;
		}
		if (i + spacing >= m_nEndCol && i != m_nEndCol - 1)
			i = m_nEndCol - 1 - spacing;
	}
		
	glFlush();
	SaveCurrentContent();

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA
}

int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		currentPoint = ImpBrush::Point(coord.x, m_nWindowHeight - coord.y);
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		currentPoint = ImpBrush::Point(coord.x, m_nWindowHeight - coord.y);
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		m_pDoc->m_pUI->m_origView->update_marker(coord.x, this->m_pDoc->m_pUI->m_origView->h() - coord.y);
		redraw();
		break;
	case FL_RELEASE:
		currentPoint = ImpBrush::Point(coord.x, m_nWindowHeight - coord.y);
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		m_pDoc->m_pUI->m_origView->update_marker(coord.x, this->m_pDoc->m_pUI->m_origView->h() - coord.y);
		redraw();
		break;
	case FL_MOVE:
		currentPoint = ImpBrush::Point(coord.x, m_nWindowHeight - coord.y);
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		m_pDoc->m_pUI->m_origView->update_marker(coord.x, this->m_pDoc->m_pUI->m_origView->h() - coord.y);
		break;
	default:
		return 0;
		break;

	}



	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	if (rl_flag) {
		RestoreContent();
		return;
	}
	// Tell OpenGL to read pixels from the front buffer
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::SaveCurrentBackContent()
{
	glReadBuffer(GL_BACK);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );

	glReadBuffer(GL_FRONT);
}

void PaintView::RestoreContent()
{
	rl_flag = false;
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

