#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>

//判斷兩點有無link
int link(double x1,double y1,double x2,double y2)
{
    if((double)(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<1) return 1;
    return 0;
}

//node資訊
struct node{
    int id;
    double x;
    double y;
    double val;
    int neighbor_cnt; //點有幾個neighbor
};

int main()
{
    int node=0,round=0;
    scanf("%d %d",&node,&round);
    struct node info[node];
    int neighbor_table[node][node]; //talbe紀錄點之間是否為neighbor
    for(int i=0;i<node;i++){        //初始化
        for(int j=0;j<node;j++){
            neighbor_table[i][j]=0;
        }
    }

    for(int i=0;i<node;i++){
        scanf("%d %lf %lf %lf",&info[i].id,&info[i].x,&info[i].y,&info[i].val);
        info[i].neighbor_cnt=0;
    }

    for(int i=0;i<node;i++){
        for(int j=i;j<node;j++){
            if(i!=j&&link(info[i].x,info[i].y,info[j].x,info[j].y)){
                info[i].neighbor_cnt++;
                info[j].neighbor_cnt++;
                neighbor_table[i][j]=1;
                neighbor_table[j][i]=1;
            }
        }
    }
    /*
    自己不算neighbor
    如果點a和b為neighbor,點b和c為neighbor
    table:
      a b c
    a 0 1 0
    b 1 0 1
    c 0 1 0
    點a->點b->點c
    過程:
      a b c         neighbor_cnt
    a 0 1 0         a 1
    b 1         =>  b 1
    c 0             c 0
    ----------------------------
      a b c         neighbor_cnt
    a 0 1 0         a 1
    b 1 0 1     =>  b 2
    c 0 1           c 1
    ----------------------------
      a b c         neighbor_cnt
    a 0 1 0         a 1
    b 1 0 1     =>  b 2
    c 0 1 0         c 1
    */

    /*  印出每點的neighbor數
        for(int i=0;i<node;i++){
        printf("id:%d neighbor:%d\n",info[i].id,info[i].neighbor_cnt);
    }
        印出neighbor_table
    for(int i=0;i<node;i++){
        for(int j=0;j<node;j++){
            printf("%d ",neighbor_table[i][j]);
        }
        printf("\n");
    }*/
    double new_val[node];
    memset(new_val,0,node*sizeof(double));
    int max_degree=0;
    double neighbor_ratio=0; //neighbor占的總權重
    printf("%d %d\n",node,round);
    for(int i=0;i<node;i++){
        printf("%.2f ",info[i].val);
    }
    printf("\n");
    for(int cnt=0;cnt<round-1;cnt++){
        for(int i=0;i<node;i++){
            neighbor_ratio=0;
            for(int j=0;j<node;j++){
                if(neighbor_table[i][j]==1){ //判斷是neighbor後,找出max_degree
                    max_degree=info[i].neighbor_cnt;
                    if(info[j].neighbor_cnt>max_degree){
                        max_degree=info[j].neighbor_cnt;
                    }
                    new_val[i]+=info[j].val/(max_degree*2); //加上neighbor_val乘權重
                    neighbor_ratio+=(double)1/(max_degree*2); //加上neighbor權重
                }
            }
            new_val[i]+=(1-neighbor_ratio)*info[i].val; //加上自己val乘剩下的權重
            //printf("%d %d\n",max_degree,neighbor_sum);
        }
        for(int i=0;i<node;i++){ //印出value,並讓newvalue取代原本value
            printf("%.2f ",new_val[i]);
            info[i].val=new_val[i];
            new_val[i]=0;
        }
        printf("\n");
    }
}
