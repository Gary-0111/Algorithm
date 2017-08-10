// Problem: POJ 1741
// Reference: 1. http://blog.csdn.net/yang_7_46/article/details/9966455
//            2. qzc.《分治算法在树的路径问题中的应用》


#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#define LL long long

using namespace std;
const int maxn = 10005;
struct node
{
    int v, w;
    node(int vv = 0, int ww = 0): v(vv), w(ww){}
};
vector<node> G[maxn];
int root, size, ans, f[maxn], s[maxn], d[maxn];
bool vis[maxn];
vector<int> dep;
int n, K;

void GetRoot(int u, int fa)
{
    s[u] = 1;
    f[u] = 0;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(v != fa && !vis[v])
	{
	    GetRoot(v, u);
	    s[u] += s[v];
	    f[u] = max(f[u], s[v]);
	}
    }
    f[u] = max(f[u], size-s[u]);
    if(f[u] < f[root]) root = u;
}

void GetDep(int u, int fa)
{
    dep.push_back(d[u]);
    s[u] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(v != fa && !vis[v])
	{
	    d[v] = d[u] + G[u][i].w;
	    GetDep(v, u);
	    s[u] += s[v];
	}
    }
}

int calc(int u, int init)
{
    dep.clear();
    d[u] = init;
    GetDep(u, 0);
    sort(dep.begin(), dep.end());
    int ret = 0;
    for(int l = 0, r = dep.size()-1; l < r;)
	if(dep[l] + dep[r] <= K) ret += (r-l++);
	else r--;
    return ret;
}

void solve(int u)
{
    ans += calc(u, 0);
    vis[u] = true;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i].v;
	if(!vis[v])
	{
	    ans -= calc(v, G[u][i].w);
	    f[0] = size = s[v];
	    GetRoot(v, root = 0);
	    solve(root);
	}
    }
}

int main()
{
    while(~scanf("%d%d", &n, &K))
    {
	if(n == 0 && K == 0) break;
	for(int i = 0; i <= n; i++) G[i].clear();
	memset(vis, false, sizeof(vis));
	ans = 0;
	int u, v, w;
	for(int i = 1; i < n; i++)
	{
	    scanf("%d%d%d", &u, &v, &w);
	    G[u].push_back(node(v, w));
	    G[v].push_back(node(u, w));
	}
	f[0] = size = n;
	GetRoot(1, root = 0);
	solve(root);
	printf("%d\n", ans);
    }
    return 0;
}
