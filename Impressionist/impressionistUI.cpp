//
// impressionistUI.cpp
//
// The user interface part for the program.
//

#include <string>

#include <FL/fl_ask.H>

#include <algorithm>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

#include "dialog.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}

//------------------------------------------------------------------------
// This displays a modal file chooser with a native look-and-feel.
// The available dialog types are:
//   BROWSE_DIRECTORY       - Open a single directory
//   BROWSE_FILE            - Open a single file
//   BROWSE_MULTI_DIRECTORY - Open directories, allowing the user to
//                            select more than one at a time
//   BROWSE_MULTI_FILE      - Open files, allowing the user to select
//                            more than one at a time
//   BROWSE_SAVE_DIRECTORY  - Save a directory
//   BROWSE_SAVE_FILE       - Save a file
//
// The filter limits the displayed files. See cb_load_image for an example.
// title is optional, use NULL for the OS default title.
// The return value is the filepath.
//------------------------------------------------------------------------
const char* ImpressionistUI::fileDialog(Fl_Native_File_Chooser::Type dialogType, const char* filter, const char* title) {
	if(!m_nativeChooser)
		m_nativeChooser = new Fl_Native_File_Chooser(dialogType);
	else
		m_nativeChooser->type(dialogType);
	m_nativeChooser->filter(filter);
	m_nativeChooser->title(title);
	int ret = m_nativeChooser->show();
	if(ret == -1 ||	ret == 1) {
		//error or cancel respectively
		return NULL;
	}
	return m_nativeChooser->filename();
}

