/*
References:
        https://wenku.baidu.com/view/1e4878196bd97f192279e941.html
        http://blog.csdn.net/acm_cxlove/article/details/8890003
*/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <stack>
#include <cstring>
#define LL long long

using namespace std;
const int maxn = 100005;
int fa[maxn];
int ne;

struct point
{
	int x, y, id;
	bool operator < (const point &p) const
	{
		return x != p.x ? (x<p.x) : (y<p.y);
	}
}P[maxn];

struct edge
{
	int u, v, w;
	bool operator < (const edge &e)const
	{
		return w<e.w;
	}
}E[maxn << 2];

struct BIT
{
	int min_val, pos;
	void init()
	{
		min_val = (1 << 30);
		pos = -1;
	}
}bit[maxn];

int lowbit(int x)
{
	return x&(-x);
}

void update(int k, int val, int pos, int m)
{
	while (k <= m)
	{
		if (bit[k].min_val>val)
		{
			bit[k].min_val = val;
			bit[k].pos = pos;
		}
		k += lowbit(k);
	}
}

int query(int k)
{
	int min_val = 1 << 30, pos = -1;
	while (k)
	{
		if (min_val>bit[k].min_val)
		{
			min_val = bit[k].min_val;
			pos = bit[k].pos;
		}
		k -= lowbit(k);
	}
	return pos;
}

void addedge(int u, int v, int w)
{
	E[ne].u = u;
	E[ne].v = v;
	E[ne++].w = w;
}

int find(int x)
{
	return x == fa[x] ? x : x = find(fa[x]);
}

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

int dist(int i, int j)
{
	return abs(P[i].x - P[j].x) + abs(P[i].y - P[j].y);
}

int Manhattan_MST(int n, int k)
{
	int A[maxn], B[maxn];
	for (int dir = 0; dir<4; dir++)
	{
		if (dir == 1 || dir == 3)
			for (int i = 0; i<n; i++)
				swap(P[i].x, P[i].y);
		else if (dir == 2)
			for (int i = 0; i<n; i++)
				P[i].x = -P[i].x;
		sort(P, P + n);
		for (int i = 0; i<n; i++)
			A[i] = B[i] = P[i].y - P[i].x;
		sort(B, B + n);
		int m = unique(B, B + n) - B;
		for (int i = 1; i <= m; i++)
			bit[i].init();
		for (int i = n - 1; i >= 0; i--)
		{
			int pos = lower_bound(B, B + m, A[i]) - B;
			int ans = query(m - pos);
			if (ans != -1)
				addedge(P[i].id, P[ans].id, dist(i, ans));
			update(m - pos, P[i].x + P[i].y, i, m);
		}
	}
	sort(E, E + ne);
	int cnt = n - k;
	for (int i = 0; i<n; i++)
		fa[i] = i;
	for (int i = 0; i<ne; i++)
	{
		int u = E[i].u, v = E[i].v;
		int fu = find(u), fv = find(v);
		if (fu != fv)
		{
			cnt--;
			fa[fu] = fv;
			if (cnt == 0)
				return E[i].w;
		}
	}
}

int main()
{
	int n, k;
	while (~scanf("%d%d", &n, &k) && n)
	{
		ne = 0;
		for (int i = 0; i<n; i++)
		{
			scanf("%d%d", &P[i].x, &P[i].y);
			P[i].id = i;
		}
		printf("%d\n", Manhattan_MST(n, k));
	}
	return 0;
}
