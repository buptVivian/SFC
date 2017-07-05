//
//  main.cpp
//  simulationFoSFC
//
//  Created by Vivian1010 on 17/4/1.
//  Copyright © 2017年 Vivian1010. All rights reserved.
//



#include "simulation.hpp"
int main()
{
    //写文件
    ofstream ofile;
    ofile.open("/Users/vivian1010/Documents/result/result.txt", ios::out);
    if (!ofile)
    {
        cout << "open result file error!" << endl;
        exit(-2);
    }
    
    
    //不断增大业务强度(提高到达率,离去率保持不变)
    for (double lamda = 1; lamda < 10; lamda += 0.5)// lamda 待修改
    {
        clock_t start = clock();
        
        //readFatTreeGraph( );   //目前不存在错误5/14
        initialize(0.1 / 1);
        cout <<"initializesuccess"<<0.1/lamda<<endl;
        
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
