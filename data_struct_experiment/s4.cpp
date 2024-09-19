	#include "stdio.h"
	#include "stdlib.h"
    #include "string.h"
	#define TRUE 1
	#define FALSE 0
	#define OK 1
	#define ERROR 0
	#define INFEASIBLE -1
	#define OVERFLOW -2
	#define MAX_VERTEX_NUM 20
	typedef int status;
	typedef int KeyType; 
    typedef enum {DG,DN,UDG,UDN,UKN} GraphKind;
	typedef struct {
         KeyType  key;
         char others[20];
	} VertexType; //顶点类型定义


	typedef struct ArcNode {         //表结点类型定义
   		 int adjvex;              //顶点位置编号 
    	 struct ArcNode  *nextarc;	   //下一个表结点指针
	} ArcNode;
	typedef struct VNode{				//头结点及其数组类型定义
   		 VertexType data;       	//顶点信息
    	 ArcNode *firstarc;      	 //指向第一条弧
    	} VNode,AdjList[MAX_VERTEX_NUM];
	typedef  struct {  //邻接表的类型定义
        AdjList vertices;     	 //头结点数组
        int vexnum,arcnum;   	  //顶点数、弧数
        GraphKind  kind;        //图的类型
       } ALGraph;
    typedef struct GNODE{
        ALGraph data;
        char name[100];
        struct GNODE* next;
    } GNode, *ALGs;

//函数声明
status CreateGraph(ALGraph &G,VertexType V[],KeyType VR[][2]);
status DestroyGraph(ALGraph &G);
int LocateVex(ALGraph G,KeyType u);
status PutVex(ALGraph &G,KeyType u,VertexType value);
int FirstAdjVex(ALGraph G,KeyType u);
int NextAdjVex(ALGraph G,KeyType v,KeyType w);
status InsertVex(ALGraph &G,VertexType v);
status DeleteVex(ALGraph &G,KeyType v);
status InsertArc(ALGraph &G,KeyType v,KeyType w);
status DeleteArc(ALGraph &G,KeyType v,KeyType w);
void visit(VertexType v);
status DFSTraverse(ALGraph &G,void (*visit)(VertexType));
status BFSTraverse(ALGraph &G,void (*visit)(VertexType));
status SaveGraph(ALGraph G, char FileName[]);
status LoadGraph(ALGraph &G, char FileName[]);
int *VerticesSetLessThanK(ALGraph G,KeyType v,int k);
int ShortestPathLength(ALGraph G,KeyType v,KeyType w);
int ConnectedComponentsNums(ALGraph G);
status Graph_one(ALGraph &G,char name[]);
status DestroyGraphs(ALGs &Gs);
status AddGraph(ALGs &Gs,char name[]);
status DeleteGraph(ALGs &Gs,char name[]);
int LocateGs(ALGs Gs,char name[]);
status ListGs(ALGs Gs);
status LoadGs(ALGs &Gs,char FileName[]);
status SaveGs(ALGs Gs,char FileName[]);

