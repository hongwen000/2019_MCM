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
    double cost;//边的费用
    int next;//链表的下一条边
    int cps1 = -1;
    int cps2 = -1;
} edges[maxm];
 
void prepare(int _node,int _src,int _dest);
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
 
void addedge(int u,int v,int f,double c, int cps1 = -1, int cps2 = -1)
{
    //printf("Adding edge %d, %d, %d, %d, %d, %d\n", u, v, f, c, cps1, cps2);
    edges[edge].from = u;
    edges[edge].flow=f;
    edges[edge].maxflow = f;
    edges[edge].cost=c;
    edges[edge].to=v;
    edges[edge].next=head[u];
    edges[edge].cps1 = cps1;
    edges[edge].cps2 = cps2;
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
struct lxredge_t
{
    int u;
    int v;
    int f;
    double c;
    int cps1 = -1;
    int cps2 = -1;
};

vector<lxredge_t> realedges;
vector<lxredge_t> fakeedges;

map<int, pair<int, int>> records;

void show_graph() {
    for (int e = 0; e < edge; e++) {
        if (edges[e].maxflow > 0 && edges[e].maxflow - edges[e].flow > 0) {
            int cps1 = edges[e].cps1;
            int cps2 = edges[e].cps2;
            if(cps1 != -1)
                records[cps1].first = edges[e].maxflow - edges[e].flow;
            if(cps2 != -1)
                records[cps2].second = edges[e].maxflow - edges[e].flow;
        }
    }
    long total_cost = 0;
    for(int i = 0; i < realedges.size(); ++i)
    {
        if(records.count(i) != 0)
        {
            printf("from %d, to %d, cost: %.2lf, for-flow: %d, rev-flow: %d\n", realedges[i].u, realedges[i].v, realedges[i].c, records[i].first, records[i].second);
        }
        total_cost += realedges[i].c * records[i].first + realedges[i].c * records[i].second;
    }
    cout << "Total cost is " << total_cost << endl;
}

int get_c(int T, int alpha, int f)
{
    return T * alpha * f * 2.24 / 8.0;
}


string datafile = "data.in";

map<lxredge_t, pair<int, int>> coresponding;
int get_ret_when_T(int T, const std::vector<int> sinks, const std::vector<int> sources, int require)
{
    realedges.clear();
    fakeedges.clear();
    std::fstream s(datafile);
    int u, v, f, type;
    double c;
    s >> node >> src >> dest;
    printf("node:%d, src:%d, dest:%d\n", node, src, dest);

    //prepare(node, src, dest);
    while(s >> u >> v >> f >> c >> type)
    {
        f = f * T;
        realedges.push_back(lxredge_t{u,v,f,c});
        //addedge(u, v, get_c(T, 1, f), c);
        //addedge(v, u, get_c(T, 1, f), c);
    }
    int allnodes = node;
    //cout << "Size of realedge" << realedges.size() << endl;
    for(int i = 0; i < realedges.size(); ++i)
    {
        int A = realedges[i].u;
        int B = realedges[i].v;
        int f = realedges[i].f;
        double c = realedges[i].c;
        int Csub = allnodes++;
        int Cadd = allnodes++;
        fakeedges.push_back(lxredge_t{Cadd, B, oo, c/2, i, -1});
        fakeedges.push_back(lxredge_t{Cadd, A, oo, c/2, -1, i});
        fakeedges.push_back(lxredge_t{B, Csub, oo, c/2});
        fakeedges.push_back(lxredge_t{A, Csub, oo, c/2});
        fakeedges.push_back(lxredge_t{Csub, Cadd, f, 0});
    }
    node = allnodes;
    prepare(node, src, dest);
    for(const auto& fakeedge: fakeedges)
    {
        //printf("Adding fakeedge %d, %d, %d, %d, %d, %d\n", fakeedge.u, fakeedge.v, fakeedge.f, fakeedge.c, fakeedge.cps1, fakeedge.cps2);
        addedge(fakeedge.u, fakeedge.v, fakeedge.f, fakeedge.c, fakeedge.cps1, fakeedge.cps2);
    }
    for(int i = 0; i < sinks.size(); ++i)
    {
        addedge(sinks[i], dest, require, 0);
    }
    for(int i = 0; i < sources.size(); ++i)
    {
        addedge(src, sources[i], (require / sources.size())+1, 0);
    }
    
    
    return spfaflow().second;
}

int main(int argc, char** argv)
{
    if(argc >= 2)
    {
        datafile = argv[1];
        cout << datafile << endl;
    }
    //const vector<int> sinks{2, 19, 34, 46};
    //const vector<int> sources{1, 3, 7, 11, 16, 19,
    //                22, 25, 29, 30, 34, 39, 42, 45,
    //                47, 50, 54, 55, 67, 70, 71, 72,
    //                74, 81, 86, 87};
    const vector<int> sinks{2};
    const vector<int> sources{1};
    const int require = 10000;
    int flow = 0;
    int T = 0;
    while(flow < require)
    {
        T += 60;
        flow = get_ret_when_T(T, sinks, sources, require);
        cout << "T = " << T << ", Current flow is " << flow << endl;
        show_graph();
    }
    return 0;
}
