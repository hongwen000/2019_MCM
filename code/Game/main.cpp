#include <iostream>
#include <algorithm>
#include <set>
#include <fstream>
#include <random>
#include "map"
#include "vector"
#include "list"
#include  "fmt/format.h"
using namespace fmt;
using namespace std;
// 定义边结构
struct edge_t {
    // 箭头节点编号
    int from;
    // 箭尾节点编号
    int to;
    // 边的容载能力
    double width;
    // 边的"长度"
    double len;
    // 逃生路径要经过这条边的人数
    int num_person = 0;
    // 经过这台边的"费用", 其中1.4是人数为0时的初始逃生速度值
    double cost = len / 1.4;
    // 拥挤程度
    double rho;
    // 逃生速度
    double vel;
    // 将经过这条边的人数设置为num
    void set_person(int num)
    {
         num_person = num;
         // 更新拥挤程度为 num / (width * len)
         rho = double(num) / (width * len);
         // 根据论文公式，根据rho更新vel
         if(rho < 0.75)
         {
             vel = 1.4;
         }
         else if (rho > 4.2)
         {
             vel = 0.42 / rho;
         }
         else
         {
             vel = 1.867 - 0.59 * rho + 0.0412 * rho * rho;
         }
//        print("[DEBUG]: rho is {}, vel is {} \n", rho, vel);
         // 更新经过该条边所耗的逃生费用
        cost = len / vel;
    }
    // 将经过这条边的人数设置减少一个
    void sub_person()
    {
        if(num_person <= 0)
        {
            char what[100];
            sprintf(what, "Wrong sub person, from %d, to %d, num_person %d", from ,to , num_person);
            throw std::runtime_error{what};
        }
        set_person(num_person-1);
    }
    // 将经过这条边的人数设置增加一个
    void add_person()
    {
        set_person(num_person+1);
    }
};

// 建筑物结构定义，可以忽略
using graph_t = map<int, vector<edge_t>>;

graph_t graph;

set<int> exits;

using path_t = list<int>;

// "人"的定义
struct person_t {
    // 初始位置
    int loc = -1;
    // 选择的路径的编号
    int choice = -1;
    // 可选的路径的列表
    vector<path_t> routes;
};

vector<person_t> people;
// 搜索函数
void explore(path_t path, person_t& person)
{
    int n = path.back();
    if(exits.count(n))
    {
        person.routes.push_back(path);
    }
    for(auto& e: graph[n])
    {
        if(std::find(path.begin(), path.end(), e.to) != path.end())
        {
            continue;
        }
        path.push_back(e.to);
        explore(path, person);
        path.pop_back();
    }
}
// 探索某个人从起始点开始的所有可能的逃生路径
void find_routes(person_t& person)
{
    explore(list<int>{person.loc}, person);
}

// 随机数函数
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 10000);

//打印路径信息
void print_route(const path_t& p, bool print_detail = true)
{
    double total_time = 0;
    for(auto it = p.begin(); it != p.end(); ++it)
    {
        auto it2 = it;
        it2++;
        if(it2 == p.end())
            break;
        for(auto& e:graph[*it])
        {
            if(e.to == *it2)
            {
                if(print_detail)
                    print("\t{} -> {}, len: {}, width: {}, people: {}, rho: {}, vel:{}, cost: {}\n", e.from, e.to, e.len, e.width, e.num_person, e.rho, e.vel, e.cost);
                total_time += e.cost;
            }
        }
    }
    for(auto& i: p)
    {
        cout << i << ", ";
    }
    cout << "Totaltime: " << total_time << endl;
}

// 打印人的信息
void print_person(const person_t& p, bool print_details = true, bool if_print_route = true)
{
    print("Person Detail: loc: {}, choice: {}\n", p.loc, p.choice);
    int i = 0;
    if (if_print_route)
    for(auto& r: p.routes)
    {
        cout << "Route " << i++  << " details" << endl;
        print_route(r, print_details);
    }
}

