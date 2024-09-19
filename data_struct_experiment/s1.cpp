/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status; 
typedef int ElemType; //数据元素类型定义

#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
typedef struct{  //顺序表（顺序结构）的定义
	ElemType * elem;
	int length;
	int listsize;
}SqList;
typedef struct { 
    char name[30];
    SqList L;	
}ElemL;
typedef struct{  //线性表的管理表定义
    ElemL *elem;
    int length;
    int listsize;
 }LISTS;

status InitList(SqList &L);
status DestroyList(SqList &L);
status ClearList(SqList &L);
status ListEmpty(SqList L);
int ListLength(SqList L);
status GetElem(SqList L,int i,ElemType &e);
int LocateElem(SqList L,ElemType e); //简化过
status PriorElem(SqList L,ElemType e,ElemType &pre);
status NextElem(SqList L,ElemType e,ElemType &next);
status ListInsert(SqList &L,int i,ElemType e);
status ListDelete(SqList &L,int i,ElemType &e);
status ListTrabverse(SqList L);  //简化过
int MaxSubArray(SqList L); 
int SubArrayNum(SqList L, int k);
status sortList(SqList &L);
status  SaveList(SqList L,char FileName[]);
status  LoadList(SqList &L,char FileName[]);
status InitLists(LISTS& L);
status AddList(LISTS &Lists,char ListName[]);
status RemoveList(LISTS &Lists,char ListName[]);
status DestroyLists(LISTS& L);
status  SaveLists(LISTS L,char FileName[]);
status  LoadLists(LISTS &L,char FileName[]);
status Listnames(LISTS L);
int LocateList(LISTS L,char ListName[]);
int SingleListsystem(SqList &L,char name[]);

