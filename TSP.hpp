//
//  TSP.hpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/6.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

#ifndef TSP_hpp
#define TSP_hpp

#include <stdio.h>
#include "iostream"
#include<algorithm>
#include<cstdio>
#include<queue>
#include<queue>

using namespace std;

const int MAX_N = 11;
const int inifite =999;
static int n_tsp ;//节点个数
static int cost_tsp[MAX_N][MAX_N];  //如果两次使用MAX_N就会报错


typedef struct Node
{
    bool visited[MAX_N];//标记哪些点走了
    int  path [MAX_N]; //标记访问路径 -1为空
    //int s;//第一个点
    //int s_p;//第一个点的邻接点
    //int e;//最后一个点
    //int e_p;//最后一个点的邻接点
    int num;//走过的点数
    int sumv;//经过路径的距离
    int lb;//目标函数的值（目标结果）
    
    bool operator <(const Node &p)const
    {
        return p.lb <= lb;//目标函数值小的先出队列
    }
};
static priority_queue<Node> pq2;//创建一个优先队列
static int low, up;//下界和上界
static bool dfs_visited[MAX_N];//在dfs过程中搜索过
int dfs(int u, int k, int l);
void get_up();
void get_low();
int get_lb(Node p);

int m_min(int a ,int b);

int * calculateOrder(int * rf, int n,vector<int>tmpt_vnfOrder);

#endif /* TSP_hpp */
