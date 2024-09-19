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
typedef struct LNode{  //单链表（链式结构）结点的定义
      ElemType data;
      struct LNode *next;
    }LNode,*LinkList;
typedef struct LsNode{ 
    char name[30];
    LinkList L;
    struct LsNode *next;
}ElemL;
typedef struct{  //线性表的管理表定义
    ElemL *elem;
    int length;
 }LISTS;

status InitList(LinkList &L);
status DestroyList(LinkList &L);
status ClearList(LinkList &L);
status ListEmpty(LinkList L);
int ListLength(LinkList L);
status GetElem(LinkList L,int i,ElemType &e);
int LocateElem(LinkList L,ElemType e); //简化过
status PriorElem(LinkList L,ElemType e,ElemType &pre);
status NextElem(LinkList L,ElemType e,ElemType &next);
status ListInsert(LinkList &L,int i,ElemType e);
status ListDelete(LinkList &L,int i,ElemType &e);
status ListTrabverse(LinkList L);  //简化过
status reverseList(LinkList &L);
status RemoveNthFromEnd(LinkList &L,int n);
status sortList(LinkList &L);
status  SaveList(LinkList L,char FileName[]);
status  LoadList(LinkList &L,char FileName[]);
status InitLists(LISTS& L);
status AddList(LISTS &Lists,char ListName[]);
status RemoveList(LISTS &Lists,char ListName[]);
status DestroyLists(LISTS& L);
status  SaveLists(LISTS L,char FileName[]);
status  LoadLists(LISTS &L,char FileName[]);
status Listnames(LISTS L);
int LocateList(LISTS L,char ListName[]);
int SingleListsystem(LinkList &L,char name[]);

