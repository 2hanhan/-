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
    RandomForest MyRandomForest(0.6,  //训练随机使用的样本比例
                                25,   //决策树最大深度
                                5,    //分支最小样本数量
                                10,   //最大分类数量
                                4,    //随机选择使用的特征个数
                                0.95, //终止精度
                                1);   //随机森林决策树最大个数
    MyRandomForest.train(optdigits_train.datas, optdigits_test.datas);

    std::cout << "进行预测模型" << std::endl;
    /*
        for (int i = 0; i < 10; ++i)
        {
            optdigits_test.datas[i].show();
            std::cout << "预测结果为：" << MyRandomForest.predict(optdigits_test.datas[i]) << std::endl;
        }
    */
    return 0;
}
