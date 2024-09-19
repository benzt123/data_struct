#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int status;
typedef int KeyType; 
typedef struct {
    KeyType  key;
    char others[20];
} TElemType; //二叉树结点类型定义


typedef struct BiTNode{  //二叉链表结点的定义
	TElemType  data;
	struct BiTNode *lchild,*rchild;
} BiTNode, *BiTree;

TElemType enulldata={0,"NULL"};//表示未定义的子节点
BiTree enull;//表示未定义的子节点

typedef struct BTsNode{
    BiTree data;
    char name[20];
    struct BTsNode * next; 
} BTsNode,*BTrees;
//函数声明
status CreateBiTree(BiTree &T,TElemType definition[]);
status DestroyBiTree(BiTree &T);
status ClearBiTree(BiTree T);
status BiTreeEmpty(BiTree T);
int BiTreeDepth(BiTree T);
BiTNode* LocateNode(BiTree T,KeyType e);
status Assign(BiTree &T,KeyType e,TElemType value);
BiTNode* GetSibling(BiTree T,KeyType e);
status InsertNode(BiTree &T,KeyType e,int LR,TElemType c);
status DeleteNode(BiTree &T,KeyType e);
void visit(BiTree T);
status PreOrderTraverse(BiTree T,void (*visit)(BiTree));
status InOrderTraverse(BiTree T,void (*visit)(BiTree));
status PostOrderTraverse(BiTree T,void (*visit)(BiTree));
status LevelOrderTraverse(BiTree T,void (*visit)(BiTree));
KeyType MaxPathSum(BiTree T);
BiTNode* LowestCommonAncestor(BiTree T,KeyType e1,KeyType e2);
status InvertTree(BiTree &T);
status SaveBiTree(BiTree T, char FileName[]);
status LoadBiTree(BiTree &T,char FileName[]);

status InitTrees(BTrees& T);
status DestroyTrees(BTrees &T);
status AddTree(BTrees &T,char name[]);
status RemoveTree(BTrees &T,char name[]);
status ListTrees(BTrees T);
status SaveTrees(BTrees T, char FileName[]);
status LoadTrees(BTrees &T,char FileName[]);
int treeoption(BiTree &T,char name[]);