int main()
{
    ALGs Gs=(ALGs)malloc(sizeof(GNode));
    Gs->next=NULL;
    Gs->data.vexnum=0;
    Gs->data.arcnum=0;
    Gs->data.kind=UKN;
    int op=1,oplist=0,inde=0;
    while(op){
        status flag;
        char name[100];
        ALGs p;
        system("cls");	printf("\n\n");
        printf("      Menu for Multipled Graphs \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. DestroyGraphs   5. ListGs\n");
        printf("    	  2. AddGraph        6. LoadGs\n");
        printf("    	  3. DeleteGraph     7. SaveGs\n");
        printf("    	  4. LocateGs        8. Graph_one\n");
        printf("    	  0. exit\n");
        printf("-------------------------------------------------\n");
        if (oplist) {
            printf("-------------------------------------------------\n");
            inde=ListGs(Gs);
            if (!inde) printf("图集为空\n"); 
            printf("-------------------------------------------------\n");
        }
        printf("    请选择你的操作[0~8]:");
        scanf("%d",&op);    
        switch (op)
        {
            case 1:
                flag=DestroyGraphs(Gs);
                if(flag==OK) printf("销毁成功\n");
                else printf("销毁失败\n");
                getchar();getchar();
                break;
            case 2:
                printf("请输入要添加的图名：");
                scanf("%s",name);
                flag=AddGraph(Gs,name);
                if(flag==OK) printf("添加成功\n");
                else printf("添加失败\n");
                getchar();getchar();
                break;
            case 3:
                printf("请输入要删除的图名：");
                scanf("%s",name);
                flag=DeleteGraph(Gs,name);
                if(flag==OK) printf("删除成功\n");
                else printf("删除失败\n");
                getchar();getchar();
                break;
            case 4:
                printf("请输入要定位的图名：");
                scanf("%s",name);
                inde=LocateGs(Gs,name);
                if(inde==-1) printf("不存在\n");
                else printf("位序为%d\n",inde);
                getchar();getchar();
                break;
            case 5:
                printf("是否展示图集所有图名（1/0）默认为0：\n");
                scanf("%d",&oplist);
                getchar();getchar();
                break;
            case 6:
                printf("请输入文件名：");
                scanf("%s",name);
                flag=LoadGs(Gs,name);
                if(flag==OK) printf("读取成功\n");
                else if (flag==ERROR) printf("读取失败,文件格式错误或者输入图不正确\n");
                else printf("读取失败,图集已存在\n");
                getchar();getchar();
                break;
            case 7:
                printf("请输入文件名：");
                scanf("%s",name);
                flag=SaveGs(Gs,name);
                if(flag==ERROR) printf("保存失败\n");
                else printf("保存成功\n");
                getchar();getchar();
                break;
            case 8:
                printf("请需要操作输入图名：");
                scanf("%s",name);
                inde=LocateGs(Gs,name);
                if(inde==-1) printf("不存在\n");
                else{
                    p=Gs;
                    for(int i=0;i<inde;i++) p=p->next;
                    Graph_one(p->data,name);
                }
                getchar();getchar();
                break;
            case 0:
                printf("退出成功!\n");
                DestroyGraphs(Gs);//结束后释放内存
                getchar();
                break;
        }
    }
    return 0;
}

status CreateGraph(ALGraph &G,VertexType V[],KeyType VR[][2])
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    if (G.kind==UDG) return INFEASIBLE;
    int n=0,m=0,flag=0;
    ArcNode *p=NULL;
    for(int i=0;V[i].key!=-1;i++) {
        n++;
        for(int j=0;j<i;j++){
            if (V[i].key==V[j].key) return ERROR;
        }
    }
    if (n>MAX_VERTEX_NUM || !n) return ERROR;
    for(int i=0;VR[i][0]!=-1;i++) {
        m++;
        flag=0;
        for(int j=0;j<n;j++){
            if (V[j].key==VR[i][0]||V[j].key==VR[i][1]) flag++;
        }
        if (flag<2) return ERROR;
    }
    G.kind=UDG;
    G.vexnum=n;
    G.arcnum=0;
    for(int i=0;i<n;i++){
        G.vertices[i].data=V[i];
        G.vertices[i].firstarc=NULL;
    }
    for(int i=0;i<m;i++){
        int k1=0,k2=0;
        while(V[k1].key!=VR[i][0] && k1<n) k1++;
        while(V[k2].key!=VR[i][1] && k2<n) k2++;
        flag=0;
        p=G.vertices[k1].firstarc;
        while(p){
            if (p->adjvex==k2) flag=1;
            p=p->nextarc;
        }
        if (flag) return ERROR;
        p=(ArcNode *)malloc(sizeof(ArcNode));
        p->adjvex=k2;
        p->nextarc=G.vertices[k1].firstarc;
        G.vertices[k1].firstarc=p;
        if (k1==k2) continue;
        p=(ArcNode *)malloc(sizeof(ArcNode));
        p->adjvex=k1;
        p->nextarc=G.vertices[k2].firstarc;
        G.vertices[k2].firstarc=p;
        G.arcnum++;
    }
    return OK;
}

status DestroyGraph(ALGraph &G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    for(int i=0;i<G.vexnum;i++)
    {
        ArcNode *p=G.vertices[i].firstarc;
        while(p!=NULL)
        {
            ArcNode *q=p;
            p=p->nextarc;
            free(q);
        }
    }
    G.vexnum=0;
    G.arcnum=0;
    G.kind=UKN;
    return OK;
}

int LocateVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
    if (!G.vexnum) return INFEASIBLE; 
    for(int i=0;i<G.vexnum;i++){
        if (G.vertices[i].data.key==u) return i;
    }
    return -1;
}

