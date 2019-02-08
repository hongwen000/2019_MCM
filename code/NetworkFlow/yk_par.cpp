#include <iostream>
#include <vector>
#include <map>
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
    int i,ret=0,delta, retr=0;
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
    return make_pair(ret, retr);
}

    const vector<int> sources{1, 3, 7, 11, 16, 19,
                    22, 25, 29, 30, 34, 39, 42, 45,
                    47, 50, 54, 55, 67, 70, 71, 72,
                    74, 81, 86, 87};
void show_graph() {
    for (int e = 0; e < edge; e++) {
        if (edges[e].maxflow > 0 && edges[e].maxflow - edges[e].flow > 0) {
            printf("from %d, to %d, flow: %d * %d\n", edges[e].from, edges[e].to, edges[e].maxflow - edges[e].flow, edges[e].cost);
        }
        if (edges[e].from == src && (edges[e].maxflow - edges[e].flow) < (6000 / sources.size()))
        {
            printf("Source %d is not fullly ok\n", edges[e].to);
        }
    }
}

int get_c(int T, double alpha, int f)
{
    return T * alpha * f * 2 * 0.85 / 8.0;
}

int get_ret_when_T(int T, const std::vector<int> sinks, const std::vector<int> sources, int require, double alpha)
{
    std::fstream s("data.in");
    int u, v, f, c, type;
    s >> node >> src >> dest;
    // printf("node:%d, src:%d, dest:%d\n", node, src, dest);
    prepare(node, src, dest);
    while(s >> u >> v >> f >> c >> type)
    {
        addedge(u, v, (1-0.10) * get_c(T, alpha, f), c);
        addedge(u, v, 0.10 * get_c(T, alpha, f), 2 * c);
        //addedge(v, u, get_c(T, 1, f), c);
    }
    for(int i = 0; i < sinks.size(); ++i)
    {
        addedge(sinks[i], dest, require, 0);
    }
    for(int i = 0; i < sources.size(); ++i)
    {
        addedge(src, sources[i], (require / sources.size())+1, 0);
    }
    auto ret = spfaflow();
    cout << ret.first << endl;
    return ret.second;

}

int main(int argc, char** argv)
{
    double alpha =  1;
    const vector<int> sinks{2, 19, 34, 46};
    const int require = 6000;
    int flow = 0;
    int T = 0;
    while(flow < require)
    {
        T += 1;
        flow = get_ret_when_T(T, sinks, sources, require, alpha);
    }
    cout <<alpha << " " << T  << endl;
    return 0;
}
