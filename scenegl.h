#ifndef SCENEGL_H
#define SCENEGL_H

#include <QGLWidget>

class SceneGL : public QGLWidget
{
    Q_OBJECT

public:
    explicit SceneGL(QWidget *parent = 0);
    ~SceneGL();
};

#endif // SCENEGL_H
