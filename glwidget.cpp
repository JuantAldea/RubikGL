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

#include <QtGui>
#include <QtOpenGL>
#include <cmath>
#include "glwidget.h"
#include <iostream>

using namespace std;

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget):
    QGLWidget(parent, shareWidget),
    clearColor(Qt::black),
    xRot(0),
    yRot(0),
    zRot(0),
    zoomLevel(0),
    cube(new rubikCube),
    pickupSelection(new QList<uint>),
    animationTimeLine(new QTimeLine),
    showAxis(false)
{
    this->animationTimeLine->setDuration(500);
    this->animationTimeLine->setUpdateInterval(500/20); //25 por segundo
    connect (this->animationTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(animate()));
    connect (this->animationTimeLine, SIGNAL(finished()), this, SLOT(finishAnimation()));
}

GLWidget::GLWidget (const QGLFormat &format, QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f):
    QGLWidget(format, parent, shareWidget, f),
    clearColor(Qt::black),
    xRot(0),
    yRot(0),
    zRot(0),
    zoomLevel(0),
    cube(new rubikCube),
    pickupSelection(new QList<uint>),
    animationTimeLine(new QTimeLine),
    showAxis(false)
{
    this->animationTimeLine->setDuration(500);
    this->animationTimeLine->setUpdateInterval(500/20); //25 refrescos
    connect (this->animationTimeLine, SIGNAL(valueChanged(qreal)), this, SLOT(animate()));
    connect (this->animationTimeLine, SIGNAL(finished()), this, SLOT(finishAnimation()));
}

GLWidget::~GLWidget()
{
    delete this->cube;
    delete this->animationTimeLine;
}

void GLWidget::axis(void)
{
    this->renderText(5, 0, 0, QString("X"),QFont("Arial", 30, QFont::Bold));
    // Eje X. (rojo)
    glColor3f(1, 0, 0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(1000, 0, 0);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(-1000, 0, 0);
    glVertex3f(0, 0, 0);
    glEnd();

    // Eje Y. (verde)
    glColor3f(0, 0, 0);
    this->renderText(0, 5, 0, QString("Y"),QFont("Arial", 30, QFont::Bold));
    glColor3f(0, 1, 0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(0, -1000, 0);
    glVertex3f(0, 0, 0);
    glEnd();

    // Eje Z. (azul)
    glColor3f(0, 0, 0);
    this->renderText(0, 0, 5, QString("Z"),QFont("Arial", 30, QFont::Bold));
    glColor3f(0, 0, 1);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1000);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, -1000);
    glVertex3f(0, 0, 0);
    glEnd();
    glLineWidth(5);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(500, 500);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void GLWidget::pick(int x, int y)
{
    int viewport[4];
    int hits;
    uint selectionBuffer[256];
    uint minDepth;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(256, selectionBuffer);

    glRenderMode(GL_SELECT);
    glInitNames();
    //glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 1.0f, 1.0f, viewport);
    this->applyProjection();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->applyCamera();
    this->draw();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    hits = glRenderMode(GL_RENDER);
    cout << "HITS " << hits << endl;

    if (hits > 0) {
        minDepth = UINT_MAX;
        int offset = 0;
        for (int i = 0; i < hits; i++) {
            if (selectionBuffer[offset + 1] < minDepth) {
                this->pickupSelection->clear();
                //selection = selectionBuffer[offset + 3]; // guardar todos los nombres
                for (uint j=0; j< selectionBuffer[offset]; j++) {
                    //cout << "name: " << selectionBuffer[offset+3+j] << endl;
                    this->pickupSelection->append(selectionBuffer[offset+3+j]);
                }

                minDepth = selectionBuffer[offset + 1];
            }
            offset += 3 + selectionBuffer[offset];
        }
        /*
        cout << "Seleccion :";
        for (int i=0; i<this->pickupSelection->size(); i++){
        	cout << this->pickupSelection->at(i) << ", ";
        }
        cout << endl;
        */
    }

    return;
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    GLint bufs, samples;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    glGetIntegerv(GL_SAMPLES, &samples);
    //cout << bufs <<' ' << samples << endl;
}

void GLWidget::paintGL()
{
    this->qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->render();
}

void GLWidget::applyProjection()
{
    gluPerspective(30.0f, this->widthHeightRatio, 0.1f, 500.0f);
}

void GLWidget::applyCamera()
{
    glTranslatef(0, 0, -this->zoomLevel);
    gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);
    glRotated(xRot, 1.0, 0.0, 0.0);
    glRotated(yRot, 0.0, 1.0, 0.0);
    glRotated(zRot, 0.0, 0.0, 1.0);
}

