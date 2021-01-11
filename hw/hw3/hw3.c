#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define INF 999999

struct Switch_Array
{
    int Id;
    int Capacity;
};

struct Rule_Array
{
    char *Src;
    char *Dst;
    char *Action;
    int *S_row;
    int *E_row;
    int *S_col;
    int *E_col;
    int Total;
};

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

//bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}


//將row(col)的起點和終點轉二進位並寫入token
//例如 bits = 2 , row 0~1 ,token = 0*
void Decimal2Binary(int *S, int *E, int Bits, char *token)
{
    int S_tmp = *S;
    int E_tmp = *E;
    char start[Bits];
    char end[Bits];
    char *ptr = token;
    int cnt = Bits - 1;
    for (int i = pow(2, cnt); i >= 1; i /= 2)
    {
        if (S_tmp >= i)
        {
            S_tmp -= i;
            start[cnt] = 1;
        }
        else
            start[cnt] = 0;

        if (E_tmp >= i)
        {
            E_tmp -= i;
            end[cnt] = 1;
        }
        else
            end[cnt] = 0;
        cnt--;
    }
    for (int i = Bits - 1; i >= 0; i--)
    {
        if (start[i] == end[i])
        {
            if (start[i] == 1)
                *ptr++ = '1';
            else
                *ptr++ = '0';
        }
        else
        {
            *ptr++ = '*';
            break;
        }
    }
    *ptr = '\0';
}

//將token轉十進位並寫入row(col)的起點和終點
//例如 bits = 2 , token = 0* ,起點 = 0,終點 = 1
void Binary2Decimal(char *token, int Bits, int *S, int *E)
{
    *S = 0;
    *E = 0;
    for (int i = Bits; i > 0; i--)
    {
        if (*token == '1')
        {
            *S += pow(2, i - 1);
            *E += pow(2, i - 1);
        }
        else if (*token == '*')
        {
            for (int j = i; j > 0; j--)
            {
                *E += pow(2, j - 1);
            }
            break;
        }
        token++;
    }
}

//取得token
char *GetToken(char *line, char *token)
{
    char *ptr = line;
    char *qtr = token;
    token[0] = '\0';
    while (isspace(*ptr))
        ptr++;
    while (!isspace(*ptr) && *ptr != '\0')
    {
        *qtr++ = *ptr++;
    }
    *qtr = '\0';
    if (token[0] == '\0')
        return NULL;
    return ptr;
}

void SetGraph(int n_cnt, int Graph[n_cnt][n_cnt])
{
    for (int i = 0; i < n_cnt; i++)
    {
        for (int j = 0; j < n_cnt; j++)
        {
            Graph[i][j] = INF;
        }
    }
}

void SetTable(int LenTable, int Src_Dst_Table[LenTable][LenTable], int Rule_Cnt)
{
    for (int i = 0; i < LenTable; i++)
    {
        for (int j = 0; j < LenTable; j++)
        {
            Src_Dst_Table[i][j] = Rule_Cnt;
        }
    }
}

