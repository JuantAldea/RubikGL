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
        zoomLevel(15),
        cube(new rubikCube),
        pickupSelection(new QList<uint>),
        animationTimeLine(new QTimeLine)
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
        zoomLevel(15),
        cube(new rubikCube),
        pickupSelection(new QList<uint>),
        animationTimeLine(new QTimeLine)
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
    this->applyCamera();

    glMatrixMode(GL_MODELVIEW);
    this->draw();
    glMatrixMode(GL_PROJECTION);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    hits = glRenderMode(GL_RENDER);
    cout << "HITS " << hits << endl;

    if (hits > 0){
        minDepth = UINT_MAX;
        int offset = 0;
        for (int i = 0; i < hits; i++){
            if (selectionBuffer[offset + 1] < minDepth){
                this->pickupSelection->clear();
                //selection = selectionBuffer[offset + 3]; // guardar todos los nombres
                for (uint j=0; j< selectionBuffer[offset]; j++){
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

void GLWidget::applyCamera()
{
    gluPerspective(30.0f, this->widthHeightRatio, 0.1f, 500.0f);
    glTranslatef(0, 0, -this->zoomLevel);
    /*
    acabo de leer en 3D computer graphics - A mathematical introduction with opengl
    pagina 76 del pdf que el lookat debe hacerse sobre la model view por un problema con las luces
    "This is because the viewer should always be placed at the origin in order for opengl's lighting
    to work properly
    */
    gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);
    glRotated(xRot, 1.0, 0.0, 0.0);
    glRotated(yRot, 0.0, 1.0, 0.0);
    glRotated(zRot, 0.0, 0.0, 1.0);
}

void GLWidget::render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->applyCamera();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->draw();
}

void GLWidget::draw()
{
    this->axis();
    this->cube->draw();
}

void GLWidget::animate()
{
    this->cube->animate();
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
    this->widthHeightRatio = width/(float)height;
    glViewport(0, 0, width, height);
    this->render();

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    this->pick(event->x(), event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (this->pickupSelection->size() == 0){ // gira el cubo si no se selecciono nada
        if (event->buttons() & Qt::LeftButton) {
            rotateBy(0.5 * dy, 0.5 * dx, 0);
        } else if (event->buttons() & Qt::RightButton) {
            rotateBy(0.5 * dy, 0, 0.5 * dx);
        }
        //lastPos = event->pos();
    }
    else{
        //SACAR ESTO DE AQUI!!!!!!!!!
        if(this->animationTimeLine->state() == QTimeLine::NotRunning){
            if(abs(dx) > 10 || abs(dy) > 10){
                this->cube->rotate(this->cube->convertirMovimiento(this->pickupSelection, dx, dy));
                this->animationTimeLine->start();
                this->updateGL();
                //lastPos = event->pos();
            }
        }
    }
    lastPos = event->pos();
    //hay que estudiar la locaclizacion de esto mas


}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event = event;//fuera warning
    emit clicked();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0){
        this->zoomLevel -= 1;
    }
    else{
        this->zoomLevel += 0.5;
    }

    this->render();
    this->updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(this->animationTimeLine->state() != QTimeLine::NotRunning){
        return;
    }

    switch(event->key()){
        case Qt::Key_Q:
            this->cube->rotate("X0");
            break;
        case Qt::Key_W:
            this->cube->rotate("X1");
            break;
        case Qt::Key_E:
            this->cube->rotate("X2");
            break;
        case Qt::Key_A:
            this->cube->rotate("Y0");
            break;
        case Qt::Key_S:
            this->cube->rotate("Y1");
            break;
        case Qt::Key_D:
            this->cube->rotate("Y2");
            break;
        case Qt::Key_Z:
            this->cube->rotate("Z0");
            break;
        case Qt::Key_X:
            this->cube->rotate("Z1");
            break;
        case Qt::Key_C:
            this->cube->rotate("Z2");
            break;
        default:
            return;
    }

    this->animationTimeLine->start();
    this->updateGL();

    return;
}
