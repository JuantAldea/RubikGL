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

#ifndef CUBO_H
#define CUBO_H

#include <QString>
#include <QVector>
#include <QSet>
#include "cubie.h"
#include <cmath>

class rubikCube
{
protected:
    static int cubiesPermutations[9][2][4];
    static color cubiesColors[27][6];
    static float cubiesPositions[27][3];
    static int cubiesSlices[9][9];

    QVector<cubie*> *cubies;
    float rotationAngle;
    float rotationAxis[4];
    int rotatingSlice;
    bool clockwiseRotation;
    QSet<int> *rotatingCubies;

    void permutate(int *p, int eje, bool clockwise);


public:
    rubikCube();
    ~rubikCube();
    bool rotate(QString rotation);
    void draw();
    void updateAnimation();
    void finishAnimation();
    QString determineRotation(QList<uint> *lista, float deltaX, float deltaY, float deltaZ);
};

#endif // CUBO_H