status PutVex(ALGraph &G,KeyType u,VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
{
    if (G.vexnum==0) return ERROR;
    int k=-1;
    for(int i=0;i<G.vexnum;i++)
    {
        if(G.vertices[i].data.key!=u && G.vertices[i].data.key==value.key) return ERROR;
        if(G.vertices[i].data.key==u) k=i;
    }
    if(k==-1) return ERROR;
    G.vertices[k].data=value;
    return OK;
}

int FirstAdjVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
    for(int i=0;i<G.vexnum;i++)
    {
        if(G.vertices[i].data.key==u)
        {
            if(G.vertices[i].firstarc==NULL) return -1;
            return G.vertices[i].firstarc->adjvex;
        }
    }
    return -1;
}

int NextAdjVex(ALGraph G,KeyType v,KeyType w)
//v对应G的一个顶点,w对应v的邻接顶点；操作结果是返回v的（相对于w）下一个邻接顶点的位序；如果w是最后一个邻接顶点，或v、w对应顶点不存在，则返回-1。
{
    for(int i=0;i<G.vexnum;i++){
        if(G.vertices[i].data.key==v){
            ArcNode *p=G.vertices[i].firstarc;
            while(p){
                if (G.vertices[p->adjvex].data.key==w){
                    if(p->nextarc==NULL) return -1;
                    return p->nextarc->adjvex;
                }
                p=p->nextarc;
            }
        }
    }
    return -1;
}

status InsertVex(ALGraph &G,VertexType v)
//在图G中插入顶点v，成功返回OK,否则返回ERROR
{
    
    if(G.vexnum==MAX_VERTEX_NUM) return ERROR;
    for(int i=0;i<G.vexnum;i++)
    {
        if(G.vertices[i].data.key==v.key) return ERROR;
    }
    G.vertices[G.vexnum].firstarc=NULL;//注意初始化
    G.vertices[G.vexnum++].data=v;
    return OK;
}

status DeleteVex(ALGraph &G,KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
    int i=0,k=0;//判断在点集中是否存在v
    for(i=0;i<G.vexnum;i++){
        if (G.vertices[i].data.key==v){
            break;
        }
    }
    if (i==G.vexnum || G.vexnum<=1) return ERROR;//不能删除最后一个点
    ArcNode *p=G.vertices[i].firstarc;//删除与v相关的弧
    ArcNode *q=NULL,*r=NULL,*pr=NULL;
    while(p){
        q=p;
        k=p->adjvex;
        r=G.vertices[k].firstarc;
        G.arcnum--;
        pr=NULL;
        while(r && k!=i){
            if(r->adjvex==i){
                if(r==G.vertices[k].firstarc){
                    G.vertices[k].firstarc=r->nextarc;
                }
                else{
                    pr->nextarc=r->nextarc;
                }
                free(r);
                break;
            }
            pr=r;
            r=r->nextarc;
        }
        p=p->nextarc;
        free(q);
    }
    for(int j=i;j<G.vexnum-1;j++){
        G.vertices[j]=G.vertices[j+1];
    }
    G.vexnum--;//删除v
    for(int j=0;j<G.vexnum;j++){//更新点的adjvex
        p=G.vertices[j].firstarc;
        while(p){
            if(p->adjvex>i){
                p->adjvex--;
            }
            p=p->nextarc;
        }
    }
    return OK;
}

