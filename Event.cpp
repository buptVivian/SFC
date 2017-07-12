  //
//  Event.cpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/10.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

#include "simulation.hpp"
#include "Event.hpp"


//事件处理函数
extern void dealWithEvent(Event &event)  //事件处理函数流程决定了事件按照一个业务到达处理完毕再处理另一个业务的过程
{
    m_currentTime = event.m_time;
    switch (event.m_eventType)
    {
            //业务到达事件
        case Arrival:
        {
            cout<<event.m_id<<"arr"<<endl;  //test
            int flagForService= 1; //判断服务是否成功的标志，若不成功则不需要改变原有资源状态
            
            //先排序再映射
            
            orderOfVNF(event); //计算vnf的顺序，存储在event.vnfOrder
            event.x = mapNode(event);  //map vnf到pod节点上event.x 长度为M
            
            if (event.x[0] == INF)
            {m_sumOfFailedService++;//业务阻塞，统计阻塞率
                flagForService =0;
            }
            else{
                //生成vnf所占pod节点路由
                event.m_podPath.push_back(event.m_sourceNode);
                event.m_podPath.push_back(event.x[event.vnfOrder[0]]);
                for(int i = 1;i<event.vnfOrder.size();i++){
                    if(event.x[event.vnfOrder[i]] != event.x[event.vnfOrder[i-1]])
                        event.m_podPath.push_back(event.x[event.vnfOrder[i]]);
                }
                event.m_podPath.push_back(event.m_destNode);
            
                //test
                for(int i=0; i<event.m_podPath.size(); i++)
                    cout<<event.m_podPath[i]<<"  ";
                cout<<endl;
                
                //计算频带
                event.m_wave.push_back(rf[event.vnfOrder[0]]); //存入第一个节点所需的频带
                int flag = event.x[event.vnfOrder[0]];
                for(int i = 1;i<event.vnfOrder.size();i++){
                    if(event.x[event.vnfOrder[i]] != flag)    //只分配不同pod节点间频带
                    {   flag = event.x[event.vnfOrder[i]];    //更新标志位
                        event.m_wave.push_back(min(rf[event.vnfOrder[i]],rf[event.vnfOrder[i-1]]));
                    }
                }
                event.m_wave.push_back(rf[event.vnfOrder[event.vnfOrder.size()-1]]);
                
                //test
                for(int i=0; i<event.m_wave.size();i++)
                    cout<<event.m_wave[i]<<" ";
                cout<<endl;
                
                //分配链路资源bool tmpt_podslot[k][k][WaveNumber];  //备份
                for(int i=0;i<k;i++)
                    for(int j=0;j<k;j++)
                        for(int m=0;m<WaveNumber;m++)
                            tmpt_podslot[i][j][m]=pod_slot[i][j][m];
                
                for(int i = 0;i<event.m_podPath.size()-1;i++)
                {
                    //分配端口
                    //MIS
                    int * port_link;
                    //port_link[0]为端口返回值，port_link[1]为链路slot返回值;eg port_link{0,1}
                    port_link =MIS_slotALLo(event.m_podPath[i],event.m_podPath[i+1],event.m_wave[i]);
                    
                    if(port_link[0] == INF)
                    {
                        m_sumOfFailedService++;
                        flagForService=0;
                        break;
                    }
                    else{
                        event.m_ocuppiedwave.push_back(port_link[1]);     //添加占用的波长序列号
                        event.m_port.push_back(port_link[0]);                 //添加端口
                        
                        //标记链路属性为不可用
                        if(i==0)
                        for(int j=port_link[1];j<port_link[1]+event.m_wave[i];j++)
                                tmpt_podslot[event.m_podPath[i+1]-100][port_link[0]][j] = false;
                        else if(i>0 and i<event.m_podPath.size()-2)
                        {
                        for(int j=port_link[1];j<port_link[1]+event.m_wave[i];j++)
                           {
                            tmpt_podslot[event.m_podPath[i]-100][port_link[0]][j] =  false;
                            tmpt_podslot[event.m_podPath[i+1]-100][port_link[0]][j] = false;
                           }
                        }
                        else
                        {   for(int j=port_link[1];j<port_link[1]+event.m_wave[i];j++)
                                tmpt_podslot[event.m_podPath[i]-100][port_link[0]][j] =  false;
                        }
                    }
                }

                if(flagForService==1){
                    //链路资源满足事件要求，更新链路信息
                    for(int i=0;i<k;i++)
                        for(int j=0;j<k;j++)
                            for(int m=0;m<WaveNumber;m++)
                                pod_slot[i][j][m]=tmpt_podslot[i][j][m];

                    //更新slot信息
                    sumOfusedSlot=sumOfusedSlot+event.m_wave[0];
                    for(int i=1;i<event.m_podPath.size()-1;i++)
                        sumOfusedSlot=sumOfusedSlot+event.m_wave[i]*2;
                    //更新pod信息
                    sumOfusedPods = sumOfusedPods+int(event.m_podPath.size())-2;
                    //生成该业务的离去事件
                    m_pq.push(generateLeavingevent(event));
                }else break;
            }
            //生成新的业务
            if (m_nextServiceId < ServiceQuantity)
            {
                generateServiceEventPair(m_nextServiceId);
                m_nextServiceId++;
            }
            break;
        }
            //业务离去事件
        case End:
        {   cout<<event.m_id<<"end"<<endl;
            //标记链路属性为可
            for(int i=0;i<event.m_podPath.size()-1;i++){
                if(i==0)
                {  for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                    pod_slot[event.m_podPath[i+1]-100][event.m_port[i]][j]=true;
                }
                else if(i>0 and i<event.m_podPath.size()-2)
                {
                    for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                    {pod_slot[event.m_podPath[i]-100][event.m_port[i]][j]=true;
                     pod_slot[event.m_podPath[i+1]-100][event.m_port[i]][j]=true;
                    }
                }else{
                for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                    pod_slot[event.m_podPath[i]-100][event.m_port[i]][j]=true;
                }
                
            }
        }//caseEnd
    }//switch
}

