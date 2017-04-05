//
//  simulation.cpp
//  simulationFoSFC
//
//  Created by Vivian1010 on 17/4/1.
//  Copyright © 2017年 Vivian1010. All rights reserved.
//




#include "simulation.hpp"
#include "set"

//读取拓扑文件
void readUSAEdgeMap()
{
    
    g_edge[0][0]=0;g_edge[0][1]=1;g_edge[0][2]=1;g_edge[0][3]=1;g_edge[0][4]=999;g_edge[0][5]=999;g_edge[0][6]=999;g_edge[0][7]=999;
    g_edge[0][8]=999;g_edge[0][9]=999;g_edge[0][10]=999;g_edge[0][11]=999;g_edge[0][12]=999;g_edge[0][13]=999;
    
    g_edge[1][0]=1;g_edge[1][1]=0;g_edge[1][2]=1;g_edge[1][3]=999;g_edge[1][4]=999;g_edge[1][5]=999;g_edge[1][6]=999;g_edge[1][7]=1;
    g_edge[1][8]=999;g_edge[1][9]=999;g_edge[1][10]=999;g_edge[1][11]=999;g_edge[1][12]=999;g_edge[1][13]=999;
    
    g_edge[2][0]=1;g_edge[2][1]=1;g_edge[2][2]=0;g_edge[2][3]=999;g_edge[2][4]=999;g_edge[2][5]=1;g_edge[2][6]=999;g_edge[2][7]=999;
    g_edge[2][8]=999;g_edge[2][9]=999;g_edge[2][10]=999;g_edge[2][11]=999;g_edge[2][12]=999;g_edge[2][13]=999;
    
    g_edge[3][0]=1;g_edge[3][1]=999;g_edge[3][2]=999;g_edge[3][3]=0;g_edge[3][4]=1;g_edge[3][5]=999;g_edge[3][6]=1;g_edge[3][7]=999;
    g_edge[3][8]=999;g_edge[3][9]=999;g_edge[3][10]=999;g_edge[3][11]=999;g_edge[3][12]=999;g_edge[3][13]=999;
    
    g_edge[4][0]=999;g_edge[4][1]=999;g_edge[4][2]=999;g_edge[4][3]=1;g_edge[4][4]=0;g_edge[4][5]=1;g_edge[4][6]=999;g_edge[4][7]=999;
    g_edge[4][8]=1;g_edge[4][9]=999;g_edge[4][10]=999;g_edge[4][11]=999;g_edge[4][12]=999;g_edge[4][13]=999;
    
    g_edge[5][0]=999;g_edge[5][1]=999;g_edge[5][2]=1;g_edge[5][3]=999;g_edge[5][4]=1;g_edge[5][5]=0;g_edge[5][6]=999;g_edge[5][7]=999;
    g_edge[5][8]=999;g_edge[5][9]=1;g_edge[5][10]=999;g_edge[5][11]=1;g_edge[5][12]=999;g_edge[5][13]=999;
    
    g_edge[6][0]=999;g_edge[6][1]=999;g_edge[6][2]=999;g_edge[6][3]=1;g_edge[6][4]=999;g_edge[6][5]=999;g_edge[6][6]=0;g_edge[6][7]=999;
    g_edge[6][8]=999;g_edge[6][9]=999;g_edge[6][10]=999;g_edge[6][11]=999;g_edge[6][12]=1;g_edge[6][13]=1;
    
    g_edge[7][0]=999;g_edge[7][1]=1;g_edge[7][2]=999;g_edge[7][3]=999;g_edge[7][4]=999;g_edge[7][5]=999;g_edge[7][6]=999;g_edge[7][7]=0;
    g_edge[7][8]=1;g_edge[7][9]=999;g_edge[7][10]=1;g_edge[7][11]=999;g_edge[7][12]=999;g_edge[7][13]=999;
    
    g_edge[8][0]=999;g_edge[8][1]=999;g_edge[8][2]=999;g_edge[8][3]=999;g_edge[8][4]=1;g_edge[8][5]=999;g_edge[8][6]=999;g_edge[8][7]=1;
    g_edge[8][8]=0;g_edge[8][9]=999;g_edge[8][10]=999;g_edge[8][11]=999;g_edge[8][12]=999;g_edge[8][13]=999;
    
    g_edge[9][0]=999;g_edge[9][1]=999;g_edge[9][2]=999;g_edge[9][3]=999;g_edge[9][4]=999;g_edge[9][5]=1;g_edge[9][6]=999;g_edge[9][7]=999;
    g_edge[9][8]=999;g_edge[9][9]=0;g_edge[9][10]=1;g_edge[9][11]=999;g_edge[9][12]=999;g_edge[9][13]=999;
    
    g_edge[10][0]=999;g_edge[10][1]=999;g_edge[10][2]=999;g_edge[10][3]=999;g_edge[10][4]=999;g_edge[10][5]=999;g_edge[10][6]=999;g_edge[10][7]=1;
    g_edge[10][8]=999;g_edge[10][9]=1;g_edge[10][10]=0;g_edge[10][11]=999;g_edge[10][12]=1;g_edge[10][13]=1;
    
    g_edge[11][0]=999;g_edge[11][1]=999;g_edge[11][2]=999;g_edge[11][3]=999;g_edge[11][4]=999;g_edge[11][5]=1;g_edge[11][6]=999;g_edge[11][7]=999;
    g_edge[11][8]=999;g_edge[11][9]=999;g_edge[11][10]=999;g_edge[11][11]=0;g_edge[11][12]=1;g_edge[11][13]=1;
    
    g_edge[12][0]=999;g_edge[12][1]=999;g_edge[12][2]=999;g_edge[12][3]=999;g_edge[12][4]=999;g_edge[12][5]=999;g_edge[12][6]=1;g_edge[12][7]=999;
    g_edge[12][8]=999;g_edge[12][9]=999;g_edge[12][10]=1;g_edge[12][11]=1;g_edge[12][12]=0;g_edge[12][13]=999;
    
    g_edge[13][0]=999;g_edge[13][1]=999;g_edge[13][2]=999;g_edge[13][3]=999;g_edge[13][4]=999;g_edge[13][5]=999;g_edge[13][6]=1;g_edge[13][7]=999;
    g_edge[13][8]=999;g_edge[13][9]=999;g_edge[13][10]=1;g_edge[13][11]=1;g_edge[13][12]=999;g_edge[13][13]=0;
}

