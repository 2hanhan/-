#ifndef RD_H_ICLUDE
#define RD_H_ICLUDE
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

/**
 * @brief 单个手写数字
 * 功能：用于存储单个手写数字样本
 */
class Digit
{
public:
    std::vector<int> feature; //特征
    int label;                //标签

    //获取标签
    void get_labe(int &label);
    //获取特征
    void get_feature(std::vector<std::string> &strArray);
    //显示数据
    void show();
    //构造函数
    Digit();
};

/**
 * @brief 数据集
 * 功能：用于加载存储数据集
 */
class ReadData
{
public:
    std::string datasdir;     //数据集存储路径
    int num;                  //数据集个数
    std::vector<Digit> datas; //数据集

    //从txt中读取数据集
    void read_datas();
    //构造函数
    ReadData(std::string dir);
};

#endif // RD_H_ICLUDE