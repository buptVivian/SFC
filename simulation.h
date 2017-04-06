#ifndef _TASK1_SIMPLESIMSYSTEM_
#define _TASK1_SIMPLESIMSYSTEM_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <set>
#include <algorithm>

using namespace std;


typedef pair<int, int> Pair;  //返回值是两个整形

//const double g_lamda = 0.01; 		//业务到达率(固定)
const double g_rou =5 ;				//业务离去率(固定)
const int INFINITE = 999;			//标示节点不可达
const int MAXINUM = 8;				//拓扑节点数 先建立8个节点的树状拓扑
const int SERVERNUMBER = 4;			//数据中心数（宿节点）
const int WaveNumber = 10;			//链路波长总数
const int ServiceQuantity = 10000;	//业务总数

const int M = 8 ;                  //vnf 类型数量
static int rf[M];                   //vnf 所需的计算资源,范围为[1，10]

int rp[MAXINUM]={10,10,10,10,10,10,10,10};            //Pod 节点剩余计算资源
int x[ServiceQuantity][M+2]; //放置在Pod节点的映射

//Event事件属性
enum EventType { Arrival, End };	//事件类型(到达/离去)

typedef struct event
{
	EventType m_eventType;		//业务的类型(到达或离去)
	double m_time;				//业务的到达时间
	double m_holdTime;			//业务的持续时间
	int m_id;					//业务的id
	int m_sourceNode;			//业务的源节点
	int m_destNode;				//业务的宿节点

	vector<int> vnfOrder;		//业务的完整路由
	int m_ocuppiedwave;			//业务的占有波长编号
	int* c_M;           // 业务需要的vnf，其中'-1'代表不需要‘1’代表需要

	}Event;

static float g_edge[MAXINUM][MAXINUM];     //网络拓扑
static float m_dist[MAXINUM][MAXINUM];     //到达节点的距离
static int m_path[MAXINUM][MAXINUM];       //路径

static double m_lamda;      
static double m_rou;
static int m_nextServiceId;
static double m_currentTime;
static int m_sumOfFailedService;
static int sumOfusedSlot;
static int sumOfusedPods;

static bool m_resourceMap[MAXINUM][MAXINUM][WaveNumber];
static set<Event> m_pq; // 事件队列 

 

//static char pathOfEdgeMap[128] = "map.data";

//重载<，优先级因子
static bool operator < (const Event& event1, const Event& event2)
{
	return event1.m_time < event2.m_time;
}

void readUSAEdgeMap( );
void readFatTreeGraph();
void initialize(const double g_lamda);
void run(ofstream& ofile);

void dealWithEvent(Event& event);
int showPath(Event& event);
void pathCalculate(const int v);

void orderOfVNF(vector<int>& tmpt_vnfOrder);
int m_min(int a ,int b);

void generateServiceEventPair(const int id);
Event generateLeavingevent(const Event& event);
int genrandom(const int ia, const int ib);
double arrive_time_gen(double beta);
double arrive_time_hol(const double beta);
Pair randomSrcDst();



#endif
