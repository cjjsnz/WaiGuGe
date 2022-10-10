//
// Created by cjj on 2022/8/16.
//
#include "tes.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

// 功能：将filename 中的数据（共cols列）读取到_vector中，_vector可视为二维数组
int read_scanf(const string &filename, const int &cols, vector<double *> &_vector)
{
    FILE *fp = fopen(filename.c_str(), "r");
    bool flag = true;
    int i = 0;
    if (!fp)
    {
        cout << "File open error!\n";
        return 0;
    }

    while (flag)
    {
        double *rowArray = new double[cols]; //new一个double类型的动态数组

        for (i = 0; i < cols; i++) //读取数据，存在_vector[cols]中
        {
            if (EOF == fscanf(fp,"%lf", &rowArray[i]))
            {
                flag = false;
                break;
            }
            //输出rowArray存入的数据
            //cout << rowArray[0] << " " << rowArray[1] << " " << rowArray[2] << " " << rowArray[3] << endl;
        }
        if (cols == i) //将txt文本文件中的一行数据存入rowArray中，并将rowArray存入vector中
            _vector.push_back(rowArray);
    }
    fclose(fp);
    return 1;
}
int  readtxt()
{
    string file ="tes.txt";
    //txt文件中有4列
    int columns = 4;
    vector<double *> output_vector;
    if (!read_scanf(file, columns, output_vector))
    {
        return 0;
    }

    //output_vector可视为二维数组;输出数组元素：
    int rows = output_vector.size();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << output_vector[i][j] << " ";
        }
        cout << endl;
    }

    system("pause");
    return 0;
}

