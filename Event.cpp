//初始vnf计算资源消耗,初始rf{1,10}
//初始vnf计算资源消耗,初始rf{1,10}
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
            vector<int>::iterator vnf;
            //test
            vector<int>::iterator itvnf=event.vnfOrder.begin();
            for(;itvnf !=event.vnfOrder.end();itvnf++)
                cout<<*itvnf<<"CPU"<<rf[*itvnf]<<" ";
            cout<<endl;
            
            mapNode(event);  //map vnf到pod节点上event.x 长度为M
            //test
            for(int a=0; a<k; a++)
                cout<<"rp"<<rp[a]<<" ";
                cout<<endl;
            
            if (event.x[0] == INF)
               {   //test
                num_pod++;
                cout<<"pod failure"<<endl;
                m_sumOfFailedService++;//业务阻塞，统计阻塞率
                flagForService =0;
                }
            else{
                //生成vnf所占pod节点路由
                event.m_podPath.push_back(event.m_sourceNode);
                vector<int>::iterator it = event.vnfOrder.begin();
                event.m_podPath.push_back(event.x[*it]);
                it++;
                for(;it != event.vnfOrder.end();it++)
                   {
                    if(event.x[*it] != event.x[*(it-1)])
                        event.m_podPath.push_back(event.x[*it]);
                    }
                event.m_podPath.push_back(event.m_destNode);
                
                //计算频带
                it=event.vnfOrder.begin();
                event.m_wave.push_back(rf[*it]); //存入第一个节点所需的频带
                int flag = event.x[*it];
                it++;
                for(;it != event.vnfOrder.end();it++)
                   {
                    if(event.x[*it] != flag)    //只分配不同pod节点间频带
                      { flag = event.x[*it];    //更新标志位
                        event.m_wave.push_back(min(rf[*it],rf[*(it-1)]));
                       }
                    }
                event.m_wave.push_back(rf[*(it-1)]);
                
                //分配链路资源bool tmpt_podslot[k][k][WaveNumber];  //备份
                tmpt_podslot = pod_slot;
                
                for(int i = 0;i<event.m_podPath.size()-1;i++)
                   {//MIS
                    int port_link =0;  //返回占用频带的序列号
                    port_link =MIS_slotALLo(event.m_podPath[i],event.m_podPath[i+1],event.m_wave[i],event.m_port);
                    
                    if(port_link == -1)
                       {//test
                        num_slot++;
                        cout<<"slotFailed"<<endl;
                        m_sumOfFailedService++;
                        flagForService=0;
                        break;
                        }
                    else{
                        int tmpt_port = event.m_port.back();
                        event.m_ocuppiedwave.push_back(port_link);     //添加占用的波长序列号
                        
                        //标记链路属性为不可用
                        if(i==0)
                           for(int j=port_link;j<port_link+event.m_wave[i];j++)
                               tmpt_podslot[event.m_podPath[i+1]-100][tmpt_port][j] = false;
                        else if(i>0 and i<event.m_podPath.size()-2)
                               {
                                for(int j=port_link;j<port_link+event.m_wave[i];j++)
                                   {
                                    tmpt_podslot[event.m_podPath[i]-100][tmpt_port][j] = false;
                                    tmpt_podslot[event.m_podPath[i+1]-100][tmpt_port][j] = false;
                                    }
                                }
                        else
                                {
                                 for(int j=tmpt_port;j<port_link+event.m_wave[i];j++)
                                     tmpt_podslot[event.m_podPath[i]-100][tmpt_port][j] = false;
                                 }
                          }
                       }
                   }
                if(flagForService==1)
                  {
                    //链路资源满足事件要求，更新链路信息
                    for(int i=0;i<k;i++)
                        for(int j=0;j<(k/2)*(k/2);j++)
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
            for(int i=0;i<event.m_podPath.size()-1;i++)
               {
                if(i==0)
                  {for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                    pod_slot[event.m_podPath[i+1]-100][event.m_port[i]][j]=true;
                  }
                else if(i>0 and i<event.m_podPath.size()-2)
                       {for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                           {pod_slot[event.m_podPath[i]-100][event.m_port[i]][j]=true;
                            pod_slot[event.m_podPath[i+1]-100][event.m_port[i]][j]=true;
                           }
                       }
                else{
                    for(int j=event.m_ocuppiedwave[i]; j<event.m_ocuppiedwave[i]+event.m_wave[i];j++)
                    pod_slot[event.m_podPath[i]-100][event.m_port[i]][j]=true;
                    }
                }
             //标记节点资源为可用
             vector<int>:: iterator it = event.m_vnf.begin();
             for(; it != event.m_vnf.end(); it++)
                rp[event.x[*it]-100]=rp[event.x[*it]-100]+rf[*it];
       }//caseEnd
    }//switch
}