extern  int * calculateOrder(int *rf,int n,vector<int> tmpt_vnfOrder);
void orderOfVNF(Event & event) //计算vnf顺序
{
    vector<int> tmpt_vnfOrder;
    int n = 0;
    //先排序再映射
    for (int i = 0;i< M;i++){
        if(event.c_M[i]== 1){
            tmpt_vnfOrder.push_back(i);
            n++;
        }
    }
     n++ ; //将源节点也加入到TSP
     int * m_vnfPath = NULL;
    
    m_vnfPath = calculateOrder(rf,n,tmpt_vnfOrder);
    //m_vnfPath[1]为原节点
    //test
    /*
    for(int i=2; i<=n; i++)
        cout<< tmpt_vnfOrder[m_vnfPath[i]-2]<<" ";
    cout<<endl;*/
    
    //求解顺序失败，采用随机顺序
    if(m_vnfPath[1]==-1){
        set<int>* tmp = new set<int>;
        //test
        cout<< "I am IN !!"<< endl;
        
        while(tmp->size() < n-1){
            tmp->insert(rand() % (n-1));
        }
        
        set<int>::iterator itr;
        
        for(itr=tmp->begin();itr !=tmp->end();itr++)
            event.vnfOrder.push_back(tmpt_vnfOrder[*itr]);
    }
    else                            //按照tsp 处理的结果将pod节点添加
    {
    for (int i=2 ;i<=n;i++)      //加入其余节点，第1位为原节点
    event.vnfOrder.push_back(tmpt_vnfOrder[m_vnfPath[i]-2]);
    }
}

//map 节点
int * mapNode(Event& event){
    int tmpt_rp[k];   //避免请求失败产生的对rp的修改
    for(int i=0;i<k;i++)
      tmpt_rp[i] = rp[i];
    
    int * result = new int[M]; //返回值
    int mapRes[M];  // mapRes对应vnf 映射结果
    for(int i = 0;i<M;i++)      //初始化
        mapRes[i] = -1;
    
    result = mapRes;
    vector<int> PodList;          // 设置可用节点list PodList为可用节点位置
    vector<int> PodDesc;          //记录降序排列后,pod的位置
    
    for (int i = 0;i<k;i++){     //计算可用节点
        if (tmpt_rp[i] > 0) PodList.push_back(i);
    }
    
    if (PodList.size() == 0) {
        result[0] =INF;   //没有可用的pod,result[0]=INF
        return result;
    }else {
        
        //对pod节点降序排列
        while(PodList.empty()!= true ){
            int point = 0;
            for(int i =1;i<PodList.size();i++){
                if(tmpt_rp[PodList[point]]<tmpt_rp[PodList[i]]){
                    point =i;
                }
            }
            PodDesc.push_back(PodList[point]);
            PodList.erase(PodList.begin()+point);
        }
    }
    
    int mapVnfNumber = 0;
    int PodForCurtEvent = 0;//从第0个位置开始扫描podlist
    
    while(mapVnfNumber < event.vnfOrder.size()){
        if (tmpt_rp[PodDesc[PodForCurtEvent]] >= rf[event.vnfOrder[mapVnfNumber]])
        {  //判断是否有充分的计算资源
            mapRes[event.vnfOrder[mapVnfNumber]] = PodDesc[PodForCurtEvent]+100;
            tmpt_rp[PodDesc[PodForCurtEvent]] = tmpt_rp[PodDesc[PodForCurtEvent]]-rf[event.vnfOrder[mapVnfNumber]] ;//更新节点容量
            mapVnfNumber++;                                 //更新完成映射节点的个数
        }else{
            PodForCurtEvent++;
            if(PodForCurtEvent >= PodDesc.size())
            { //如果pod不能支持业务,result[0]=INF
                result[0]=INF;
                return result;
            }
        }
    }
    
    for(int i=0;i<k;i++)
        rp[i] = tmpt_rp[i];
    for(int i=0; i<M;i++)
    { //test
      cout<<mapRes[i]<<" ";
      result[i]=mapRes[i];
    }
    cout<<endl;
    return result;
}

