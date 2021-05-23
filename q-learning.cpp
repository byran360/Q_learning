#include <stdlib.h>   
#include <math.h>   
#include <time.h>   
#include <stdio.h>
#include <conio.h>

#define EPISODES 100   		// 算法训练幕数
#define STEP 10        		//算法训练每一幕运行步数
#define INITIAL_STATE 0   
#define FINAL_STATE 13   

/*从所有可能的动作中找出一个最大值动作*/ 
int max_q_act(float d[4]){                    
	float max_q;                  	  /*找到当前动作下对应的最大Q值*/   
	int i, c;  
	max_q = d[0];                
	c = 0;   

	for (i = 0; i < 4; i++){   
		if(d[i] > max_q){   
			max_q = d[i];   
			c = i;                    //把Q值最大的对应的动作标号返回
  		}   
	}   
	return c;                        //返回动作标号 
}   

/*最大值函数帮助从当前状态s中选择最大Q值'*/
float max(float b[4]){                                                             
	float max;   
	int i;   
	max = b[0];   
	for(i = 0; i < 4; i++){   //比较选择最大Q值
		if(b[i] > max)  
			max = b[i];    
 	}   
 	return max;     		  //返回最大Q值
}   

/*主函数*/
int main(){   
	int max__q_act(float d[4]);                    /*函数定义*/    
	float max(float b[4]);                                                     

/*
  这有15个状态，14表示的是障碍物，16是环境边界，机器人可以在0-14中移动，指的是下面的next_s数组
  机器人可以从起始状态0运动到终止状态13，对于每一个状态，机器人都有4中动作。0-前，1-左，2-后，3-右.  
*/   

	int r[14][4]={               /*R奖赏矩阵，在每个状态，机器人采取不同的行动将有不同的奖励*/   
		{-1,-300,-300,-100},   	 /*前=-1，左=-300，后=-300，右=-100*/ 
		{-1,-300,-1,-1},   
		{-1,-300,-1,-1},   
		{-300,-300,-1,-1},   
		{-300,-1,-1,-1},   
		{-300,-1,-1,-1},   
		{-300,-1,-1,-300},   
		{-1,-1,-1,-300},   
		{-1,-1,-1,-1},   
		{-1,-1,-1,-1},   
		{-1,-1,-100,-1},   
		{-1,-1,-100,-1},   
		{-1,-1,100,-300},   
		{-1,-100,-300,-300}};   

  	int p[14][4]={       		 /*A动作矩阵，p[s][act]在状态s中指示它可以选择的操作，1表示指示操作可以选择，而0不能选择 */   
		{1,0,0,0},   
		{1,0,1,1},   
		{1,0,1,1},   
		{0,0,1,1},   
		{0,1,1,1},   
		{0,1,1,1},   
		{0,1,1,0},   
		{1,1,1,0},   
		{1,1,1,1},   
		{1,1,1,1},   
		{1,1,0,1},   
		{1,1,0,1},  
		{1,1,1,0},   
		{0,0,0,0}};   
  
	int next_s[14][4]={          /*S状态矩阵，在当前状态s，采取不同的动作将进入下一个状态. */   
		{1,16,16,14},   
		{2,16,0,10},   
		{3,16,1,9},   
		{16,16,2,4},   
		{16,3,9,5},   
		{16,4,8,6},   
		{16,5,7,16},   
		{6,8,12,16},   
		{5,9,11,7},   
		{4,2,10,8},   
		{9,1,14,11},   
		{8,10,14,12},   
		{7,11,13,16},   
		{12,14,16,16}};   

	float g = 0.8;             /*g折扣因子*/   
    float a = 0.1;             /*a学习率*/   
	float max_q_next;   
    int j, k, count, next_state, s, act, step; 	 	/*s:当前状态，next_state下一状态*/
	
	float q[14][4]={                         /*Q值表，初始化为0*/   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0},  
		{0,0,0,0},   
		{0,0,0,0},   
		{0,0,0,0}};   

    srand(4);                       /*设置随机数种子为4*/   

/*Q学习部分*/                                                                  
for(count = 0; count < EPISODES; count++){                     /*EPISODES机器人的学习次数.*/   
	s = INITIAL_STATE;   //初始状态

	while((s != FINAL_STATE) || (s == 14) || (s == 16)){   
	
/*采用贪婪策略选择一个动作:  
对于贪婪策略, e=0.1, 从上次的九步中选择一个随机的动作，选择的这个动作可以使Q值最大*/   
		for(step = 0; step < (STEP-1); step++){             /*选择一个随机动作九次*/    
			act = rand()/(RAND_MAX/4);                      /*随机选择一个动作*/
	        if(p[s][act] == 0)    max_q_next = 0; 			/*如果当前状态选择的动作导致下一个状态没有动作，则下一个状态Q'(s',a')=0*/                                                                                
	        else{
				next_state = next_s[s][act];                /*寻找下一个状态*/
	        	max_q_next = max(q[next_state]);            /*q-table里选最大值*/               
	        }   
			// Q(St,At) = Q(St,At) + a[Rt-1+ymaxQ(St+1,a)-Q(St,At)] = (1-a)Q(St,At) + aRt-1 + aymaxQ(St-1,a)
			q[s][act] = (1-a)*q[s][act] + a*r[s][act] + a*g*max_q_next;         /*更新Q值根据Q值更新公式*/   
	
			printf("qr[%d][%d]=%f.1\n",s,act,q[s][act]);                      
	        s = next_state;									/*更新状态*/ 
	    }                                                            
	
	    if(step == (STEP-1)) {                      		/*得到最大的Q值对应的动作*/   
	    	act = max_q_act(q[s]);   
	    	if(p[s][act] == 0)    max_q_next = 0;   
		    else{
				next_state = next_s[s][act];   
	        	max_q_next = max(q[next_state]);
			}   
			// Q(St,At) = Q(St,At) + a[Rt-1+ymaxQ(St+1,a)-Q(St,At)] = (1-a)Q(St,At) + aRt-1 + aymaxQ(St-1,a)
	  		q[s][act] = (1-a)*q[s][act] + a*r[s][act] + a*g*max_q_next;           /*更新Q值*/   
	
	  		printf("QM[%d][%d]=%f.1\n",s,act,q[s][act]);   
	        s = next_state;
		} 
	}
}   

/*学习结束后输出Q值表*/   
	for(j = 0; j < 14; j++)          
		for(k = 0; k < 4; k++)  
			printf("Q[%d][%d]=%f.1\n",j,k,q[j][k]);   

/*寻找最短路径 :  
从初始状态到最终状态，在每一个状态处，找到动作对应最大的Q值*/   
	s = INITIAL_STATE;   
    act = 0;   
    while((s != FINAL_STATE) || (s == 14) || (s == 16)){   
    	act = max_q_act(q[s]);    
    	printf("state=%daction=%d\n",s,act);   
    	s = next_s[s][act];   
    	getch();
	}   
   	system("PAUSE");   				/*用于观察屏幕结果*/ 
  	return 0;   
}   
