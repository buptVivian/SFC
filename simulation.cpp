    
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
