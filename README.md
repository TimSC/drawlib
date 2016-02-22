# drawlib
Abstraction for different drawing libraries, such as cairo, OpenGL, etc.

Goals:

* Allow different drawing backends
* Store drawing instructions within an object to enable redrawing of UI widgets
* Allow drawing instructions to be moved to GPU by OpenGL for speed
* Cairo like drawing interface
* Pango integration for fonts
* Allow many similar objects to be stored in OpenGL VBOs

