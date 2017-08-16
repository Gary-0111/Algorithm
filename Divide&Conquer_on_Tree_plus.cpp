// Problem: SPOJ - FTOUR2
// Reference: qzc.IOI2009 中国国家集训队论文

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#define LL long long

using namespace std;
const double Pi = acos(-1.0);
const int maxn = 200005;

struct edge
{
    int v, w;
    edge(int vv, int ww): v(vv), w(ww) {}
};

vector<edge> G[maxn];
vector<int> maxl[maxn];
int root, ans, size, f[maxn], s[maxn];
bool vis[maxn], isblack[maxn];
int len[maxn];
int N, K, M;

void GetRoot(int u, int fa)
{
    s[u] = 1;
    f[u] = 0;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(v == fa || vis[v]) continue;
	GetRoot(v, u);
	s[u] += s[v];
	f[u] = max(f[u], s[v]);
    }
    f[u] = max(f[u], size - s[u]);
    if(f[u] < f[root]) root = u;
}

int GetLen(int u, int fa, int length, int cnt)
{
    int mxd = cnt;
    len[cnt] = max(len[cnt], length);
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(v == fa || vis[v]) continue;
	int d = GetLen(v, u, length + G[u][i].w, cnt + isblack[v]);
	mxd = max(mxd, d);
    }
    return mxd;
}

int Getson(int u, int fa)
{
    int son = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(v == fa || vis[v]) continue;
	son += Getson(v, u);
    }
    return son;
}

void solve(int u)
{
    int mxd = 0;
    vis[u] = true;
    for(int i = 0; i <= f[u]; i++)
	maxl[u].push_back(0);
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(!vis[v])
	{
	    int son = Getson(v, u);
	    for(int j = 0; j <= son; j++) len[j] = 0;
	    //printf("son[%d] = %d\n", v, son);
	
	    int d = GetLen(v, u, G[u][i].w, isblack[v]);
	    for(int j = 1; j <= d; j++)
		len[j] = max(len[j], len[j-1]);
	    //printf("d = %d\n", d);
	    //printf("--------root = %d  v = %d--------\n", u, v);
	    for(int j = 0; j <= d; j++)
	    {
		//printf("len[%d] = %d\n", j, len[j]);
		int fuck = min(K - isblack[u] - j, mxd);
		if(fuck < 0) break;
		ans = max(ans, len[j] + maxl[u][fuck]);
	    }
	    //printf("ans = %d\n", ans);
	    mxd = max(mxd, d);
	    for(int j = 0; j <= mxd; j++)
	    {
		maxl[u][j] = max(maxl[u][j], len[min(j, d)]);
		//printf("maxl[%d][%d] = %d\n", u, j, maxl[u][j]);
	    }
	    f[0] = size = son;
	    GetRoot(v, root = 0);
	    solve(root);
	}
    }
}

int main()
{
    scanf("%d%d%d", &N, &K, &M);
    int x;
    memset(isblack, false, sizeof(isblack));
    for(int i = 0; i < M; i++)
    {
	scanf("%d", &x);
	isblack[x] = true;
    }
    int u, v, w;
    for(int i = 1; i < N; i++)
    {
	scanf("%d%d%d", &u, &v, &w);
	G[u].push_back(edge(v, w));
	G[v].push_back(edge(u, w));
    }
    ans = 0;
    f[0] = size = N;
    GetRoot(1, root = 0);
    solve(root);
    printf("%d\n", ans);
    return 0;
}