void readFatTreeGraph(){
    
}
//初始化
void initialize(const double g_lamda)
{
    m_lamda = g_lamda;	//业务到达率
    m_rou = g_rou;
    m_currentTime = 0;
    
    
    for (int i = 0; i < MAXINUM; i++){         //给拓扑中所有相连路劲分配全波长
        for (int j = 0; j < MAXINUM; j++){
            for (int k = 0; k < WaveNumber; k++){
                if (g_edge[i][j] < INFINITE - 1 && g_edge[i][j] != 0){
                    m_resourceMap[i][j][k] = true;		//所有链路的所有波长均可用
                }
            }
        }
    }
    //初始vnf计算资源消耗
    //srand(1);
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
        dealWithEvent( (Event&) *m_pq.begin() );		//按照优先因子处理优先级队列事件
    }
    
    ofile << (double)m_sumOfFailedService / ServiceQuantity;
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
    event0.m_sourceNode = node.first;
    event0.m_destNode = node.second;
    event0.c_m = randvnf();  //获取vnf类型
    
    m_pq.insert(event0);
}

//随机生成源宿节点(不能相同且宿节点为数据中心0,4,8,12)
Pair randomSrcDst()
{
    Pair SrcDst;
    
    do
    {
        SrcDst.first = genrandom(0, MAXINUM - 1);
        SrcDst.second = genrandom(0, MAXINUM - 1);
    } while (SrcDst.first == SrcDst.second);
    return SrcDst;
}

int* randvnf(){
    int numberOfVnf;
    numberOfVnf=genrandom(2,M);
    int* vnf = new int[M];
    for (int j=0;j<M;j++){
        vnf[j]=-1;
    }
    srand((unsigned)time(NULL));
    set<int>* tmp = new set<int>;
    while(tmp->size() < numberOfVnf){
        tmp->insert(rand() % M );
    }
    
    set<int>::iterator itr;
    int*  tmpt= new int[numberOfVnf];
    
    int i = 0;
    for(itr=tmp->begin();itr !=tmp->end();itr++)
    {
        tmpt[i++] = *itr;
    }
    
    for (int i = 0;i<numberOfVnf;i++){
        vnf[tmpt[i]]=1;
    }
    return vnf;}

