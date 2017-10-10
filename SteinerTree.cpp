// Problem: HDU - 3311
/*  Reference
 *  1. SteinerTree: http://blog.csdn.net/gzh1992n/article/details/9119543
 *  2. http://blog.csdn.net/u010372095/article/details/44656931 
 */


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

using namespace std;

typedef long long LL;

const int INF = 0x3f3f3f3f;
const int maxn = 1e3+10;
const LL mod = 1e9+7;

struct edge
{
    int v, w;
    edge(int vv, int ww):v(vv), w(ww) {}
};

vector<edge> G[maxn];
int a[maxn];
int n, m, p;

/*
 *  Steiner Tree：求，使得指定K个点连通的生成树的最小总权值
 *  st[i] 表示顶点i的标记值，如果i是指定集合内第m(0 <= m < K)个点，则st[i] = 1<<m
 *  endSt = 1<<K
 *  dp[i][state] 表示以i为根，连通状态为state的生成树值
 */

int dp[maxn][1<<7], st[maxn], endSt;
bool vis[maxn][1<<7];
queue<int> Q;

void Init()
{
    for(int i = 0; i <= (n+m); i++) G[i].clear();
    memset(dp, -1, sizeof(dp));
    memset(st, 0, sizeof(st));
    for(int i = 0; i <= (n+m); i++) memset(vis[i], false, sizeof(vis[i]));
    endSt = (1<<(n+1));
    for(int i = 0; i <= n; i++) st[i] = (1<<i);
    for(int i = 0; i <= (n+m); i++) dp[i][st[i]] = 0;
}

void update(int &a, int x)
{
    a = ((a > x) || (a == -1))? x : a;
}

void SPFA(int state)
{
    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        vis[u][state] = false;
        for(int i = 0; i < G[u].size(); i++)
        {
            int v = G[u][i].v;
            if((dp[v][st[v]|state] == -1) || (dp[v][st[v]|state] > dp[u][state] + G[u][i].w))
            {
                dp[v][st[v]|state] = dp[u][state] + G[u][i].w;
                if(((st[v]|state) != state) || vis[v][state]) continue;
                vis[v][state] = true;
                Q.push(v);
            }
        }
    }
}

void steinerTree()
{
    for(int state = 1; state < endSt; state++)
    {
        for(int i = 0; i <= (n+m); i++)
        {
            if(st[i] && ((st[i]&state) == 0)) continue;
            for(int sub = (state-1)&state; sub; sub = (sub-1)&state)
            {
                int x = st[i]|sub;
                int y = st[i]|(state-sub);
                if(dp[i][x] != -1 && dp[i][y] != -1)
                    update(dp[i][state], dp[i][x]+dp[i][y]);
            }
            if(dp[i][state] != -1)
                Q.push(i), vis[i][state] = true;
        }
        SPFA(state);
    }
}

int main()
{
    while(~scanf("%d%d%d", &n, &m, &p))
    {
        Init();
        for(int i = 1; i <= (n+m); i++)
            scanf("%d", &a[i]);
        int u, v, w;
        for(int i = 1; i <= p; i++)
        {
            scanf("%d%d%d", &u, &v, &w);
            G[u].push_back(edge(v, w));
            G[v].push_back(edge(u, w));
        }
        for(int i = 1; i <= (n+m); i++)
        {
            G[0].push_back(edge(i, a[i]));
            G[i].push_back(edge(0, a[i]));
        }
        steinerTree();
        printf("%d\n", dp[0][endSt-1]);
    }
    return 0;
}

