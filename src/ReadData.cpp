#include <ReadData.h>

/**
 * @brief 模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
 *
 * @tparam Type 模板类型
 * @param str 待转换的字符串
 * @return Type
 */
template <class Type>
Type stringToNum(const std::string &str)
{
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

/**
 * @brief Construct a new Digit:: Digit object
 * 设置feature大小
 */
Digit::Digit()
{
    feature.resize(64);
}

/**
 * @brief 获取标签
 *
 * @param label
 */
void Digit::get_labe(int &label)
{
    this->label = label;
}

/**
 * @brief 获取特征
 *
 * @param strArray特征的字符串
 */
void Digit::get_feature(std::vector<std::string> &strArray)
{
    for (int i = 0; i < 64; ++i)
    {
        feature[i] = stringToNum<int>(strArray[i]);
    }
}

/**
 * @brief 显示数字
 *
 */
void Digit::show()
{
    //显示标签
    std::cout << "手写数字标签为：" << label
              << "  具体数据为：" << std::endl;
    for (int i = 0; i < 8; ++i)
    {

        for (int j = 0; j < 8; ++j)
        {
            std::cout << "[";
            //显示数据
            if (feature[i * 8 + j] <= 9)
            {
                std::cout << " ";
            }
            std::cout << feature[i * 8 + j] << "]";
        }
        std::cout << "|";
        for (int j = 0; j < 8; ++j)
        {

            //显示数据
            if (feature[i * 8 + j] == 0)
            {
                std::cout << " ";
            }
            else if (feature[i * 8 + j] > 12)
            {
                std::cout << "#";
            }
            else if (feature[i * 8 + j] > 8)
            {
                std::cout << "%";
            }
            else if (feature[i * 8 + j] > 4)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << ".";
            }
        }
        //换行
        std::cout << "|" << std::endl;
    }
}

/**
 * @brief Construct a new Read Data:: Read Data object
 *
 * @param dir数据集路径
 */
ReadData::ReadData(std::string dir)
{
    //加载数据集路径
    num = 0;
    datasdir = dir;
    std::cout << "数据集存储路径为：" << datasdir << std::endl;
    //加载数据集
    read_datas();
};

/**
 * @brief 读取数据集数据
 *
 */
void ReadData::read_datas()
{

    std::cout << "加载数据集..." << std::endl;
    std::ifstream inFile(datasdir, std::ios::in);

    // step 1 读取成字符串类型
    std::string lineStr;
    std::vector<std::vector<std::string>> strArray; //字符串
    while (getline(inFile, lineStr))
    {
        // 存成二维表结构
        std::stringstream ss(lineStr);
        std::string str;
        std::vector<std::string> lineArray;
        // 按照逗号分隔
        while (getline(ss, str, ','))
            lineArray.push_back(str);
        strArray.push_back(lineArray);
        ++num;
    }
    std::cout << "数据集共有" << num
              << "个数据" << std::endl;

    // step 2  将字符串转换为int 传入 feature和label
    datas.resize(num);
    for (int i = 0; i < num; ++i)
    {
        // step 2.1 传入label
        int label;
        label = stringToNum<int>(strArray[i][strArray[i].size() - 1]);
        datas[i].get_labe(label);

        // step 2.2 传入feature
        datas[i].get_feature(strArray[i]);
    }
};
