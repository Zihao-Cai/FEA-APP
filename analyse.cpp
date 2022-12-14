#include "analyse.h"


bool analyse(Element* ele){
    for(int i=0;i<2;i++){
        if(ele[i].E <= 0 || ele[i].v <=-1 || ele[i].t <= 0 || ele[i].v >0.5) return false;
    }

    const int row = 8;
    const int col = row+1;//value col == 9
    double** K = new double* [row]();
    double** K0 = new double* [row-2]();
    double** K1 = new double* [row-2]();
    for(int i = 0; i < row; i++) {
        K[i] = new double[col]();
    }
    for(int i=0;i<row-2;i++){
        K0[i] = new double[col-3]();
        K1[i] = new double[col-3]();
    }



    double r0 = ele[0].E*ele[0].t/4/ele[0].A/(1-ele[0].v*ele[0].v);
    double r1 = ele[1].E*ele[1].t/4/ele[1].A/(1-ele[1].v*ele[1].v);

    //下面分别构造出两单元的单刚度矩阵K0,K1  6x6
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            K0[2*i][2*j] = ele[0].eq_b[i]*ele[0].eq_b[j]+(1-ele[0].v)/2*ele[0].eq_c[i]*ele[0].eq_c[j];K0[2*i][2*j]*=r0;
            K0[2*i][2*j+1] = ele[0].v*ele[0].eq_b[i]*ele[0].eq_c[j]+(1-ele[0].v)/2*ele[0].eq_c[i]*ele[0].eq_b[j];K0[2*i][2*j+1]*=r0;
            K0[2*i+1][2*j] = ele[0].v*ele[0].eq_c[i]*ele[0].eq_b[j]+(1-ele[0].v)/2*ele[0].eq_b[i]*ele[0].eq_c[j];K0[2*i+1][2*j]*=r0;
            K0[2*i+1][2*j+1] = ele[0].eq_c[i]*ele[0].eq_c[j]+(1-ele[0].v)/2*ele[0].eq_b[i]*ele[0].eq_b[j];K0[2*i+1][2*j+1]*=r0;
            K1[2*i][2*j] = ele[1].eq_b[i]*ele[1].eq_b[j]+(1-ele[1].v)/2*ele[1].eq_c[i]*ele[1].eq_c[j];K1[2*i][2*j]*=r1;
            K1[2*i][2*j+1] = ele[1].v*ele[1].eq_b[i]*ele[1].eq_c[j]+(1-ele[1].v)/2*ele[1].eq_c[i]*ele[1].eq_b[j];K1[2*i][2*j+1]*=r1;
            K1[2*i+1][2*j] = ele[1].v*ele[1].eq_c[i]*ele[1].eq_b[j]+(1-ele[1].v)/2*ele[1].eq_b[i]*ele[1].eq_c[j];K1[2*i+1][2*j]*=r1;
            K1[2*i+1][2*j+1] = ele[1].eq_c[i]*ele[1].eq_c[j]+(1-ele[1].v)/2*ele[1].eq_b[i]*ele[1].eq_b[j];K1[2*i+1][2*j+1]*=r1;
        }
    }
    //下面构造出矩形的总刚度矩阵K  8x8
    //并入K0
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            K[2*i][2*j] += K0[2*i][2*j];
            K[2*i][2*j+1] += K0[2*i][2*j+1];
            K[2*i+1][2*j] += K0[2*i+1][2*j];
            K[2*i+1][2*j+1] += K0[2*i+1][2*j+1];
        }
        K[2*i][6] += K0[2*i][4];
        K[2*i][7] += K0[2*i][5];
        K[2*i+1][6] += K0[2*i+1][4];
        K[2*i+1][7] += K0[2*i+1][5];
    }
    for(int i=0;i<2;i++){
         K[6][2*i] += K0[4][2*i];
         K[6][2*i+1] += K0[4][2*i+1];
         K[7][2*i] += K0[5][2*i];
         K[7][2*i+1] += K0[5][2*i+1];
    }
    K[6][6] += K0[4][4];
    K[6][7] += K0[4][5];
    K[7][6] += K0[5][4];
    K[7][7] += K0[5][5];
    //并入K1
    for(int i=1;i<4;i++){
        for(int j=1;j<4;j++){
            K[2*i][2*j] += K1[2*(i-1)][2*(j-1)];
            K[2*i][2*j+1] += K1[2*(i-1)][2*(j-1)+1];
            K[2*i+1][2*j] += K1[2*(i-1)+1][2*(j-1)];
            K[2*i+1][2*j+1] += K1[2*(i-1)+1][2*(j-1)+1];
        }
    }
    //总刚度矩阵K构造完成
    double flist[row];
    for(int i=0;i<4;i++){
        flist[2*i] = ele[0].nodes[i].fx+ele[1].nodes[i].fx;
        flist[2*i+1] = ele[0].nodes[i].fy+ele[1].nodes[i].fy;
        if(ele[0].nodes[i].isvalid&&ele[1].nodes[i].isvalid){
            flist[2*i] /= 2;
            flist[2*i+1] /= 2;
        }
    }
    //根据F判断模型是否平衡,否则分析失败
    double fx=0,fy=0;
    for(int i=0,j=1;j<row;i+=2,j+=2){
        fx += flist[i];
        fy += flist[j];
    }
    if(fx!=0 || fy!=0)  return false;
    //判断完成
    for(int i=0;i<row;i++){
        K[i][8] = flist[i];
    }
    //加上位移边界条件,将矩阵改成可解矩阵:这里已知delta 0,1,3都为0//
    for(int j=0;j<row;j++){
        K[0][j] = 0;
        K[1][j] = 0;
        K[3][j] = 0;
        K[j][0] = 0;
        K[j][1] = 0;
        K[j][3] = 0;
    }
    K[0][8] = 0;
    K[1][8] = 0;
    K[3][8] = 0;
    K[0][0] = 1;
    K[1][1] = 1;
    K[3][3] = 1;
    //边界条件加载完成

    double* tempdelta = new double[row]();
    decoding_matrix(K,row,col,tempdelta);
    //delta求解结果置于临时空间tempdelta[8]内,下面将其存入单元属性
    for(int i=0;i<row;i++){
        if(qIsNaN(*(tempdelta+i)))  return false;
    }
    ele[0].delta[0] = *(tempdelta);
    ele[0].delta[1] = *(tempdelta+1);
    ele[0].delta[2] = *(tempdelta+2);
    ele[0].delta[3] = *(tempdelta+3);
    ele[0].delta[4] = *(tempdelta+6);
    ele[0].delta[5] = *(tempdelta+7);
    ele[1].delta[0] = *(tempdelta+2);
    ele[1].delta[1] = *(tempdelta+3);
    ele[1].delta[2] = *(tempdelta+4);
    ele[1].delta[3] = *(tempdelta+5);
    ele[1].delta[4] = *(tempdelta+6);
    ele[1].delta[5] = *(tempdelta+7);
    delete [] tempdelta;
    for(int i=0;i<row;i++){
        delete K[i];
    }
    delete [] K;
    //下一步要计算区域内部的应变关系//
    double** B0 = new double* [3]();
    double** B1 = new double* [3]();
    for(int i=0;i<3;i++){
        B0[i] = new double[6]();
        B1[i] = new double[6]();
    }
    //构造B矩阵
    double ratio0 = 1.0/2.0/ele[0].A;
    double ratio1 = 1.0/2.0/ele[1].A;
    for(int j=0;j<3;j++){
        B0[0][2*j] = ratio0*ele[0].eq_b[j];B0[0][2*j+1] = 0;
        B0[1][2*j] = 0;B0[1][2*j+1] = ratio0*ele[0].eq_c[j];
        B0[2][2*j] = ratio0*ele[0].eq_c[j];B0[0][2*j+1] = ratio0*ele[0].eq_b[j];
        B1[0][2*j] = ratio1*ele[1].eq_b[j];B1[0][2*j+1] = 0;
        B1[1][2*j] = 0;B1[1][2*j+1] = ratio1*ele[1].eq_c[j];
        B1[2][2*j] = ratio1*ele[1].eq_c[j];B1[0][2*j+1] = ratio1*ele[1].eq_b[j];
    }
    //B矩阵构造完成，调用求解函数求解应变关系//
    double** temp_epson0 = new double*[3]();
    double** temp_epson1 = new double*[3]();
    double** delta0 = new double*[6]();
    double** delta1 = new double*[6]();
    for(int i=0;i<6;i++){
        if(i<3){
            temp_epson0[i] = new double();
            temp_epson1[i] = new double();
            }
        delta0[i] = new double();
        delta0[i][0] = ele[0].delta[i];
        delta1[i] = new double();
        delta1[i][0] = ele[1].delta[i];
    }
    matrix_multiplication(B0,delta0,3,6,6,1,temp_epson0);
    matrix_multiplication(B1,delta1,3,6,6,1,temp_epson1);
    for(int i=0;i<3;i++){
        if(qIsNaN(temp_epson0[i][0]) || qIsNaN(temp_epson1[i][0]))  return false;
    }
    //下面将结果存入单元属性
    for(int i=0;i<3;i++){
        ele[0].epson[i] = temp_epson0[i][0];
        ele[1].epson[i] = temp_epson1[i][0];
    }
    for(int i=0;i<6;i++){
        delete delta0[i];
        delete delta1[i];
    }
    delete [] delta0;
    delete [] delta1;

    //下一步要计算区域内部的应力关系//
    double** D0 = new double* [3]();
    double** D1 = new double* [3]();
    double** temp_sigma0 = new double*[3]();
    double** temp_sigma1 = new double*[3]();
        for(int i=0;i<3;i++){
            D0[i] = new double[3]();
            D1[i] = new double[3]();
            temp_sigma0[i] = new double();
            temp_sigma1[i] = new double();
        }
        //构造D矩阵
        double ra0 = ele[0].E/(1-ele[0].v*ele[0].v);
        double ra1 = ele[1].E/(1-ele[1].v*ele[1].v);
        D0[0][0] = D0[1][1] = 1*ra0;
        D0[0][1] = D0[1][0] = ele[0].v*ra0;
        D0[0][2] = D0[2][0] = D0[1][2] = D0[2][1] = 0;
        D0[2][2] = (1-ele[0].v)/2*ra0;
        D1[0][0] = D1[1][1] = 1*ra1;
        D1[0][1] = D1[1][0] = ele[1].v*ra1;
        D1[0][2] = D1[2][0] = D1[1][2] = D1[2][1] = 0;
        D1[2][2] = (1-ele[1].v)/2*ra1;
        //D矩阵构造完成//
        //求解区域内部的应力关系
        matrix_multiplication(D0,temp_epson0,3,3,3,1,temp_sigma0);
        matrix_multiplication(D1,temp_epson1,3,3,3,1,temp_sigma1);
        for(int i=0;i<3;i++){
           if(qIsNaN(temp_sigma0[i][0]) || qIsNaN(temp_sigma1[i][0]))  return false;
        }
        //将结果存入单元属性
        for(int i=0;i<3;i++){
            ele[0].sigma[i] = temp_sigma0[i][0];
            ele[1].sigma[i] = temp_sigma1[i][0];
        }
        for(int i=0;i<3;i++){
            delete temp_epson0[i];
            delete temp_epson1[i];
            delete temp_sigma0[i];
            delete temp_sigma1[i];
        }
        delete [] temp_epson0;
        delete [] temp_epson1;
        delete [] temp_sigma0;
        delete [] temp_sigma1;
        //释放无用内存
    return true;
}



