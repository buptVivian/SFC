//
//  simulation.cpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/6.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//  k 是重要参数，不要使用k 作为循环变量

#include "simulation.hpp"
#include "set"
//extern parameter
int pod_slot[k][k][WaveNumber];
int MIS[k][k];
double m_currentTime;
int m_nextServiceId;
int m_sumOfFailedService;
int sumOfusedSlot;
int sumOfusedPods;
priority_queue<Event> m_pq;
int rf[M];
int rp[k]={50,50,50,50};
//

void initialize(const double g_lamda)
{
    m_lamda = g_lamda;	//业务到达率
    m_rou = g_rou;
    m_currentTime = 0;
    
    for(int i = 0; i<k ;i++ )   //初始化
        for(int j=0 ; j<k ;j++)
            MIS[i][j]=0;
    
    for (int i = 0; i < k; i++){         //给拓扑中所有相连路径分配全波长
        for (int j = 0; j <k; j++){
            for (int m = 0; m < WaveNumber; m++){
                pod_slot[i][j][m] = true ;		//所有pod的端口的所有波长均可用
            }
        }
    }
    //初始vnf计算资源消耗,初始rf{1,10}
    srand((unsigned)time(0));
    for (int i = 0;i<M;i++)
    {  rf[i]=rand()%10+1;
        
    }
    
}

//仿真处理函数
void run(ofstream& ofile)
{
    m_sumOfFailedService = 0;
    sumOfusedPods = 0;
    sumOfusedSlot = 0;
    
    //生成第一个业务到达事件(id = 0)
    m_nextServiceId = 0;
    generateServiceEventPair(m_nextServiceId);
    m_nextServiceId++;
    
    while (!m_pq.empty())
    {
        Event event=m_pq.top();
         m_pq.pop();
        dealWithEvent(event);		//按照优先因子处理优先级队列事件
    }
    
    ofile << (double)m_sumOfFailedService ;
    ofile << '\n';
    
    cout << "the failure rate is:" << '\t' << (double)m_sumOfFailedService / ServiceQuantity << endl;
    cout << endl;
}

//生成业务到达事件
void generateServiceEventPair(const int id)
{
    Event event0;  //结构体定义放在了头文件
    event0.m_eventType = Arrival;
    event0.m_id = id;
    event0.m_time = m_currentTime + arrive_time_gen(m_lamda);  // = t0+ m_λ*log（1/p)
    event0.m_holdTime = arrive_time_hol(m_rou);
    
    Pair node = randomSrcDst();  // 获取源节点和目的节点
    event0.m_sourceNode = node.first+1000;
    event0.m_destNode = node.second+1000;
    
    event0.c_M = randvnf();  //获取vnf类型
    
    m_pq.push(event0);
}

//随机生成源宿节点(暂时为不同)
Pair randomSrcDst()
{
    Pair SrcDst;
    set<int>* tmp = new set<int>;
    while(tmp->size() < 2){
        tmp->insert(rand() % k );
    }
    set<int>::iterator itr;
    itr=tmp->begin();
    SrcDst.first= *itr;
    itr++;
    SrcDst.second=*itr;
    return SrcDst;
}

//返回c_M确定使用的vnf
int* randvnf(){
    //srand((unsigned)time(NULL));
    int numberOfVnf=genrandom(2,M);//该业务包含的vnf个数
    int* vnf = new int[M];
    for (int j=0;j<M;j++){
        vnf[j]=-1;
    }
    //随机生成vnf类型,{0,M-1}
    set<int>* tmp = new set<int>;
    while(tmp->size() < numberOfVnf){
        tmp->insert(rand() % M );
    }
    
    set<int>::iterator itr;
    int* tmpt= new int[numberOfVnf];
    
    int i = 0;
    for(itr=tmp->begin();itr !=tmp->end();itr++)
    {
        tmpt[i++] = *itr;
        
    }
       
    for (int i = 0;i<numberOfVnf;i++){
        vnf[tmpt[i]]=1;
    }
    delete[] tmpt;
    return vnf;
}

//生成业务离去事件
Event generateLeavingevent(const Event& event)
{
    Event event1 = event;
    event1.m_eventType = End;
    event1.m_time = event.m_time + event.m_holdTime;	//"业务离去事件的发生时间" = "业务到达事件发生的时间" + "业务持续时间"
    
    return event1;
}

//泊松分布
double arrive_time_gen(const double beta)
{
    double u, x, ln;
    u = rand() * 1.0 / RAND_MAX;
    ln = log(u);
    
    x = beta * ln;
    x = -1 * x;
    return x;
}
double arrive_time_hol(const double beta)//数据中心流量（持续时间）
{
    double u, x, ln;
    u = rand() * 1.0 / RAND_MAX;
    ln = log(u);
    x = beta * ln;
    x= -1 * x;
    if(u<=0.1)   //到达时间和持续时间是正向关系。即到达时间越短持续时间越短。
    {
        
        x=x*2.81;
        return x;
    }
    else
    {
        x=x*0.15;
        return x;
        
    }
    
}


//在[ia,ib]之间产生一个整形随机数
int genrandom( int ia,  int ib)
{   //srand((unsigned)time(NULL));
    int fr = rand()%(ib-ia+1) + ia;
    return fr;
}