int main(){
    int op=1,oplist=0;
    LISTS Ls;
    Ls.elem=NULL;
    Ls.length=0;
    Ls.listsize=0;
    while(op){
        status flag;
        int i;
        system("cls");	printf("\n\n");
        printf("      Menu for Multi-Linear Table On Sequence Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. InitLists       5. RemoveList\n");
        printf("    	  2. DestroyLists    6. Listnames\n");
        printf("    	  3. AddList         7. SaveLists\n");
        printf("    	  4. LoadLists       8. SingleListsystem\n");
        printf("    	  0. exit            9. LocateList\n");
        printf("-------------------------------------------------\n");
        if (oplist) {
            if (!Ls.elem) printf("多线性表未创建\n");
            else{
                flag=Listnames(Ls);
                if (flag==0) printf("多线性表是空表！\n");
            }
        } 
        printf("    请选择你的操作[0~9]:");
        scanf("%d",&op);
        switch(op){
        case 1:
            if(InitLists(Ls)==OK) printf("多线性表创建成功！\n");
                else printf("多线性表已存在无法再次创建！如需创建请先删除旧表。\n");
            getchar();getchar();
            break;
        case 2:
            if(DestroyLists(Ls)==OK) printf("多线性表删除成功！\n");  
                else printf("多线性表不存在，无法删除！\n");   
            getchar();getchar();
            break;
        case 3:
            printf("请输入要添加的线性表名：");
            char ListName[30];
            scanf("%s",ListName);
            flag=AddList(Ls,ListName);
            if (flag==OK) printf("添加成功！\n");
                else printf("添加失败！\n");
            getchar();getchar();
            break;
        case 4:
            printf("请输入文件名与路径（以.dat为后缀，默认为.exe所在文件夹）：");
            char FileName[100];
            scanf("%s",FileName);
            flag=LoadLists(Ls,FileName);
            if (flag==INFEASIBLE) printf("多线性表已存在！\n");
                else if (flag==ERROR) printf("读取失败,文件格式错误或不存在！\n");
                    else printf("读取成功！\n");
            getchar();getchar();
            break;
        case 5:
            printf("请输入要删除的线性表名：");
            scanf("%s",ListName);
            flag=RemoveList(Ls,ListName);
            if (flag==INFEASIBLE) printf("多线性表不存在！\n");
                else if (flag==ERROR) printf("删除失败，线性表不存在！\n");
                    else printf("删除成功！\n");
            getchar();getchar();
            break;
        case 6:
            printf("是/否显示表名（1/0）：\n");
            scanf("%d",&oplist);
            getchar();getchar();
            break;
        case 7:
            printf("请输入文件名与路径（以.dat为后缀，默认为.exe所在文件夹）：");
            scanf("%s",FileName);
            flag=SaveLists(Ls,FileName);
            if (flag==INFEASIBLE) printf("多线性表不存在！\n");
                else printf("保存成功！\n");
            getchar();getchar();
            break;
        case 8:
            if (!Ls.elem) {printf("多线性表不存在！\n");getchar();getchar();break;}
            printf("请输入要操作的线性表名：");
            scanf("%s",ListName);
            i=0;
            for(;i<Ls.length;i++)
                if (!strcmp(Ls.elem[i].name,ListName)) break;
            if (i>=Ls.length) printf("线性表不存在！\n");
                else SingleListsystem(Ls.elem[i].L,ListName);
            getchar();getchar();
            break;
        case 9:
            if (!Ls.elem) {printf("多线性表不存在！\n");getchar();getchar();break;}
            printf("请输入要查找的线性表名：");
            scanf("%s",ListName);
            i=LocateList(Ls,ListName);
            if (i==-1) printf("线性表不存在！\n");
                else printf("线性表%s在第%d个位置！\n",ListName,i);
            getchar();getchar();
            break;
        case 0:
            if (Ls.elem) DestroyLists(Ls);//健壮性考虑
            
            break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    getchar();getchar();
    return 0;
}

int SingleListsystem(SqList &L,char name[]){
    int op=1;
    while(op){
        status flag; int n,i,inde;
        ElemType pre_x,next_x,e,x;
        system("cls");	printf("\n\n");
        printf("      Menu for Linear Table %s On Sequence Structure \n",name);
        printf("-------------------------------------------------\n");
        printf("    	  1. InitList       10. ListInsert\n");
        printf("    	  2. DestroyList    11. ListDelete\n");
        printf("    	  3. ClearList      12. ListTrabverse\n");
        printf("    	  4. ListEmpty      13. MaxSubArray\n");
        printf("    	  5. ListLength     14. SubArrayNum\n");
        printf("    	  6. GetElem        15. sortList\n");
        printf("    	  7. LocateElem     16. SaveList\n");
        printf("    	  8. PriorElem      17. LoadList\n");
        printf("    	  9. NextElem       0. exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~17]:");
        scanf("%d",&op);
        switch(op){
        case 1:
            if(InitList(L)==OK) printf("线性表创建成功！\n");
                else printf("线性表已存在无法再次创建！如需创建请先删除旧表。\n");
            getchar();getchar();
            break;
        case 2:
            if(DestroyList(L)==OK) printf("线性表删除成功！\n");  
                else printf("线性表不存在，无法删除！\n");   
            getchar();getchar();
            break;
        case 3:
            if(ClearList(L)) printf("线性表清空成功！\n"); 
                else printf("线性表不存在，无法清空！\n");    
            getchar();getchar();
            break;
        case 4:
            flag=ListEmpty(L);
            if(flag==INFEASIBLE) printf("线性表不存在！\n");
                else {
                    if (flag) printf("线性表是空表！\n");
                        else printf("线性表不是空表！\n");
                }
            getchar();getchar();
            break;
        case 5:
            n=ListLength(L);
            if (n==INFEASIBLE) printf("线性表不存在！\n");
                else printf("线性表的长度是：%d\n",n);
            getchar();getchar();
            break;
        case 6:
            printf("请输入要查找的元素的位置：");
            scanf("%d",&x);
            flag=GetElem(L,x,e);
            if (flag==OK) printf("第%d个元素是：%d\n",x,e);
                else {
                    if (flag==ERROR) printf("查找失败，位置不合法！\n");
                        else printf("查找失败，线性表不存在！\n");
                }
            getchar();getchar();
            break;
        case 7:
            printf("请输入要查找的元素：");
            scanf("%d",&x);
            inde=LocateElem(L,x);
            if (inde==INFEASIBLE) printf("线性表不存在！\n");
                else {
                    if (inde) printf("元素%d在线性表中的位置是：%d\n",x,inde);
                        else printf("元素%d不在线性表中！\n",x);
                }
            getchar();getchar();
            break;
        case 8:
            printf("请输入要查找前驱的元素：");
            scanf("%d",&x);
            flag=PriorElem(L,x,pre_x);
            if (flag==OK) printf("元素%d的前驱是：%d\n",x,pre_x);
                else {
                    if (flag==ERROR) printf("元素%d没有前驱或不在线性表中！\n",x);
                        else printf("线性表不存在！\n");
                }    
            getchar();getchar();
            break;
        case 9:
            printf("请输入要查找后驱的元素：");
            scanf("%d",&x);
            flag=NextElem(L,x,next_x);
            if (flag==OK) printf("元素%d的后驱是：%d\n",x,next_x);
                else {
                    if (flag==ERROR) printf("元素%d没有后驱或不在线性表中！\n",x);
                        else printf("线性表不存在！\n");
                } 
            getchar();getchar();
            break;
        case 10:
            printf("请输入要插入的位置和元素：");
            scanf("%d%d",&i,&e);
            flag=ListInsert(L,i,e);
            if (flag==OK) printf("插入成功！\n");
                else {
                    if (flag==ERROR) printf("插入失败，位置不合法！\n");
                        else printf("插入失败，线性表不存在！\n");
                }   
            getchar();getchar();
            break;
        case 11:
            printf("请输入要删除的位置：");
            scanf("%d",&i);
            flag=ListDelete(L,i,e);
            if (flag==OK) printf("删除成功！\n");
                else {
                    if (flag==ERROR) printf("删除失败，位置不合法！\n");
                        else printf("删除失败，线性表不存在！\n");
                }  
            getchar();getchar();
            break;
        case 12: 
            flag=ListTrabverse(L);
            if (flag==INFEASIBLE) printf("线性表不存在！\n");
                else if(!flag) printf("线性表是空表！\n");
            getchar();getchar();
            break;
        case 13:
            n=MaxSubArray(L);
            if (n==INFEASIBLE) printf("线性表不存在！\n");
                else printf("最大子序列和是：%d\n",n);
            getchar();getchar();
            break;
        case 14:
            printf("请输入要查找的和：");
            scanf("%d",&n);
            i=SubArrayNum(L,n);
            if (i==INFEASIBLE) printf("线性表不存在！\n");
                else printf("和为%d的子序列个数是：%d\n",n,i);
            getchar();getchar();
            break;
        case 15:
            flag=sortList(L);
            if (flag==INFEASIBLE) printf("线性表不存在！\n");
                else printf("排序成功！\n");
            getchar();getchar();
            break;
        case 16:
            printf("请输入文件名与路径（以.dat为后缀，默认为.exe所在文件夹）：");
            char FileName[100];
            scanf("%s",FileName);
            flag=SaveList(L,FileName);
            if (flag==INFEASIBLE) printf("线性表不存在！\n");
                else printf("保存成功！\n");
            getchar();getchar();
            break;
        case 17:
            printf("请输入文件名与路径（以.dat为后缀，默认为.exe所在文件夹）：");
            scanf("%s",FileName);
            flag=LoadList(L,FileName);
            if (flag==INFEASIBLE) printf("线性表已存在！\n");
                else if (flag==ERROR) printf("读取失败,文件格式错误或不存在！\n");
                    else printf("读取成功！\n");
            getchar();getchar();
            break;
        case 0:
            printf("退出成功！");
            break;
        }//end of switch
    }//end of while
    return 0;
}//end of main()

status InitList(SqList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if (L.elem!=NULL) return INFEASIBLE;
    ElemType*a=(ElemType*)malloc(sizeof (ElemType)*LIST_INIT_SIZE);
    while(!a) a=(ElemType*)malloc(sizeof (ElemType)*LIST_INIT_SIZE);
    L.elem=a;
    L.length=0;
    L.listsize=LIST_INIT_SIZE;
    return OK;
}

status DestroyList(SqList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    free(L.elem);
    L.elem=NULL;
    L.length=0;
    L.listsize=0;
    return OK;
}

status ClearList(SqList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    L.length=0;
    return OK;
}

status ListTrabverse(SqList L){
    if (!L.elem) return INFEASIBLE;
    int i;
    printf("\n-----------all elements -----------------------\n");
    for(i=0;i<L.length;i++) printf("%d ",L.elem[i]);
    printf("\n------------------ end ------------------------\n");
    return L.length;
}

status ListEmpty(SqList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    if (L.length) return FALSE;
    else return TRUE;

}

int ListLength(SqList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    return L.length;

}

status GetElem(SqList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    if (i>L.length || i<1) return ERROR;
    e=L.elem[i-1];
    return OK;
}

int LocateElem(SqList L,ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；
{//当线性表L不存在时，返回INFEASIBLE。
    if (!L.elem) return INFEASIBLE;
    ElemType *i=L.elem;
    for(;i<L.elem+L.length;i++)
        if (*i == e) return (i-L.elem+1);
    return 0;
}

status PriorElem(SqList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    ElemType *i=L.elem;
    for(;i<L.elem+L.length-1;i++){
        if (*(i+1) == e) {
            pre=*i;
            return OK;
        }
    }
    return ERROR;
}

status NextElem(SqList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return  INFEASIBLE;
    ElemType* i=L.elem;
    for(;i<L.elem+L.length-1;i++)
        if (*i==e){
            next=*(i+1);
            return OK;
        }
    return ERROR;
}

status ListInsert(SqList &L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    if (i<1 || i>L.length+1) return ERROR; 
    if (L.length+1>L.listsize) {
        ElemType *q=(ElemType *)realloc(L.elem,sizeof(ElemType)*(L.listsize+LISTINCREMENT));
        while (!q) q=(ElemType *)realloc(L.elem,sizeof(ElemType)*(L.listsize+LISTINCREMENT));
        L.elem=q;
        L.listsize=L.listsize+LISTINCREMENT;
    }//若表长不够，需要进行增加操作
    for(int j=L.length;j>=i;j--)
        L.elem[j]=L.elem[j-1];
    L.elem[i-1]=e;
    L.length++;
    return OK;
}

status ListDelete(SqList &L,int i,ElemType &e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    if (i<1 || i>L.length) return ERROR;
    ElemType *a=L.elem;
    e=a[i-1];
    for(int k=i;k<L.length;k++)
        a[k-1]=a[k];
    L.length--;
    return  OK;
}

int MaxSubArray(SqList L)
// 如果线性表L存在，返回线性表L的最大子序列和；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    int max=0,sum=0;
    for(int i=0;i<L.length;i++){
        sum+=L.elem[i];
        if (sum<0) sum=0;
        if (sum>max) max=sum;
    }
    return max;
}

int SubArrayNum(SqList L, int k)
// 如果线性表L存在，返回线性表L中和为k的子序列个数；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    int num=0;
    for(int i=0;i<L.length;i++){
        int sum=0;
        for(int j=i;j<L.length;j++){
            sum+=L.elem[j];
            if (sum==k) num++;
        }
    }
    return num;
}

status sortList(SqList &L)
// 如果线性表L存在，对线性表L进行升序排序，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    int l,r;
    int qu[100000][2],he=0,en=0;
    ElemType *a=L.elem;
    qu[en][0]=0;qu[en++][1]=L.length-1;//采用队列
    while(he<en){
        l=qu[he][0];r=qu[he++][1];
        srand(time(NULL));
        int mid=l+rand()%(r-l+1);//随机化快排
        int i=l,j=r;
        int t=a[mid];
        a[mid]=a[i];
        a[i]=t;
        while(i<j){
            while (a[j]>t && j>i) j--;
            if (i<j) {a[i]=a[j];i++;}  
            while (a[i]<t && j>i) i++;
            if (i<j) {a[j]=a[i];j--;}  
        }
        a[i]=t;//一次划分
        if (l<j-1) {qu[en][0]=l;qu[en++][1]=i-1;};
        if (r>i+1) {qu[en][0]=i+1;qu[en++][1]=r;};
    }
    return OK;
}

