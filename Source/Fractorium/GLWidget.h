#pragma once

#include "FractoriumEmberController.h"

/// <summary>
/// GLWidget class.
/// </summary>

class Fractorium;//Forward declaration since Fractorium uses this widget.
class GLEmberControllerBase;
template<typename T> class GLEmberController;
template<typename T> class FractoriumEmberController;

static const float GridStep = 1.0f / 8.0f;

/// <summary>
/// The main drawing area.
/// This uses the Qt wrapper around OpenGL to draw the output of the render to a texture whose
/// size matches the size of the window.
/// On top of that, the circles that represent the pre and post affine transforms for each xform are drawn.
/// Based on values specified on the GUI, it will either draw the presently selected xform, or all of them.
/// It can show/hide pre/post affine as well.
/// The currently selected xform is drawn with a circle around it, with all others only showing their axes.
/// The current xform is set by either clicking on it, or by changing the index of the xforms combo box on the main window.
/// A problem here is that all drawing is done using the legacy OpenGL fixed function pipeline which is deprecated
/// and even completely disabled on Mac OS. This will need to be replaced with shader programs for every draw operation.
/// Since this window has to know about various states of the renderer and the main window, it retains pointers to 
/// the main window and several of its members.
/// This class uses a controller-based design similar to the main window.
/// </summary>
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_0//QOpenGLFunctions_3_2_Compatibility//QOpenGLFunctions_3_2_Core//, protected QOpenGLFunctions
{
	Q_OBJECT

	friend Fractorium;
	friend FractoriumEmberController<float>;
	friend GLEmberControllerBase;
	friend GLEmberController<float>;
	
#ifdef DO_DOUBLE
	friend GLEmberController<double>;
	friend FractoriumEmberController<double>;
#endif
	
public:
	GLWidget(QWidget* p = nullptr);
	~GLWidget();
	void InitGL();
	void DrawQuad();
	void SetMainWindow(Fractorium* f);
	bool Init();
	bool Drawing();
	GLint MaxTexSize();
	GLuint OutputTexID();

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void keyPressEvent(QKeyEvent* e) override;
	virtual void keyReleaseEvent(QKeyEvent* e) override;
	virtual void mousePressEvent(QMouseEvent* e) override;
	virtual void mouseReleaseEvent(QMouseEvent* e) override;
	virtual void mouseMoveEvent(QMouseEvent* e) override;
	virtual void wheelEvent(QWheelEvent* e) override;
	//virtual void resizeEvent(QResizeEvent* e) override;
	
private:
	void SetDimensions(int w, int h);
	bool Allocate(bool force = false);
	bool Deallocate();
	void SetViewport();
	void DrawGrid();
	void DrawUnitSquare();
	void DrawAffineHelper(int index, bool selected, bool pre, bool final, bool background);
	GLEmberControllerBase* GLController();

	bool m_Init;
	bool m_Drawing;
	GLint m_MaxTexSize;
	GLint m_TexWidth;
	GLint m_TexHeight;
	GLint m_ViewWidth;
	GLint m_ViewHeight;
	GLuint m_OutputTexID;
	Fractorium* m_Fractorium;
};
