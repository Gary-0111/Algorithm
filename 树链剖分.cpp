// Problem: SPOJ - QTREE
// Reference: 1. http://blog.csdn.net/acdreamers/article/details/10591443
//            2. http://blog.csdn.net/ACdreamers/article/details/10639685
//            3. qzc. IOI2009 中国国家集训队论文



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

const int maxn = 50005;
const int INF = 1<<30;

int siz[maxn];	    //子树节点个数
int top[maxn];	    //节点所在链
int son[maxn];	    //各节点的重儿子
int dep[maxn];	    //节点深度
int fa[maxn];	    //节点父亲
int tid[maxn];	    //节点剖分后的新编号
int n, tim;

struct edge
{
    int v, w;
    edge(int vv = -1, int ww = 0): v(vv), w(ww) {}
};
vector<edge> E;
vector<int> G[maxn];

void init()
{
    memset(son, -1, sizeof(son));
    tim = 0;
    E.clear();
    for(int i = 0; i < maxn; i++)
	G[i].clear();
}

void addedge(int u, int v, int w)
{
    G[u].push_back(E.size());
    E.push_back(edge(v, w));
    G[v].push_back(E.size());
    E.push_back(edge(u, w));
}

//树链剖分
// dfs1 找出重边, dfs2 链接重边形成重链
void dfs1(int u, int f, int d)
{
    dep[u] = d;
    fa[u] = f;
    siz[u] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = E[G[u][i]].v;
	if(v == f) continue;
	dfs1(v, u, d+1);
	siz[u] += siz[v];
	if(son[u] == -1 || siz[v] > siz[son[u]])
	    son[u] = v;
    }
}

void dfs2(int u, int tp)
{
    top[u] = tp;
    tid[u] = ++tim;
    if(son[u] == -1) return;
    dfs2(son[u], tp);
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = E[G[u][i]].v;
	if(v == son[u] || v == fa[u]) continue;
	dfs2(v, v);
    }
}

//线段树
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
int Max[maxn<<2], num[maxn];

void PushUp(int rt)
{
    Max[rt] = max(Max[rt<<1], Max[rt<<1|1]);
}

void Build(int l, int r, int rt)
{
    if(l == r)
    {
	Max[rt] = num[l];
	return;
    }
    int m = (l+r) >> 1;
    Build(lson);
    Build(rson);
    PushUp(rt);
}

void Update(int p, int val, int l, int r, int rt)
{
    if(l == r)
    {
	Max[rt] = val;
	return;
    }
    int m = (l+r) >> 1;
    if(p <= m) Update(p, val, lson);
    else Update(p, val, rson);
    PushUp(rt);
}

int Query(int L, int R, int l, int r, int rt)
{
    if(L <= l && r <= R)
	return Max[rt];
    int m = (l+r) >> 1;
    int ret = -INF;
    if(L <= m) ret = max(ret, Query(L, R, lson));
    if(R > m) ret = max(ret, Query(L, R, rson));
    return ret;
}

void change(int i, int val)
{
    if(dep[E[i<<1].v] > dep[E[i<<1|1].v])
	Update(tid[E[i<<1].v], val, 2, n, 1);
    else
	Update(tid[E[i<<1|1].v], val, 2, n, 1);
}

int query(int u, int v)
{
    int ans = -INF;
    while(top[u] != top[v])
    {
	if(dep[top[u]] < dep[top[v]]) swap(u, v);
	ans = max(ans, Query(tid[top[u]], tid[u], 2, n, 1));
	u = fa[top[u]];
    }
    if(dep[u] > dep[v]) swap(u, v);
    if(u != v) ans = max(ans, Query(tid[u] + 1, tid[v], 2, n, 1));
    return ans;
}

int main()
{
    int T;
    scanf("%d", &T);
    while(T--)
    {
	init();
	scanf("%d", &n);
	int u, v, w;
	for(int i = 1; i < n; i++)
	{
	    scanf("%d%d%d", &u, &v, &w);
	    addedge(u, v, w);
	}
	dfs1(1, -1, 1);
	dfs2(1, 1);
	for(int i = 0; i < E.size(); i+=2)
	{
	    if(dep[E[i].v] > dep[E[i+1].v])
		num[tid[E[i].v]] = E[i].w;
	    else
		num[tid[E[i+1].v]] = E[i].w;
	}
	Build(2, n, 1);
	char op[10];
	int a, b;
	while(1)
	{
	    scanf("%s", op);
	    if(op[0] == 'D') break;
	    scanf("%d%d", &a, &b);
	    if(op[0] == 'Q')
		printf("%d\n", query(a, b));
	    else
		change(a-1, b);
	}
    }
    return 0;
}