vector<int> sources;
// 初始化人群，平均分布在每个起始点，并找到所有可能的逃生路径
void init_people(int N)
{
    vector<person_t> p0;
    for(auto& s: sources)
    {
        person_t p;
        p.loc = s;
        find_routes(p);
        p0.push_back(p);
    }
    size_t M = sources.size();
    for(int i = 0; i < N; ++i)
    {
        person_t p;
        p = p0[i%M];
        p.choice = static_cast<int>(dis(gen)) % p.routes.size();
        if(p.routes[p.choice].size() < 2)
            continue;
        for(auto it = p.routes[p.choice].begin(); it != p.routes[p.choice].end(); ++it)
        {
            auto it2 = it;
            it2++;
            if(it2 == p.routes[p.choice].end())
                break;
            for(auto& e:graph[*it])
            {
                if(e.to == *it2)
                {
                    e.add_person();
                }
            }
        }
        people.push_back(p);
    }
}

// 读取建筑物结构
void read_network()
{
    std::fstream s("data.in");
    int n_node, src, dest, u, v;
    double f, c;
    s >> n_node >> src >> dest;
    while(s >> u >> v >> f >> c)
    {
        edge_t e{u,v,f/8,c};
        graph[u].push_back(e);
        if(v == dest)
        {
            exits.insert(u);
        }
        if(u == src)
        {
            sources.push_back(v);
        }
    }
}

// 获取采用某条路径p逃生消耗的时间
double get_route_time(const path_t& p)
{
    double total_time = 0;
    for(auto it = p.begin(); it != p.end(); ++it)
    {
        auto it2 = it;
        it2++;
        if(it2 == p.end()) break;
        for(auto& e:graph[*it])
            if(e.to == *it2)
                total_time += e.cost;
    }
    return total_time;
}

// 决定不采用某条路径
void drop_route(const path_t& p)
{
    //遍历路径包含的每一条边，将改边上经过的人数的数量-1
    for(auto it = p.begin(); it != p.end(); ++it)
    {
        auto it2 = it;
        it2++;
        if(it2 == p.end()) break;
        for(auto& e:graph[*it])
            if(e.to == *it2)
            {
                e.sub_person();
            }
    }
}

// 决定采用某条路径逃生
void select_route(const path_t& p)
{
    //遍历路径包含的每一条边，将改边上经过的人数的数量+1
    for(auto it = p.begin(); it != p.end(); ++it)
    {
        auto it2 = it;
        it2++;
        if(it2 == p.end()) break;
        for(auto& e:graph[*it])
            if(e.to == *it2)
                e.add_person();
    }
}


// 更新person p的决策
bool update_policy(person_t & p, int idx)
{
    // 记录选择哪条逃生路线
    int c = -1;
    // 记录最小的逃生代价
    double m = 1E9;
    // 对每条逃生路线  
    for(int i = 0; i < p.routes.size(); ++i)
    {
        // 尝试改为这条路线
        drop_route(p.routes[p.choice]);
        select_route(p.routes[i]);
        // 获取改为这条路线后，逃生的代价
        auto ret = get_route_time(p.routes[i]);
        // 如果新的代价更小，那么有可能选择这个路径
        if(ret < m)
        {
            m = ret;
            c = i;
        }
        // 尝试下一条路线前恢复相关变量的值
        drop_route(p.routes[i]);
        select_route(p.routes[p.choice]);
    }
    // 如果没有更新决策
    if(p.choice == c)
        // 告知调用者我没有更新决策
        return false;
    // 其余情况，如果更新了决策
    // 抛弃当前选择的路径
    drop_route(p.routes[p.choice]);
    print("{} (loc:{} ) updated policy:  {} to {}, cost {} to {}\n", idx, p.loc, p.choice, c, get_route_time(p.routes[p.choice]), get_route_time(p.routes[c]));
    // 改为新的选择
    p.choice = c;
    select_route(p.routes[p.choice]);
    print_route(p.routes[p.choice]);
    return true;
}

int main()
{
    // 读取建筑物结构
    read_network();
    // 初始化10000人，平均分布在每个起始点，并找到所有可能的逃生路径
    init_people(10000);
    // 打印每个人的信息
    for(auto&p: people)
    {
        print_person(p);
    }
    // 记录是否有人更新了自己的决策
    bool flag = true;
    int T = 0;
    while (flag)
    {
        // 进行第T次更新
        print("Updating, round: {}\n", T++);
        flag = false;
        // 对每个人
        for(int i = 0; i < people.size(); ++i)
        {
            //更新决策
            auto ret = update_policy(people[i], i);
            //如果有一个人更新了策略
            if(ret){
                flag = true;
            }
        }
    }
    return 0;
}
