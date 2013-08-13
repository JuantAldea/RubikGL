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
    //X
    {{0, 18, 24, 6}, {7, 9, 25, 15}},
    {{1, 19, 23, 5}, {8, 10, 26, 14}},
    {{2, 20, 22, 4}, {3, 11, 21, 13}},
    //Y
    {{0, 6, 4, 2}, {1, 7, 5, 3}},
    {{9, 15, 13, 11}, {10, 16, 14, 12}},
    {{18, 24, 22, 20}, {19, 25, 23, 21}},
    //Z
    {{4, 6, 24, 22}, {5, 15, 23, 13}},
    {{3, 7, 25, 21}, {8, 16, 26, 12}},
    {{0, 18, 20, 2}, {1, 9, 19, 11}},
};

float rubikCube::cubiesPositions[27][3] = {
    //primera rodaja horizontal
    {-1, -1, 1}, {0, -1, 1}, {1, -1, 1}, {1, -1, 0}, {1, -1, -1}, {0, -1, -1}, {-1, -1, -1}, {-1, -1, 0}, {0, -1, 0},
    //segunda rodaja horizontal
    {-1, 0, 1}, {0, 0, 1}, {1, 0, 1}, {1, 0, 0}, {1, 0, -1}, {0, 0, -1}, {-1, 0, -1}, {-1, 0, 0}, {0, 0, 0},
    //tercera rodaja horizontal
    {-1, 1, 1}, {0, 1, 1}, {1, 1, 1}, {1, 1, 0}, {1, 1, -1}, {0, 1, -1}, {-1, 1, -1}, {-1, 1, 0}, {0, 1, 0},
};

color rubikCube::cubiesColors[27][6] = {
    //(trasera, arriba, derecha, izquierda, abajo, frontal)
    //primera rodaza horizontal
    {negro, negro, negro, amarillo, rojo, blanco},
    {negro, negro, negro, negro,    rojo, blanco},
    {negro, negro, verde, negro,    rojo, blanco},
    {negro, negro, verde, negro,    rojo, negro},
    {azul,  negro, verde, negro,    rojo, negro},
    {azul,  negro, negro, negro,    rojo, negro},
    {azul,  negro, negro, amarillo, rojo, negro},
    {negro, negro, negro, amarillo, rojo, negro},
    {negro, negro, negro, negro,    rojo, negro},

    //2ª rodaza horizontal

    {negro, negro, negro, amarillo, negro, blanco},
    {negro, negro, negro, negro,    negro, blanco},
    {negro, negro, verde, negro,    negro, blanco},
    {negro, negro, verde, negro,    negro, negro},
    {azul,  negro, verde, negro,    negro, negro},
    {azul,  negro, negro, negro,    negro, negro},
    {azul,  negro, negro, amarillo, negro, negro},
    {negro, negro, negro, amarillo, negro, negro},
    {negro, negro, negro, negro,    negro, negro},

    //3ª rodaza horizontal

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
    //X
    {6, 7, 0, 9, 18, 25, 24, 15, 16},
    {1, 19, 23, 5, 8, 10, 26, 14, 17},
    {2, 20, 22, 4, 3, 11, 21, 13, 12},
    //Y
    {0, 6, 4, 2, 1, 7, 5, 3, 8},
    {9, 15, 13, 11, 10, 16, 14, 12, 17},
    {18, 24, 22, 20, 19, 25, 23, 21, 26},
    //Z
    {4, 6, 24, 22, 5, 15, 23, 13, 14},
    {3, 7, 25, 21, 8, 16, 26, 12, 17},
    {0, 18, 20, 2, 1, 9, 19, 11, 10}
};

rubikCube::rubikCube() :
        cubies(new QVector<cubie*>),
        rotatingCubies(new QSet<int>)

{
    for (int i=0; i<27; i++){
        this->cubies->push_back(new cubie(rubikCube::cubiesColors[i]));
    }
}

rubikCube::~rubikCube()
{
    for (int i=0; i<this->cubies->size(); i++){
        delete this->cubies->at(i);
    }

    this->cubies->clear();
    delete this->cubies;
    delete this->rotatingCubies;
}

