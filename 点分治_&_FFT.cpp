// Problem: CodeChef - PRIMEDST
// Reference: http://blog.csdn.net/u011332631/article/details/46709149

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
const int maxn = 50005;

vector<int> G[maxn];
int root, size, f[maxn], s[maxn], d[maxn];
bool vis[maxn], valid[maxn];
int prime[10005], tot;
int cnt[maxn];
vector<int> dep;
LL tmp[maxn<<2], ans;

struct complex
{
    double re, im;
    complex(double r = 0.0, double i = 0.0): re(r), im(i) {}
    complex operator + (const complex &a) 
    {
	return complex(re+a.re, im+a.im);
    }
    complex operator - (const complex &a)
    {
	return complex(re-a.re, im-a.im);
    }
    complex operator * (const complex &a)
    {
	return complex(re*a.re - im*a.im, re*a.im + im*a.re);
    }
}x1[maxn<<2];

void GetPrime()
{
    memset(valid, true, sizeof(valid));
    for(int i = 2; i < maxn; i++)
    {
	if(valid[i])
	    prime[tot++] = i;
	for(int j = 0; (j < tot) && (i*prime[j] < maxn); j++)
	{
	    valid[i*prime[j]] = false;
	    if(i%prime[j] == 0) break;
	}
    }
}

void BRC(complex *y, int len)
{
    for(int i = 1, j = len/2, k; i < len -1; i++)
    {
	if(i < j) swap(y[i], y[j]);
	k = len/2;
	while(j >= k)
	{
	    j -= k;
	    k /= 2;
	}
	j += k;
    }
}

void FFT(complex *y, int len, double on)
{
    BRC(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
	complex wn(cos(on*2*Pi/h), sin(on*2*Pi/h));
	for(int j = 0; j < len; j += h)
	{
	    complex w(1, 0);
	    for(int k = j; k < j + h/2; k++)
	    {
		complex u = y[k];
		complex t = w*y[k+h/2];
		y[k] = u + t;
		y[k+h/2] = u - t;
		w = w*wn;
	    }
	}
    }
    if(on == -1)
	for(int i = 0; i < len; i++)
	    y[i].re /= len;
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
    f[u] = max(f[u], size-s[u]);
    if(f[u] < f[root]) root = u;
}

void GetDep(int u, int fa)
{
    dep.push_back(d[u]);
    s[u] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i];
	if(v != fa && !vis[v])
	{
	    d[v] = d[u] + 1;
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
    int maxd = 0;
    for(int i = 0; i < dep.size(); i++)
    {
	cnt[dep[i]]++;
       	maxd = max(maxd, dep[i]);
    }
    int len = 1;
    while(len <= 2*maxd) len<<=1;
    for(int i = 0; i <= maxd; i++) x1[i] = complex(cnt[i], 0.0);
    for(int i = maxd+1; i < len; i++) x1[i] = complex(0.0, 0.0);
    FFT(x1, len, 1);
    for(int i = 0; i < len; i++) x1[i] = x1[i]*x1[i];
    FFT(x1, len, -1);
    for(int i = 0; i < len; i++) tmp[i] = (LL)(x1[i].re+0.5);
    for(int i = 0; i < dep.size(); i++) tmp[dep[i]<<1]--;
    for(int i = 0; i < len; i++) tmp[i] >>= 1;
    LL ret = 0;
    for(int i = 0; i < tot && prime[i] <= maxd*2; i++) ret += tmp[prime[i]];
    for(int i = 0; i < dep.size(); i++) cnt[dep[i]]--;
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
	    ans -= calc(v, 1);
	    f[0] = size = s[v];
	    GetRoot(v, root = 0);
	    solve(root);
	}
    }
}

int main()
{
    int n;
    GetPrime();
    scanf("%d", &n);
    int u, v;
    for(int i = 1; i < n; i++)
    {
	scanf("%d%d", &u, &v);
	G[u].push_back(v);
	G[v].push_back(u);
    }
    ans = 0;
    f[0] = size = n;
    GetRoot(1, root = 0);
    solve(root);
    printf("%.7lf\n", (ans*2.0)/((LL)n*(n-1)));
    return 0;
}
