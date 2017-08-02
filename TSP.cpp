//
//  TSP.cpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/6.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//  TSP.cpp
//  test
//
//  Created by vivian_bytedance on 2017/7/10.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

#include "TSP.hpp"

int dfs(int u, int num, int l)//当前节点，目标节点，已经消耗的路径
{
    if (num == n_tsp) return l + cost_tsp[u][1];//如果已经检查了n个节点，则直接返回路径消耗+第n个节点回归起点的消耗
    int minlen = inifite, b=0;
    for(int i =1;i<=n_tsp;i++)
    {
        if(!dfs_visited[i] && minlen > cost_tsp[u][i]){
            minlen = cost_tsp[u][i];
            b = i;
        }
    }
    dfs_visited[b] = true;//以p为下一个节点继续搜索
    return dfs(b, num + 1, l + minlen);
}
void get_up()
{
    for(int i=0;i<MAX_N;i++){  //初始化dfs_visited
        dfs_visited[i] = false;
    }
    
    dfs_visited[1] = true;//以第一个点作为起点
    up = dfs(1, 1, 0);
}
//用这种简单粗暴的方法获取必定小于结果的一个值
void get_low()
{
    //取每行最小值之和作为下界
    low = 0;
    for (int i = 1; i <= n_tsp; i++)
    {
        //创建一个等同于map的临时数组，可用memcpy
        int tmpA[MAX_N];
        //tmpA[0]=inifite;
        for (int j = 1; j <= n_tsp; j++)
        {
            tmpA[j] = cost_tsp[i][j];
        }
        sort(tmpA + 1, tmpA + 1 + n_tsp);//对临时的数组进行排序
        low += tmpA[1];
    }
}
int get_lb(Node p)
{
    int ret = p.sumv * 2;//路径上的点的距离的二倍
    int min1 = inifite, min2 = inifite;//起点和终点连出来的边
    for (int i = 1; i <= n_tsp; i++)
    {
        //cout << p.visited[i] << endl;
        if (!p.visited[i] && min1 > cost_tsp[i][p.path[1]])
        {
            min1 = cost_tsp[i][p.path[1]];
        }
        //cout << min1 << endl;
    }
    ret += min1;
    for (int i = 1; i <= n_tsp; i++)
    {
        if (!p.visited[i] && min2 > cost_tsp[p.path[p.num]][i])
        {
            min2 = cost_tsp[p.path[p.num]][i];
        }
        //cout << min2 << endl;
    }
    ret += min2;
    for (int i = 1; i <= n_tsp; i++)
    {
        if (!p.visited[i])
        {
            min1 = min2 = inifite;
            for (int j = 1; j <= n_tsp; j++)
            {
                if (min1 > cost_tsp[i][j])
                    min1 = cost_tsp[i][j];
            }
            for (int j = 1; j <= n_tsp; j++)
            {
                if (min2 > cost_tsp[j][i])
                    min2 = cost_tsp[j][i];
            }
            ret += min1 + min2;
        }
    }
    return (ret + 1) / 2;
}
int calculateOrder(const int * rf, int n,int * vnf)
{
    pq2 = priority_queue<Node>();
    
    n_tsp = n;
    //不使用第0行和第0列
    //设置费用矩阵
    for (int i=0;i<n_tsp+1;i++)
    {
        for(int j=0;j<n_tsp+1;j++){
            if(i == j || i==0 || j==0 ) cost_tsp[i][j]=inifite-1;  //不使用cost中的0作为行标和列标
            else if(i == 1) cost_tsp[i][j] = rf[vnf[j-2]];   //从s出发的的费用设置为2
            else if (j == 1) cost_tsp[i][j] = rf[vnf[i-2]]; //从vnf回到s的费用等同于回到d的费用等同于vnf的计算资源需求
            else cost_tsp[i][j] = m_min(rf[vnf[i-2]],rf[vnf[j-2]]);
        }
    }
    
    //贪心法确定上界
    get_up();
    //取每行最小的边之和作为下界
    //cout << up << endl;//test
    get_low();
    //设置初始点,默认从1开始
    Node star;
    for(int i=0;i<MAX_N;i++)
        star.path[i]=-1;
    
    star.path[1] = 1;//起点为1
    star.num = 1;//走过了1个点
    star.path[star.num] = 1; //路径终点为1；
    
    for (int i = 1; i <=n_tsp; i++)
    {   star.visited[i] = false;
    }
    star.visited[1] = true;
    star.sumv = 0;//经过的路径距离初始化
    star.lb = low;//让目标值先等于下界
    int ret = inifite;//ret为问题的解
    pq2.push(star);//将起点加入队列
    while (pq2.size())
    {
        
        Node tmp = pq2.top();pq2.pop();
        //tmp.path[tmp.num]=tmp.path[n_tsp];
        if (tmp.num == n_tsp - 1)//如果已经走过了n-1个点
        {
            //找最后一个没有走的点
            int p=0;
            for (int i = 1; i <= n_tsp; i++)
            {
                if (!tmp.visited[i])
                {
                    p = i;//让没有走的那个点为最后点能走的点
                    break;
                }
            }
            int ans = tmp.sumv + cost_tsp[p][tmp.path[1]] + cost_tsp[tmp.path[tmp.num]][p];
            //已消耗+回到开始消耗+走到P的消耗
            //如果当前的路径和比所有的目标函数值都小则跳出
            if (ans <= tmp.lb)
            {
                ret = min(ans, ret);
                tmp.num=+1;
                tmp.path[n_tsp]=p; //更新最后走的节点
                for(int i=2;i<= n_tsp;i++)  //采用赋值的方式可以保留局部函数的值
                 vnf[i-2]= tmp.path[i];
                 return 1;
            
            }
            //否则继续求其他可能的路径和，并更新上界
            else
            {
                up = min(up, ans);//上界更新为更接近目标的ans值
                ret = min(ret, ans);
                continue;
            }
        }
        
        //当前点可以向下扩展的点入优先级队列,将下一级的节点入队列
        Node next;
        for (int i = 1; i <= n_tsp; i++)
        {
            if (!tmp.visited[i])
            {
                //cout << "test" << endl;
                next.path[1]=tmp.path[1];
                next.num = tmp.num + 1;//更新走过的顶点数
                next.sumv = tmp.sumv + cost_tsp[tmp.path[tmp.num]][i];//更新路径和
                
                for (int j = 1; j <= n_tsp; j++) next.visited[j] = tmp.visited[j];//tmp经过的点也是next经过的点
                for (int j = 1; j <= n_tsp; j++) next.path[j] = tmp.path[j]; //更新next 的路径
                next.path[next.num] = i;//更新最后一个点
                next.visited[i] = true;//自然也要更新当前点
                
                next.lb = get_lb(next);//求目标函数
                if (next.lb > up) continue;//如果大于上界就不加入队列
                pq2.push(next);//否则加入
                //cout<<next.e<<" ";
                //cout << "test" << endl;
            }
        }
    }
    // 求解最优顺序失败 re[1]=-1;
    return 0;
}

int m_min(int a ,int b){
    if (a<b) return a;
    else return b;
}






