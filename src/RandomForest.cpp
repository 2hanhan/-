#include <RandomForest.h>

/**
 * @brief Set the node object设置节点参数
 *
 * @param parent
 * @param left
 * @param right
 * @param depth
 * @param sample_count
 * @param split_feature
 * @param split_threshold
 * @param result
 */
void Node::set_node(int parent,          //父节点索引
                    int left,            //子节点索引
                    int right,           //子节点索引
                    int depth,           //节点深度
                    int split_feature,   //分裂使用的特征
                    int split_threshold, //分裂阈值
                    int result,          //该节点的预测结果 没有结果为-1
                    bool same            //是否所以的样本都相同 相同也就不需要继续分裂
)
{
    this->parent = parent;                   //父节点索引
    this->left = left;                       //子节点索引
    this->right = right;                     //子节点索引
    this->depth = depth;                     //节点深度
    this->split_feature = split_feature;     //分裂使用的特征
    this->split_threshold = split_threshold; //分裂阈值
    this->result = result;                   //该节点的预测结果 没有结果为-1
    this->same = same;                       //是否所以的样本都相同 相同也就不需要继续分裂
}

/**
 * @brief 设置分裂参数基尼系数
 *
 * @param split_feature
 * @param split_threshold
 * @param gini
 */
void Node::set_split_param(int &split_feature, int &split_threshold, int &result, bool &same)
{
    this->split_feature = split_feature;
    this->split_threshold = split_threshold;
    this->result = result;
    this->same = same;
}

Node::Node()
{
    // 没有计算前默认参数均设置为-1
    parent = -1; //父节点索引
    left = -1;   //子节点索引
    right = -1;  //子节点索引
    depth = -1;  //节点深度
    result = -1; //该节点的预测结果 没有结果为-1
}

/**
 * @brief 添加节点
 *
 * @param node
 */
void Tree::push_node(Node &node)
{
    nodes.push_back(node);
}

Tree::Tree(/* args */)
{
}

/**
 * @brief 单个决策树预测结果
 *
 * @param digit
 * @return int
 */
int Tree::predict(Digit &digit)
{
    int max_depth = nodes[nodes.size() - 1].depth;

    int node_index = 0;
    // std::cout << 0;
    int split_feature;
    int split_threshold;
    for (int i = 0; i < max_depth; ++i)
    {
        // std::cout << "if:" << (nodes[node_index].same || nodes[node_index].left == -1 || nodes[node_index].right == -1) << std::endl;
        //  step 1 如果所有样本结果相同返回决策树结果
        if (nodes[node_index].same || nodes[node_index].left == -1 || nodes[node_index].right == -1)
        {
            // std::cout << std::endl;
            return nodes[node_index].result;
        }
        split_feature = nodes[node_index].split_feature;
        split_threshold = nodes[node_index].split_threshold;

        // step 2 如果没有结果则继续进行下一个分支的判断
        if (digit.feature[split_feature] < split_threshold)
        {
            node_index = nodes[node_index].left;
            // std::cout << "->" << node_index;
        }
        else
        {
            node_index = nodes[node_index].right;
            // std::cout << "->" << node_index;
        }
    }
    // step 3 到达了最大深度依然没有结果则返回-1
    // std::cout << std::endl;
    return nodes[node_index].result;
}

/**
 * @brief 决策树训练
 *
 * @param datas
 * @return int 1：精度达到终止条件 或者 -1：森林决策树个数达到终止条件
 */
