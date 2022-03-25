> 2022/01/15 更新

> 参考可以把star留下QAQ 2022/3/25更新了文档

> 等一下，如果老师批作业搜索一下，不会给我判定一个我抄我自己吧QAQ
> 问题不大成绩出87还行吧，毕竟试卷部分错了挺多的了
# 手写随机森林算法
- 使用c++实现
- 仅使用STL库
## 数据集 [网址](https://archive.ics.uci.edu/ml/datasets/Optical+Recognition+of+Handwritten+Digits)
- 实现的手写数字识别
## 算法原理
### 随机森林
 - 使用随机的样本训练
 - 随机选择特征进行训练
 - 使用多个决策树投票的结果作为最终结果
### 使用CART算法生成单个决策树组成随机森林
- 二叉树
- 无剪枝（可以有但是这里我没写）
- 使用gini系数作为分裂依据
#### gini系数 
gini = P_left*(1-SUM(P_left_k* P_left_k))+P_right*(1-SUM(P_right_k* P_right_k))
## 文档说明
其实吧C++的程序直接看`.h`文件不就好了吗
### 读取数据集
`ReadData.h`
### 训练随机森林
`RandomForest.h`
## 举个栗子
### 下载编译
```bash
git clone https://github.com/2hanhan/Random-Forest.git
cd Random-Forest
sudo chmod +x build.sh
./build.sh
```
### 运行
```bash
./main
```
### 训练参数设置
其实main函数里面都有的
```c++
    RandomForest MyRandomForest(0.8,  //训练随机使用的样本比例
                                25,   //决策树最大深度
                                10,   //分支最小样本数量
                                10,   //最大分类数量
                                16,   //随机选择使用的特征个数
                                0.95, //终止精度
                                1000); //随机森林决策树最大个数
```
### 说明文档
既然是课程作业那就把作业的[pdf](说明文档.pdf)也放上了
