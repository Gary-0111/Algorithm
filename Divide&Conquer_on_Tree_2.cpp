// Problem: HDU 5977
// Reference: http://blog.csdn.net/u014665013/article/details/71158560
// Hint: std::map 太慢啦!!!!!!!!

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
const int maxn = 1e5+5;
const LL mod = 1e9+7;

int n, k;
int kind[maxn];
vector<int> G[maxn];

int root, size, f[maxn], s[maxn], d[maxn];
unsigned int ans;
bool vis[maxn];

void Init()
{
    for(int i = 1; i <= n; i++) G[i].clear();
    memset(vis, false, sizeof(vis));
    ans = 0;
}

void GetRoot(int u, int fa)
{
    s[u] = 1;
    f[u] = 0;
    for(int i = 0; i < G[u].size(); i++)
    {
        int v = G[u][i];
        if(v != fa && !vis[v])
        {
            GetRoot(v, u);
            s[u] += s[v];
            f[u] = max(f[u], s[v]);
        }
    }
    f[u] = max(f[u], size - s[u]);
    if(f[u] < f[root]) root = u;
}

vector<unsigned int> dep;
int mxd;
void GetDep(int u, int fa)
{
    d[u] = (d[fa]|kind[u]);
    mxd = max(mxd, d[u]);
    dep.push_back(d[u]);
    s[u] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
        int v = G[u][i];
        if(v != fa && !vis[v])
        {
            GetDep(v, u);
            s[u] += s[v];
        }
    }
}

unsigned int cnt[1100];
int stack[1100], top;
unsigned int calc(int u, int init)
{
    dep.clear();
    d[0] = init;
    GetDep(u, 0);
    for(int i = 0; i <= mxd; i++) cnt[i] = 0;
    for(int i = 0; i < dep.size(); i++) cnt[dep[i]]++;
    top = 0;
    for(int i = 0; i <= mxd; i++) if(cnt[i]) stack[top++] = i;

    unsigned int ret = 0;
    for(int i = 0; i < top; i++)
    {
        unsigned int count = 0;                                 //stack[i]的补集的超集个数
        for(int S0 = stack[i]; S0; S0 = ((S0-1)&(stack[i])))    //枚举状态stack[i]的补集的超集,也就是枚举stack[i]的子集的补集
            count += cnt[((1<<k)-1)^(S0)];
        count += cnt[(1<<k)-1];                                 //特判子集为空集的情况
        ret += (cnt[stack[i]]*count);                           //stack[i]的个数乘上补集的超集个数
    }
    return ret;
}

void solve(int u)
{
    ans += calc(u, 0);
    vis[u] = true;
    for(int i = 0; i < G[u].size(); i++)
    {
        int v = G[u][i];
        if(!vis[v])
        {
            ans -= calc(v, kind[u]);
            f[0] = size = s[v];
            GetRoot(v, root = 0);
            solve(root);
        }
    }
}

int main()
{
    while(~scanf("%d%d", &n, &k))
    {
        Init();
        int t;
        for(int i = 1; i <= n; i++)
        {
            scanf("%d", &t);
            kind[i] = (1<<(t-1));
        }
        int u, v;
        for(int i = 1; i < n; i++)
        {
            scanf("%d%d", &u, &v);
            G[u].push_back(v);
            G[v].push_back(u);
        }
        f[0] = size = n;
        GetRoot(1, root = 0);
        solve(root);
        printf("%u\n", ans);
    }
	return 0;
}