void GLWidget::render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->applyCamera();
    this->draw();
}

void GLWidget::draw()
{
    if (this->showAxis) {
        this->axis();
    }
    this->cube->draw();
}

void GLWidget::animate()
{
    this->cube->updateAnimation();
    this->updateGL();
}

void GLWidget::finishAnimation()
{
    this->cube->finishAnimation();
    this->pickupSelection->clear();
    this->updateGL();
}

void GLWidget::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->widthHeightRatio = width/(float)height;
    glViewport(0, 0, width, height);
    this->applyProjection();
    this->render();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    this->pick(event->x(), event->y());
    this->windowToWorldCoords(event->x(), event->y(), this->lastWorldCoords);
}

void GLWidget::windowToWorldCoords(float x, float y, float *v)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    v[0] = posX;
    v[1] = posY;
    v[2] = posZ;

    //cout << "WORLD COORDS " << posX << ' ' << posY << ' ' << posZ << endl;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    float worldPos[3];
    float dWorldPos[3];

    this->windowToWorldCoords(event->x(), event->y(), worldPos);

    dWorldPos[0] = worldPos[0] - this->lastWorldCoords[0];
    dWorldPos[1] = worldPos[1] - this->lastWorldCoords[1];
    dWorldPos[2] = worldPos[2] - this->lastWorldCoords[2];
    //No se debe salir del cubo
    if (fabs(dWorldPos[0]) <= 3 || fabs(dWorldPos[1]) <= 3 || fabs(dWorldPos[2]) <= 3) {
        if(this->animationTimeLine->state() == QTimeLine::NotRunning) {
            if(fabs(dx) > 10 || fabs(dy) > 10) {
                if(this->cube->rotate(this->cube->determineRotation(this->pickupSelection, dWorldPos[0], dWorldPos[1], dWorldPos[2]))) {
                    this->animationTimeLine->start();
                    this->updateGL();
                }
                lastPos = event->pos();
            }
        }
    } else {
        //se ha salido fuera de rango, tal vez con una seleccion, eliminar dicha seleccion
        this->pickupSelection->clear();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //Girar el cubo si no se selecciono nada
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (this->pickupSelection->size() == 0) {
        if (event->buttons() & Qt::LeftButton) {
            rotateBy(dy, dx, 0);
        } else if (event->buttons() & Qt::RightButton) {
            rotateBy(dy, 0, dx);
        }
        lastPos = event->pos();
    }
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        this->zoomLevel -= 1;
        this->zoomLevel = (this->zoomLevel > -16) ? this->zoomLevel : -15;
    } else {
        this->zoomLevel += 1;
        this->zoomLevel = (this->zoomLevel < 20) ? this->zoomLevel : 20;
    }
    this->render();
    this->updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //si hay una rotacion relalizandose se ignora el evento
    if(this->animationTimeLine->state() != QTimeLine::NotRunning) {
        return;
    }

    QString rotation_to_perform = "";

    switch(event->key()) {
    case Qt::Key_Space:
        this->showAxis = !this->showAxis;
        break;
    case Qt::Key_Q:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-X0";
        } else {
            rotation_to_perform = "+X0";
        }
        break;
    case Qt::Key_W:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-X1";
        } else {
            rotation_to_perform = "+X1";
        }
        break;
    case Qt::Key_E:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-X2";
        } else {
            rotation_to_perform = "+X2";
        }
        break;
    case Qt::Key_A:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Y0";
        } else {
            rotation_to_perform = "+Y0";
        }
        break;
    case Qt::Key_S:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Y1";
        } else {
            rotation_to_perform = "+Y1";
        }
        break;
    case Qt::Key_D:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Y2";
        } else {
            rotation_to_perform = "+Y2";
        }
        break;
    case Qt::Key_Z:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Z0";
        } else {
            rotation_to_perform = "+Z0";
        }
        break;
    case Qt::Key_X:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Z1";
        } else {
            rotation_to_perform = "+Z1";
        }
        break;
    case Qt::Key_C:
        if (event->modifiers().testFlag(Qt::ShiftModifier)) {
            rotation_to_perform = "-Z2";
        } else {
            rotation_to_perform = "+Z2";
        }
        break;
    default:
        return;
    }

    if (rotation_to_perform.length() > 0) {
        this->cube->rotate(rotation_to_perform);
    }

    if (event->key() != Qt::Key_Space) {
        this->animationTimeLine->start();
    }

    this->updateGL();
}