status  SaveList(SqList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    FILE *fp=NULL;
    fp=fopen(FileName,"w");
    fprintf(fp,"List\n");
    fprintf(fp,"%d\n",L.length);
    for(int i=0;i<L.length;i++) {
        fprintf(fp,"%d ",L.elem[i]);
    }
    fclose(fp);
    return OK;
}

status  LoadList(SqList &L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
// 若文件格式错误，返回ERROR。
{
    if (L.elem) return INFEASIBLE;
    FILE *fp=NULL;
    fp=fopen(FileName,"r");
    if (!fp) return ERROR;
    char s[10];
    fscanf(fp,"%s",s);
    if (strcmp(s,"List") ) return ERROR;
    fscanf(fp,"%d",&L.length);
    L.elem=(ElemType *)malloc(sizeof (ElemType) * (L.length+1));
    while (!L.elem) L.elem=(ElemType *)malloc(sizeof (ElemType) * (L.length+1));
    for(int i=0;i<L.length;i++)
        fscanf(fp,"%d",&L.elem[i]);
    fclose(fp);
    return OK;
}

status InitLists(LISTS& L)
// 多线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if (L.elem!=NULL) return INFEASIBLE;
    ElemL*a=(ElemL*)malloc(sizeof (ElemL)*LIST_INIT_SIZE);
    while(!a) a=(ElemL*)malloc(sizeof (ElemL)*LIST_INIT_SIZE);
    L.elem=a;
    L.length=0;
    L.listsize=LIST_INIT_SIZE;
    return OK;
}

