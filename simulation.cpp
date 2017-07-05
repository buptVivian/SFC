
#include "simulation.hpp"
#include "TSP.hpp"
#include "set"
#include<vector>



void initialize(const double g_lamda)
{
    m_lamda = g_lamda;	//业务到达率
    m_rou = g_rou;
    m_currentTime = 0;
    
    for(int i = 0; i<k; i++){   //所有端口都可用
        for(int j = 0;j<k; j++)
          pod_port[i][j]=true;
    }
    for (int i = 0; i < k; i++){         //给拓扑中所有相连路径分配全波长
        for (int j = 0; j <k; j++){
            for (int m = 0; m < WaveNumber; m++){
                pod_slot[i][j][m] = 0 ;		//所有pod的端口的所有波长均可用
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
        dealWithEvent();		//按照优先因子处理优先级队列事件
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

//事件处理函数
void dealWithEvent()  //事件处理函数流程决定了事件按照一个业务到达处理完毕再处理另一个业务的过程
{   Event event=m_pq.top();
    m_pq.pop();
    
    m_currentTime = event.m_time;
    switch (event.m_eventType)
    {
            //业务到达事件
        case Arrival:
        {
            cout<<event.m_id<<"arr"<<endl;  //test
            int flagForService= 1; //判断服务是否成功的标志，若不成功则不需要改变原有资源状态
            vector<int> tmpt_vnfOrder;
            
            //先排序再映射
            for (int i = 0;i< M;i++)
                if(event.c_M[i]== 1) tmpt_vnfOrder.push_back(i);
            orderOfVNF(tmpt_vnfOrder,event); //计算vnf的顺序
            
            event.x = mapNode(event);  //map vnf到pod节点上event.x 长度为M

            if (event.x[0]!= INF)
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
                
                //生成pod节点路由所需的波长
                event.m_wave.push_back(2); //从源节点出发是所需的slot
                int flag = event.x[event.vnfOrder[0]];
                 for(int i = 1;i<event.vnfOrder.size();i++){
                    if(event.x[event.vnfOrder[i]] != flag)    //只分配不同pod节点间频带
                    {   flag = event.x[event.vnfOrder[i]];    //更新标志位
                        event.m_wave.push_back(min(rf[event.vnfOrder[i]],rf[event.vnfOrder[i-1]]));
                    }
                 }
                event.m_wave.push_back(rf[event.vnfOrder[event.vnfOrder.size()-1]]);
                
                //分配链路资源
                bool tmpt_resourceMap[PodNumber][PodNumber][WaveNumber];
                for(int i=0;i<PodNumber;i++)
                    for(int j=0;j<PodNumber;j++)
                        for(int m=0;m<WaveNumber;m++)
                            tmpt_resourceMap[i][j][k]=m_resourceMap[i][j][m];
                
                /*cout<<event.m_id<<endl;
                for(int i=0;i<event.m_podPath.size();i++)
                    cout<<event.m_podPath[i]<<" ";
                cout<<endl;*/
                for(int i = 0;i<event.m_podPath.size()-1;i++)
                {
                    vector<int> path = showPath(event.m_podPath[i],event.m_podPath[i+1]); //计算路径
                    if(path[0] == -1) {
                        m_sumOfFailedService++;
                        flagForService=0;
                        break;
                    }
                    else{
                        //计算使用的波长序列
                        int reg_wave = calculate_wave_index(path,event.m_wave[i]);
                        if (reg_wave == -1)
                            //业务阻塞，统计阻塞率
                        { m_sumOfFailedService++;
                            flagForService=0;
                            break;}
                        else{
                            event.m_ocuppiedwave.push_back(reg_wave);     //添加占用的波长序列号
                            event.m_path.push_back(path);                 //添加路径
                            
                            //标记链路属性为不可用
                            for(int j=reg_wave;j<reg_wave+event.m_wave[i];j++)
                                for (vector<int>::iterator it = path.begin(); it != path.end()-1; it++)
                                {
                                    tmpt_resourceMap[*it][*(it + 1)][j] = false;
                                }
                            }
                         }
                 }
            
            if(flagForService==1){
            //链路资源满足事件要求，更新m_resourcemap
                for(int i=0;i<PodNumber;i++)
                    for(int j=0;j<PodNumber;j++)
                        for(int m=0;m<WaveNumber;m++)
                            m_resourceMap[i][j][m]=tmpt_resourceMap[i][j][m];
            //更新slot信息
            for(int i=0;i<event.m_path.size();i++)
                sumOfusedSlot=sumOfusedSlot+int(event.m_path[i].size()-1)*event.m_wave[i];
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
            for(int m=0;m<event.m_path.size();m++){
                for(int j=event.m_ocuppiedwave[m];j<event.m_ocuppiedwave[m]+event.m_wave[m];j++)
                    for (vector<int>::iterator it = event.m_path[m].begin(); it != event.m_path[m].end()-1; it++)
                        m_resourceMap[*it][*(it + 1)][j] = true;
                
            }
        }//caseEnd
    }//switch

}

void orderOfVNF(vector<int> tmpt_vnfOrder,Event & event) //计算vnf顺序
{
    int n= int(tmpt_vnfOrder.size()+1) ; //将源节点也加入到TSP
        
    int * m_vnfPath = NULL;
    m_vnfPath = calculateOrder(rf,n,tmpt_vnfOrder);  //m_vnfPath长度在tsp算法设置，第0位是源节点，第一位开始是tmpt_vnfOrder标志位，位置相差2
    
    for(int i = 0;i <= n;i++)  //TEST
     cout<< m_vnfPath[i] << " ";
    cout<<endl;
    
    //求解顺序失败，采用随机顺序
        if(m_vnfPath[0]==-1){
        set<int>* tmp = new set<int>;
    
        cout<< "I am IN !!"<< endl; //test
        
        while(tmp->size() < n-1){
            tmp->insert(rand() % (n-1));
        }
        
        set<int>::iterator itr;
        
        for(itr=tmp->begin();itr !=tmp->end();itr++)
            event.vnfOrder.push_back(tmpt_vnfOrder[*itr]);
       }
      else                            //按照tsp 处理的结果将pod节点添加
      {for (int i=1 ;i<n;i++)      //加入其余节点
      event.vnfOrder.push_back(tmpt_vnfOrder[m_vnfPath[i]-2]);
      }
    /*for(int i=0;i<n+1;i++)
        cout<<event.vnfOrder[i]<<" ";
    cout<<endl;*/
    //return m_vnfOrder;
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
    
    int mapVnfNumber = 0;   //置1从vnf开始
    int PodForCurrentEvent = 0;   //从第0个位置开始扫描podlist
    
    while(mapVnfNumber<event.vnfOrder.size()){   // -2 排除源节点和目的节点
        if (tmpt_rp[PodDesc[PodForCurrentEvent]] >= rf[event.vnfOrder[mapVnfNumber]]){  //判断是否有充分的计算资源
            mapRes[event.vnfOrder[mapVnfNumber]] = PodDesc[PodForCurrentEvent]+100; //+1是有vnf类型是从0-1产生
           
            tmpt_rp[PodDesc[PodForCurrentEvent]] = tmpt_rp[PodDesc[PodForCurrentEvent]]-rf[event.vnfOrder[mapVnfNumber]] ;//更新节点容量
            mapVnfNumber++;                                 //更新完成映射节点的个数
        }else{
            PodForCurrentEvent++;
            if(PodForCurrentEvent >= PodDesc.size()) { //如果pod不能支持业务,result[0]=INF
                result[0]=INF;
                return result;
            }
        }
    }
    
    for(int i=0;i<k;i++)
        rp[i] = tmpt_rp[i];
    for(int i=0; i<M;i++)
        result[i]=mapRes[i];

    /*for(int i=0;i<M+2;i++)
        cout<<result[i]<<" ";
    cout<<endl;*/
    return result;
}


//计算路径
vector<int> showPath(int s,int d)  //计算从's'到'd'
{   vector<int> current_workPath;
    
    if (m_dist[s][d] > INFINITE - 1) //节点不连通
    {
        //cout << "节点不可达" << endl;
        current_workPath.push_back(-1);
        
    }
    
    //计算业务路径，储存到current_workPath中
    int previousNode = s;
    int formerNode = m_path[d][previousNode];	//返回previousNode到d的下一跳节点,返回-1表明到达宿节点
    while (formerNode != -1)
    {
        current_workPath.push_back(previousNode);//存储路径
        previousNode = formerNode;
        formerNode = m_path[d][previousNode];
    }
    current_workPath.push_back(previousNode);	//添加宿节点到路径
    return current_workPath;
}

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
            if(m_resourceMap[current_workPath[j]][current_workPath[j+1]][point] != true)
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