//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();
	/*pDoc->m_pUI->m_chooser = new Fl_File_Chooser(".", "*.{png,jpg,bmp}", 0, "Open File?" );
    pDoc->m_pUI->m_chooser->show();
    while(pDoc->m_pUI->m_chooser->shown())
        { Fl::wait(); }
	const char *filename = pDoc->m_pUI->m_chooser->value();
	while (pDoc->m_pUI->m_chooser->visible()) {
		pDoc->m_pUI->m_chooser->preview(0);
		Fl::wait();
	}
	if (filename) {
		pDoc->loadImage(filename);
	}*/
	const char* filename = pDoc->m_pUI->fileDialog( Fl_Native_File_Chooser::BROWSE_FILE,
													"Image File (*.png; *.jpg, *.bmp)\t*.{png,jpg,bmp}");
	if(filename)
		pDoc->loadImage(filename);
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();
	/*pDoc->m_pUI->m_chooser = new Fl_File_Chooser(".", "*.png\t*.jpg", Fl_File_Chooser::CREATE, "Save");
    pDoc->m_pUI->m_chooser->show();
    while(pDoc->m_pUI->m_chooser->shown())
        { Fl::wait(); }*/
	
	const char* filename = pDoc->m_pUI->fileDialog( Fl_Native_File_Chooser::BROWSE_SAVE_FILE,
													"PNG Image File (*.png)\t*.png\nJPEG Image File (*.jpg)\t*.jpg");
	if(filename) {
		std::string strFileName = (std::string)filename;
		std::string ext;
		int quality = 95;
		switch(pDoc->m_pUI->m_nativeChooser->filter_value()) {
			case 0:	ext = ".png"; break;
			case 1:	ext = ".jpg"; break;
		}
#ifdef WIN32
		char szExt[_MAX_EXT];
		_splitpath_s(strFileName.c_str(), NULL,0, NULL,0, NULL, 0, szExt,_MAX_EXT);
		if (_stricmp(szExt,".jpg") && _stricmp(szExt,".png")) {
			strFileName += ext;
		}
		else
			ext = szExt;
#endif

#ifdef WIN32
		if(!_strcmpi(ext.c_str(),".jpg")) {
#else
		if(!strcasecmp(ext.c_str(),".jpg")) {
#endif		
			Dialog2 x(0,0,0,0,"ok");
			quality = x.getValue();
		}
		pDoc->saveImage(strFileName.c_str(), ext.c_str(), quality);
	}
	/*const char *szFileName = pDoc->m_pUI->m_chooser->value();
	if (szFileName) {
		std::string strFileName = szFileName;
		std::string ext = (std::string)pDoc->m_pUI->m_chooser->filter();
		ext = ext.substr(1, ext.length() - 1);
		#ifdef WIN32
			char szExt[_MAX_EXT];
			_splitpath_s(strFileName.c_str(), NULL,0, NULL,0, NULL, 0, szExt,_MAX_EXT);
			// If user didn't type supported ext, add default one.
			if (_stricmp(szExt,".jpg") && _stricmp(szExt,".png")) {
				strFileName += ext;
			}
			else
				ext = szExt;
		#endif
		int quality = 95;
		if (!strcmp(ext.c_str(),".jpg")){
			Dialog2 x(0,0,0,0,"ok");
			quality = x.getValue();
		}
		pDoc->saveImage(strFileName.c_str(), ext.c_str(), quality);
	}*/
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

void ImpressionistUI::cb_filter_kernel(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	// Need to back up the current paint view status
	// so we can easily preview filters
	//If no image has been loaded, do not open filter kernel
	if (pDoc->m_ucPainting != NULL) {
		memcpy( pDoc->m_ucPreviewBackup, pDoc->m_ucPainting, 
			pDoc->m_nPaintWidth * pDoc->m_nPaintHeight * 3 );
		whoami(o)->fltDesignUI->show();
	}
	else
		fl_alert("Must load an image first!");
}

//------------------------------------------------------------
// Swap the paintview canvas.
// Called by the UI when the swap canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_swap_paint(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	for (int i = 0; i < pDoc->m_nPaintHeight; i ++) {
		for (int j = 0; j < pDoc->m_nPaintWidth; j ++) {
			pDoc->m_ucImage[3 * (i * pDoc->m_nPaintWidth + j) + 0] = pDoc->m_ucPainting[3 * (i * pDoc->m_nPaintWidth + j) + 0];
			pDoc->m_ucImage[3 * (i * pDoc->m_nPaintWidth + j) + 1] = pDoc->m_ucPainting[3 * (i * pDoc->m_nPaintWidth + j) + 1];
			pDoc->m_ucImage[3 * (i * pDoc->m_nPaintWidth + j) + 2] = pDoc->m_ucPainting[3 * (i * pDoc->m_nPaintWidth + j) + 2];
		}
	}
	pDoc->clearCanvas();
	pDoc->m_pUI->m_origView->refresh();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

void ImpressionistUI::cb_colors(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorDialog->show();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CSE 457");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;
	pDoc->setBrushType(type);
}

