// GA-Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<cmath>
#include <ctime>//让时间生成随机数
#include<fstream>
#define GENE_RANGE 32768
#define GENE_RANGE_HALF 16384
const double EPS = 1e-10;
using namespace std;

struct entity
{
    unsigned  int x;
    unsigned  int y;
    double fitness=0;//适应度
    double pro = 0;//概率
};
double maxNumber = DBL_MIN;  //储存函数的全局最大值
vector<entity> group;//群体

bool create_population(int number);
bool calculate_fitness();
bool bet();
entity cross(entity father, entity mother);
entity mutate(entity son);


int main()
{
    srand(time(NULL)); //根据时间设置随机数种子
    //创建一个种群,只使用gene的15位,2的15次方是32768，  32768/2=16384
    create_population(50);//创建50个个体
    //for循环,循环100次
    vector<double> y;
    for (int i = 0; i < 100; i++)
    {
     printf("第%d轮迭代\n", i+1);
    //解码并评价函数，对每个体进行评价，返回群体的set
     calculate_fitness();
    //赌盘淘汰,输入,交叉，变异
     bet();

     //将maxNumber作为适应度加入到数组vector容器之中，方便后面利用python，绘制适应度曲线图
     y.push_back(maxNumber);
    }
    ofstream outfile("./out.txt");
    for (auto i : y)
    {
        outfile << i<<endl;
    }
    outfile.close();


    printf("最大值是%f\n", maxNumber);
    char x;
    cin >> x;
    return 0;

}

/*
     创建一个种群
     int number 输入个体数目
     这个种群应该是两串
      只使用gene的15位,2的15次方是32768，
*/
bool create_population(int number)
{
    for (int i = 0; i < number; i++)
    {
        unsigned int x = rand() % GENE_RANGE + 1;
        unsigned int y = rand() % GENE_RANGE + 1;
        printf("%d,%d\n", x, y);
        entity one={x,y};
        group.push_back(one);
    }
    printf("创建种群成功\n");
    return true;
}

bool calculate_fitness()
{
    int i = 0;
    for (vector<entity>::iterator vi=group.begin();vi!=group.end();vi++)
    {
        
        double x =(double((*vi).x) - GENE_RANGE_HALF) / GENE_RANGE * 200;
        double y =(double((*vi).y) - GENE_RANGE_HALF)/GENE_RANGE*200;
        printf("坐标%d+%d***%f+%f\n", (*vi).x, (*vi).y, x, y);
        double c = pow(x, 2) + pow(y, 2);
        double z = 0.5 - (pow(sin(sqrt(c)),2) - 0.5) / pow((1 + 0.001 * c),2);
       
        (*vi).fitness = z;
        i++;
        printf("第%d适应度是%f\n",i,z);
    }
    //计算
    printf("-----------------------------\n");
    printf("函数最大值为%f\n",maxNumber);
    printf("-----------------------------\n");
    return true;
}
/*
     选择算子，
     轮盘赌博
*/
bool bet()
{   //计算出适应度的总和,
    double all=0;
    //找到最小的那个适应度和最大的那个适应度
    double min = (*group.begin()).fitness;
    for (auto element : group)
    {
        if (min > element.fitness+EPS)
        {
            min = element.fitness;
        }

        if (maxNumber+EPS < element.fitness)
        {
            maxNumber = element.fitness;
            printf("\n-------------\n");
            printf("%f", maxNumber);
            printf("\n-------------\n");
        }
    }

    //计算总适应度
    for (auto element : group)
    {
        all += element.fitness-min; //减去最小的，可以保证最后算出来的总数是个正值
    }
    //计算适应度的概率
    for (vector<entity>::iterator vi = group.begin(); vi != group.end(); vi++)
    {
        (*vi).pro = ((*vi).fitness - min) / all;
    }
    //把前一个的概率加到后面那个去
    vector<entity>::iterator i;
    for (i=group.begin()+1;i!=group.end();i++)
    {
        (*i).pro += (*(i - 1)).pro;
    }
    
    //直接开始选择25对，适应度高的被选中的概率就高
    int number = group.size();
    vector<entity> new_entity;
    for (int i = 0; i < number; i++)
    {
        //选择两个出来交叉
        
        double r1 = rand()% (100 + 1) / (float)(100 + 1);
       
        double r2 = rand() % (100 + 1) / (float)(100 + 1);

        entity father;
        entity mother;

        //获取一个父亲
        for (auto x : group)
        {
            if (x.pro > r1+EPS)
            {
                father = x;
                break;
            }
        }
        //获取一个母亲
        for (auto x : group)
        {
            if (x.pro > r2+EPS)
            {
                mother = x;
                break;
            }
        }
        //交叉生成子代
        entity son=cross(father, mother);
        //子代变异
        
        entity son2=mutate(son);
        //子代加入新的vector
        new_entity.push_back(son2);
    }
    //得到新一代,新的一代替换老的一代
    group = new_entity;
    return true;
}
/*
     交叉算子
*/
entity cross(entity father,entity mother)
{
    
    entity son;
    //交换位点
    int index = rand() % 15+1;
    unsigned int temp = 1;
    while (--index)
    {
        temp=temp*2+1;
    }
    //father的前index位加上 mother的后面
    son.x = ((father.x) & temp) + ((mother.x )& (~temp));
    son.y = ((father.y) & temp) + ((mother.y )& (~temp));
    //去掉15位以上的
    unsigned  int temp15 = 1;
    int i = 15;
    while (--i)
    {
        temp15 = temp15 * 2 + 1;
    }
    son.x = (son.x) & temp15;
    son.y = (son.y)& temp15;
    return son;
}
/*
     变异算子
*/
entity mutate(entity son)
{
    //变异一个位点
    
    int index = rand() % 15+1;
   // printf("%d\n", index);
    unsigned int temp = 1;
    while (--index)
    {
        temp = temp * 2 ;
    }
    /*
    printf("-------------\n");
    printf("temp值%u\n", temp);
    printf("son1值%u\n", son.x);
    printf("~temp值%u\n", ~temp);
    printf("son.x & (~temp)值%d\n", ((son.x) & (~temp)));
    */
    if (son.x & temp)
    {
        son.x &= ~temp;
    }else {
        son.x |= temp;
    }
    //son.x = (son.x) & (~temp);
    if (son.y & temp)
    {
        son.y &= ~temp;
    }
    else {
        son.y |= temp;
    }
    //son.y = (son.y) & (~temp);
    /*printf("son2值%u\n", son.x);
    printf("-------------\n");
    */
    //去掉15位以上的
    unsigned  int temp15 = 1;
    int i = 15;
    while (--i)
    {
        temp15 = temp15 * 2 + 1;
    }
   // printf("%d\n", temp15);
    son.x = (son.x) & temp15;
    son.y = (son.y) & temp15;

    return son;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
