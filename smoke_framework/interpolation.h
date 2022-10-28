#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cmath>
#include <vector>

#include <QDebug>

namespace interpolation
{
    /* Input
     * values: You may assume this is of the type std::vector<float>. This contains the values (e.g. densities) to be interpolated.
     * sideSize: The input size of the square matrix "values". This is equal to m_DIM in the Simulation and Visualization classes.
     * xMax, yMax: The desired dimensions of the output vector. xMax is the horizontal size (number of columns), yMax is the vertical size (number of rows).
     *
     * Output
     * interpolatedValues: A 1D row-major container of std::vector<float> type containing the interpolated values.
     */
    template <typename inVector>
    std::vector<float> interpolateSquareVector(inVector const &values, size_t const sideSize, size_t const xMax, size_t const yMax)
    {
        std::vector<float> interpolatedValues(xMax*yMax);

        float xDist = float(sideSize/xMax); // The distance between sample points in x direction
        float yDist = float(sideSize/yMax); // The distance between sample points in y direction

        for(int j = 0; j < yMax; j++){
            for(int i = 0; i < xMax; i++){

                int idx = i + j* yMax;

                // Point to find the value of
                float x = xDist*i;
                float y = yDist*j;

                // The coordinat of the cell origin (bottol left corner)
                int originX = floor(x);
                int originY = floor(y);

                // Index of the corners of the cell the point is in
                int corner00 = originX + originY * sideSize;
                int corner01 = corner00 + 1;
                int corner10 = corner00 + sideSize;
                int corner11 = corner00 + sideSize + 1;

                // Coordinat of the corners of the cell the point is in
                float x1 = originX;
                float x2 = originX + 1;
                float y1 = originY;
                float y2 = originY + 1;

                // Values of the coorners of the
                float v00 = values[corner00];
                float v01 = values[corner01];
                float v10 = values[corner10];
                float v11 = values[corner11];

                float R1 = ((x2-x)/(x2-x1)) * v00 + ((x-x1)/(x2-x1)) * v01;
                float R2 = ((x2-x)/(x2-x1)) * v10 + ((x-x1)/(x2-x1)) * v11;

                interpolatedValues[idx] = ((y2-y)/(y2-y1)) * R1 + ((y-y1)/(y2-y1)) * R2;
            }
        }

        return interpolatedValues;
    }
};

#endif // INTERPOLATION_H