int RandomForest ::train(std::vector<Digit> &datas, std::vector<Digit> &test)
{
    int radom_datas_num = S * datas.size(); //训练使用的样本

    for (; num_of_trees_in_the_forest < max_num_of_trees_in_the_forest; ++num_of_trees_in_the_forest)
    {
        // step 0 判读是否达到终止条件
        if (forest_accuracy >= regression_accuracy)
        {
            return 1;
        }
        if (num_of_trees_in_the_forest >= max_num_of_trees_in_the_forest)
        {
            return -1;
        }

        // step 1 随机选择数据集进行训练
        std::vector<int> datas_random_index(datas.size());
        for (int i = 0; i < datas.size(); ++i)
        {
            datas_random_index[i] = i;
        }
        srand(unsigned(time(0))); //随机种子
        std::random_shuffle(datas_random_index.begin(), datas_random_index.end());
        // 任意获取S个样本
        std::vector<int> root_samples_index;
        for (int i = 0; i < radom_datas_num; ++i)
        {
            root_samples_index.push_back(datas_random_index[i]);
        }

        // step 2 随机选择特征进行训练
        std::vector<int> feature_random_index(64);
        for (int i = 0; i < 64; ++i)
        {
            feature_random_index[i] = i;
        }
        srand(unsigned(time(0))); //随机种子
        std::random_shuffle(feature_random_index.begin(), feature_random_index.end());

        // step 3 训练单个决策树
        Tree one_tree;
        std::vector<std::vector<int>> all_samples_index; //存储所以节点样本的索引 和Tree中的nodes使用一样的索引

        Node the_root_node;                                    //根节点
        the_root_node.set_node(0, -1, -1, 0, 0, 0, -1, false); //设置参数
        one_tree.push_node(the_root_node);                     //将根节点保存到tree
        all_samples_index.push_back(root_samples_index);       //根节点的样本索引

        int new_node_index_i = 0;                       //新增节点的索引
        int max_new_node_index = one_tree.nodes.size(); //新增节点的索引的最大值
        // step 3.1 生成决策树
        for (int depth_i = 0; depth_i < max_depth; ++depth_i)
        {

            // - 训练单个节点
            // 遍历所以的父节点
            for (new_node_index_i; new_node_index_i < max_new_node_index; ++new_node_index_i)
            {
                std::vector<int> samples_index;
                samples_index = all_samples_index[new_node_index_i];
                //计算分裂参数
                Node best_node = set_node_with_gini(datas, samples_index, feature_random_index);
                // std::cout << " best_node ：" << best_node.same << std::endl;

                if ((!best_node.same) && samples_index.size() > min_sample_count) //需要继续分裂
                {
                    // std::cout << "开始分裂节点" << std::endl;
                    //  分裂父节点中的样本
                    std::vector<int> left_samples_index;
                    std::vector<int> right_samples_index;
                    for (int samples_index_i = 0; samples_index_i < samples_index.size(); ++samples_index_i)
                    {
                        if (datas[samples_index[samples_index_i]].feature[best_node.split_feature] < best_node.split_threshold)
                        {
                            left_samples_index.push_back(samples_index[samples_index_i]);
                        }
                        else
                        {
                            right_samples_index.push_back(samples_index[samples_index_i]);
                        }
                    }
                    //添加左节点
                    Node left;
                    left.set_node(new_node_index_i, -1, -1, depth_i + 1, 0, 0, -1, false);
                    one_tree.push_node(left);                        //添加到tree
                    all_samples_index.push_back(left_samples_index); //添加到样本
                    //添加右节点
                    Node right;
                    right.set_node(new_node_index_i, -1, -1, depth_i + 1, 0, 0, -1, false);
                    one_tree.push_node(right);                        //添加到tree
                    all_samples_index.push_back(right_samples_index); //添加到样本

                    //使用best_node更新当前节点的信息
                    one_tree.nodes[new_node_index_i].set_node(best_node.parent, one_tree.nodes.size() - 2, one_tree.nodes.size() - 1, depth_i, best_node.split_feature, best_node.split_threshold, best_node.result, best_node.same);
                }
                else
                {
                    //使用best_node更新当前节点的信息
                    one_tree.nodes[new_node_index_i].set_node(best_node.parent, -1, -1, depth_i, best_node.split_feature, best_node.split_threshold, best_node.result, best_node.same);
                }
                /*
                std::cout << std::endl
                          << new_node_index_i
                          << " 当前节点深度为：" << one_tree.nodes[new_node_index_i].depth
                          << " 分裂特征为：" << one_tree.nodes[new_node_index_i].split_feature
                          << " 分裂阈值为：" << one_tree.nodes[new_node_index_i].split_threshold << std::endl
                          << "  left：" << one_tree.nodes[new_node_index_i].left
                          << "  left：" << one_tree.nodes[new_node_index_i].right
                          << "   same?：" << one_tree.nodes[new_node_index_i].same << std::endl
                          << " 该节点结果为：" << one_tree.nodes[new_node_index_i].result
                          << " 当前节点样本个数为：" << samples_index.size() << std::endl
                          << std::endl;
                          */
            }
            max_new_node_index = one_tree.nodes.size(); //更新新增的节点索引的最大值
        }

        // step 4 将单个决策树放入随机森林中
        Forest.push_back(one_tree);
        // step 5 计算准确度
        int error_num = 0;
        for (int test_i = 0; test_i < test.size(); ++test_i)
        {
            if (predict(test[test_i]) != test[test_i].label)
            {
                ++error_num;
            }
        }
        forest_accuracy = 1.0 - double(error_num) / double(test.size());
        std::cout << "当前随机森林中决策树个数为：" << num_of_trees_in_the_forest + 1 << " 随机森林在测试集上的准确度为" << forest_accuracy * 100 << "%" << std::endl;

        if (forest_accuracy >= regression_accuracy)
        {
            break;
        }
    }
}

/**
 * @brief   设置单个节点 CART方法
 * @param datas 数据集
 * @param datas_random_index 当前节点的样本的索引
 * @param feature_random_index 随机选取的特征的索引
 * @return Node
 */
