// GA-Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<cmath>
#include <ctime>//��ʱ�����������
#include<fstream>
#define GENE_RANGE 32768
#define GENE_RANGE_HALF 16384
const double EPS = 1e-10;
using namespace std;

struct entity
{
    unsigned  int x;
    unsigned  int y;
    double fitness=0;//��Ӧ��
    double pro = 0;//����
};
double maxNumber = DBL_MIN;  //���溯����ȫ�����ֵ
vector<entity> group;//Ⱥ��

bool create_population(int number);
bool calculate_fitness();
bool bet();
entity cross(entity father, entity mother);
entity mutate(entity son);


int main()
{
    srand(time(NULL)); //����ʱ���������������
    //����һ����Ⱥ,ֻʹ��gene��15λ,2��15�η���32768��  32768/2=16384
    create_population(50);//����50������
    //forѭ��,ѭ��100��
    vector<double> y;
    for (int i = 0; i < 100; i++)
    {
     printf("��%d�ֵ���\n", i+1);
    //���벢���ۺ�������ÿ����������ۣ�����Ⱥ���set
     calculate_fitness();
    //������̭,����,���棬����
     bet();

     //��maxNumber��Ϊ��Ӧ�ȼ��뵽����vector����֮�У������������python��������Ӧ������ͼ
     y.push_back(maxNumber);
    }
    ofstream outfile("./out.txt");
    for (auto i : y)
    {
        outfile << i<<endl;
    }
    outfile.close();


    printf("���ֵ��%f\n", maxNumber);
    char x;
    cin >> x;
    return 0;

}

/*
     ����һ����Ⱥ
     int number ���������Ŀ
     �����ȺӦ��������
      ֻʹ��gene��15λ,2��15�η���32768��
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
    printf("������Ⱥ�ɹ�\n");
    return true;
}

bool calculate_fitness()
{
    int i = 0;
    for (vector<entity>::iterator vi=group.begin();vi!=group.end();vi++)
    {
        
        double x =(double((*vi).x) - GENE_RANGE_HALF) / GENE_RANGE * 200;
        double y =(double((*vi).y) - GENE_RANGE_HALF)/GENE_RANGE*200;
        printf("����%d+%d***%f+%f\n", (*vi).x, (*vi).y, x, y);
        double c = pow(x, 2) + pow(y, 2);
        double z = 0.5 - (pow(sin(sqrt(c)),2) - 0.5) / pow((1 + 0.001 * c),2);
       
        (*vi).fitness = z;
        i++;
        printf("��%d��Ӧ����%f\n",i,z);
    }
    //����
    printf("-----------------------------\n");
    printf("�������ֵΪ%f\n",maxNumber);
    printf("-----------------------------\n");
    return true;
}
/*
     ѡ�����ӣ�
     ���̶Ĳ�
*/
bool bet()
{   //�������Ӧ�ȵ��ܺ�,
    double all=0;
    //�ҵ���С���Ǹ���Ӧ�Ⱥ������Ǹ���Ӧ��
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

    //��������Ӧ��
    for (auto element : group)
    {
        all += element.fitness-min; //��ȥ��С�ģ����Ա�֤���������������Ǹ���ֵ
    }
    //������Ӧ�ȵĸ���
    for (vector<entity>::iterator vi = group.begin(); vi != group.end(); vi++)
    {
        (*vi).pro = ((*vi).fitness - min) / all;
    }
    //��ǰһ���ĸ��ʼӵ������Ǹ�ȥ
    vector<entity>::iterator i;
    for (i=group.begin()+1;i!=group.end();i++)
    {
        (*i).pro += (*(i - 1)).pro;
    }
    
    //ֱ�ӿ�ʼѡ��25�ԣ���Ӧ�ȸߵı�ѡ�еĸ��ʾ͸�
    int number = group.size();
    vector<entity> new_entity;
    for (int i = 0; i < number; i++)
    {
        //ѡ��������������
        
        double r1 = rand()% (100 + 1) / (float)(100 + 1);
       
        double r2 = rand() % (100 + 1) / (float)(100 + 1);

        entity father;
        entity mother;

        //��ȡһ������
        for (auto x : group)
        {
            if (x.pro > r1+EPS)
            {
                father = x;
                break;
            }
        }
        //��ȡһ��ĸ��
        for (auto x : group)
        {
            if (x.pro > r2+EPS)
            {
                mother = x;
                break;
            }
        }
        //���������Ӵ�
        entity son=cross(father, mother);
        //�Ӵ�����
        
        entity son2=mutate(son);
        //�Ӵ������µ�vector
        new_entity.push_back(son2);
    }
    //�õ���һ��,�µ�һ���滻�ϵ�һ��
    group = new_entity;
    return true;
}
/*
     ��������
*/
entity cross(entity father,entity mother)
{
    
    entity son;
    //����λ��
    int index = rand() % 15+1;
    unsigned int temp = 1;
    while (--index)
    {
        temp=temp*2+1;
    }
    //father��ǰindexλ���� mother�ĺ���
    son.x = ((father.x) & temp) + ((mother.x )& (~temp));
    son.y = ((father.y) & temp) + ((mother.y )& (~temp));
    //ȥ��15λ���ϵ�
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
     ��������
*/
entity mutate(entity son)
{
    //����һ��λ��
    
    int index = rand() % 15+1;
   // printf("%d\n", index);
    unsigned int temp = 1;
    while (--index)
    {
        temp = temp * 2 ;
    }
    /*
    printf("-------------\n");
    printf("tempֵ%u\n", temp);
    printf("son1ֵ%u\n", son.x);
    printf("~tempֵ%u\n", ~temp);
    printf("son.x & (~temp)ֵ%d\n", ((son.x) & (~temp)));
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
    /*printf("son2ֵ%u\n", son.x);
    printf("-------------\n");
    */
    //ȥ��15λ���ϵ�
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
