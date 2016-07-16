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

#include "rubikCube.h"
#include <iostream>

using namespace std;

int rubikCube::cubiesPermutations[9][2][4] = {
    //Permutaciones sobre el eje X
    {{0, 18, 24, 6}, {7, 9, 25, 15}},
    {{1, 19, 23, 5}, {8, 10, 26, 14}},
    {{2, 20, 22, 4}, {3, 11, 21, 13}},
    //Permutaciones sobre el eje Y
    {{0, 6, 4, 2}, {1, 7, 5, 3}},
    {{9, 15, 13, 11}, {10, 16, 14, 12}},
    {{18, 24, 22, 20}, {19, 25, 23, 21}},
    //Permutaciones sobre el eje Z
    {{4, 6, 24, 22}, {5, 15, 23, 13}},
    {{3, 7, 25, 21}, {8, 16, 26, 12}},
    {{0, 18, 20, 2}, {1, 9, 19, 11}},
};

float rubikCube::cubiesPositions[27][3] = {
    //1ª rodaja horizontal
    {-1, -1, 1}, {0, -1, 1}, {1, -1, 1}, {1, -1, 0}, {1, -1, -1}, {0, -1, -1}, {-1, -1, -1}, {-1, -1, 0}, {0, -1, 0},
    //2ª rodaja horizontal
    {-1, 0, 1}, {0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {1, 0, -1}, {0, 0, -1}, {-1, 0, -1}, {-1, 0, 0}, {0, 0, 0},
    //3ª rodaja horizontal
    {-1, 1, 1}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {1, 1, -1}, {0, 1, -1}, {-1, 1, -1}, {-1, 1, 0}, {0, 1, 0},
};

color rubikCube::cubiesColors[27][6] = {
    //(trasera, arriba, derecha, izquierda, abajo, frontal)
    //primera rodaja horizontal
    {negro, negro, negro, amarillo, rojo, blanco},
    {negro, negro, negro, negro,    rojo, blanco},
    {negro, negro, verde, negro,    rojo, blanco},
    {negro, negro, verde, negro,    rojo, negro},
    {azul,  negro, verde, negro,    rojo, negro},
    {azul,  negro, negro, negro,    rojo, negro},
    {azul,  negro, negro, amarillo, rojo, negro},
    {negro, negro, negro, amarillo, rojo, negro},
    {negro, negro, negro, negro,    rojo, negro},

    //2ª rodaja horizontal

    {negro, negro, negro, amarillo, negro, blanco},
    {negro, negro, negro, negro,    negro, blanco},
    {negro, negro, verde, negro,    negro, blanco},
    {negro, negro, verde, negro,    negro, negro},
    {azul,  negro, verde, negro,    negro, negro},
    {azul,  negro, negro, negro,    negro, negro},
    {azul,  negro, negro, amarillo, negro, negro},
    {negro, negro, negro, amarillo, negro, negro},
    {negro, negro, negro, negro,    negro, negro},

    //3ª rodaja horizontal

    {negro, naranja, negro, amarillo, negro, blanco},
    {negro, naranja, negro, negro,    negro, blanco},
    {negro, naranja, verde, negro,    negro, blanco},
    {negro, naranja, verde, negro,    negro, negro},
    {azul,  naranja, verde, negro,    negro, negro},
    {azul,  naranja, negro, negro,    negro, negro},
    {azul,  naranja, negro, amarillo, negro, negro},
    {negro, naranja, negro, amarillo, negro, negro},
    {negro, naranja, negro, negro,    negro, negro},

};

int rubikCube::cubiesSlices[9][9] = {
    //Rodajas en X
    {6, 7, 0, 9, 18, 25, 24, 15, 16},
    {1, 19, 23, 5, 8, 10, 26, 14, 17},
    {2, 20, 22, 4, 3, 11, 21, 13, 12},
    //Rodajas en Y
    {0, 6, 4, 2, 1, 7, 5, 3, 8},
    {9, 15, 13, 11, 10, 16, 14, 12, 17},
    {18, 24, 22, 20, 19, 25, 23, 21, 26},
    //Rodajas en Z
    {4, 6, 24, 22, 5, 15, 23, 13, 14},
    {3, 7, 25, 21, 8, 16, 26, 12, 17},
    {0, 18, 20, 2, 1, 9, 19, 11, 10}
};

rubikCube::rubikCube() :
    cubies(new QVector<cubie*>),
    rotatingCubies(new QSet<int>)

{
    for (int i=0; i<27; i++) {
        this->cubies->push_back(new cubie(rubikCube::cubiesColors[i]));
    }
}

rubikCube::~rubikCube()
{
    for (int i=0; i<this->cubies->size(); i++) {
        delete this->cubies->at(i);
    }

    this->cubies->clear();
    delete this->cubies;
    delete this->rotatingCubies;
}

bool rubikCube::rotate(QString rotation)
{
    if(rotation[0] == '-') {
        this->clockwiseRotation = true;
    } else {
        this->clockwiseRotation = false;
    }

    rotation.remove(0, 1);//elimina el caracter de signo
    if (rotation[0] == 'H') {
        return false;
    }

    //marca la rodaja N como rodaja a rotar y establece el eje de rotacion
    if (rotation == "X0") {
        this->rotatingSlice = 0;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
    } else if (rotation == "X1") {
        this->rotatingSlice = 1;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
    } else if (rotation == "X2") {
        this->rotatingSlice = 2;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
    } else if (rotation == "Y0") {
        this->rotatingSlice = 3;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    } else if (rotation == "Y1") {
        this->rotatingSlice = 4;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    } else if (rotation == "Y2") {
        this->rotatingSlice = 5;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    } else if (rotation == "Z0") {
        this->rotatingSlice = 6;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    } else if (rotation == "Z1") {
        this->rotatingSlice = 7;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    } else if (rotation == "Z2") {
        this->rotatingSlice = 8;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    }

    //Marca los cubitos pertenecientes a la la rodaja seleccionada como cubitos a rotar
    this->rotationAngle = 0;
    for (int i = 0; i < 9; i++) {
        this->rotatingCubies->insert(rubikCube::cubiesSlices[this->rotatingSlice][i]);
    }

    return true;
}

void rubikCube::draw()
{
    glColor3d(255, 0, 0);
    for (int i = 0; i <this->cubies->size(); i++) {
        glPushMatrix();
        if(this->rotatingCubies->contains(i)) {
            glRotatef(this->rotationAngle, this->rotationAxis[0], this->rotationAxis[1], this->rotationAxis[2]);
        }
        glTranslated(rubikCube::cubiesPositions[i][0], rubikCube::cubiesPositions[i][1], rubikCube::cubiesPositions[i][2]);
        glPushName(i);
        this->cubies->at(i)->draw();
        glPopName();
        glPopMatrix();

    }

    return;
}

void rubikCube::permutate(int *p, int axis, bool clockwise)
{
    cubie *aux1, *aux2;
    if (clockwise) {
        aux2 = this->cubies->at(p[3]);
        for (int i = 0; i < 4; i++) {
            aux1 = this->cubies->at(p[i]);
            this->cubies->operator [](p[i]) = aux2;
            aux2 = aux1;
            aux2->permutate(axis, clockwise);
        }
    } else {
        aux2 = this->cubies->at(p[0]);
        for (int i = 3; i >= 0; i--) {
            aux1 = this->cubies->at(p[i]);
            this->cubies->operator [](p[i]) = aux2;
            aux2 = aux1;
            aux2->permutate(axis, clockwise);
        }
    }
}

void rubikCube::updateAnimation()
{
    //90 grados en 1 segundo con 25 actualizaciones por segundo (40ms)
    if (this->clockwiseRotation) {
        this->rotationAngle -= 90/20.0;
    } else {
        this->rotationAngle += 90/20.0;
    }
}

void rubikCube::finishAnimation()
{
    this->rotationAngle = 0;
    //no hay cubos rotando
    this->rotatingCubies->clear();
    //acabada la animacion, aplicar la permutacion.
    this->permutate(rubikCube::cubiesPermutations[this->rotatingSlice][0], this->rotatingSlice/3, this->clockwiseRotation);
    this->permutate(rubikCube::cubiesPermutations[this->rotatingSlice][1], this->rotatingSlice/3, this->clockwiseRotation);
}


//busca el grupo al que pertenece el cubito pulsado para devolver el nombre del movimiento:d X0, X1, X2...
QString rubikCube::determineRotation(QList<uint> *selectionList, float deltaX, float deltaY, float deltaZ)
{
    /*
    (trasera, arriba, derecha, izquierda, abajo, frontal)
    (0, 1, 2, 3, 4, 5)
    frontal <=> trasera
    arriba <=> abajo
    izquierda <=> derecha
    */

    float maxDelta = fabs(deltaX);
    uint cubie = selectionList->at(0);
    uint face = selectionList->at(1);
    bool found = false;
    int slice = -1;
    int offset;
    char axis = 'H';
    char rotationSign = '+';
    char mouseAxis = 'X';

    //cout << cubie << ' ' << face << endl;
    //cout << deltaX << endl;

    if (fabs(deltaY) > maxDelta) {
        maxDelta = fabs(deltaY);
        mouseAxis = 'Y';
    }

    if (fabs(deltaZ) > maxDelta) {
        maxDelta = fabs(deltaZ);
        mouseAxis = 'Z';
    }

    //cout << maxDelta << endl;
    //cout << mouseAxis << endl;

    /*
     * a partir de la cara seleccionada y el movimiento realizado con el raton
     * se determina el eje de rotacion, cada eje tiene permutaciones posibles
     * (seis contando las invertidas) hay que buscar en cual de las tres permutaciones
     * se encuentra el cubo seleccionado.
     * Determinada la permutacion a aplicar se calcula el signo esto es, si se aplica
     * en orden normal o en orden inverso.
     */

    switch(face) {
    case 0:
    case 5:
        if(mouseAxis == 'X') { //giro en Y
            axis = 'Y';
            offset = 3; //en 3 empiezan las rodajas de Y
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        } else if(mouseAxis == 'Y') { //giro en X
            axis = 'X';
            offset = 0; //en 0 empiezan las rodajas de X
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        }

        break;

    case 1:
    case 4:
        if(mouseAxis == 'X') {
            //giro en Z
            axis = 'Z';
            offset = 6; //en 6 empiezan las rodajas de Z
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        } else if(mouseAxis == 'Z') {
            //giro en X
            axis = 'X';
            offset = 0; //en 3 empiezan las rodajas de X
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        }
        break;

    case 2:
    case 3:
        if(mouseAxis == 'Z') {
            //giro en Y
            axis = 'Y';
            offset = 3; //en 6 empiezan las rodajas de Z
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        } else if(mouseAxis == 'Y') {
            //giro en Z
            axis = 'Z';
            offset = 6; //en 6 empiezan las rodajas de Z
            for (int i = 0; i < 3 && !found; i++) {
                for (int j = 0; j < 9 && !found; j++) {
                    if (rubikCube::cubiesSlices[offset + i][j] == (int)cubie) {
                        found = true;
                        slice = i;
                    }
                }
            }
        }
        break;
    }

    //calculo del signo de la rotacion
    switch (face) {
    case 0: //Trasera
        if (mouseAxis == 'Y') {
            rotationSign = (deltaY > 0)?'+':'-';
        } else if(mouseAxis == 'X') {
            rotationSign = (deltaX > 0)?'-':'+';
        }
        break;

    case 1://arriba
        if (mouseAxis == 'X') {
            rotationSign = (deltaX > 0)?'-':'+';
        } else if(mouseAxis == 'Z') {
            rotationSign = (deltaZ > 0)?'+':'-';
        }
        break;

    case 2://derecha
        if (mouseAxis == 'Z') {
            rotationSign = (deltaZ > 0)?'-':'+';
        } else if(mouseAxis == 'Y') {
            rotationSign = (deltaY > 0)?'+':'-';
        }
        break;

    case 3://izquierda
        if (mouseAxis == 'Z') {
            rotationSign = (deltaZ > 0)?'+':'-';
        } else if(mouseAxis == 'Y') {
            rotationSign = (deltaY > 0)?'-':'+';
        }
        break;

    case 4://abajo
        if (mouseAxis == 'X') {
            rotationSign = (deltaX > 0)?'+':'-';
        } else if(mouseAxis == 'Z') {
            rotationSign = (deltaZ > 0)?'-':'+';
        }
        break;

    case 5://frontal
        if (mouseAxis == 'Y') {
            rotationSign = (deltaY > 0)?'-':'+';
        } else if(mouseAxis == 'X') {
            rotationSign = (deltaX > 0)?'+':'-';
        }
        break;
    }

    return QString("%1%2%3").arg(rotationSign).arg(axis).arg(slice);
}
