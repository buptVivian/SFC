//
//  main.cpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/6.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

//test
//#include "test7_10.hpp"

#include "simulation.hpp"
int main()
{
    //for test 2017.7.10
    //写文件
    ofstream ofile;
    ofile.open("/Users/vivian_bytedance/Documents/VNF/code/result/re5.txt", ios::out);
    if (!ofile)
    {
        cout << "open result file error!" << endl;
        exit(-2);
    }
    
    //初始vnf计算资源消耗,初始rf{1,10}
    srand((unsigned)time(0));
    for (int i = 0;i<M;i++)
    {  rf[i]=rand()%10+1;
        //test
        cout<<rf[i]<<" ";
    }
   
    //不断增大业务强度(提高到达率,离去率保持不变)
    for (double lamda = 10; lamda <20; lamda += 1)// lamda越大，时间间隔越短
     { 
        clock_t start = clock();
        cout <<"到达率"<<lamda<<endl;
        initialize(1.0/lamda);

        run(ofile);
        
        clock_t end = clock();
        double result = (double)(end - start) / CLOCKS_PER_SEC;
        cout << result << "s" << endl;
    
     }
    
    
    ofile.close();
    system("pause");
    cout << "finished" << endl;
    
    
    return 0;
    
}