extern  int calculateOrder(const int * rf, int n,int * vnf);
void orderOfVNF(Event & event) //计算vnf顺序
{
    int vnfOrder[event.numberOfVnf];
    for(int i=0; i<event.numberOfVnf; i++)
        vnfOrder[i]=event.m_vnf[i];
    int flag = calculateOrder(rf,event.numberOfVnf+1,vnfOrder);
    //m_vnfPath[1]为原节点
    
    //求解顺序失败，采用随机顺序
    if(flag == 0){
        set<int> tmp ;
        //test
        cout<< "I am IN !!"<< endl;
        
        while(tmp.size() < event.numberOfVnf){
            tmp.insert(rand() % (event.numberOfVnf));
        }
        
        set<int>::iterator itr;
        for(itr=tmp.begin();itr !=tmp.end();itr++)
            event.vnfOrder.push_back(event.m_vnf[*itr]);

        }
        else                            //按照tsp 处理的结果将pod节点添加
        {
          vector<int>::iterator vnf = event.m_vnf.begin();
        for (int i=0 ;i<event.numberOfVnf;i++)      //加入其余节点，第1位为原节点
            //tests vnfOrder 也没有问题。event.m_vnf 无误；*(vnf+vnfOrder[i]-2)以及event.m_vnf[i]皆有误
            event.vnfOrder.push_back(*(vnf+vnfOrder[i]-2));
        
         }

}
//map 节点
void mapNode(Event& event){
    int tmpt_rp[k] ;//避免请求失败产生的对rp的修改
    for(int i=0; i<k; i++)
        tmpt_rp[i]=rp[i];
     for(int i=0; i<M; i++)
         event.x[i]=-1;
    //result = mapRes;
    vector<int> PodList;          // 设置可用节点list PodList为可用节点位置
    vector<int> PodDesc;          //记录降序排列后,pod的位置
    
    for (int i = 0;i<k;i++){     //计算可用节点
        if (tmpt_rp[i] > 0) PodList.push_back(i);
    }
    
    if (PodList.size() == 0) {
        event.x[0] =INF;   //没有可用的pod;x[0]INF
        //test
        //cout<<" podlist为空 "<<endl;
    }else
        {
        //对pod节点降序排列
         while(PodList.empty()!= true )
              {
               int point = 0;
               for(int i =1;i<PodList.size();i++)
                   if(tmpt_rp[PodList[point]]<tmpt_rp[PodList[i]])
                      point =i;
                   
               PodDesc.push_back(PodList[point]);
               PodList.erase(PodList.begin()+point);
              }
        //降序
        
         int mapVnfNumber = 0;
         int PodForCurtEvent = 0;//从第0个位置开始扫描podlist
    
         while(mapVnfNumber < event.numberOfVnf and event.x[0]!=INF)
              {
               if (tmpt_rp[PodDesc[PodForCurtEvent]] >= rf[event.vnfOrder[mapVnfNumber]])
                  {  //判断是否有充分的计算资源
                   event.x[event.vnfOrder[mapVnfNumber]] = PodDesc[PodForCurtEvent]+100;
                   tmpt_rp[PodDesc[PodForCurtEvent]] = tmpt_rp[PodDesc[PodForCurtEvent]]-rf[event.vnfOrder[mapVnfNumber]] ;//更新节点容量
                   mapVnfNumber++;                                 //更新完成映射节点的个数
                   }
               else{
                    PodForCurtEvent++;
                    if(PodForCurtEvent >= PodDesc.size())
                        //如果pod不能支持业务,result[0]=INF
                       {event.x[0]=INF;
                        //tets
                        for(int a=0; a<k; a++)
                            cout<<"tmpt_rp"<<tmpt_rp[a]<<" ";
                            cout<<endl;
                            cout<< "pod 容量不够" <<endl;
                       }
                    
                    }
              }
    }
    if(event.x[0]!= INF)
      {
       for(int i=0;i<k;i++)
       rp[i] = tmpt_rp[i];
    /*for(int i=0; i<M;i++)
    { //test
      cout<<event.x[i]<<" ";
    }
    cout<<endl;*/
      }
}

//MIS方式分配slot
int  MIS_slotALLo(int m,int n, int wave, vector<int> & link_port)  //return -1 代表没有合适的端口或是slot
{   
    if(m < 999 and n<999)   //非原节点
    {   m=m-100;
        n=n-100;
        //求解mis,遍历每个端口
        vector<int> link_mis;  //链路mis
        vector<int> port;
        for(int i=0 ; i<(k/2)*(k/2) ; i++)
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
                 {return -1;}
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
                        link_port.push_back(point);
                        return index;
                    }
                }
                else {
                    index=i+1;
                    count=0;
                }
                
            }
        }
        }
        return -1;
    }
    else if (m>=1000)  //m::source
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
                    link_port.push_back(m);
                    return index;
                }
                
            }
            else
            {   count=0;
                index=i+1;
            }
            
        }
        return -1;
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
                    link_port.push_back(n);
                    return index;
                }
                
            }
            else
            {   count=0;
                index=i+1;
            }
            
        }
        return -1;
    }
}

//计算mis pod[m],port[p]
int cal_MIS (int m,int p)
{int res =0;
    for(int j=0 ;j<WaveNumber ;j++)
    {
        if(tmpt_podslot[m][p][j] == 0)
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


