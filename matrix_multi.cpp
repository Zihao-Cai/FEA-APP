#include "matrix_multi.h"
#include <iostream>
void matrix_multiplication(double** m1, double** m2,const int row1,const int col1,const int row2, const int col2,double** result) {
    //注意矩阵相乘结果应该是size m1 X col m2 的形式,即result要作考虑

    if (col1 != row2) { return; }
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            for (int k = 0; k < col1; k++) {
                *(*(result + i) + j) += (*(*(m1 + i) + k)) * (*(*(m2 + k) + j));
            }
        }
    }




}