int *Dijkstra(int n_cnt, int G[n_cnt][n_cnt], int Ingress, int Engress, int *length)
{
    //Mind[i],從起點到i的最短距離
    //pred[i],i的前一個switch的id
    int new_seq[n_cnt];
    int old_seq[n_cnt];
    int MinD[n_cnt], pred[n_cnt];
    int Visited[n_cnt], cnt, mindist, nextnode;
    for (int i = 0; i < n_cnt; i++)
    {
        MinD[i] = G[Ingress][i];
        pred[i] = Ingress;
        Visited[i] = 0;
    }
    MinD[Ingress] = 0;
    Visited[Ingress] = 1;
    cnt = 1;

    while (cnt < n_cnt - 1)
    {
        mindist = INF;
        for (int i = 0; i < n_cnt; i++)
        {
            if (MinD[i] < mindist && !Visited[i])
            {
                mindist = MinD[i];
                nextnode = i;
            }
        }
        //check if a better path exists through nextnode
        Visited[nextnode] = 1;
        for (int i = 0; i < n_cnt; i++)
        {
            if (!Visited[i])
            {

                //若ingress到nextnode的最短距離加上nextnode到點i的距離
                //小於ingress到點i最短距離，則更新
                if (mindist + G[nextnode][i] < MinD[i])
                {
                    MinD[i] = mindist + G[nextnode][i];
                    pred[i] = nextnode;
                }
                //如果兩條路徑相同
                //計算hop，並將沿途switch id 存到陣列
                else if (mindist + G[nextnode][i] == MinD[i])
                {
                    int new_path = nextnode, old_path = pred[i];
                    //printf("%d %d\n",nextnode,i);
                    //printf("%d %d\n",new_path,old_path);
                    int hop_new = 0, hop_old = 0;
                    while (new_path != Ingress)
                    {
                        new_seq[hop_new] = new_path;
                        new_path = pred[new_path];
                        hop_new++;
                    }
                    while (old_path != Ingress)
                    {
                        old_seq[hop_old] = old_path;
                        old_path = pred[old_path];
                        hop_old++;
                    }
                    if (hop_old > hop_new)
                        pred[i] = nextnode;
                    else if (hop_old == hop_new)
                    {
                        bubbleSort(new_seq, hop_new);
                        bubbleSort(old_seq, hop_old);

                        for (int x = 0; x < hop_old; x++)
                        {
                            if (new_seq[x] != old_seq[x])
                            {
                                if (new_seq[x] < old_seq[x])
                                {
                                    pred[i] = nextnode;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        cnt++;
    }
    /*
    int j;
    for(int i=0;i<n_cnt;i++){
        if(i!=Ingress){
            printf("\nDistance of node%d=%d",i,MinD[i]);
            printf("\nPath=%d",i);
            j=i;
            while(j!=Ingress){
                j=pred[j];
                printf("<-%d",j);
            }
        }
        printf("\n");
    }
    */
    //儲存從engress到ingress的路徑並反轉
    int *result = malloc(sizeof(int) * n_cnt);
    int tmp;
    int *ptr = result;
    *length = 0;
    while (Engress != Ingress)
    {
        *length += 1;
        *ptr++ = Engress;
        Engress = pred[Engress];
    }
    *length += 1;
    *ptr++ = Ingress;
    *ptr = -1;
    for (int i = 0; i < *length / 2; i++)
    {
        tmp = *(result + i);
        *(result + i) = *(result + *length - i - 1);
        *(result + *length - i - 1) = tmp;
    }
    /*
    for(int i = 0;i < *length;i++){
        printf("%d ",*(result + i));
    }
    printf("\n");
    */
    return result;
}

int main()
{
    int Bits;
    scanf("%d", &Bits);
    int Ingress, Engress;
    scanf("%d %d", &Ingress, &Engress);
    int Switch_Cnt;
    scanf("%d", &Switch_Cnt);
    struct Switch_Array Switch[Switch_Cnt];
    for (int i = 0; i < Switch_Cnt; i++)
    {
        scanf("%d %d", &(Switch + i)->Id, &(Switch + i)->Capacity);
    }
    int Link_Cnt;
    scanf("%d", &Link_Cnt);
    int Graph[Switch_Cnt][Switch_Cnt];
    SetGraph(Switch_Cnt, Graph);
    int Id1, Id2, W;
    for (int i = 0; i < Link_Cnt; i++)
    {
        scanf("%d %d %d", &Id1, &Id2, &W);
        Graph[i][i] = 0;
        Graph[Id1][Id2] = W;
        Graph[Id2][Id1] = W;
    }
    int Rule_Cnt;
    scanf("%d", &Rule_Cnt);
    struct Rule_Array Rule[Rule_Cnt];
    struct Rule_Array Rule_tmp[Rule_Cnt];
    char *line = malloc(sizeof(char) * 256);
    char *token = malloc(sizeof(char) * 32);
    char *ptr = NULL;
    fgets(line, 256, stdin);
    for (int i = 0; i < Rule_Cnt; i++)
    {
        fgets(line, 256, stdin);
        ptr = line;
        Rule[i].Src = malloc(sizeof(char) * 32);
        Rule[i].Dst = malloc(sizeof(char) * 32);
        Rule[i].Action = malloc(sizeof(char) * 10);
        Rule_tmp[i].Src = malloc(sizeof(char) * 32);
        Rule_tmp[i].Dst = malloc(sizeof(char) * 32);
        Rule_tmp[i].Action = malloc(sizeof(char) * 10);
        ptr = GetToken(ptr, token);
        strcpy(Rule[i].Src, token);
        ptr = GetToken(ptr, token);
        strcpy(Rule[i].Dst, token);
        ptr = GetToken(ptr, token);
        strcpy(Rule[i].Action, token);
    }
    //table長度
    int LenTable = pow(2, Bits);
    int *length = malloc(sizeof(int));
    int *Rounting = Dijkstra(Switch_Cnt, Graph, Ingress, Engress, length);
    int Src_Dst_Table[LenTable][LenTable];

    int inner;
    int change;
    int cnt;
    float Max_Utinity, utinity, Internal, Overlap;
    //要cover的row(col)的起點終點
    int *S_row = malloc(sizeof(int));
    int *E_row = malloc(sizeof(int));
    int *S_col = malloc(sizeof(int));
    int *E_col = malloc(sizeof(int));
    //visted存每個rule的出現次數
    //cover存rule的狀態
    //internal 1
    //overlap 0
    //outside -1
    int Visited[Rule_Cnt], Cover[Rule_Cnt];
    //1. with the more internal rule IDs
    int max_IDs, IDs;
    //2. with the smaller area size
    int min_area, area;
    //3. with the smaller internal rule ID in the sequence
    int max_internal;
    int min_seq[Rule_Cnt];
    int seq[Rule_Cnt];

    printf("%d\n", *length);
    for (int i = 0; i < Rule_Cnt; i++)
    {
        Rule[i].S_row = malloc(sizeof(int));
        Rule[i].E_row = malloc(sizeof(int));
        Rule[i].S_col = malloc(sizeof(int));
        Rule[i].E_col = malloc(sizeof(int));
        Rule_tmp[i].S_row = malloc(sizeof(int));
        Rule_tmp[i].E_row = malloc(sizeof(int));
        Rule_tmp[i].S_col = malloc(sizeof(int));
        Rule_tmp[i].E_col = malloc(sizeof(int));
    }
    while (*Rounting != -1)
    {
        memset(Cover, 0, sizeof(Cover));
        SetTable(LenTable, Src_Dst_Table, Rule_Cnt);
        cnt = 0;
        Max_Utinity = 0;
        max_IDs = 0;
        min_area = INF;
        max_internal = 0;
        for (int i = 0; i < Rule_Cnt; i++)
        {
            Binary2Decimal(Rule[i].Src, Bits, Rule[i].S_row, Rule[i].E_row);
            Binary2Decimal(Rule[i].Dst, Bits, Rule[i].S_col, Rule[i].E_col);
            if (i != Rule_Cnt - 1)
            {
                Rule[i].Total = 0;
                for (int j = *Rule[i].S_row; j <= *Rule[i].E_row; j++)
                {
                    for (int k = *Rule[i].S_col; k <= *Rule[i].E_col; k++)
                    {
                        if (Src_Dst_Table[j][k] == Rule_Cnt)
                        {
                            Src_Dst_Table[j][k] = i + 1;
                            Rule[i].Total++;
                            Rule[Rule_Cnt - 1].Total--;
                        }
                    }
                }
            }
            //printf("%s\t%s\t%s\n",Rule[i].Src,Rule[i].Dst,Rule[i].Action);
        }
        /*
        for(int j = 0;j < LenTable;j++){
            for(int k = 0;k < LenTable;k++){
                printf("%d ",Src_Dst_Table[j][k]);
            }
            printf("\n");
        }
        產生下列結果
        1 6 6 3
        1 6 2 2
        5 5 6 3
        4 4 4 3
        */
        //i row 1,2,4....
        //j col 1,2,4....
        for (int i = 1; i <= LenTable; i *= 2)
        {
            for (int j = 1; j <= LenTable; j *= 2)
            {
                //k l 用同一個框遍歷整個table
                for (int k = 0; k < LenTable; k += i)
                {
                    for (int l = 0; l < LenTable; l += j)
                    {
                        memset(Visited, 0, sizeof(Visited));
                        Internal = 0;
                        Overlap = 0;
                        int m, n;
                        //m n 遍歷框內
                        for (m = 0; m < i; m++)
                        {
                            for (n = 0; n < j; n++)
                            {
                                //printf("%d\t%d\n",k + m,l + n);
                                Visited[Src_Dst_Table[k + m][l + n] - 1]++;
                            }
                        }
                        //internal 和 overlap
                        for (int x = 0; x < Rule_Cnt; x++)
                        {
                            if (Visited[x] != 0)
                            {
                                if (k <= *Rule[x].S_row && *Rule[x].E_row <= (k + m - 1) && l <= *Rule[x].S_col && *Rule[x].E_col <= (l + n - 1))
                                {
                                    Internal++;
                                }
                                else
                                    Overlap++;
                            }
                        }
                        utinity = (Internal - 1) / (Overlap + 1);
                        //printf("%f\t%f\t%f\n",Internal - 1,Overlap + 1,utinity);
                        if (utinity >= Max_Utinity && utinity > 0)
                        {
                            //IDs 框內有幾個internal rule
                            //inner 框內有幾個rule
                            change = 0;
                            IDs = 0;
                            area = m * n;
                            inner = 0;
                            for (int x = 0; x < Rule_Cnt; x++)
                            {
                                if (Visited[x] != 0)
                                    inner++;
                                if (Visited[x] == Rule[x].Total)
                                    seq[IDs++] = x + 1;
                            }
                            //若Switch容量夠
                            if (inner <= Switch[*Rounting].Capacity)
                            {
                                if (utinity > Max_Utinity)
                                {
                                    Max_Utinity = utinity;
                                    change = 1;
                                }
                                //tie breaking
                                else if (utinity == Max_Utinity)
                                {
                                    if (IDs > max_IDs)
                                        change = 1;
                                    else if (IDs == max_IDs)
                                    {
                                        if (area < min_area)
                                            change = 1;
                                        else if (area == min_area)
                                        {
                                            bubbleSort(min_seq, IDs);
                                            bubbleSort(seq, IDs);
                                            for (int x = 0; x < IDs; x++)
                                            {
                                                if (min_seq[x] > seq[x])
                                                {
                                                    break;
                                                    change = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (change)
                            {
                                cnt = 0;
                                memset(Cover, 0, sizeof(Cover));
                                for (int x = 0; x < IDs; x++)
                                {
                                    min_seq[x] = seq[x];
                                }
                                for (int x = 0; x < Rule_Cnt; x++)
                                {
                                    if (Visited[x] == 0)
                                    {
                                        Cover[x] = -1;
                                    }
                                    else
                                    {
                                        if (Visited[x] == Rule[x].Total)
                                            Cover[x] = 1;
                                        cnt++;
                                    }
                                }
                                *S_row = k;
                                *E_row = k + m - 1;
                                *S_col = l;
                                *E_col = l + n - 1;
                                max_IDs = IDs;
                                min_area = area;
                                max_internal = Internal;
                            }
                        }
                        //printf("%d %d %d %d\n",*S_row,*E_row,*S_col,*E_col);
                        //printf("%d %d %d\n",max_IDs,min_area,min_seq);
                    }
                }
            }
        }
        if (Max_Utinity == 0)
        {
            printf("%d 0\n", *Rounting);
        }
        else
        {
            printf("%d %d\n", *Rounting, cnt);
            Decimal2Binary(S_row, E_row, Bits, Rule_tmp[0].Src);
            Decimal2Binary(S_col, E_col, Bits, Rule_tmp[0].Dst);
            strcpy(Rule_tmp[0].Action, "Fwd");
            //rule的row(col)超出框框，則print框框的起點和終點
            for (int i = 0; i < Rule_Cnt; i++)
            {
                if (Cover[i] != -1)
                {
                    if (strcmp(Rule[i].Src, "*") == 0 && strcmp(Rule[i].Dst, "*") == 0)
                    {
                        printf("%s %s %s\n", Rule_tmp[0].Src, Rule_tmp[0].Dst, Rule[i].Action);
                    }
                    else if (*S_row <= *Rule[i].S_row && *Rule[i].E_row <= *E_row && *S_col <= *Rule[i].S_col && *Rule[i].E_col <= *E_col)
                    {
                        printf("%s %s %s\n", Rule[i].Src, Rule[i].Dst, Rule[i].Action);
                    }
                    else if (*S_row <= *Rule[i].S_row && *Rule[i].E_row <= *E_row)
                    {
                        printf("%s %s %s\n", Rule[i].Src, Rule_tmp[0].Dst, Rule[i].Action);
                    }
                    else if (*S_col <= *Rule[i].S_col && *Rule[i].E_col <= *E_col)
                    {
                        printf("%s %s %s\n", Rule_tmp[0].Src, Rule[i].Dst, Rule[i].Action);
                    }
                }
            }
            /*
            for(int i = 0;i< Rule_Cnt ;i++){
                printf("%d ",Cover[i]);
            }
            printf("\n");
            */
            /*
            for(int i = 0;i < Rule_Cnt;i++){
                printf("%s %s %s\n",Rule[i].Src,Rule[i].Dst,Rule[i].Action);
            }
            */
            //產生下一次的table，cover = 1(internal)會去掉
            for (int i = 0, j = 1; i < Rule_Cnt; i++)
            {
                if (Cover[i] != 1)
                {
                    strcpy(Rule_tmp[j].Src, Rule[i].Src);
                    strcpy(Rule_tmp[j].Dst, Rule[i].Dst);
                    strcpy(Rule_tmp[j].Action, Rule[i].Action);
                    j++;
                }
            }
            Rule_Cnt -= max_internal - 1;
            //printf("%d %d %d %d\n",*S_row,*E_row,*S_col,*E_col);
            for (int i = 0; i < Rule_Cnt; i++)
            {
                strcpy(Rule[i].Src, Rule_tmp[i].Src);
                strcpy(Rule[i].Dst, Rule_tmp[i].Dst);
                strcpy(Rule[i].Action, Rule_tmp[i].Action);
                //printf("%s %s %s\n",Rule[i].Src,Rule[i].Dst,Rule[i].Action);
            }
        }
        /*
        for(int i = 0;i< Rule_Cnt ;i++){
            printf("%d ",Cover[i]);
        }
        printf("\n");
        */
        //printf("%d %d\n",Rule_Cnt,max_internal);
        Rounting++;
    }
}
