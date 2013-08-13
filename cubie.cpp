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

#include "cubie.h"

float cubie::colorPegatinas[7][3] = {
    {1, 1, 1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
    {1, 1, 0}, {1, 0.5, 0},{0, 0, 0}
};

int cubie::geometriacubie[6][4][3] = {
    { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } }, //trasera
    { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } }, //arriba
    { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } }, //derecha
    { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } }, //izquierda
    { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } }, //abajo
    { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }  //frontal
};

int cubie::permutacionPegatinas[3][4] = {
    //(trasera, arriba, derecha, izquierda, abajo, frontal)
    {0, 4, 5, 1},//rotacion en X
    {0, 2, 5, 3},//Rotacion en Y
    {1, 2, 4, 3} //Rotacion en Z
};

void cubie::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 6; i++){
        glPushName(i);
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            glColor3fv(colorPegatinas[this->pegatinas->at(i)]);
            //glVertex3d(0.2 * geometriacubie[i][j][0], 0.2 * geometriacubie[i][j][1], 0.2 * geometriacubie[i][j][2]);
            glVertex3d(0.5 * geometriacubie[i][j][0], 0.5 * geometriacubie[i][j][1], 0.5 * geometriacubie[i][j][2]);

        }
        glEnd();
        glPopName();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);
    glColor3fv(colorPegatinas[negro]);
    for (int i = 0; i < 6; i++){
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            //glVertex3d(0.21 * geometriacubie[i][j][0], 0.21 * geometriacubie[i][j][1], 0.21 * geometriacubie[i][j][2]);
            glVertex3d(0.5 * geometriacubie[i][j][0], 0.5 * geometriacubie[i][j][1], 0.5 * geometriacubie[i][j][2]);
        }
        glEnd();
    }
}

cubie::cubie(cubie &orig) :
        pegatinas(new QVector<color>)
{
    for (int i=0; i<6; i++){
        this->pegatinas->push_back(orig.pegatinas->at(i));
    }
}

cubie::cubie() :
        pegatinas(new QVector<color>)
{
    for (int i=0; i<6; i++){
        this->pegatinas->push_back(negro);
    }
}

cubie::cubie(color *pegatinas) :
        pegatinas(new QVector<color>)
{
    for (int i=0; i<6; i++){
        this->pegatinas->push_back(pegatinas[i]);
    }
}

cubie::~cubie()
{
    delete this->pegatinas;
}

void cubie::permutate(int p)
{
    color aux1, aux2;
    aux2 = this->pegatinas->at(permutacionPegatinas[p][3]);
    for (int i = 0; i < 4; i++){
        aux1 = this->pegatinas->at(permutacionPegatinas[p][i]);
        this->pegatinas->operator [](permutacionPegatinas[p][i]) = aux2;
        aux2 = aux1;
    }
    return;
}