status InsertArc(ALGraph &G,KeyType v,KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    int k1=-1,k2=-1;//简单图
    ArcNode *p=NULL;
    for(int i=0;i<G.vexnum;i++){
        if(G.vertices[i].data.key==v) k1=i;
        if(G.vertices[i].data.key==w) k2=i;
    }
    if (k1==-1 || k2==-1 || k1==k2) return ERROR;
    p=G.vertices[k1].firstarc;
    while(p){
        if(p->adjvex==k2) return ERROR;
        p=p->nextarc;
    }
    p=(ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex=k2;
    p->nextarc=G.vertices[k1].firstarc;
    G.vertices[k1].firstarc=p;
    p=(ArcNode *)malloc(sizeof(ArcNode));
    p->adjvex=k1;
    p->nextarc=G.vertices[k2].firstarc;
    G.vertices[k2].firstarc=p;
    G.arcnum++;
    return OK;
}

status DeleteArc(ALGraph &G,KeyType v,KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    int k1=-1,k2=-1,flag=0;
    ArcNode *p=NULL,*q=NULL;
    for(int i=0;i<G.vexnum;i++){
        if(G.vertices[i].data.key==v) k1=i;
        if(G.vertices[i].data.key==w) k2=i;
    }
    if (k1==-1 || k2==-1) return ERROR;//判断是否存在v和w
    p=G.vertices[k1].firstarc;//删除<v,w>
    q=NULL;
    while(p){
        if(p->adjvex==k2){
            if(q==NULL){
                G.vertices[k1].firstarc=p->nextarc;
            }
            else{
                q->nextarc=p->nextarc;
            }
            flag=1;
            free(p);
            break;
        }
        q=p;
        p=p->nextarc;
    }
    if (!flag) return ERROR;//判断是否存在<v,w>
    p=G.vertices[k2].firstarc;
    q=NULL;
    while(p){
        if(p->adjvex==k1){
            if(q==NULL){
                G.vertices[k2].firstarc=p->nextarc;
            }
            else{
                q->nextarc=p->nextarc;
            }
            free(p);
            break;
        }
        q=p;
        p=p->nextarc;
    }
    G.arcnum--;
    return OK;
}

void visit(VertexType v)
{
    printf(" %d %s ",v.key,v.others);
}
status DFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    if (G.vexnum==0) return ERROR;
    int vis[MAX_VERTEX_NUM+1]={0};
    int top=0;
    ArcNode *p=NULL,*s[MAX_VERTEX_NUM+1];//栈
    for(int i=0;i<=G.vexnum;i++) vis[i]=0;
    for(int i=0;i<G.vexnum;i++){
        if(!vis[i]){
            top=0;
            p=s[top++]=G.vertices[i].firstarc;
            vis[i]=1;
            visit(G.vertices[i].data);
            while(top && p){
                if(!p ||
                vis[p->adjvex]){ 
                    while(top){
                        p=s[top-1]->nextarc;
                        while (p && 
                        vis[p->adjvex]) p=p->nextarc;
                        if (p) break;
                        top--;
                    }
                }
                if (p &&
                !vis[p->adjvex]){
                    visit(G.vertices[p->adjvex].data);
                    vis[p->adjvex]=1;
                    p=s[top++]=G.vertices[p->adjvex].firstarc;
                }
            }
        }
    }
    return OK;
}

status BFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    if (G.vexnum==0) return ERROR;
    int vis[MAX_VERTEX_NUM]={0};
    int qu[MAX_VERTEX_NUM+1];//队列
    int l=0,r=0;
    for(int i=0;i<G.vexnum;i++){
        if(!vis[i]){
            l=r=0;
            qu[r++]=i;
            vis[i]=1;
            while(l<r){
                int k=qu[l++];
                visit(G.vertices[k].data);
                ArcNode *p=G.vertices[k].firstarc;
                while(p){
                    if(!vis[p->adjvex]){
                        qu[r++]=p->adjvex;
                        vis[p->adjvex]=1;
                    }
                    p=p->nextarc;
                }
            }
        }
    }
    return OK;
}