//D算法
void pathCalculate(const int v)
{
    int m_shortestPoint[MAXINUM][MAXINUM];  //标记与v发生联系的节点
    for (int i = 0; i < MAXINUM; i++)
    {
        m_dist[v][i] = g_edge[v][i];
        m_shortestPoint[v][i] = 0;
        if (i != v && m_dist[v][i] < INFINITE - 1)
        {
            m_path[v][i] = v;
        }
        else
        {
            m_path[v][i] = -1;
        }
    }
    //m_dist[v][v] = 0;
    
    
    m_shortestPoint[v][v] = 1;			//
    for(int i = 0; i < MAXINUM - 1; i++) //控制从除v外挑选其他关联节点的空循环
    {
        float min = INFINITE;
        int u = v;
        for (int j = 0; j < MAXINUM; j++)  // ｍｉｎ值由最小开始，找到序列最小的节点，把节点序列返回给ｕ
        {
            if (!m_shortestPoint[v][j] && m_dist[v][j] < min)
            {
                u = j;
                min = m_dist[v][j];
            }
        }
        m_shortestPoint[v][u] = 1; //更新节点的使用情况，通过置1废除已经使用的节点
        for (int w = 0; w < MAXINUM; w++)  //  寻找u节点一步辅助节点w,更新到达w 节点的路径
        {
            if (!m_shortestPoint[v][w] && g_edge[u][w] < INFINITE - 1 && m_dist[v][u] + g_edge[u][w] < m_dist[v][w])
            {
                m_dist[v][w] = m_dist[v][u] + g_edge[u][w];
                m_path[v][w] = u;
            }
        }
    }
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

//生成业务离去事件
Event generateLeavingevent(const Event& event)
{
    Event event1 = event;
    event1.m_eventType = End;
    event1.m_time = event.m_time + event.m_holdTime;	//"业务离去事件的发生时间" = "业务到达事件发生的时间" + "业务持续时间"
    return event1;
}

//事件处理函数
void dealWithEvent(Event& event)  //事件处理函数流程决定了事件按照一个业务到达处理完毕再处理另一个业务的过程
{
    m_currentTime = event.m_time;
    switch (event.m_eventType)
    {
            //业务到达事件
        case 0:
        {
            int reg_wave = showPath(event);
            if (reg_wave == -1)
                
                //业务阻塞，统计阻塞率
                m_sumOfFailedService++;
            
            else
            {
                //为业务分配资源
                event.m_ocuppiedwave = reg_wave;
                
                //标记链路属性为不可用
                for (vector<int>::iterator it = event.m_workPath.begin(); it != event.m_workPath.end() - 1; it++)
                {
                    m_resourceMap[*it][*(it + 1)][event.m_ocuppiedwave] = false;
                }
                
                //生成该业务的离去事件
                m_pq.insert( generateLeavingevent(event) );
            };
            
            //生成下一个到达事件
            if (m_nextServiceId < ServiceQuantity)
            {
                generateServiceEventPair(m_nextServiceId);
                m_nextServiceId++;
            }
            
            //删除当前事件
            m_pq.erase(event);
            break;
        }
            //业务离去事件
        case 1:
        {
            //标记链路属性为可用
            for (vector<int>::iterator it = event.m_workPath.begin(); it != event.m_workPath.end() - 1; it++)
            {
                m_resourceMap[*it][*(it + 1)][event.m_ocuppiedwave] = true;
            }
            
            //删除当前事件
            m_pq.erase(event);
            break;
        }
    }
}

//计算路径，并返回可用波长编号
int showPath(Event& event)
{
    int s = event.m_sourceNode;
    int d = event.m_destNode;
    
    if (m_dist[s][d] > INFINITE - 1)
    {
        //cout << "节点不可达" << endl;
        return -1;
    }
    
    //计算业务路径，储存到m_workPath中
    event.m_workPath.reserve(MAXINUM * MAXINUM);
    int previousNode = s;
    int formerNode = m_path[d][previousNode];	//返回previousNode到d的下一跳节点,返回-1表明到达宿节点
    while (formerNode != -1)
    {
        event.m_workPath.push_back(previousNode);	//存储路径
        previousNode = formerNode;
        formerNode = m_path[d][previousNode];
    }
    event.m_workPath.push_back(previousNode);	//添加宿节点到路径
    
    
    //计算可用波长(满足波长连续性约束，且每个业务只占用1个波长)
    set<int> vacantWave;
    for (int i = 0; i < WaveNumber; i++)
        vacantWave.insert(i);
    
    for (vector<int>::iterator it = event.m_workPath.begin(); it != event.m_workPath.end() - 1; it++)
    {
        
        for (int i = 0; i < WaveNumber; i++)
        {
            if (m_resourceMap[*it][*(it + 1)][i] == false)
            {
                vacantWave.erase(i);	//删除被占用波长
            }
            
            if (vacantWave.size() == 0)
            {
                //cout << "无可用波长" << endl;
                return -1;
            }
        }
    }
    return *vacantWave.begin();	//取"可用波长"的第一个波长分配给当前业务
    
    /*int dd=genrandom(0,vacantWave.size());//随机命中
     vector<int> wavefree;
     int count=0;int as;
     for(set<int>::iterator pos=vacantWave.begin();pos!=vacantWave.end();pos++)
     {
     
     as=*pos;
     count++;
     if(count==dd)
     break;
     }
     
     
     return as; */
    
    
}

//在[ia,ib]之间产生一个整形随机数
int genrandom(const int ia, const int ib)
{   srand((unsigned)time(0))
    int fr = (int)((double)rand() / ((double)RAND_MAX + 1) * (ib - ia + 1) + ia);
    return fr;
}