void ImpressionistUI::cb_strokeDirection(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setStrokeDirection(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}

void ImpressionistUI::cb_paint_it_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->paintIt();
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_rSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nrColor = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_gSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_ngColor = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_bSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nbColor = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_spacingSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSpacing = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_randButton(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nRand = int(((Fl_Light_Button *)o)->value());
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize = size;
	if (size <= 40) 
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

void ImpressionistUI::setAngle( int angle )
{
	m_nAngle = angle;
	if (angle <= 360)
		m_BrushAngleSlider->value(m_nAngle);
}

int ImpressionistUI::getWidth()
{
	return m_nWidth;
}

float ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

float ImpressionistUI::getR()
{
	return m_nrColor;
}

float ImpressionistUI::getG()
{
	return m_ngColor;
}

float ImpressionistUI::getB()
{
	return m_nbColor;
}

int ImpressionistUI::getSpacing()
{
	return m_nSpacing;
}

int ImpressionistUI::getRand()
{
	return m_nRand;
}

// Getting/setting methods for the filter design UI
void ImpressionistUI::initFltDesignUI(void)
{
	fltDesignUI->ele0->value(fltKernel[0]);
	fltDesignUI->ele1->value(fltKernel[1]);
	fltDesignUI->ele2->value(fltKernel[2]);
	fltDesignUI->ele3->value(fltKernel[3]);
	fltDesignUI->ele4->value(fltKernel[4]);
	fltDesignUI->ele5->value(fltKernel[5]);
	fltDesignUI->ele6->value(fltKernel[6]);
	fltDesignUI->ele7->value(fltKernel[7]);
	fltDesignUI->ele8->value(fltKernel[8]);
	fltDesignUI->ele9->value(fltKernel[9]);
	fltDesignUI->ele10->value(fltKernel[10]);
	fltDesignUI->ele11->value(fltKernel[11]);
	fltDesignUI->ele12->value(fltKernel[12]);
	fltDesignUI->ele13->value(fltKernel[13]);
	fltDesignUI->ele14->value(fltKernel[14]);
	fltDesignUI->ele15->value(fltKernel[15]);
	fltDesignUI->ele16->value(fltKernel[16]);
	fltDesignUI->ele17->value(fltKernel[17]);
	fltDesignUI->ele18->value(fltKernel[18]);
	fltDesignUI->ele19->value(fltKernel[19]);
	fltDesignUI->ele20->value(fltKernel[20]);
	fltDesignUI->ele21->value(fltKernel[21]);
	fltDesignUI->ele22->value(fltKernel[22]);
	fltDesignUI->ele23->value(fltKernel[23]);
	fltDesignUI->ele24->value(fltKernel[24]);

	fltDesignUI->scale->value(scale);
	fltDesignUI->offset->value(offset);
}

void ImpressionistUI::updateFilter()
{
	fltKernel[0] = fltDesignUI->ele0->value();
	fltKernel[1] = fltDesignUI->ele1->value();
	fltKernel[2] = fltDesignUI->ele2->value();
	fltKernel[3] = fltDesignUI->ele3->value();
	fltKernel[4] = fltDesignUI->ele4->value();
	fltKernel[5] = fltDesignUI->ele5->value();
	fltKernel[6] = fltDesignUI->ele6->value();
	fltKernel[7] = fltDesignUI->ele7->value();
	fltKernel[8] = fltDesignUI->ele8->value();
	fltKernel[9] = fltDesignUI->ele9->value();
	fltKernel[10] = fltDesignUI->ele10->value();
	fltKernel[11] = fltDesignUI->ele11->value();
	fltKernel[12] = fltDesignUI->ele12->value();
	fltKernel[13] = fltDesignUI->ele13->value();
	fltKernel[14] = fltDesignUI->ele14->value();
	fltKernel[15] = fltDesignUI->ele15->value();
	fltKernel[16] = fltDesignUI->ele16->value();
	fltKernel[17] = fltDesignUI->ele17->value();
	fltKernel[18] = fltDesignUI->ele18->value();
	fltKernel[19] = fltDesignUI->ele19->value();
	fltKernel[20] = fltDesignUI->ele20->value();
	fltKernel[21] = fltDesignUI->ele21->value();
	fltKernel[22] = fltDesignUI->ele22->value();
	fltKernel[23] = fltDesignUI->ele23->value();
	fltKernel[24] = fltDesignUI->ele24->value();

	scale = fltDesignUI->scale->value();
	offset = fltDesignUI->offset->value();

//	printf("update filter\n");
}




// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		{ "&Filter Kernel...", FL_CTRL + 'f', (Fl_Callback *)ImpressionistUI::cb_filter_kernel }, 
		{ "&Swap Canvas", FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_swap_paint},
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },

		{ "Colors", FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_colors },
		
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Triangle",			FL_ALT+'t', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_TRIANGLE},
  { "S Circles", FL_ALT + 'z', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES_EDIT},
  {0}
};

Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[NUM_LDIRECTION_TYPE + 1] = {
	{ "Slider/Right Mouse", FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_strokeDirection, (void*)LDIRECTION_SLIDER_RIGHT},
	{ "Gradient", FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_strokeDirection, (void*)LDIRECTION_GRADIENT},
	{ "Brush Direction", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_strokeDirection, (void*)LDIRECTION_BRUSH_DIRECTION},
	{ 0 }
};