status AddList(LISTS &Lists,char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表，线性表数据由后台测试程序插入。
{
    if (!Lists.elem) return INFEASIBLE;
    int i=Lists.length;
    if (i==Lists.listsize) {
        ElemL *a=(ElemL *)realloc(Lists.elem,sizeof (ElemL)*(Lists.listsize+LISTINCREMENT));
        while (!a) a=(ElemL *)realloc(Lists.elem,sizeof (ElemL)*(Lists.listsize+LISTINCREMENT));
        Lists.elem=a;
        Lists.listsize+=LISTINCREMENT;
    }
    strcpy(Lists.elem[i].name,ListName);
    Lists.elem[i].L.elem=NULL;
    InitList(Lists.elem[i].L);
    Lists.length++;
    return OK;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    if (!Lists.elem) return INFEASIBLE;
    int i=0;
    for(;i<Lists.length;i++)
        if (!strcmp(Lists.elem[i].name,ListName)) {
            DestroyList(Lists.elem[i].L);
            break;
        }
    if (i>=Lists.length) return ERROR;
    else{
        for(;i<Lists.length;i++){
            Lists.elem[i]=Lists.elem[i+1];
        }
        Lists.length--;
        return OK;
    }
}

status DestroyLists(LISTS& L)
// 如果多线性表L存在，销毁多线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    for(int i=0;i<L.length;i++)
        DestroyList(L.elem[i].L);
    free(L.elem);
    L.elem=NULL;
    L.length=0;
    L.listsize=0;
    return OK;
}