status SaveGraph(ALGraph G, char FileName[])
//将图的数据写入到文件FileName中,为了读取方便，直接保存了邻接表
{
    if (G.vexnum==0) return ERROR;
    FILE *fp=fopen(FileName,"w");
    if (fp==NULL) return ERROR;
    fprintf(fp,"Graph1\n");//表示邻接表式读入
    int m=0;
    for(int i=0;i<G.vexnum;i++){
        ArcNode *p=G.vertices[i].firstarc;
        while(p){
            if(i<=p->adjvex){
                m++;
            }
            p=p->nextarc;
        }
    }
    fprintf(fp,"%d %d\n",G.vexnum,m);
    for(int i=0;i<G.vexnum;i++){
        fprintf(fp,"%d %s\n",G.vertices[i].data.key,G.vertices[i].data.others);
    }
    for(int i=0;i<G.vexnum;i++){
        ArcNode *p=G.vertices[i].firstarc;
        while(p){
            fprintf(fp,"%d ",p->adjvex);
            p=p->nextarc;
        }
        fprintf(fp,"-1\n");
    }
    fclose(fp);
    return OK;
}
status LoadGraph(ALGraph &G, char FileName[])
//读入文件FileName的图数据，创建图的邻接表
{
    if (G.vexnum!=0) return OVERFLOW;
    FILE *fp=fopen(FileName,"r");
    if (fp==NULL) return INFEASIBLE;
    char gra[100];
    fscanf(fp,"%s",gra);
    if (strcmp(gra,"Graph1")==0) {
        int n=0,m=0;
        fscanf(fp,"%d %d",&n,&m);
        if (n>MAX_VERTEX_NUM || !n) {fclose(fp);return ERROR;}
        G.vexnum=n;
        for(int i=0;i<n;i++){
            fscanf(fp,"%d %s",&G.vertices[i].data.key,G.vertices[i].data.others);
            for(int j=0;j<i;j++)
                if (G.vertices[i].data.key==G.vertices[j].data.key) {
                    G.vexnum=0;
                    fclose(fp);
                    return ERROR;
                }
            G.vertices[i].firstarc=NULL;
        }
        G.arcnum=m;
        ArcNode *p=NULL,*pr=NULL;
        for(int i=0;i<n;i++){
            int k;
            pr=NULL;
            while(1){
                fscanf(fp,"%d",&k);
                if (k==-1) break;
                p=(ArcNode*)malloc(sizeof(ArcNode));
                p->adjvex=k;
                p->nextarc=NULL;
                if (!pr) G.vertices[i].firstarc=p;  
                else pr->nextarc=p;
                pr=p;
            }
        }
        G.kind=UDG;
        return OK;
    }
    else if (strcmp(gra,"Graph2")==0){
        VertexType V[MAX_VERTEX_NUM];
        KeyType VR[MAX_VERTEX_NUM][2];
        for(int i=0;i<MAX_VERTEX_NUM;i++){
                    fscanf(fp,"%d%s",&V[i].key,V[i].others);
                    if(V[i].key==-1) break;
                }
        for(int i=0;i<MAX_VERTEX_NUM;i++){
            fscanf(fp,"%d %d",&VR[i][0],&VR[i][1]);
            if(VR[i][0]==-1) break;
        }
        fclose(fp);
        status flag=CreateGraph(G,V,VR);
        if (flag==OK) {fclose(fp);return OK;}
        else {fclose(fp);return ERROR;}
    }
    else {
        fclose(fp);
        return ERROR;
    }
}

int *VerticesSetLessThanK(ALGraph G,KeyType v,int k)
//返回图G中与顶点v距离小于k的所有顶点集合，返回的顶点集合以a[0]代表集合元素个数，后面是集合元素的位序
{
    int *a=(int *)malloc(sizeof(int)*(MAX_VERTEX_NUM+1));
    int vis[MAX_VERTEX_NUM]={0};
    int qu[MAX_VERTEX_NUM+1];//队列
    int we[MAX_VERTEX_NUM+1]={0};//记录路径长度
    int l=0,r=0;
    a[0]=0;
    for(int i=0;i<G.vexnum;i++){
        if(G.vertices[i].data.key==v){
            l=r=0;
            we[i]=0;
            qu[r++]=i;
            vis[i]=1;
            while(l<r){
                int o=qu[l++];
                if(we[o]<k){
                    if (o!=i) a[++a[0]]=o;
                } 
                else break;
                ArcNode *p=G.vertices[o].firstarc;
                while(p){
                    if(!vis[p->adjvex]){
                        we[p->adjvex]=we[o]+1;
                        qu[r++]=p->adjvex;
                        vis[p->adjvex]=1;
                    }
                    p=p->nextarc;
                }
            }
            break;
        }
    }
    return a;
}

int ShortestPathLength(ALGraph G,KeyType v,KeyType w) 
//返回图G中从顶点v到顶点w的最短路径长度，如果路径不存在，返回-1
{
    int vis[MAX_VERTEX_NUM]={0};
    int qu[MAX_VERTEX_NUM+1];//队列
    int we[MAX_VERTEX_NUM+1]={0};//记录路径长度
    int l=0,r=0;
    int dmin=-1;
    for(int i=0;i<G.vexnum;i++){
        if(G.vertices[i].data.key==v){
            l=r=0;
            we[i]=0;
            qu[r++]=i;
            vis[i]=1;
            while(l<r){
                int o=qu[l++];
                if(G.vertices[o].data.key==w){
                    dmin=we[o];
                    break;
                }
                ArcNode *p=G.vertices[o].firstarc;
                while(p){
                    if(!vis[p->adjvex]){
                        we[p->adjvex]=we[o]+1;
                        qu[r++]=p->adjvex;
                        vis[p->adjvex]=1;
                    }
                    p=p->nextarc;
                }
            }
            break;
        }
    }
    return dmin;
}