int main(){
    int op=1,oplist=0,i=0;
    LISTS Ls;
    Ls.elem=NULL;
    Ls.length=0;
    while(op){
        status flag;
        ElemL *p=NULL;
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
            p=Ls.elem->next;   
            for(;p;p=p->next)
                if (!strcmp(p->name,ListName)) break;
            if (!p) printf("线性表不存在！\n");
                else {
                    SingleListsystem(p->L,ListName);
                    if (!p->L) RemoveList(Ls,ListName);//状态传递
                }
            getchar();getchar();
            break;
        case 9:
            if (!Ls.elem) {printf("多线性表不存在！\n");getchar();getchar();break;}
            printf("请输入要查找的线性表名：");
            scanf("%s",ListName);
            i=LocateList(Ls,ListName);
            if (i==-1) printf("线性表不存在！\n");
                else printf("线性表%s在多线性表中的位置是：%d\n",ListName,i);
            getchar();getchar();
            break;
        case 0:
            if (Ls.elem) DestroyLists(Ls);//健壮性考虑
            break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}

int SingleListsystem(LinkList &L,char name[]){
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
        printf("    	  4. ListEmpty      13. reverseList\n");
        printf("    	  5. ListLength     14. RemoveNthFromEnd\n");
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
            getchar();getchar();
            break;
        case 13:
            flag=reverseList(L);
            if (flag==INFEASIBLE) printf("线性表不存在！\n");
                else printf("逆置成功！\n");
            break;
        case 14:
            printf("请输入要删除的倒数第几个元素：");
            scanf("%d",&n);
            flag=RemoveNthFromEnd(L,n);
            if (flag==INFEASIBLE) printf("线性表不存在！\n");
                else if (flag==ERROR) printf("删除失败，位置不合法！\n");
                    else printf("删除成功！\n");
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

status InitList(LinkList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if (L) return INFEASIBLE;
    L=(LinkList) malloc(sizeof (LNode));
    (*L).next=NULL;
    return OK;
}

status DestroyList(LinkList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p;
    while(L){
        p=L->next;
        free(L);
        L=p;
    }
    return OK;
}

status ClearList(LinkList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    if (!L) return INFEASIBLE; 
    LinkList p=L->next;
    LinkList q=NULL;
    while(p){
        q=p->next;
        free(p);
        p=q;
    }
    L->next=NULL;
    return OK;
}

status ListTrabverse(LinkList L){
    if (!L) return INFEASIBLE;
    
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    if (!p) {
        printf("线性表是空表！\n");
        return OK;
    }
    printf("\n-----------all elements -----------------------\n");
    while(p->next){
        printf("%d ",p->data);
        p=p->next;
    }
    printf("%d\n",p->data);
    printf("\n------------------ end ------------------------\n");
    return OK;
}

status ListEmpty(LinkList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    if (L->next) return FALSE;
    return TRUE; 
}

int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    int l=0;
    while(p){
        l++;
        p=p->next;
    }
    return l;
}

status GetElem(LinkList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    int j=0;
    if (i<=0) return ERROR;
    while(p){
        j++;
        if (j==i) {e=p->data;return OK;}
        p=p->next;
    }
    return ERROR;
}

int LocateElem(LinkList L,ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；
{//当线性表L不存在时，返回INFEASIBLE。
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    int i=0;
    while(p){
        i++;
        if (p->data==e) return i;
        p=p->next;
    }
    return ERROR;
}

status PriorElem(LinkList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    if (!p) return ERROR;
    LinkList q=L->next->next;
    while(q){//计算第一个有前驱的
        if (q->data==e) {
            pre=p->data;
            return OK;
        }
        p=q;
        q=q->next;
    }
    return ERROR;
}

status NextElem(LinkList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    if (!p) return ERROR;
    LinkList q=p->next;
    while(q){
        if (p->data==e){
            next=q->data;
            return OK;
        }
        p=q;
        q=q->next;
    }
    return ERROR;
}

status ListInsert(LinkList &L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L;
    LinkList s=NULL;
    int j=0;
    while(p){
        if (j==i-1){
            s=(LinkList)malloc(sizeof (LNode));
            s->data=e;
            s->next=p->next;
            p->next=s;
            return OK;
        }
        j++;
        p=p->next;
    }
    return ERROR;
}

status ListDelete(LinkList &L,int i,ElemType &e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    LinkList p=L;
    LinkList q=p->next;
    int j=0;
    while(q){
        j++;
        if (j==i){
            e=q->data;
            p->next=q->next;
            free(q);
            return OK;
        }
        p=q;
        q=q->next;
    }
    return ERROR;
}

status reverseList(LinkList &L){
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    LinkList q=NULL;
    LinkList s=NULL;
    while(p){
        s=p->next;
        p->next=q;
        q=p;
        p=s;
    }
    L->next=q;
    return OK;

}

status RemoveNthFromEnd(LinkList &L,int n){
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    LinkList q=L;
    LinkList s=NULL;
    int l=0;
    while(p){
        l++;
        p=p->next;
    }
    if (n>l) return ERROR;
    p=L->next;
    for(int i=0;i<l-n;i++){
        q=p;
        p=p->next;
    }
    s=p;
    q->next=p->next;
    free(s);
    return OK;
}

status sortList(LinkList &L){
    if (!L) return INFEASIBLE;
    LinkList p=L->next;
    LinkList q=NULL;
    ElemType s;
    p=L->next;
    for(;p->next;p=p->next){
        
        for(q=p->next;q;q=q->next){
            if (p->data>q->data) {
                s=p->data;
                p->data=q->data;
                q->data=s;
            }
        }
    }
    return OK;
}

status  SaveList(LinkList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if (!L) return INFEASIBLE;
    FILE *fp=NULL;
    fprintf(fp,"List\n");
    fp=fopen(FileName,"w");
    LinkList p=L->next;
    int l=0;
    while (p) {
        l++;
        p=p->next;
    }
    fprintf(fp,"%d ",l);
    p=L->next;
    while (p) {
        fprintf(fp,"%d ",p->data);
        p=p->next;
    }
    fclose(fp);
    return OK;
}

status  LoadList(LinkList &L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
// 若文件格式错误或不存在，返回ERROR。
{
    if (L) return INFEASIBLE;
    FILE *fp=NULL;
    fp=fopen(FileName,"r");
    if (!fp) return ERROR;
    char s[10];
    fscanf(fp,"%s",s);
    if (strcmp(s,"List")) return ERROR;  
    L=(LinkList)malloc(sizeof(LNode));
    L->next=NULL;
    LinkList p=L,t=NULL;
    int l;
    ElemType e;
    fscanf(fp,"%d",&l);
    for(int i=1;i<=l;i++){
        fscanf(fp,"%d",&e);
        t=(LinkList)malloc(sizeof (LNode));
        t->data=e;
        t->next=NULL;
        p->next=t;
        p=t;
    }
    fclose(fp);
    return OK;
}

status InitLists(LISTS& L)
// 多线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if (L.elem!=NULL) return INFEASIBLE;
    ElemL*a=(ElemL*)malloc(sizeof (ElemL));
    L.elem=a;
    L.elem->next=NULL;
    L.length=0;
    return OK;
}

status AddList(LISTS &Lists,char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表。
{
    if (!Lists.elem) return INFEASIBLE;
    ElemL *p=Lists.elem;
    while(p->next) p=p->next;
    p->next=(ElemL *)malloc(sizeof (ElemL));
    p=p->next;
    strcpy(p->name,ListName);
    p->L=NULL;
    InitList(p->L);
    p->next=NULL;
    Lists.length++;
    return OK;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    if (!Lists.elem) return INFEASIBLE;
    ElemL *p=Lists.elem->next;
    ElemL *q=Lists.elem;
    for(;p;q=p,p=p->next)
        if (!strcmp(p->name,ListName)) {
            DestroyList(p->L);
            q->next=p->next;
            free(p);
            Lists.length--; 
            return OK;
        }
    return ERROR;
}

status DestroyLists(LISTS& L)
// 如果多线性表L存在，销毁多线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    ElemL *p=L.elem;
    ElemL *q=NULL;
    while(p){
        q=p->next;
        DestroyList(p->L);
        free(p);
        p=q;
    }
    L.elem=NULL;
    L.length=0;
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
    ElemL *p=L.elem->next;
    for(int i=0;i<L.length;i++){
        fprintf(fp,"%s\n",p->name);
        fprintf(fp,"%d\n",ListLength(p->L));
        LinkList q=p->L->next;
        while(q){
            fprintf(fp,"%d ",q->data);
            q=q->next;
        }
        fprintf(fp,"\n");
        p=p->next;
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
    ElemL *p=(ElemL*)malloc(sizeof (ElemL));
    L.elem=p;
    p->L=NULL;
    p->next=NULL;
    ElemL *q=NULL;
    for(int i=0;i<L.length;i++){
        q=(ElemL*)malloc(sizeof (ElemL));
        fscanf(fp,"%s",q->name);
        q->L=NULL;
        q->next=NULL;
        InitList(q->L);
        p->next=q;
        p=q;
        int l;
        fscanf(fp,"%d",&l);
        ElemType e;
        for(int j=1;j<=l;j++){
            fscanf(fp,"%d",&e);
            ListInsert(q->L,j,e);
        }
    }
    fclose(fp);
    return OK;
}

int LocateList(LISTS L,char ListName[])
// 如果多线性表L存在，查找线性表名ListName在多线性表L中的位置序号并返回该序号；如果ListName不存在，返回-1；
{
    if (!L.elem) return INFEASIBLE;
    ElemL *p=L.elem->next;
    int i=0;
    while(p){
        i++;
        if (!strcmp(p->name,ListName)) return i;
        p=p->next;
    }
    return -1;
}

status Listnames(LISTS L)
// 如果多线性表L存在，输出多线性表L的所有线性表名，返回OK；如果多线性表L不存在，返回INFEASIBLE。
{
    if (!L.elem) return INFEASIBLE;
    printf("\n----------------all names----------------------\n");
    ElemL *p=L.elem->next;
    if (!p) {
        printf("多线性表是空表！\n");
        return OK;
    }
    while(p){
        printf("%s\n",p->name);
        p=p->next;
    }
    printf("\n------------------ end ------------------------\n");
    return OK;
}
// first single list
// seceond mainl(&L)
// third main()