//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() : m_nativeChooser(NULL) {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// Create the filter kernel UI
	fltDesignUI = new FltDesignUI;
	fltDesignUI->impUI = this;

	// initialize the filter kernel
	for (int i=0;i<FLT_HEIGHT*FLT_WIDTH;i++)
	{
		fltKernel[i] = 0;
	}
	fltKernel[(FLT_HEIGHT/2)*FLT_WIDTH+(FLT_HEIGHT/2)]=1;
	scale = 1;
	offset = 0;

	initFltDesignUI();

	// init values

	m_nSize = 10;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_DirectionChoice = new Fl_Choice(115, 45, 150, 25, "&Stroke Direction");
		m_DirectionChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_DirectionChoice->menu(strokeDirectionMenu);
		m_DirectionChoice->callback(cb_strokeDirection);
		m_DirectionChoice->deactivate();

		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		m_nWidth = 1;
		m_BrushWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_BrushWidthSlider->user_data((void*)(this));
		m_BrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushWidthSlider->labelfont(FL_COURIER);
		m_BrushWidthSlider->labelsize(12);
		m_BrushWidthSlider->minimum(0);
		m_BrushWidthSlider->maximum(50);
		m_BrushWidthSlider->step(1);
		m_BrushWidthSlider->value(m_nWidth);
		m_BrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_BrushWidthSlider->callback(cb_widthSlides);
		m_BrushWidthSlider->deactivate();

		m_nAngle = 0; 
		m_BrushAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle"); 
		m_BrushAngleSlider->user_data((void*)(this));
		m_BrushAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAngleSlider->labelfont(FL_COURIER);
		m_BrushAngleSlider->labelsize(12);
		m_BrushAngleSlider->minimum(0); 
		m_BrushAngleSlider->maximum(360); 
		m_BrushAngleSlider->step(1);
		m_BrushAngleSlider->value(m_nAngle); 
		m_BrushAngleSlider->align(FL_ALIGN_RIGHT);
		m_BrushAngleSlider->callback(cb_angleSlides);		m_BrushAngleSlider->deactivate();
		m_nAlpha = 1.0f; 
		m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha"); 
		m_BrushAlphaSlider->user_data((void*)(this));
		m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_BrushAlphaSlider->labelfont(FL_COURIER);
		m_BrushAlphaSlider->labelsize(12);
		m_BrushAlphaSlider->minimum(0.0f); 
		m_BrushAlphaSlider->maximum(1.00f); 
		m_BrushAlphaSlider->step(0.01f);
		m_BrushAlphaSlider->value(m_nAlpha); 
		m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
		m_BrushAlphaSlider->callback(cb_alphaSlides);

		m_AutoPaintBox = new Fl_Group(10, 200, 380, 40, "");
			m_AutoPaintBox->box(FL_UP_BOX);
			m_AutoPaintBox->align(FL_ALIGN_TOP_LEFT);

			m_nSpacing = 4;
			m_SpacingSlider = new Fl_Value_Slider(20, 210, 150, 20, "Spacing");
			m_SpacingSlider->user_data((void*)(this));
			m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
			m_SpacingSlider->labelfont(FL_COURIER);
			m_SpacingSlider->labelsize(12);
			m_SpacingSlider->minimum(1);
			m_SpacingSlider->maximum(16);
			m_SpacingSlider->step(1);
			m_SpacingSlider->value(m_nSpacing);
			m_SpacingSlider->align(FL_ALIGN_RIGHT);
			m_SpacingSlider->callback(cb_spacingSlides);

			m_nRand = 1;
			m_RandButton = new Fl_Light_Button(230, 210, 60, 20, "Rand");
			m_RandButton->user_data((void*)(this));
			m_RandButton->value(m_nRand);
			m_RandButton->callback(cb_randButton);

			m_PaintItButton = new Fl_Button(300, 206, 80, 28, "&Paint it");
			m_PaintItButton->user_data((void*)(this));
			m_PaintItButton->callback(cb_paint_it_button);
		m_AutoPaintBox->end();

    m_brushDialog->end();	

	m_colorDialog = new Fl_Window(412, 130, "Color Dialog");
		m_nrColor = 1.0f;
		m_rSlider = new Fl_Value_Slider(10, 10, 300, 20, "Red Channel");
		m_rSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_rSlider->type(FL_HOR_NICE_SLIDER);
		m_rSlider->labelfont(FL_COURIER);
		m_rSlider->labelsize(12);
		m_rSlider->minimum(0.0f);
		m_rSlider->maximum(1.0f);
		m_rSlider->step(0.01f);
		m_rSlider->value(m_nrColor);
		m_rSlider->align(FL_ALIGN_RIGHT);
		m_rSlider->callback(cb_rSlides);

		m_ngColor = 1.0f;
		m_gSlider = new Fl_Value_Slider(10, 40, 300, 20, "Green Channel");
		m_gSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_gSlider->type(FL_HOR_NICE_SLIDER);
		m_gSlider->labelfont(FL_COURIER);
		m_gSlider->labelsize(12);
		m_gSlider->minimum(0.0f);
		m_gSlider->maximum(1.0f);
		m_gSlider->step(0.01f);
		m_gSlider->value(m_ngColor);
		m_gSlider->align(FL_ALIGN_RIGHT);
		m_gSlider->callback(cb_gSlides);

		m_nbColor = 1.0f;
		m_bSlider = new Fl_Value_Slider(10, 70, 300, 20, "Blue Channel");
		m_bSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_bSlider->type(FL_HOR_NICE_SLIDER);
		m_bSlider->labelfont(FL_COURIER);
		m_bSlider->labelsize(12);
		m_bSlider->minimum(0.0f);
		m_bSlider->maximum(1.0f);
		m_bSlider->step(0.01f);
		m_bSlider->value(m_nbColor);
		m_bSlider->align(FL_ALIGN_RIGHT);
		m_bSlider->callback(cb_bSlides);
	m_colorDialog->end();
}

