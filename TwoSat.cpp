// Problem: POJ - 3683
// Reference: 由对称性解2-SAT问题. 伍昱,03年IOI国家集训队论文ppt

#include <cstdio>
#include <queue>
#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <cmath>
#include <stack>

using namespace std;

typedef long long LL;

const int INF = 0x3f3f3f3f;
const int maxn = 1e3+10;
const LL mod = 1e9+7;

struct edge
{
    int u, v, next;
    edge(int uu, int vv, int nn):u(uu), v(vv), next(nn) {}
};

struct node
{
    int l, r;
    node(int ll = 0, int rr = 0):l(ll), r(rr) {}
}sch[maxn][2];

// 原图
vector<edge> E;
int head[maxn*2];
// 缩点之后的图
vector<int> G[maxn*2];

int dfn[maxn*2], low[maxn*2], stamp;
int stk[maxn*2], top;
int belong[maxn*2], scc;
bool instk[maxn*2];
bool fk[maxn*2][maxn*2];

void Init(int n)
{
    E.clear();
    memset(head, -1, sizeof(head));
    stamp = top = scc = 0;
    memset(instk, false, sizeof(instk));
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(belong, 0, sizeof(belong));
    for(int i = 0; i < 2*n; i++) G[i].clear();
}

void addedge(int u, int v)
{
    E.push_back(edge(u, v^1, head[u]));
    head[u] = E.size() - 1;
    E.push_back(edge(v, u^1, head[v]));
    head[v] = E.size() - 1;
}

void tarjan(int u)
{
    dfn[u] = low[u] = ++stamp;
    instk[u] = true;
    stk[++top] = u;
    for(int i = head[u]; i != -1; i = E[i].next)
    {
        int v = E[i].v;
        if(!dfn[v])
        {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else if(instk[v])
            low[u] = min(low[u], dfn[v]);
    }
    if(dfn[u] == low[u])
    {
        int v;
        do
        {
            v = stk[top--];
            instk[v] = false;
            belong[v] = scc;
        }while(v != u);
        scc++;
    }
}

int du[maxn*2];     // 各点的入度数
int L[maxn*2], tot; // L拓扑排序的结果
bool del[maxn*2];   //
int op[maxn*2];     // tarjan缩点之后各点的对称点

void toposort(int n)
{
    tot = 0;
    memset(du, 0, sizeof(du));
    queue<int> Q;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < G[i].size(); j++)
            du[G[i][j]]++;
    for(int i = 0; i < n; i++)
        if(!du[i]) Q.push(i);
    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        L[tot++] = u;
        for(int i = 0; i < G[u].size(); i++)
        {
            du[G[u][i]]--;
            if(!du[G[u][i]]) Q.push(G[u][i]);
        }
    }
}

bool fuck(node a, node b)
{
    if(a.l >= b.l && a.l < b.r) return true;
    if(b.l >= a.l && b.l < a.r) return true;
    return false;
}

bool check(int n)
{
    for(int i = 0; i < n; i++)
        if(belong[2*i] == belong[2*i+1])
            return false;
    return true;
}

void Delete(int u)
{
    del[op[u]] = true;
    for(int i = 0; i < G[u].size(); i++)
    {
        int v = G[u][i];
        if(!del[op[v]]) Delete(v);
    }
}

int main()
{
    int n;
    while(~scanf("%d", &n))
    {
        Init(n);
        int h1, m1, h2, m2, t;
        for(int i = 0; i < n; i++)
        {
            scanf("%d:%d %d:%d %d", &h1, &m1, &h2, &m2, &t);
            sch[i][0] = node(h1*60+m1, h1*60+m1+t);
            sch[i][1] = node(h2*60+m2-t, h2*60+m2);
        }
        for(int i = 0; i < n; i++)
            for(int j = i+1; j < n; j++)
                for(int a = 0; a < 2; a++)
                    for(int b = 0; b < 2; b++)
                        if(fuck(sch[i][a], sch[j][b]))
                        {
                            //printf("(%d,%d)\n", 2*i+a, 2*j+b);
                            addedge(2*i+a, 2*j+b);
                        }
        for(int i = 0; i < 2*n; i++)
            if(!dfn[i])
                tarjan(i);
        if(!check(n))
            printf("NO\n");
        else
        {
            printf("YES\n");
            memset(fk, false, sizeof(fk));
            for(int i = 0; i < E.size(); i++)
            {
                //printf("(%d,%d)\n", E[i].u, E[i].v);
                if((belong[E[i].u] != belong[E[i].v]) && !fk[belong[E[i].u]][belong[E[i].v]])
                {
                    //printf("(%d,%d)\n", belong[E[i].u], belong[E[i].v]);
                    fk[belong[E[i].u]][belong[E[i].v]] = true;
                    G[belong[E[i].u]].push_back(belong[E[i].v]);
                }
            }

            // 求各强联通分量的对称点
            for(int i = 0; i < 2*n; i++)
                op[belong[i]] = belong[i^1];

            toposort(scc);
            memset(del, false, sizeof(del));
            for(int i = tot-1; i >= 0; i--)
            {
                //printf("%d ", L[i]);
                if(del[L[i]]) continue;
                Delete(L[i]);
            }
            for(int i = 0; i < 2*n; i++)
                if(!del[belong[i]])
                {
                    int t1 = sch[i/2][i%2].l;
                    int t2 = sch[i/2][i%2].r;
                    printf("%02d:%02d %02d:%02d\n", t1/60, t1%60, t2/60, t2%60);
                }
        }
    }
    return 0;
}