void rubikCube::rotate(QString rotation)
{
    if (rotation == "X0"){
        this->rotatingSlice = 0;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
        //testigo, no borrar
        //this->permutate(rubikCube::permutacionesCubies[0][0], 0);
        //this->permutate(rubikCube::permutacionesCubies[0][1], 0);
    }
    else if (rotation == "X1"){
        this->rotatingSlice = 1;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
    }
    else if (rotation == "X2"){
        this->rotatingSlice = 2;
        this->rotationAxis[0] = 1;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 0;
    }
    else if (rotation == "Y0"){
        this->rotatingSlice = 3;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    }
    else if (rotation == "Y1"){
        this->rotatingSlice = 4;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    }
    else if (rotation == "Y2"){
        this->rotatingSlice = 5;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 1;
        this->rotationAxis[2] = 0;
    }
    else if (rotation == "Z0"){
        this->rotatingSlice = 6;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    }
    else if (rotation == "Z1"){
        this->rotatingSlice = 7;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    }
    else if (rotation == "Z2"){
        this->rotatingSlice = 8;
        this->rotationAxis[0] = 0;
        this->rotationAxis[1] = 0;
        this->rotationAxis[2] = 1;
    }

    this->rotationAngle = 0;
    for (int i=0; i<9; i++){
        this->rotatingCubies->insert(rubikCube::cubiesSlices[this->rotatingSlice][i]);
    }

    return;
}

void rubikCube::draw()
{
    glColor3d(255, 0, 0);
    for (int i=0; i<this->cubies->size(); i++){
        glPushMatrix();
            if(this->rotatingCubies->contains(i)){
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

void rubikCube::permutate(int *p, int eje)
{
    cubie *aux1, *aux2;
    aux2 = this->cubies->at(p[3]);
    for (int i = 0; i < 4; i++){
        aux1 = this->cubies->at(p[i]);
        this->cubies->operator [](p[i]) = aux2;
        aux2 = aux1;
        aux2->permutate(eje);
    }

    return;
}

void rubikCube::animate()
{
    this->rotationAngle -= 90/25.0; //90 grados en 1 segundo con 25 actualizaciones por segundo (40ms)
}

void rubikCube::finishAnimation()
{
    this->rotationAngle = 0;
    this->rotatingCubies->clear();
    this->permutate(rubikCube::cubiesPermutations[this->rotatingSlice][0], this->rotatingSlice/3);
    this->permutate(rubikCube::cubiesPermutations[this->rotatingSlice][1], this->rotatingSlice/3);

    return;
}

//pensar mas en este prototipo, deberia estar en la clase rubikcube
QString rubikCube::convertirMovimiento(QList<uint> *lista, int deltaX, int deltaY)
{
    bool movimientoEnX = abs(deltaX) > abs(deltaY);
    uint cubito = lista->at(0);
    uint cara = lista->at(1);
    //(trasera, arriba, derecha, izquierda, abajo, frontal)
    //(0, 1, 2, 3, 4, 5)
    /*
    frontal:
        deltaX -> giro sobre Y
        deltaY -> giro sobre X
    trasera:
        deltaX -> giro sobre Y
        deltaY -> giro sobre X

    arriba:
        deltaX -> giro sobre Z
        deltaY -> giro sobre X
    abajo:
        deltaX -> giro sobre Z
        deltaY -> giro sobre X

    izquierda:
        deltaX -> giro sobre Y
        deltaY -> giro sobre Z
    derecha:
        deltaX -> giro sobre Y
        deltaY -> giro sobre Z

      frontal <=> trasera
       arriba <=> abajo
    izquierda <=> derecha
    */
    bool encontrado = false;
    int rodaja, desplazamiento;
    char eje = 'H';
    rodaja = -1;
    //SACAR UNA FUNCION PARA ESTA MIERDA
    switch(cara){
        case 0:
        case 5:
            if(movimientoEnX){//giro en Y
                eje = 'Y';
                desplazamiento = 3; //en 3 empiezan las rodajas de Y
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }
            else{//giro en X
                eje = 'X';
                desplazamiento = 0; //en 3 empiezan las rodajas de X
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }

            break;

        case 1:
        case 4:
            if(movimientoEnX){
                //giro en Z
                eje = 'Z';
                desplazamiento = 6; //en 6 empiezan las rodajas de Z
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }
            else{
                //giro en X
                eje = 'X';
                desplazamiento = 0; //en 3 empiezan las rodajas de X
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }

            break;

        case 2:
        case 3:
            if(movimientoEnX){
                //giro en Y
                eje = 'Y';
                desplazamiento = 3; //en 6 empiezan las rodajas de Z
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }
            else{
                //giro en Z
                eje = 'Z';
                desplazamiento = 6; //en 6 empiezan las rodajas de Z
                for (int i = 0; i < 3 && !encontrado; i++){
                    for (int j=0; j < 9 && !encontrado; j++){
                        if (rubikCube::cubiesSlices[desplazamiento + i][j] == cubito){
                            encontrado = true;
                            rodaja = i;
                        }
                    }
                }
            }
            break;
    }
    cout << (QString("%1%2").arg(eje).arg(rodaja)).toStdString() << endl;

    return QString("%1%2").arg(eje).arg(rodaja);
}

