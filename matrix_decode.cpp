#include <stdlib.h>
#include <stdio.h>
#include "matrix_decode.h"
void decoding_matrix(double** data,const int row, const int col,double* result) {

    double **temple_data = new double* [row]();
    for (int k = 0; k < row; k++) { temple_data[k] = new double[col](); }

    for (int m = 0; m < row; m++) {
        for (int n = 0; n < col; n++) { *(* (temple_data + m) + n) = *(*(data + m) + n); }
    }/*use a temple array to protect the source data array in parent*/

    /*loops to transformation the matrix*/
    for (int r = 0; r <= col - 3; r++) {//按列从左向右
        //从下向上消元前做一次首元置换操作,将最大数值首元找出置换到r行,此举有利于提高求解精确度
        int maxindex = r;
        for(int n=r;n<row;n++){
            if(qAbs(*(*(temple_data + n) + r)) > qAbs(*(*(temple_data + r) + r)))   maxindex = n;
        }
        if(maxindex != r){
            for(int j=0;j<col;j++){
                double tm = *(*(temple_data + r) + j);
                *(*(temple_data + r) + j) = *(*(temple_data + maxindex) + j);
                *(*(temple_data + maxindex) + j) = tm;
            }

        }
        //置换完毕,此时r行是数值最大首元行
        for (int i = row - 1; i >= r+1; i--) {//按行从下向上
            if (*(*(temple_data + i) + r) == 0) { continue; }//元素为0则跳过继续向上
            else {
                double k = (*(*(temple_data + i) + r)) /(*(*(temple_data + r) + r));
                for (int j = 0; j < col; j++) {
                    *(*(temple_data + i) + j) = *(*(temple_data + i) + j) - k*(*(*(temple_data + r) + j));
                }
                /*decrease first elements to 0 from each row*/
            }
        }


    }       /*finished operations for transformation data matrix to a triangle matrix*/


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < i; j++) {
            *(*(temple_data + i) + j) = 0;
        }
    }/*this loop is aimed to decrease the probably happened redundant non-0  to 0 in triangle matrix without changing other numbers existed*/

    double sum = 0;
    for (int i = row - 1, n = 0; i >= 0; i--, n++) {
        if (i == row - 1) { *(result+i) = *(*(temple_data + i) + col-1) / *(*(temple_data + i) + col - 2); }
        else {
            for (int k = 1; k <= n; k++) {
                sum += *(result+i+k) *( * (*(temple_data + i) + i + k));
            }
            *(result+i) = (*(*(temple_data + i) + col - 1) - sum) / *(*(temple_data + i) + i);
            sum = 0;
        }
    }/*here,all results gotten are located into array result*/

    for (int i = 0; i < row; i++) {
        delete temple_data[i];
    }delete[]temple_data;//relesae the old memory

    return;
}
