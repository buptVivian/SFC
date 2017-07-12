//
//  Event.hpp
//  SFC
//
//  Created by vivian_bytedance on 2017/7/10.
//  Copyright © 2017年 vivian_bytedance. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <vector>

using namespace std;


bool tmpt_podslot[k][k][WaveNumber];  //备份

void orderOfVNF(Event& event);
int * mapNode(Event& event);


int calculate_wave_index(vector<int> current_workPath,int wave_need);

int * MIS_slotALLo(int m,int n, int wave);
int cal_MIS (int m,int p);


#endif /* Event_hpp */
