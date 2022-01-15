#include <RandomForest.h>
#include <ReadData.h>

int main(int argc, char **argv)
{
    // step 1 加载数据集
    std::cout << "加载训练集" << std::endl;
    std::string optdigits_train_datas; //训练集文件名
    optdigits_train_datas = "optdigits.tra";
    ReadData optdigits_train(optdigits_train_datas);

    // 显示前10个数据
    for (int i = 0; i < 10; ++i)
    {
        optdigits_train.datas[i].show();
    }

    std::cout << "加载测试集" << std::endl;
    std::string optdigits_test_datas; //测试集文件名
    optdigits_test_datas = "optdigits.tes";
    ReadData optdigits_test(optdigits_test_datas);

    std::cout << "训练模型" << std::endl;
    // step 2 训练随机森林
    RandomForest MyRandomForest(0.8,            //训练随机使用的样本比例
                                25,             //决策树最大深度
                                10,             //分支最小样本数量
                                10,             //最大分类数量
                                16,             //随机选择使用的特征个数
                                0.95,           //终止精度
                                1000);          //随机森林决策树最大个数
    MyRandomForest.train(optdigits_train.datas, //训练数据集
                         optdigits_test.datas); //测试数据集

    // step 3 使用随机森林进行预测
    std::cout << "任意取10个进行预测结果展示" << std::endl;
    std::vector<int> datas_random_index(optdigits_test.datas.size());
    for (int i = 0; i < optdigits_test.datas.size(); ++i)
    {
        datas_random_index[i] = i;
    }
    srand(unsigned(time(0))); //随机种子
    std::random_shuffle(datas_random_index.begin(), datas_random_index.end());
    for (int i = 0; i < 10; ++i)
    {
        optdigits_test.datas[datas_random_index[i]].show();
        std::cout << "预测结果为：" << MyRandomForest.predict(optdigits_test.datas[datas_random_index[i]]) << std::endl
                  << std::endl;
    }

    return 0;
}