void ImpressionistUI::f_activation(int type)
{
	switch (type)
	{
	case BRUSH_POINTS:
	case BRUSH_CIRCLES:
	case BRUSH_SCATTERED_POINTS:
	case BRUSH_SCATTERED_CIRCLES:
		this->m_DirectionChoice->deactivate();
		this->m_BrushWidthSlider->deactivate();
		this->m_BrushAngleSlider->deactivate();
		break;
	case BRUSH_LINES:
	case BRUSH_SCATTERED_LINES:
		this->m_DirectionChoice->activate();
		this->m_BrushWidthSlider->activate();
		this->m_BrushAngleSlider->activate();
		break;

	}
}

ImpressionistUI::~ImpressionistUI()
{
	if(m_nativeChooser) delete m_nativeChooser;
	delete fltDesignUI;
}

void ImpressionistUI::cancelFilter(void)
{
	std::swap( m_pDoc->m_ucPainting, m_pDoc->m_ucPreviewBackup );
	fltDesignUI->hide();
	m_paintView->refresh();
}

void ImpressionistUI::applyFilter(void)
{
	m_pDoc->applyFilter(m_pDoc->m_ucPreviewBackup, 
		m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight,
		m_pDoc->m_ucPainting, 
		fltKernel,FLT_WIDTH,FLT_HEIGHT,scale,offset);
	fltDesignUI->hide();
	m_paintView->refresh();
}

void ImpressionistUI::previewFilter(void)
{
	m_pDoc->applyFilter(m_pDoc->m_ucPreviewBackup, 
		m_pDoc->m_nPaintWidth, m_pDoc->m_nPaintHeight,
		m_pDoc->m_ucPainting, 
		fltKernel,FLT_WIDTH,FLT_HEIGHT,scale,offset);
	m_paintView->refresh();
}

void ImpressionistUI::normalizeFilter(void)
{
	m_pDoc->m_nNormalizeChecker = fltDesignUI->normalize_checker->value();
}