int ConnectedComponentsNums(ALGraph G)
//返回图G的连通分量个数
{
    int vis[MAX_VERTEX_NUM]={0};
    int qu[MAX_VERTEX_NUM+1];//队列
    int l=0,r=0,sum=0;
    for(int i=0;i<G.vexnum;i++){
        if(!vis[i]){
            sum++;
            l=r=0;
            qu[r++]=i;
            vis[i]=1;
            while(l<r){
                int k=qu[l++];
                ArcNode *p=G.vertices[k].firstarc;
                while(p){
                    if(!vis[p->adjvex]){
                        qu[r++]=p->adjvex;
                        vis[p->adjvex]=1;
                    }
                    p=p->nextarc;
                }
            }
        }
    }
    return sum;
}

status Graph_one(ALGraph &G,char name[])
//图G的基本操作集，name为图的名字
{
    int op=1;
    while(op){
        status flag; 
        int i,j,inde;
        char FileName[100];
        KeyType x;
        int *a=NULL;
        KeyType e,e1,e2;
        VertexType V[MAX_VERTEX_NUM];
        KeyType VR[MAX_VERTEX_NUM][2];
        system("cls");	printf("\n\n");
        printf("      Menu for UDG %s  \n",name);
        printf("-------------------------------------------------\n");
        printf("    	  1. CreateGraph    10. DeleteArc\n");
        printf("    	  2. DestroyGraph   11. DFSTraverse\n");
        printf("    	  3. LocateVex      12. BFSTraverse\n");
        printf("    	  4. PutVex         13. SaveGraph\n");
        printf("    	  5. FirstAdjVex    14. LoadGraph\n");
        printf("    	  6. NextAdjVex     15. VerticesSetLessThanK\n");
        printf("    	  7. InsertVex      16. ShortestPathLength\n");
        printf("    	  8. DeleteVex      17. ConnectedComponentsNums\n");
        printf("    	  9. InsertArc      0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~17]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                printf("请输入顶点信息，以-1结束,边信息，以-1 -1结束：\n");
                for(int i=0;i<MAX_VERTEX_NUM;i++){
                    scanf("%d%s",&V[i].key,V[i].others);
                    if(V[i].key==-1) break;
                }
                for(int i=0;i<MAX_VERTEX_NUM;i++){
                    scanf("%d %d",&VR[i][0],&VR[i][1]);
                    if(VR[i][0]==-1) break;
                }
                flag=CreateGraph(G,V,VR);
                if(flag==OK) printf("创建成功\n");
                else if(flag==ERROR) printf("创建失败\n");
                else if(flag==INFEASIBLE) printf("图已存在\n");
                getchar();getchar();
                break;
            case 2:
                flag=DestroyGraph(G);
                if(flag==OK) printf("销毁成功\n");
                else printf("销毁失败\n");
                getchar();getchar();
                break;
            case 3:
                printf("请输入查找顶点关键字：");
                scanf("%d",&x);
                inde=LocateVex(G,x);
                if(inde==-1) printf("不存在\n");
                else printf("位序为%d\n",inde);
                getchar();getchar();
                break;
            case 4:
                printf("请输入顶点关键字：");
                scanf("%d",&x);
                printf("请输入新的顶点信息：");
                scanf("%d %s",&V[0].key,V[0].others);
                flag=PutVex(G,x,V[0]);
                if(flag==OK) printf("修改成功\n");
                else printf("修改失败\n");
                getchar();getchar();
                break;
            case 5:
                printf("请输入顶点关键字：");
                scanf("%d",&x);
                inde=FirstAdjVex(G,x);
                if(inde==-1) printf("不存在\n");
                else printf("第一个连接点位序为%d\n",inde);
                getchar();getchar();
                break;
            case 6:
                printf("请输入主顶点关键字：");
                scanf("%d",&e1);
                printf("请输入关联顶点关键字：");
                scanf("%d",&e2);
                inde=NextAdjVex(G,e1,e2);
                if(inde==-1) printf("不存在\n");
                else printf("下一个连接点位序为%d\n",inde);
                getchar();getchar();
                break;
            case 7:
                printf("请输入新的顶点信息：");
                scanf("%d %s",&V[0].key,V[0].others);
                flag=InsertVex(G,V[0]);
                if(flag==OK) printf("插入成功\n");
                else printf("插入失败，点不合法\n");
                getchar();getchar();
                break;
            case 8:
                printf("请输入需要删除顶点关键字：");
                scanf("%d",&x);
                flag=DeleteVex(G,x);
                if(flag==OK) printf("删除成功\n");
                else printf("删除失败\n");
                getchar();getchar();
                break;
            case 9:
                printf("请输入插入边的两个顶点关键字：");
                scanf("%d %d",&e1,&e2);
                flag=InsertArc(G,e1,e2);
                if(flag==OK) printf("插入成功\n");
                else printf("插入失败，边不合法\n");
                getchar();getchar();
                break;
            case 10:
                printf("请输入删除边的两个顶点关键字：");
                scanf("%d %d",&e1,&e2);
                flag=DeleteArc(G,e1,e2);
                if(flag==OK) printf("删除成功\n");
                else printf("删除失败\n");
                getchar();getchar();
                break;
            case 11:
                printf("深度优先搜索遍历结果为：");
                flag=DFSTraverse(G,visit);
                if (flag==ERROR) printf("图为空\n");
                printf("\n");
                getchar();getchar();
                break;
            case 12:
                printf("广度优先搜索遍历结果为：");
                flag=BFSTraverse(G,visit);
                if (flag==ERROR) printf("图为空\n");
                printf("\n");
                getchar();getchar();
                break;
            case 13:
                printf("请输入文件名：");
                scanf("%s",FileName);
                flag=SaveGraph(G,FileName);
                if(flag==ERROR) printf("保存失败\n");
                else printf("保存成功\n");
                getchar();getchar();
                break;
            case 14:
                printf("请输入文件名：");
                scanf("%s",FileName);
                flag=LoadGraph(G,FileName);
                if(flag==OK) printf("读取成功\n");
                else if (flag==ERROR) printf("读取失败,文件格式错误或者输入图不正确\n");
                else printf("读取失败,图已存在\n");
                getchar();getchar();
                break;
            case 15:
                printf("请输入顶点关键字和k：");
                scanf("%d %d",&e,&i);
                a=VerticesSetLessThanK(G,e,i);
                printf("与顶点%d距离小于%d的顶点集合为：",e,i);
                for(j=1;j<=a[0];j++){
                    printf(" %d %s",G.vertices[a[j]].data.key,G.vertices[a[j]].data.others);
                }
                printf("\n");
                free(a);
                getchar();getchar();
                break;
            case 16:
                printf("请输入两个顶点关键字：");
                scanf("%d %d",&e1,&e2);
                inde=ShortestPathLength(G,e1,e2);
                if(inde==-1) printf("路径不存在\n");
                else printf("最短路径长度为%d\n",inde);
                getchar();getchar();
                break;
            case 17:
                inde=ConnectedComponentsNums(G);
                printf("连通分量个数为%d\n",inde);
                getchar();getchar();
                break;
            case 0:
                printf("退出成功\n");
                getchar();
                break;
        }
    }
    return OK;
}
status DestroyGraphs(ALGs &Gs)
//销毁图集Gs,删除Gs的全部图
{
    ALGs p=Gs->next,q=NULL;
    while(p){
        q=p;
        p=p->next;
        DestroyGraph(q->data);
        free(q);
    }
    Gs->next=NULL;
    return OK;
}
status AddGraph(ALGs &Gs,char name[])
//在图集Gs中增加一个名字为name的图，成功返回OK,否则返回ERROR
{
    ALGs p=(ALGs)malloc(sizeof(GNode)),q=Gs;
    p->data.vexnum=0;
    p->data.arcnum=0;
    p->data.kind=UKN;
    strcpy(p->name,name);
    p->next=NULL;
    while(q->next) q=q->next;
    q->next=p;
    return OK;
}
status DeleteGraph(ALGs &Gs,char name[])
//在图集Gs中删除一个名字为name的图，成功返回OK,否则返回ERROR    
{
    ALGs p=Gs->next,q=Gs;
    while(p){
        if(strcmp(p->name,name)==0){
            q->next=p->next;
            DestroyGraph(p->data);
            free(p);
            return OK;
        }
        q=p;
        p=p->next;
    }
    return ERROR;
}
int LocateGs(ALGs Gs,char name[])
//根据name在图集Gs中查找图，查找成功返回位序，否则返回-1；
{
    ALGs p=Gs->next;
    int i=0;
    while(p){
        i++;
        if(strcmp(p->name,name)==0) return i;
        p=p->next;
    }
    return -1;
}
int ListGs(ALGs Gs){
    ALGs p=Gs->next;
    int i=0;
    while(p){
        i++;
        printf("%d %s\n",i,p->name);
        p=p->next;
    }
    return i;
}
status SaveGs(ALGs Gs,char Filename[])
//将图集Gs的数据写入到文件Filename中
{
    ALGs pn=Gs->next;
    if (pn==NULL) return ERROR;
    FILE *fp=fopen(Filename,"w");
    fprintf(fp,"Graphs1\n");
    if (fp==NULL) return ERROR;
    int n=0;
    while(pn){
        n++;
        pn=pn->next;
    }
    pn=Gs->next;
    fprintf(fp,"%d\n",n);
    while(pn){
        fprintf(fp,"%s\n",pn->name);
        ALGraph G=pn->data;
        int m=0;
        for(int i=0;i<G.vexnum;i++){
            ArcNode *p=G.vertices[i].firstarc;
            while(p){
                if(i<=p->adjvex){
                    m++;
                }
                p=p->nextarc;
            }
        }
        fprintf(fp,"%d %d\n",G.vexnum,m);
        for(int i=0;i<G.vexnum;i++){
            fprintf(fp,"%d %s\n",G.vertices[i].data.key,G.vertices[i].data.others);
        }
        for(int i=0;i<G.vexnum;i++){
            ArcNode *p=G.vertices[i].firstarc;
            while(p){
                fprintf(fp,"%d ",p->adjvex);
                p=p->nextarc;
            }
            fprintf(fp,"-1\n");
        }
        pn=pn->next;
    }
    fclose(fp);
    return OK;
}
status LoadGs(ALGs &Gs,char Filename[])
//读入文件Filename的图集数据，创建图集Gs
{
    if (Gs->next) return INFEASIBLE;
    FILE *fp=fopen(Filename,"r");
    if (!fp) return ERROR;
    char gra[30];
    ALGs a=Gs,b=NULL;
    fscanf(fp,"%s",gra);
    if(strcmp(gra,"Graphs1")==0){
        int t,n,m;
        fscanf(fp,"%d",&t);
        while(t--){
            b=(ALGs)malloc(sizeof(GNode));
            b->next=NULL;
            fscanf(fp,"%s",b->name);
            ALGraph G;
            G.vexnum=0;
            n=0,m=0;
            fscanf(fp,"%d %d",&n,&m);
            if (n>MAX_VERTEX_NUM || !n) {fclose(fp);return ERROR;}
            G.vexnum=n;
            for(int i=0;i<n;i++){
                fscanf(fp,"%d %s",&G.vertices[i].data.key,G.vertices[i].data.others);
                for(int j=0;j<i;j++)
                    if (G.vertices[i].data.key==G.vertices[j].data.key) {
                        G.vexnum=0;
                        fclose(fp);
                        return ERROR;
                    }
                G.vertices[i].firstarc=NULL;
            }
            G.arcnum=m;
            ArcNode *p=NULL,*pr=NULL;
            for(int i=0;i<n;i++){
                int k;
                pr=NULL;
                while(1){
                    fscanf(fp,"%d",&k);
                    if (k==-1) break;
                    p=(ArcNode*)malloc(sizeof(ArcNode));
                    p->adjvex=k;
                    p->nextarc=NULL;
                    if (!pr) G.vertices[i].firstarc=p;  
                    else pr->nextarc=p;
                    pr=p;
                }
            }
            G.kind=UDG;
            b->data=G;
            a->next=b;
            a=b;
        }
        fclose(fp);
        return OK;
    }
    else if (strcmp(gra,"Graphs2")==0){
        int t,i=0;
        fscanf(fp,"%d",&t);       
        while(t--){
            b=(ALGs)malloc(sizeof(GNode));
            b->next=NULL;
            b->data.vexnum=0;
            b->data.arcnum=0;
            b->data.kind=UKN;
            fscanf(fp,"%s",b->name); 
            VertexType V[MAX_VERTEX_NUM];
            KeyType VR[MAX_VERTEX_NUM][2];
            for(i=0;i<MAX_VERTEX_NUM;i++){
                        fscanf(fp,"%d%s",&V[i].key,V[i].others);
                        if(V[i].key==-1) break;
                    }
            i=0;
            do{
                fscanf(fp,"%d %d",&VR[i][0],&VR[i][1]);
                i++;
            }while(VR[i-1][0]!=-1);
            status flag=CreateGraph(b->data,V,VR);  
            if (flag!=OK) {DestroyGraphs(Gs);fclose(fp);return ERROR;}
            a->next=b;
            a=b;
        }
        fclose(fp);
        return OK;
    }
    else{
        fclose(fp);
        return ERROR;
    }
}
//samples:
//5 线性表 8 集合 7 二叉树 6 无向图 -1 nil  5 6  5 7 6 7 7 8 -1 -1
