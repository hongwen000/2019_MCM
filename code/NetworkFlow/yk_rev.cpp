#include <iostream>
#include <cstdio>
#include <fstream>
#include <utility>

 
using namespace std;
 
const int oo=1e9;//无穷大
const int maxm=1111111;//边的最大数量，为原图的两倍
const int maxn=2222;//点的最大数量
 
int node,src,dest,edge;//node节点数，src源点，dest汇点，edge边数
int head[maxn],p[maxn],dis[maxn],q[maxn],vis[maxn];//head链表头，p记录可行流上节点对应的反向边，dis计算距离
 
struct edgenode
{
    int from;
    int to;//边的指向
    int flow;//边的容量
    int maxflow;
    int cost;//边的费用
    int next;//链表的下一条边
} edges[maxm];
 
void prepare(int _node,int _src,int _dest);
void addedge(int u,int v,int f,int c);
bool spfa();
 
inline int min(int a,int b)
{
    return a<b?a:b;
}
 
inline void prepare(int _node,int _src,int _dest)
{
    node=_node;
    src=_src;
    dest=_dest;
    for (int i=0; i<node; i++)
    {
        head[i]=-1;
        vis[i]=false;
    }
    edge=0;
}
 
void addedge(int u,int v,int f,int c)
{
    edges[edge].from = u;
    edges[edge].flow=f;
    edges[edge].maxflow = f;
    edges[edge].cost=c;
    edges[edge].to=v;
    edges[edge].next=head[u];
    head[u]=edge++;
    edges[edge].from = v;
    edges[edge].flow=0;
    edges[edge].maxflow = 0;
    edges[edge].cost=-c;
    edges[edge].to=u;
    edges[edge].next=head[v];
    head[v]=edge++;
}
 
bool spfa()
{
    int i,u,v,l,r=0,tmp;
    for (i=0; i<node; i++) dis[i]=oo;
    dis[q[r++]=src]=0;
    p[src]=p[dest]=-1;
    for (l=0; l!=r; ((++l>=maxn)?l=0:1))
    {
        for (i=head[u=q[l]],vis[u]=false; i!=-1; i=edges[i].next)
        {
            if (edges[i].flow&&dis[v=edges[i].to]>(tmp=dis[u]+edges[i].cost))
            {
                dis[v]=tmp;
                p[v]=i^1;
                if (vis[v]) continue;
                vis[q[r++]=v]=true;
                if (r>=maxn) r=0;
            }
        }
    }
    return p[dest]>=0;
}
 
pair<int,int> spfaflow()
{
    int i,ret=0,delta, retr;
    while (spfa())
    {
        //按记录原路返回求流量
 
        for (i=p[dest],delta=oo; i>=0; i=p[edges[i].to])
        {
            delta=min(delta,edges[i^1].flow);
        }
        for (int i=p[dest]; i>=0; i=p[edges[i].to])
        {
            edges[i].flow+=delta;
            edges[i^1].flow-=delta;
        }
        ret+=delta*dis[dest];
        retr+=delta;
    }
    cout << "Ret is " << ret << endl;
    return make_pair(ret, retr);
}

int show_graph() {
    for (int e = 0; e < edge; e++) {
        if (edges[e].maxflow > 0 && edges[e].maxflow - edges[e].flow > 0) {
            printf("from %d, to %d, flow: %d * %d\n", edges[e].from, edges[e].to, edges[e].maxflow - edges[e].flow, edges[e].cost);
        }
    }
}

int get_c(int T, int alpha, int f)
{
    return T * alpha * f * 2.24 / 8.0;
}

int get_ret_when_T(int T)
{
    std::fstream s("data.in");
    int n_node, src, dest, u, v, f, c;
    s >> n_node >> src >> dest;
    prepare(n_node, src-1, dest-1);
    while(s >> u >> v >> f >> c)
    {
        addedge(u-1, v-1, get_c(T, 1, f), c);
    }
    return spfaflow().second;
}

int main()
{
    int require = 10000;
    int flow = 0;
    int T = 0;
    while(flow < require)
    {
        T += 60;
        flow = get_ret_when_T(T);
        cout << "T = " << T << "Current flow is " << flow << endl;
    }
    show_graph();
    return 0;
}