//MIS方式分配slot
int * MIS_slotALLo(int m,int n, int wave)  //re[0]=INF 代表没有合适的端口或是slot
{
    int * re = new int[2];
    if(m < 999 and n<999)   //非原节点
    {   m=m-100;
        n=n-100;
        //求解mis,遍历每个端口
        vector<int> link_mis;  //链路mis
        vector<int> port;
        for(int i=0 ; i<k ; i++)
        {
                MIS[m][i] = cal_MIS(m, i);
                MIS[n][i] = cal_MIS(n, i);
                link_mis.push_back(MIS[m][i]>MIS[n][i] ? MIS[m][i] : MIS[n][i]);
                port.push_back(i);
        }
        //倒序尝试
        while (!port.empty())
        {
            //求当前最小序列链路
            vector<int>::iterator it=port.begin();
            int point = *it;
            for(; it!=port.end(); it++)
                if(link_mis[point]>link_mis[*it]) point = *it;
            if(link_mis[point] == WaveNumber-1)
                 {re[0]=INF;
                     return re;}
            else{
            port.erase(port.begin()+point); //删除欲使用端口
            
            //查看是否有足够slot
            int index=0;
            int count=0;
            for(int i=0 ;i<WaveNumber; i++)
            {
                if(pod_slot[m][point][i]==true and pod_slot[n][point][i]==true)
                {
                    count++;
                    if(count == wave)
                    {
                        re[0]=point;
                        re[1]=index;
                        return re;
                    }
                }
                else {
                    index=i+1;
                    count=0;
                }
                
            }
        }
        }
        re[0] = INF; //failed
        return re;
    }
    else if (m>=1000)
    {
        //查看是否有足够的slot
        m=m-1000;
        n=n-100;
        int count=0;
        int index=0;
        for(int i=0 ; i<WaveNumber ;i++)
        {
            if(pod_slot[n][m][i] == true)
            {
                count++;
                if(count == wave)
                {
                    re[0]=m;
                    re[1]=index;
                    return re;
                }
                
            }
            else
            {   count=0;
                index=i+1;
            }
            
        }
        re[0]=INF;
        return re;
    }else{
        m=m-100;
        n=n-1000;
        int count=0;
        int index=0;
        for(int i=0 ; i<WaveNumber ;i++)
        {
            if(pod_slot[m][n][i] == true)
            {
                count++;
                if(count == wave)
                {
                    re[0]=n;
                    re[1]=index;
                    return re;
                }
                
            }
            else
            {   count=0;
                index=i+1;
            }
            
        }
        re[0]=INF;
        return re;
    }
}

//计算mis pod[m],port[p]
int cal_MIS (int m,int p)
{int res =0;
    for(int j=0 ;j<WaveNumber ;j++)
    {
        if(tmpt_podslot[m][p][j] == false)
            res=j;
    }
    return res;
}
/*
//计算可用波长(满足波长连续性约束,满足链路资源要求）
int calculate_wave_index(vector<int> current_workPath,int wave_need)
{
    int lengthOfWave=0;
    int point = 0;
    while(lengthOfWave< wave_need && point<WaveNumber){
        int j=0;
        //cout<<current_workPath.size()<<endl;
        for( ;j<current_workPath.size()-1; )
        {
            if(pod_slot[current_workPath[j]][current_workPath[j+1]][point] != true)
                break; //有一段为空就跳出
            else j++;
        }
        if(j == current_workPath.size()-1 ) lengthOfWave++;
        else lengthOfWave = 0;
        point++;
        
    }
    
if( lengthOfWave == wave_need)
        return point-wave_need;//取可用波长块的最小序列号
    
    else return -1;
    
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


