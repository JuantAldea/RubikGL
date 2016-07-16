/*
Copyright (C) 2009, Juan Antonio Aldea Armenteros
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimeLine>
#include <QGLFormat>
#include <QList>

#include "rubikCube.h"

//optimizacion: quitar el conjunto de cubitos rotando y añadir un flag a los cubitos

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    GLWidget(const QGLFormat &format, QWidget *parent = 0, const QGLWidget *shareWidget = 0, Qt::WindowFlags f = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void render();
    void setClearColor(const QColor &color);


signals:
    void clicked();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void draw();
    void applyCamera();
    void applyProjection();
    void pick(int x, int y);
    void windowToWorldCoords(float x, float y, float *v);
    void axis(void);

    QColor clearColor;
    QPoint lastPos;
    float lastWorldCoords[3];
    int xRot;
    int yRot;
    int zRot;
    float zoomLevel;
    float widthHeightRatio;
    rubikCube *cube;
    QList<uint> *pickupSelection;
    QTimeLine *animationTimeLine;
    bool showAxis;

private slots:
    void animate();
    void finishAnimation();

};

#endif