Node RandomForest::set_node_with_gini(std::vector<Digit> &datas, std::vector<int> &samples_index, std::vector<int> &feature_random_index)
{
    Node best_node;
    int best_feature = feature_random_index[0];
    int best_threshold = 0;
    int result = -1;   //分裂结果
    bool same = false; //节点是否都是一种样本
    double best_gini = std::numeric_limits<double>::max();
    double gini_0 = 1.0;
    // step 1  统计样本中各个类别个数
    int v[max_categories] = {0}; //总样本分类统计
    for (int samples_i = 0; samples_i < samples_index.size(); ++samples_i)
    {
        // std::cout << " [datas index:" << samples_index[samples_i] << " label:" << datas[samples_index[samples_i]].label << "]";
        ++v[datas[samples_index[samples_i]].label];
    }
    // std::cout << "samples_index.size()：" << samples_index.size() << std::endl;
    int max_samples = v[0]; //统计最多的样本个数
    for (int i = 0; i < max_categories; ++i)
    {
        // std::cout << "{" << i << ": " << v[i] << "}  ";
        if (v[i] == samples_index.size())
        {
            result = i;
            same = true;
            best_node.set_split_param(best_feature, best_threshold, result, same);
            return best_node; //如果全部样本为同一个类别则结果为改类别
        }
        if (v[i] >= max_samples)
        {
            max_samples = v[i];
            result = i;
        }
        double Pk = double(v[i]) / double(samples_index.size());
        gini_0 = gini_0 - Pk * Pk;
    }

    for (int feature_i = 0; feature_i < number_of_feature; ++feature_i)
    {
        // std::cout << " [feature index:" << feature_random_index[feature_i] << "]";
        //  step 2 计算最佳特征的分裂属性
        for (int threshold_i = 0; threshold_i < 17; ++threshold_i)
        {
            // step 3 计算最佳分裂阈值
            //遍历每个样本计算gini系数
            double gini = 0;
            double gini_left = 0;
            double gini_right = 0;
            int left_num = 0;                  //左边子节点样本个数
            int right_num = 0;                 //右边子节点样本个数
            int v_left[max_categories] = {0};  //左边子节点分类统计
            int v_right[max_categories] = {0}; //右边子节点分类统计
            for (int samples_i = 0; samples_i < samples_index.size(); ++samples_i)
            {
                if (datas[samples_index[samples_i]].feature[feature_random_index[feature_i]] < threshold_i)
                {
                    ++v_left[datas[samples_index[samples_i]].label];
                    ++left_num;
                }
                else
                {
                    ++v_right[datas[samples_index[samples_i]].label];
                    ++right_num;
                }
            }
            // ! 注意阈值可能会出现分母为0的情况
            if (left_num == samples_index.size() | right_num == samples_index.size())
            {
                gini = gini_0;
            }
            else
            {
                for (int i = 0; i < 10; ++i)
                {
                    double Pk_left = double(v_left[i]) / double(left_num);
                    double Pk_right = double(v_right[i]) / double(right_num);
                    gini_left = gini_left + Pk_left * Pk_left;
                    gini_right = gini_right + Pk_right * Pk_right;
                }
                gini = double(left_num) / double(samples_index.size()) * (1.0 - gini_left) + double(right_num) / double(samples_index.size()) * (1.0 - gini_right);
            }

            // step 4 比较gini保存最佳的分裂阈值
            if (best_gini >= gini)
            {
                best_gini = gini;
                best_feature = feature_random_index[feature_i];
                best_threshold = threshold_i;
                best_node.set_split_param(best_feature, best_threshold, result, same);
            }
        }
    }
    return best_node;
}

/**
 * @brief 使用随机森林进行预测
 *
 * @param digit
 * @return int
 */
int RandomForest::predict(Digit &digit)
{
    //投票统计
    int votes[10] = {0};
    int result = -1;
    int max_nvotes = 0; //表示最多投票数
    // step 1 遍历决策树进行投票
    for (int i = 0; i < Forest.size(); ++i)
    {
        // step 1.1 使用获取单个决策树的结果
        int one_result = Forest[i].predict(digit);

        // step 1.2 统计单个决策树的结果
        int nvotes; //表示得票数
        nvotes = ++votes[one_result];
        // step 1.3 比较获取当前票数做多的结果
        if (nvotes > max_nvotes) //更新最多投票数
        {
            max_nvotes = nvotes; //最多投票数
            result = one_result; //最多投票数的分类为最终预测结果
        }
    }
    // step 2 返回最终投票结果
    return result;
}

/**
 * @brief 随机森林构造函数
 * 设置相关参数
 * @param S 训练随机使用的样本比例
 * @param max_depth 最大深度
 * @param min_sample_count 分支最小样本数量
 * @param max_categories 最大分类数量
 * @param number_of_feature 随机选择使用的特征个数
 * @param regression_accuracy 终止精度
 * @param max_num_of_trees_in_the_forest//随机森林决策树最大个数
 * @return RandomForest
 */
RandomForest ::RandomForest(double S = 0.6,
                            int max_depth = 25,
                            int min_sample_count = 5,
                            int max_categories = 10,
                            int number_of_feature = 4,
                            double regression_accuracy = 0.99,
                            int max_num_of_trees_in_the_forest = 100)
    : S(S),
      max_depth(max_depth), min_sample_count(min_sample_count), max_categories(max_categories), number_of_feature(number_of_feature),
      regression_accuracy(regression_accuracy), max_num_of_trees_in_the_forest(max_num_of_trees_in_the_forest)
{
    forest_accuracy = 0;
    num_of_trees_in_the_forest = 0;
}
