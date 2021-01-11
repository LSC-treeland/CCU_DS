#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//node��T
struct Node{
    int id;
    struct ListNode *children;
    struct Node *parent;
};

//node�Ulinked list�`�I
struct ListNode{
    struct Node *data;
    struct ListNode *next;
};

//insert listnode
struct ListNode *L_insert(struct ListNode *head,struct Node *child){
    struct ListNode *new_child=malloc(sizeof(struct ListNode));
    new_child->next=head;
    new_child->data=child;
    return new_child;
}

//free�C�Ӹ`�I�W��listnode
void L_free(struct Node *ptr,int nodes_cnt){
    struct ListNode *tmp_child=NULL;
    for(int i=0;i<nodes_cnt+1;i++){
        while(ptr->children){
            tmp_child=ptr->children;
            ptr->children=ptr->children->next;
            free(tmp_child);
        }
        ptr++;
    }
}

//insert ans
void Ans_insert(int *ans,struct Node *ptr,int nodes_cnt){
    for(int i=0;i<nodes_cnt;i++){
        *ans=ptr->parent->id;
        ptr++;
        ans++;
    }
}


int main()
{
    int nodes_cnt;
    scanf("%d",&nodes_cnt);
    int offset;
    int switched_cnt=0;
    int round=0;
    int result[nodes_cnt];
    int switched[nodes_cnt];
    memset(switched,0,sizeof(switched));
    int tmp_switched[nodes_cnt];
    memset(tmp_switched,0,sizeof(tmp_switched));
    int **ans=malloc(sizeof(int *)*1000);
    int **tmp_ans=ans;
    int *tmp=NULL;
    struct Node *NodePtr=calloc(nodes_cnt+1,sizeof(struct Node));
    struct Node *ptr=NodePtr+1;

    //����child parent���Y
    NodePtr->id=-1;
    for(int i=0;i<nodes_cnt;i++){
        scanf("%d",&offset);
        ptr->id=i;
        ptr->parent=NodePtr+offset+1;
        ptr->parent->children=L_insert(ptr->parent->children,ptr);
        ptr++;
    }

    for(int i=0;i<nodes_cnt;i++){
        scanf("%d",&result[i]);
    }

    /*
        result[] �C���I�̲�parent
        switched[] �����C���I���ഫ�L�{
        switched[]=0 ����sparent
        switched[]=1 �w��sparent
        switched[]=-1 �U����child�w��sparent
        switched_cnt �����wswitehed���I�A�j����X����
    */

    //�w�]�̫��I�w��sparent
    switched[nodes_cnt-1]=1;
    switched_cnt++;

    round++;
    *tmp_ans=malloc(sizeof(int)*nodes_cnt);
    Ans_insert(*tmp_ans,NodePtr+1,nodes_cnt);

    while(switched_cnt<nodes_cnt){

        for(int i=0;i<nodes_cnt;i++){
            tmp_switched[i]=switched[i];
        }

    //���X�C���I�A������1round
        for(int i=0;i<nodes_cnt;i++){
            if(switched[i]==1){
                for(int j=0;j<nodes_cnt;j++){
                    if(result[j]==i){
                        (NodePtr+j+1)->parent=NodePtr+i+1;
                        switched_cnt++;
                        tmp_switched[j]=1;
                    }
                }
                tmp_switched[i]=-1;
            }
        }

        for(int i=0;i<nodes_cnt;i++){
            switched[i]=tmp_switched[i];
        }
        round++;
        tmp_ans++;
        *tmp_ans=malloc(sizeof(int)*nodes_cnt);
        Ans_insert(*tmp_ans,NodePtr+1,nodes_cnt);
    }

    printf("%d\n",round);
    tmp_ans=ans;
    for(int i=0;i<round;i++){
        tmp=*tmp_ans;
        for(int j=0;j<nodes_cnt;j++){
            printf("%d ",*tmp++);
        }
        printf("\n");
        tmp_ans++;
    }
    for(int i=0;i<round;i++){
        tmp_ans=ans;
        ans++;
        free(*tmp_ans);
    }
    L_free(NodePtr,nodes_cnt);
    free(NodePtr);
}
