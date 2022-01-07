#ifndef RF_H_ICLUDE
#define RF_H_ICLUDE
#include <ReadData.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

class Node
{
public:
    int parent; //父节点索引
    int left;   //子节点索引
    int right;  //子节点索引

    int depth; //节点深度

    int split_feature;   //分裂使用的特征
    int split_threshold; //分裂阈值

    int result; //该节点的预测结果 没有结果为-1

    //设置节点参数
    void set_node(int parent,          //父节点索引
                  int left,            //子节点索引
                  int right,           //子节点索引
                  int depth,           //节点深度
                  int split_feature,   //分裂使用的特征
                  int split_threshold, //分裂阈值
                  int result           //该节点的预测结果 没有结果为-1
    );
    //设置分裂参数
    void set_split_param(int &split_feature, int &split_threshold, int result);
    Node();
};

/**
 * @brief  决策树
 *
 */
class Tree
{
public:
    std::vector<Node> nodes; //决策树的节点
    double OOB_error;        //单个tree OOB袋外误差
    double gini;             //基尼系数

    //增加节点
    void push_node(Node &node);
    //单个决策树预测结果
    int predict(Digit &digit);

    Tree(/* args */);
};

/**
 * @brief 随机森林
 *
 */
class RandomForest
{
public:
    //随机森林决策树各个参数
    double S;                 //随机训练样本的比例
    std::vector<Tree> Forest; //决策树
    int max_depth;            //最大深度
    int min_sample_count;     //决策树节点的最小样本数
    int max_categories;       //分类个数
    int number_of_feature;    //随机选择使用的特征个数

    //终止指标
    double regression_accuracy;         //精度终止条件
    int max_num_of_trees_in_the_forest; //表示森林中决策树的最大数量

    //评价指标
    double forest_accuracy;            //当前精度
    double num_of_trees_in_the_forest; //当前森林决策树个数

    //训练
    int train(std::vector<Digit> &datas, std::vector<Digit> &test);
    //计算分裂指标
    Node set_node(std::vector<Digit> &datas, std::vector<int> &datas_random_index, std::vector<int> &feature_random_index);
    //预测函数
    int predict(Digit &digit);

    RandomForest(double S,
                 int max_depth, int min_sample_count, int max_categories, int number_of_feature,
                 double regression_accuracy, int max_num_of_trees_in_the_forest);
};

#endif // RF_H_ICLUDE
