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

float cubie::stickerColor[7][3] = {
    {1, 1, 1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
    {1, 1, 0}, {1, 0.5, 0},{0, 0, 0}
};

//Quads para formar los cubitos
int cubie::cubieGeometry[6][4][3] = {
    { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } }, //trasera
    { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } }, //arriba
    { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } }, //derecha
    { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } }, //izquierda
    { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } }, //abajo
    { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }  //frontal
};

//Permutaciones sobre las pegatinas
int cubie::stickerPermutations[3][4] = {
    //(trasera, arriba, derecha, izquierda, abajo, frontal)
    {0, 4, 5, 1}, //Rotacion en X
    {0, 2, 5, 3}, //Rotacion en Y
    {1, 2, 4, 3}  //Rotacion en Z
};

void cubie::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int i = 0; i < 6; i++) {
        glPushName(i);
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            glColor3fv(stickerColor[this->stickers->at(i)]);
            glVertex3d(0.5 * cubieGeometry[i][j][0], 0.5 * cubieGeometry[i][j][1], 0.5 * cubieGeometry[i][j][2]);
        }

        glEnd();
        glPopName();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);
    glColor3fv(stickerColor[negro]);
    for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            glVertex3d(0.5 * cubieGeometry[i][j][0], 0.5 * cubieGeometry[i][j][1], 0.50 * cubieGeometry[i][j][2]);
            //AÑADIR CILINDROS
            /*
            glPushMatrix();
            glTranslated(0,5,0);
            gluCylinder(quadratic, 1.0f, 1.0f, 10.0f, 100, 1);
            glPopMatrix();
            */
        }
        glEnd();
    }
}

cubie::cubie(cubie &orig) :
    stickers(new QVector<color>),
    quadratic(NULL)

{
    for (int i=0; i<6; i++) {
        this->stickers->push_back(orig.stickers->at(i));
    }

    this->createCylinder();
}

cubie::cubie() :
    stickers(new QVector<color>),
    quadratic(NULL)
{
    for (int i=0; i<6; i++) {
        this->stickers->push_back(negro);
    }

    this->createCylinder();
}

cubie::cubie(color *stickers) :
    stickers(new QVector<color>),
    quadratic(NULL)

{
    for (int i=0; i<6; i++) {
        this->stickers->push_back(stickers[i]);
    }

    this->createCylinder();
}

cubie::~cubie()
{
    delete this->stickers;
    gluDeleteQuadric(this->quadratic);
}

void cubie::createCylinder()
{
    if (this->quadratic != NULL) {
        return;
    }

    this->quadratic = gluNewQuadric();
    gluQuadricNormals(this->quadratic, GLU_SMOOTH);
}

void cubie::permutate(int p, bool clockwise)
{
    color aux1, aux2;

    if (clockwise) {
        aux2 = this->stickers->at(stickerPermutations[p][3]);
        for (int i = 0; i < 4; i++) {
            aux1 = this->stickers->at(stickerPermutations[p][i]);
            this->stickers->operator [](stickerPermutations[p][i]) = aux2;
            aux2 = aux1;
        }
    } else {
        aux2 = this->stickers->at(stickerPermutations[p][0]);
        for (int i = 3; i >= 0; i--) {
            aux1 = this->stickers->at(stickerPermutations[p][i]);
            this->stickers->operator [](stickerPermutations[p][i]) = aux2;
            aux2 = aux1;
        }
    }
}
