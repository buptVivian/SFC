//
//  simulation.hpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/6.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

//#ifndef _TASK1_SIMPLESIMSYSTEM_
//#define _TASK1_SIMPLESIMSYSTEM_
//static VS extern

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <set>
#include<queue>
#include <cmath>

using namespace std;

//topology
const int INF = 999;			//标示节点不可达
const int k = 4;  //pod 的数量
//int agg_switch[k][k/2];   //agg_switch 以分组的方式存贮

const int WaveNumber = 10;			//链路波长总数?
extern  int pod_slot[k][k][WaveNumber]; //‘0’代表空‘1’代表占
extern int MIS[k][k];       //衡量端口占用频带序列{0-waveNumber-1}
//coreSwitch{1000,1001,1002,1003}
//pod{100,101,102,103}


//initial
//const double g_lamda = 0.01; 		//业务到达率(固定)
const double g_rou =5 ;				//业务离去率(固定)
static double m_lamda;
static double m_rou;
extern double m_currentTime;
const int M = 6 ;                  //vnf 类型数量
extern int rf[M];                   //vnf 所需的计算资源,范围为[1，10]

typedef pair<int, int> Pair;  //返回值是两个整形

extern int rp[k];            //Pod 节点剩余计算资源
//int x[ServiceQuantity][M+2]; //放置在Pod节点的映射

//run
extern int m_nextServiceId;
extern int m_sumOfFailedService;
extern int sumOfusedSlot;
extern int sumOfusedPods;
const int ServiceQuantity = 10;

//Event事件属性
enum EventType { Arrival, End};	//事件类型(到达/离去)

typedef struct event
{
    EventType m_eventType;		//业务的类型(到达或离去)
    double m_time;				//业务的到达时间
    double m_holdTime;			//业务的持续时间
    int m_id;					//业务的id
    int m_sourceNode;			//业务的源节点
    int m_destNode;				//业务的宿节点
    
    int* c_M;           // 业务需要的vnf，其中'-1'代表不需要‘1’代表需要
    //int numberOfVnf;    //业务需要的vnf个数
    vector<int> vnfOrder;		//从第0开始记录业务的完整vnf 顺序
    vector<int> m_podPath;         //从源节点到目的节点路由
     vector<int> m_port ;           //逻辑链路段对应实际端口
    vector<int> m_wave;         //业务需要的波长
    vector<int> m_ocuppiedwave;			//业务的占有波长编号
   
    
    int* x;                //节点映射
    
    bool operator < (const event& event1) const
    {
        return m_time > event1.m_time;
    }
    
    
}Event;

extern priority_queue<Event> m_pq; // 事件队列
////重载<，优先级因子
//static bool operator > (const Event& event1, const Event& event2)
//{
//    return event1.m_time < event2.m_time;
//}


void initialize(const double g_lamda);
int* randvnf();
void dealWithEvent(Event & event);

void run(ofstream& ofile);

void generateServiceEventPair(const int id);
Event generateLeavingevent(const Event& event);

int genrandom( int ia,  int ib);
double arrive_time_gen(double beta);
double arrive_time_hol(const double beta);
Pair randomSrcDst();




//#endife

