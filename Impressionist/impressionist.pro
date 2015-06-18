TEMPLATE = app
TARGET = impressionist

QT =

HEADERS += CImg.h\
	dialog.h\
	impressionistUI.h\
	scatterCircleBrush.h\
	FltDesignUI.h\
	imageio.h\
	lineBrush.h\
	scatterLineBrush.h\
	aaron.h\
	impBrush.h\		
	originalView.h\	
	scatterPointBrush.h \
	brushes.h \		
	impressionist.h	\
	paintView.h \
	circleBrush.h	\	
        impressionistDoc.h	\
	pointBrush.h

SOURCES += imageio.cpp \
	FltDesignUI.cpp		impBrush.cpp		lineBrush.cpp		scatterCircleBrush.cpp \
	aaron.cpp		impressionist.cpp	originalView.cpp	scatterLineBrush.cpp \
	circleBrush.cpp		impressionistDoc.cpp	paintView.cpp		scatterPointBrush.cpp \
	impressionistUI.cpp	pointBrush.cpp \
	"file dialog/Fl_Native_File_Chooser.cxx"	"file dialog/Fl_Native_File_Chooser_WIN32.cxx" \
	"file dialog/Fl_Native_File_Chooser_FLTK.cxx"	"file dialog/common.cxx" \
	"file dialog/Fl_Native_File_Chooser_MAC.cxx" \



win32:INCLUDEPATH += ../FLTK ../FLTK/png ../FLTK/zlib ../FLTK/jpeg "file dialog"
win32:LIBPATH += ../FLTK/lib 
win32:LIBS += opengl32.lib glu32.lib -lfltk -lfltkgl -lfltkjpeg -lfltkpng -lfltkz Comctl32.lib wsock32.lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib  shell32.lib ole32.lib oleaut32.lib 
win32:DEFINES = _CRT_SECURE_NO_WARNINGS WIN32 FLTK1 SAMPLE_SOLUTION cimg_display_type=0 
win32:QMAKE_CXXFLAGS = -Oi -EHsc -nologo -Zm200
win32:QMAKE_CFLAGS = -nologo -Zm200

mac:INCLUDEPATH += /usr/local/include/FL/images "file dialog" 
mac:CONFIG += x86
mac:DEFINES += FLTK1 SAMPLE_SOLUTION cimg_display_type=0 
mac:LIBS += -lfltk -lfltk_gl -lfltk_jpeg -lfltk_png -lfltk_images -l fltk_z -framework Carbon -framework ApplicationServices -framework AGL -framework OpenGL -framework AppKit

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
