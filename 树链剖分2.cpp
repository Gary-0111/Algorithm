// Problem: SPOJ - QTREE2
// Reference: 1. 树链剖分
//            2. LCA : 余勇.ACM国际大学生程序设计竞赛: 算法与实现

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
#define LL long long
using namespace std;

const int maxn = 100005;
const int INF = 1<<30;

int siz[maxn];	    //子树节点个数
int son[maxn];	    //各节点的重儿子
int dep[maxn];	    //节点深度
int fa[maxn];	    //节点父亲
int top[maxn];	    //节点所在重链的头节点
int tid[maxn];	    //节点剖分后的新编号
int Rank[maxn];
int dist[maxn];
int n, q, tim, a[maxn];
struct edge
{
    int v, w;
    edge(int vv = 0, int ww = 0): v(vv), w(ww) {}
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
void dfs1(int u, int f, int d, int dis)
{
    siz[u] = 1;
    fa[u] = f;
    dep[u] = d;
    dist[u] = dis;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = E[G[u][i]].v;
	if(v == f) continue;
	dfs1(v, u, d+1, dis+E[G[u][i]].w);
	siz[u] += siz[v];
	if(son[u] == -1 || siz[son[u]] < siz[v])
	    son[u] = v;
    }
}

void dfs2(int u, int tp)
{
    top[u] = tp;
    tid[u] = ++tim;
    Rank[tid[u]] = u;
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
/*
#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1
int sum[maxn<<2], num[maxn];

void PushUp(int rt)
{
    sum[rt] = sum[rt<<1] + sum[rt<<1|1];
}

void Build(int l, int r, int rt)
{
    if(l == r)
    {
	sum[rt] = num[l];
	return;
    }
    int m = (l + r) >> 1;
    Build(lson);
    Build(rson);
    PushUp(rt);
}

int Query(int L, int R, int l, int r, int rt)
{
    if(L <= l && r <= R)
    {
	return sum[rt];
    }
    int m = (l+r) >> 1;
    int ret = 0;
    if(L <= m) ret += Query(L, R, lson);
    if(R > m) ret += Query(L, R, rson);
    return ret;
}
*/

//LCA
const int maxh = 14;
int anc[maxn][15];
void GetAnc(int root)
{
    static int Stack[maxn];
    static int cur[maxn];
    memset(cur, 0, sizeof(cur));
    int top = 0;
    for(int i = 0; i <= maxh; i++)
	anc[root][i] = root;
    Stack[++top] = root;
    while(top)
    {
	int x = Stack[top];
	if(x != root)
	{
	    for(int i = 1; i <= maxh; i++)
	    {
		int y = anc[x][i-1];
		anc[x][i] = anc[y][i-1];
	    }
	}
	for(int &i = cur[x]; i < G[x].size(); i++)
	{
	    int y = E[G[x][i]].v;
	    if(y != anc[x][0])
	    {
		anc[y][0] = x;
		Stack[++top] = y;
	    }
	}
	while(top && G[Stack[top]].size() == cur[Stack[top]]) top--;
    }
}

void swim(int &x, int H)
{
    for(int i = 0; H > 0; i++)
    {
	if(H&1) x = anc[x][i];
	H>>=1;
    }
}

int LCA(int x, int y)
{
    int i;
    if(dep[x] > dep[y]) swap(x, y);
    swim(y, dep[y] - dep[x]);
    if(x == y) return x;
    for(;;)
    {
	for(i = 0; anc[x][i] != anc[y][i]; i++);
	if(i == 0)
	    return anc[x][0];
	x = anc[x][i-1];
	y = anc[y][i-1];
    }
    return -1;
}

int Dist(int u, int v)
{
    int lca = LCA(u, v);
    LL ret = (LL)dist[u] + dist[v] - 2*dist[lca];
    return (int)ret;
}

int Kth(int u, int v, int k)
{
    int lca = LCA(u, v);
    int d = dep[u] + dep[v] - 2*dep[lca] + 1;
    //printf("lca = %d   d = %d\n", lca, d);
    if(k > dep[u] - dep[lca] + 1)
    {
	k = d - k + 1;
	swap(u, v);
    }
    while(1)
    {
	int l = dep[u] - dep[top[u]] + 1;
	if(l >= k)
	    return Rank[tid[u] - k + 1];
	u = fa[top[u]];
	k -= l;
    }
}

int main()
{
    int T;
    scanf("%d", &T);
    while(T--)
    {
	scanf("%d", &n);
	init();
	int u, v, w;
	for(int i = 1; i < n; i++)
	{
	    scanf("%d%d%d", &u, &v, &w);
	    addedge(u, v, w);
	}
	dfs1(1, -1, 1, 0);
	dfs2(1, 1);
	GetAnc(1);
	/*
	for(int i = 0; i < E.size(); i+=2)
	{
	    if(dep[E[i].v] > dep[E[i+1].v])
		num[tid[E[i].v]] = E[i].w;
	    else
		num[tid[E[i+1].v]] = E[i].w;
	}
	num[1] = 0;
	Build(1, n, 1);
	*/
	char op[10];
	int a, b, c;
	while(1)
	{
	    scanf("%s", op);
	    if(op[1] == 'O') break;
	    scanf("%d%d", &a, &b);
	    if(op[1] == 'I') printf("%d\n", Dist(a, b));
	    else 
	    {
		scanf("%d", &c);
		printf("%d\n", Kth(a,b,c));
	    }
	}
    }
    return 0;
}
