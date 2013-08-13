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

#include <GL/gl.h>
#include <QVector>

typedef enum {blanco, rojo, verde, azul, amarillo, naranja, negro} color;

class cubie {
    private:
        QVector<color> *pegatinas;
        static int permutacionPegatinas[3][4];
        static int geometriacubie[6][4][3];
        static float colorPegatinas[7][3];
    public:
        cubie();
        cubie(cubie &orig);
        cubie(color *pegatinas);
        ~cubie();
        void draw();
        void permutate(int p);

};