int main(){
    enull=(BiTree)malloc(sizeof(BiTNode));
    enull->data=enulldata;
    enull->lchild=NULL;
    enull->rchild=NULL;
    int op=1,oplist=0;
    BTrees Ts=NULL;
    while(op){
        status flag;
        char name[100];
        BTrees p;
        system("cls");	printf("\n\n");
        printf("      Menu for Binary Trees \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. initTrees       5. ListTrees\n");
        printf("    	  2. DestroyTrees    6. LoadTrees\n");
        printf("    	  3. AddTree         7. SaveTrees\n");
        printf("    	  4. RemoveTree      8. Treeoption\n");
        printf("    	  0. exit\n");
        printf("-------------------------------------------------\n");
        if (oplist) {
            flag=ListTrees(Ts);
            if (flag==INFEASIBLE) printf("森林不存在\n");
        } 
        printf("    请选择你的操作[0~8]:");
        scanf("%d",&op);
        switch(op){
        case 1:
            flag=InitTrees(Ts);
            if (flag==OK) printf("初始化成功\n");
            else printf("初始化失败,森林已存在\n");
            getchar();getchar();
            break;
        case 2:
            flag=DestroyTrees(Ts);
            if (flag==OK) printf("销毁成功\n");
            else printf("销毁失败,森林不存在\n");
            getchar();getchar();
            break;
        case 3:
            printf("请输入树的名字:");
            scanf("%s",name);
            flag=AddTree(Ts,name);
            if (flag==OK) printf("添加成功\n");
            else printf("添加失败,森林不存在\n");
            getchar();getchar();
            break;
        case 4:
            printf("请输入树的名字:");
            scanf("%s",name);
            flag=RemoveTree(Ts,name);
            if (flag==OK) printf("删除成功\n");
            else printf("删除失败,森林不存在或树不存在\n");
            getchar();getchar();
            break;
        case 5:
            printf("是否展示所有树(1表示是，0表示否，默认为0):");
            scanf("%d",&oplist);
            break;
        case 6:
            printf("请输入文件名:");
            scanf("%s",name);
            flag=LoadTrees(Ts,name);
            if (flag==OK) printf("读取成功\n");
            else printf("读取失败,格式错误或文件不存在\n");
            getchar();getchar();
            break;
        case 7:
            printf("请输入文件名:");
            scanf("%s",name);
            flag=SaveTrees(Ts,name);
            if (flag==OK) printf("保存成功\n");
            else printf("保存失败,森林不存在\n");
            getchar();getchar();
            break;
        case 8:
            if(!Ts) printf("森林不存在\n");
            else{
                printf("请输入树的名字:");
                scanf("%s",name);
                p=Ts->next;
                while(p && strcmp(p->name,name)) p=p->next;
                if (!p) printf("树不存在\n");
                else {
                    treeoption(p->data,name);
                    if (!p->data) RemoveTree(Ts,name);//销毁的传递性
                }
            }
            getchar();getchar();
            break;
        case 0:
            if (Ts) DestroyTrees(Ts);//健壮性考虑
            break;
        }//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
    free(enull);
}

status InitTrees(BTrees& T)
// 初始化森林表头指针
{
    if (T!=NULL) return INFEASIBLE;
    BTrees a=(BTrees)malloc(sizeof (BTsNode));
    T=a;
    T->data=NULL;
    T->next=NULL;
    return OK;
}

status DestroyTrees(BTrees& T)
// 销毁森林表
{
    if (T==NULL) return INFEASIBLE;
    BTrees p=NULL;
    while(T){
        p=T;
        T=T->next;
        if (p->data && p->data!=enull) DestroyBiTree(p->data);
        free(p);
    }
    return OK;
}

status AddTree(BTrees& T,char name[])
// 添加一棵空树
{
    if (T==NULL) return INFEASIBLE;
    BTrees p=T;
    while(p->next) p=p->next;
    BTrees a=(BTrees)malloc(sizeof (BTsNode));
    p->next=a;
    a->next=NULL;
    strcpy(a->name,name);
    a->data=enull;
    return OK;
}

status RemoveTree(BTrees& T,char name[])
// 删除一棵树
{
    if (T==NULL) return INFEASIBLE;
    BTrees p=T;
    while(p->next && strcmp(p->next->name,name)) p=p->next;
    if (p->next==NULL) return ERROR;
    BTrees q=p->next;
    p->next=q->next;
    if (q->data && q->data!=enull) DestroyBiTree(q->data);
    free(q);
    return OK;
}

status ListTrees(BTrees T)
// 列出所有树
{
    if (T==NULL) return INFEASIBLE;
    BTrees p=T->next;
    if (!p) printf("没有树\n");
    else printf("森林如下:\n");
    while(p){
        printf("%s\n",p->name);
        p=p->next;
    }
    return OK;
}

status SaveTrees(BTrees T, char FileName[])
// 将森林保存到文件,自制格式Trees n name key others key others -1 null...
{
    if (T==NULL) return INFEASIBLE;
    FILE *fp;
    fp=fopen(FileName,"w");
    fprintf(fp,"Trees\n");
    BTrees q=T->next;
    BiTree s[100],p=NULL;
    int top=0;
    int n=0;
    while(q){
        q=q->next;
        n++;
    }
    fprintf(fp,"%d\n",n);
    q=T->next;
    while(q){
        fprintf(fp,"%s\n",q->name);
        p=q->data;
        while(p && p!=enull){
            fprintf(fp,"%d %s ",p->data.key,p->data.others);
            s[top++]=p;
            p=p->lchild;
            if (!p){
                fprintf(fp,"0 null ");
                while(!s[top-1]->rchild && top) {
                    top--;
                    fprintf(fp,"0 null ");
                }
                if (top) p=s[--top]->rchild;
            }
        }
        fprintf(fp,"-1 null\n");
        q=q->next;
    }
    fclose(fp);
    return OK;
}

status LoadTrees(BTrees &T, char FileName[])
// 从文件读取森林
{
    if (T!=NULL) return INFEASIBLE;
    FILE *fp;
    fp=fopen(FileName,"r");
    if (!fp) return ERROR;
    char gra[100];
    fscanf(fp,"%s",gra);
    if (strcmp(gra,"Trees")) return ERROR;
    int n=0;
    fscanf(fp,"%d",&n);
    BTrees p=(BTrees)malloc(sizeof (BTsNode)),q=NULL;
    T=p;
    p->data=NULL;
    p->next=NULL;
    TElemType *definition=(TElemType *)malloc(sizeof (TElemType) *(100));
    for(int k=0;k<n;k++){
        q=(BTrees)malloc(sizeof (BTsNode));
        q->data=NULL;
        q->next=NULL;
        fscanf(fp,"%s",q->name);
        fscanf(fp,"%d %s",&definition[0].key,definition[0].others);
        for(int i=0;definition[i].key!=-1;i++) fscanf(fp,"%d %s",&definition[i+1].key,definition[i+1].others);
        status flag=CreateBiTree(q->data,definition);
        if (flag==INFEASIBLE) printf("二叉树已存在\n");
        else if (flag==ERROR) {
            printf("二叉树%s存在重复关键字,已忽略\n",q->name);
            free(q);
            continue;
        }
        p->next=q;
        p=q;
    }
    free(definition);
    fclose(fp);
    return OK;
}

status CreateBiTree(BiTree &T,TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。使用手动栈完成任务，先判断definition中是否存在重复关键字，
注意definition中的结尾关键字需要是-1*/
{
    if (T&&T!=enull) return INFEASIBLE;
    for(int i=0;definition[i].key!=-1;i++){
        if (definition[i].key==0) continue;
        for(int j=i+1;definition[j].key!=-1;j++)
            if (definition[i].key==definition[j].key) return ERROR;
    }
    TElemType *a=&definition[0];
    BiTree s[100],p=NULL;
    int top=0;
    if (a->key && a->key!=-1) T=(BiTree)malloc(sizeof(BiTNode));
    else {
        T=NULL;
        return OK;
    }
    T->data=definition[0];
    T->lchild=enull;
    T->rchild=enull;
    s[top++]=T;
    a++;
    while(a->key!=-1){
        if (a->key){
            p=(BiTree)malloc(sizeof(BiTNode));
            p->data=*a;
            p->lchild=enull;
            p->rchild=enull;
        }
        else p=NULL;
        if (top){
            if (s[top-1]->lchild==enull) s[top-1]->lchild=p;
            else {
                s[top-1]->rchild=p;
                top--;
            }
        }
        if (p) s[top++]=p;
        a++;
    }
    return OK;
}

status DestroyBiTree(BiTree &T)
//将二叉树设置成空，并删除所有结点，释放结点空间
{
    if (!T) return OK;
    if (T->lchild) DestroyBiTree(T->lchild);
    if (T->rchild) DestroyBiTree(T->rchild);
    free(T);
    T=NULL;
    return OK;
}

status ClearBiTree(BiTree T){
//将二叉树设置成空，保留根节点，删除所有子节点，释放子节点空间
    if (!T) return OK;
    if (T->lchild) ClearBiTree(T->lchild);
    if (T->rchild) ClearBiTree(T->rchild);
    free(T->lchild);
    free(T->rchild);
    T->lchild=NULL;
    T->rchild=NULL;
    return OK;
}

status BiTreeEmpty(BiTree T){
//判断二叉树是否为空
    if (!T) return INFEASIBLE;
    if (T==enull) return TRUE;
    else return FALSE;
}

int BiTreeDepth(BiTree T)
//求二叉树T的深度
{
    if (!T || T==enull) return 0;
    int dl=0,dr=0;
    if (T->lchild) dl=BiTreeDepth(T->lchild);
    if (T->rchild) dr=BiTreeDepth(T->rchild);
    if (dl>dr) return dl+1;
    else return dr+1;
}

BiTNode* LocateNode(BiTree T,KeyType e)
//查找结点,返回指向关键字为e的结点的指针，若不存在返回NULL
{
    if (!T) return NULL;
    if ((T->data).key==e) return T;
    BiTree el=NULL,er=NULL;
    el=LocateNode(T->lchild,e);
    if (el) return el;
    er=LocateNode(T->rchild,e);
    if (er) return er;
    return NULL;
}

status sitenode1(BiTree &T,KeyType e,KeyType vk,BiTree &flag)
{//辅助查找函数，查找关键字为e的结点，若存在返回flag值，同时如果vk重复返回ERROR
    if (!T) return OK;
    if (T->data.key==e) flag=T;
    if (T->data.key==vk && vk!=e){
        return ERROR;
    }
    int sta=OK;
    if (T->lchild) sta=sitenode1(T->lchild,e,vk,flag);
    if (sta!=OK) return ERROR;
    if (T->rchild) sta=sitenode1(T->rchild,e,vk,flag);
    if (sta!=OK) return ERROR;
    return sta;
}
status Assign(BiTree &T,KeyType e,TElemType value)
//实现结点赋值，注意查重。
{
    BiTree flag=NULL;
    status sta=sitenode1(T,e,value.key,flag);
    if (sta==OK && flag ) {
        flag->data=value;
        return OK;
    }
    return ERROR;
}

BiTNode* GetSibling(BiTree T,KeyType e)
//实现获得兄弟结点
{
    if (!T) return NULL;
    if (T->lchild) {
        if (T->lchild->data.key==e) return T->rchild;
    }
    if (T->rchild) {
        if (T->rchild->data.key==e) return T->lchild;
    }
    BiTree ans=NULL;
    if (T->lchild) {
        ans=GetSibling(T->lchild,e);
        if (ans) return ans;
    }
    if (T->rchild) {
        ans=GetSibling(T->rchild,e);
        if (ans) return ans;
    }
    return ans;
}

status sitenode2(BiTree T,KeyType e,KeyType vk,BiTree &flag)
{//辅助查找函数，查找关键字为e的结点，若存在返回flag值，同时如果vk重复返回ERROR
    if (!T) return 0;
    BiTree s[100],p;
    int top=0;
    p=T;
    flag=NULL;
    while(p && p!=enull){
        if (p->data.key==e) flag=p;
        if (p->data.key==vk){
            return ERROR;
        }
        s[top++]=p;
        p=p->lchild;
        if (!p){
            while(!s[top-1]->rchild && top) top--;
            if (top) p=s[--top]->rchild;
        }
    }
    return OK;
}
status InsertNode(BiTree &T,KeyType e,int LR,TElemType c)
//插入结点。LR=0表示插入左子树，LR=1表示插入右子树,LR=-1表示插入根结点
{
    BiTree p=NULL;
    if (LR==-1) {
        p=(BiTree)malloc(sizeof (BiTNode));
        p->data=c;
        if (T==enull) p->rchild=NULL;
        else p->rchild=T;
        p->lchild=NULL;
        T=p;
        return OK;
    }
    BiTree f=NULL;
    sitenode2(T,e,c.key,f);
    if (f!=NULL){
        p=(BiTree)malloc(sizeof (BiTNode));
        if (LR) {
            p->data=c;
            p->rchild=f->rchild;
            p->lchild=NULL;
            f->rchild=p;
        }
        else {
            p->data=c;
            p->rchild=f->lchild;
            p->lchild=NULL;
            f->lchild=p;
        }
        return OK;
    }
    return ERROR;
}

status DeleteNode(BiTree &T,KeyType e)
//删除结点，记录所要修改的子树地址pr与子树结点f，采用手动栈。
{
    if (!T) return ERROR;
    BiTree f=NULL;
    BiTree *pr=&T,*pre=&T;
    BiTree s[100],p;
    int top=0;
    p=T;//查找结点
    while(p){
        if (p->data.key==e){f=p;pr=pre;break;}
        s[top++]=p;
        pre=&(p->lchild);
        p=p->lchild;
        if (!p){
            while(!s[top-1]->rchild && top) top--;
            if (top) {p=s[top-1]->rchild;pre=&(s[--top]->rchild);}
        }
    }
    if (f) {
        if (!f->lchild && !f->rchild){
            *pr=NULL;
            free(f);
        }
        else if (f->lchild && !f->rchild) {
            *pr=f->lchild;
            free(f);
        }
        else if (!f->lchild && f->rchild) {
            *pr=f->rchild;
            free(f);
        }
        else if (f->lchild && f->rchild) {
            *pr=f->lchild;
            BiTree p=f->lchild;
            while (p->rchild) p=p->rchild;
            p->rchild=f->rchild;
            free(f);
        }
        return OK;
    }
    return ERROR;
}

void visit(BiTree T)
{//辅助函数，表示遍历方式
    if (T && T!=enull) printf(" %d,%s",T->data.key,T->data.others);
}
status PreOrderTraverse(BiTree T,void (*visit)(BiTree))
//先序遍历二叉树T,手动栈版
{
    if (!T) return 0;
    BiTree s[100],p;
    int top=0;
    p=T;
    while(p && p!=enull){
        visit(p);
        s[top++]=p;
        p=p->lchild;
        if (!p){
            while(!s[top-1]->rchild && top) top--;
            if (top) p=s[--top]->rchild;
        }
    }
    return OK;
}

status InOrderTraverse(BiTree T,void (*visit)(BiTree))
//中序遍历二叉树T
{
    if (!T) return 0;
    InOrderTraverse(T->lchild,visit);
    visit(T);
    InOrderTraverse(T->rchild,visit);
    return OK;
}

status PostOrderTraverse(BiTree T,void (*visit)(BiTree))
//后序遍历二叉树T
{
    if (!T) return 0;
    PostOrderTraverse(T->lchild,visit);
    PostOrderTraverse(T->rchild,visit);
    visit(T);
    return OK;
}

status LevelOrderTraverse(BiTree T,void (*visit)(BiTree))
//按层遍历二叉树T
{
    if (!T) return 0;
    BiTree q[100];
    int l=0,r=1;
    q[0]=T;
    while(l!=r){
        BiTree h=q[l++];
        visit(h);
        if (h->lchild) q[r++]=h->lchild;
        if (h->rchild) q[r++]=h->rchild;
    }
    return OK;
}

KeyType MaxPathSum(BiTree T)
//求二叉树T中从根结点到叶结点的最大路径和
{
    if (!T) return 0;
    KeyType max=0;
    KeyType lmax=MaxPathSum(T->lchild);
    KeyType rmax=MaxPathSum(T->rchild);
    if (lmax>rmax) max=lmax;
    else max=rmax;
    return max+T->data.key;
}

BiTNode* LowestCommonAncestor(BiTree T,KeyType e1,KeyType e2)
//求二叉树T中关键字为e1和e2的最近公共祖先结点
{
    if (!T) return NULL;
    if (T->data.key==e1 || T->data.key==e2) return T;
    BiTree l=NULL,r=NULL;
    l=LowestCommonAncestor(T->lchild,e1,e2);
    r=LowestCommonAncestor(T->rchild,e1,e2);
    if (l && r) return T;
    if (l) return l;
    if (r) return r;
    return NULL;
}

status InvertTree(BiTree &T)
//翻转二叉树
{
    if (!T) return INFEASIBLE;
    BiTree tmp=T->lchild;
    T->lchild=T->rchild;
    T->rchild=tmp;
    InvertTree(T->lchild);
    InvertTree(T->rchild);
    return OK;
}

status SaveBiTree(BiTree T, char FileName[])
//将二叉树的结点数据写入到文件FileName中
{
    if (!T) return INFEASIBLE;
    FILE *fp;
    fp=fopen(FileName,"w");
    BiTree s[100],p;
    int top=0,n=0;
    p=T;
    while(p){
        n++;
        s[top++]=p;
        p=p->lchild;
        if (!p){
            n++;
            while(!s[top-1]->rchild && top) {
                top--;
                n++;
            }
            if (top) p=s[--top]->rchild;
        }
    }
    fprintf(fp,"Tree\n");
    fprintf(fp,"%d\n",n+1);
    p=T;
    while(p && p!=enull){
        fprintf(fp,"%d %s ",p->data.key,p->data.others);
        s[top++]=p;
        p=p->lchild;
        if (!p){
            fprintf(fp,"0 null ");
            while(!s[top-1]->rchild && top) {
                top--;
                fprintf(fp,"0 null ");
            }
            if (top) p=s[--top]->rchild;
        }
    }
    fprintf(fp,"-1 null");
    fclose(fp);
    return OK;
}

status LoadBiTree(BiTree &T,  char FileName[])
//读入文件FileName的结点数据，创建二叉树
{
    if (T && T!=enull) return INFEASIBLE;
    FILE *fp;
    fp=fopen(FileName,"r");
    if (!fp) return ERROR;
    int n=0;
    char gra[100];
    fscanf(fp,"%s",gra);
    if (strcmp(gra,"Tree")) return ERROR;
    fscanf(fp,"%d",&n);
    TElemType *definition=(TElemType *)malloc(sizeof (TElemType) *(n+1));
    for(int i=0;i<=n;i++) fscanf(fp,"%d %s",&definition[i].key,definition[i].others);
    fclose(fp);
    for(int i=0;definition[i].key!=-1;i++){
        if (definition[i].key==0) continue;
        for(int j=i+1;definition[j].key!=-1;j++)
            if (definition[i].key==definition[j].key) return ERROR;
    }
    TElemType *a=&definition[0];
    BiTree s[100],p=NULL;
    int top=0;
    if (a->key && a->key!=-1) T=(BiTree)malloc(sizeof(BiTNode));
    else {
        T=NULL;
        return OK;
    }
    T->data=definition[0];
    T->lchild=enull;
    T->rchild=enull;
    s[top++]=T;
    a++;
    while(a->key!=-1){
        if (a->key){
            p=(BiTree)malloc(sizeof(BiTNode));
            p->data=*a;
            p->lchild=enull;
            p->rchild=enull;
        }
        else p=NULL;
        if (top){
            if (s[top-1]->lchild==enull) s[top-1]->lchild=p;
            else {
                s[top-1]->rchild=p;
                top--;
            }
        }
        if (p) s[top++]=p;
        a++;
    }
    free(definition);
    return OK;
}

int treeoption(BiTree &T,char name[])
{
    int op=1;
    while(op){
        status flag; 
        int i,inde;
        char FileName[100];
        TElemType x;
        TElemType *definition=NULL;
        BiTree p=NULL,lca=NULL;
        KeyType e,e1,e2;
        BiTree sib=NULL;
        system("cls");	printf("\n\n");
        printf("      Menu for Binary Tree %s  \n",name);
        printf("-------------------------------------------------\n");
        printf("    	  1. CreateBiTree   10. DeleteNode\n");
        printf("    	  2. DestroyBiTree  11. PreOrderTraverse\n");
        printf("    	  3. ClearBiTree    12. InOrderTraverse\n");
        printf("    	  4. BiTreeEmpty    13. PostOrderTraverse\n");
        printf("    	  5. BiTreeDepth    14. LevelOrderTraverse\n");
        printf("    	  6. LocateNode     15. MaxPathSum\n");
        printf("    	  7. Assign         16. LowestCommonAncestor\n");
        printf("    	  8. GetSibling     17. InvertTree\n");
        printf("    	  9. InsertNode     18. SaveBiTree\n");
        printf("    	  0. exit           19. LoadBiTree\n");
        printf("-------------------------------------------------\n");
    
        printf("    请选择你的操作[0~19]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                definition=(TElemType *)malloc(sizeof (TElemType) *(100));
                printf("请输入二叉树的结点数据（以-1 null null结尾）:\n");
                scanf("%d %s",&definition[0].key,definition[0].others);
                for(i=0;definition[i].key!=-1;i++) scanf("%d %s",&definition[i+1].key,definition[i+1].others);
                flag=CreateBiTree(T,definition);
                if (flag==INFEASIBLE) printf("二叉树已存在\n");
                else if (flag==ERROR) printf("存在重复关键字\n");
                else if (flag==OK) printf("二叉树创建成功\n");  
                free(definition);
                getchar();getchar();
                break;
            case 2:
                flag=DestroyBiTree(T);
                if (flag==OK) printf("二叉树销毁成功\n");
                else printf("二叉树不存在\n"); 
                getchar();getchar();
                break;
            case 3:
                flag=ClearBiTree(T);
                if (T) T=enull;
                if (flag==OK) printf("二叉树清空成功\n");
                else printf("二叉树不存在\n"); 
                getchar();getchar();
                break;
            case 4:
                if (BiTreeEmpty(T)==TRUE) printf("二叉树为空\n");
                else if (BiTreeEmpty(T)==FALSE) printf("二叉树不为空\n");
                else printf("二叉树不存在\n"); 
                getchar();getchar();
                break;
            case 5:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("二叉树的深度为%d\n",BiTreeDepth(T)); 
                getchar();getchar();
                break;
            case 6:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("请输入要查找的关键字:");
                scanf("%d",&e);
                p=LocateNode(T,e);
                if (p) printf("关键字为%d的结点存在:%d %s\n",e,p->data.key,p->data.others);
                else printf("关键字为%d的结点不存在\n",e); 
                getchar();getchar();
                break;
            case 7:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("请输入要赋值的关键字:");
                scanf("%d",&e);
                printf("请输入要赋的值:");
                scanf("%d %s",&x.key,x.others);
                flag=Assign(T,e,x);
                if (flag==OK) printf("赋值成功\n");
                else printf("赋值失败,所赋关键字重复或所找关键字不存在\n"); 
                getchar();getchar();
                break;
            case 8:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("请输入要查找兄弟结点的关键字:");
                scanf("%d",&e);
                sib=GetSibling(T,e);
                if (sib) printf("关键字为%d的结点的兄弟结点为:%d %s\n",e,sib->data.key,sib->data.others);
                else printf("关键字为%d的结点不存在或没有兄弟结点\n",e); 
                getchar();getchar();
                break;
            case 9:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("请输入要插入的关键字结点:");
                scanf("%d",&e);
                printf("请输入要插入的位置(0表示左子树，1表示右子树,-1表示在根结点处插入):");
                scanf("%d",&inde);
                printf("请输入要插入的结点数据:");
                scanf("%d%s",&x.key,x.others);
                flag=InsertNode(T,e,inde,x);
                if (flag==OK) printf("插入成功\n");
                else printf("插入失败,所插关键字重复或所找关键字不存在\n"); 
                getchar();getchar();
                break;
            case 10:
                if (!T) {printf("二叉树不存在\n");getchar();getchar();break;}
                printf("请输入要删除的关键字:");
                scanf("%d",&e);
                flag=DeleteNode(T,e);
                if (flag==OK) printf("删除成功\n");
                else printf("删除失败,所找关键字不存在\n"); 
                getchar();getchar();
                break;
            case 11:
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    printf("先序遍历二叉树:");
                    PreOrderTraverse(T,visit);
                    printf("\n");
                } 
                getchar();getchar();
                break;
            case 12:
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    printf("中序遍历二叉树:");
                    InOrderTraverse(T,visit);
                    printf("\n");
                } 
                getchar();getchar();
                break;
            case 13:
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    printf("后序遍历二叉树:");
                    PostOrderTraverse(T,visit);
                    printf("\n");
                } 
                getchar();getchar();
                break;
            case 14:
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    printf("按层遍历二叉树:");
                    LevelOrderTraverse(T,visit);
                    printf("\n");
                } 
                getchar();getchar();
                break;
            case 15:           
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    printf("二叉树的最大路径和为:%d\n",MaxPathSum(T));
                } 
                getchar();getchar();
                break;
            case 16:
                printf("请输入要查找的两个关键字:");
                scanf("%d %d",&e1,&e2);
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    lca=LowestCommonAncestor(T,e1,e2);
                    if (lca) printf("关键字为%d和%d的最近公共祖先为:%d %s\n",e1,e2,lca->data.key,lca->data.others);
                    else printf("关键字为%d和%d的最近公共祖先不存在\n",e1,e2);
                } 
                getchar();getchar();
                break;
            case 17:
                if (!T) printf("二叉树不存在\n");
                else if (T==enull) printf("二叉树为空\n");
                else{
                    flag=InvertTree(T);
                    if (flag==OK) printf("翻转成功\n");
                    else printf("翻转失败\n");
                } 
                getchar();getchar();
                break;
            case 18:
                printf("请输入文件名:");
                scanf("%s",FileName);
                flag=SaveBiTree(T,FileName);
                if (flag==OK) printf("保存成功\n");
                else printf("保存失败,树不存在\n"); 
                getchar();getchar();
                break;
            case 19:
                printf("请输入文件名:");
                scanf("%s",FileName);
                flag=LoadBiTree(T,FileName);
                if (flag==OK) printf("读取成功\n");
                else printf("读取失败,树非空或格式错误\n"); 
                getchar();getchar();
                break;
            case 0:
                printf("退出成功！");
                getchar();
                getchar();
                break;
            }
        }
    return 0;
}


//1 a 2 b 3 c 0 null 0 null 4 d 5 e 0 null 0 null 0 null 0 null -1 null