status  SaveLists(LISTS L,char FileName[])
// 如果多线性表L存在，将多线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    FILE *fp=NULL;
    fp=fopen(FileName,"w");
    fprintf(fp,"Lists\n");
    fprintf(fp,"%d\n",L.length);
    for(int i=0;i<L.length;i++) {
        fprintf(fp,"%s\n",L.elem[i].name);
        fprintf(fp,"%d\n",L.elem[i].L.length);
        for(int j=0;j<L.elem[i].L.length;j++) {
            fprintf(fp,"%d ",L.elem[i].L.elem[j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
    return OK;
}

status  LoadLists(LISTS &L,char FileName[])
// 如果多线性表L不存在，将FileName文件中的数据读入到多线性表L中，返回OK，否则返回INFEASIBLE。
// 若文件格式错误，返回ERROR。
{
    if (L.elem) return INFEASIBLE;
    FILE *fp=NULL;
    char s[10];
    fp=fopen(FileName,"r");
    if (!fp) return ERROR;
    fscanf(fp,"%s",s);
    if (strcmp(s,"Lists")) return ERROR;
    fscanf(fp,"%d",&L.length);
    L.elem=(ElemL *)malloc(sizeof (ElemL) * (L.length+1));
    L.listsize=L.length+1;
    while (!L.elem) L.elem=(ElemL *)malloc(sizeof (ElemL) * (L.length+1));
    for(int i=0;i<L.length;i++){
        fscanf(fp,"%s",L.elem[i].name);
        fscanf(fp,"%d",&L.elem[i].L.length);
        L.elem[i].L.elem=(ElemType *)malloc(sizeof (ElemType) * (L.elem[i].L.length+1));
        while (!L.elem[i].L.elem) L.elem[i].L.elem=(ElemType *)malloc(sizeof (ElemType) * (L.elem[i].L.length+1));
        L.elem[i].L.listsize=L.elem[i].L.length+1;
        for(int j=0;j<L.elem[i].L.length;j++)
            fscanf(fp,"%d",&L.elem[i].L.elem[j]);
    }
    fclose(fp);
    return OK;
}

status Listnames(LISTS L)
// 如果多线性表L存在，显示多线性表L的所有线性表名，返回表长；如果多线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    printf("\n----------------all names----------------------\n");
    for(int i=0;i<L.length;i++) printf("%s\n",L.elem[i].name);
    printf("\n------------------ end ------------------------\n");
    return L.length;
}

int LocateList(LISTS L,char ListName[])
// 如果多线性表L存在，查找线性表名为ListName的线性表，返回该线性表在多线性表中的位置序号；如果ListName不存在，返回-1；
{
    if (!L.elem) return INFEASIBLE;
    for(int i=0;i<L.length;i++)
        if (!strcmp(L.elem[i].name,ListName)) return i+1;
    return -1;
}
// first single list
// seceond mainl(&L)
// third main()