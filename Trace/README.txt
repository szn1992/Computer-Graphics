Important Changes (That may be to blame for hiding errors in the future)

Got rid of warnings by adding a preprocessor compile flag. (11/03/10)
Added “_CRT_SECURE_NO_WARNINGS” preprocessor flag to 

    Project properties->ConfigurationProperties->C/C++->Preprocessor->Preprocessor

definitions.

The warnings are all in classes with really low level methods,  trying to get us to use methods like “strcpy_s” instead of “strcpy” in string.h
The best solution is to replace the methods, but these are in the filedialog stuff, which i dont know anything about... hence the hammer approach!
