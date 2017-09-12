// Problem: HDU 5909
// Reference: 1: http://blog.csdn.net/xuanandting/article/details/70991372
//            2: http://blog.csdn.net/liangzhaoyang1/article/details/52826303
//            3: http://picks.logdown.com/posts/179290-fast-walsh-hadamard-transform
//            4: http://blog.csdn.net/liangzhaoyang1/article/details/52819835

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

const int maxn = 1100;
const int mod = 1e9+7;
const int rev = (mod+1)>>1;
int val[maxn];
int dp[maxn][maxn];
int ans[maxn], temp[maxn];
vector<int> G[maxn];
int n, m;

void FWT(int a[], int len)
{
    for(int d = 1; d < len; d <<= 1)
    {
	for(int k = d<<1, i = 0; i < len; i += k)
	{
	    for(int j = 0; j < d; j++)
	    {
		int x = a[i+j], y = a[i+j+d];
		a[i+j] = (x+y)%mod, a[i+j+d] = (x-y+mod)%mod;
		//xor: a[i+j] = (x+y)%mod, a[i+j+d] = (x-y+mod)%mod;
		//ans: a[i+j] = x+y;
		//or:  a[i+j+d] = y+x;
	    }
	}
    }
}

void UFWT(int a[], int len)
{
    for(int d = 1; d < len; d <<= 1)
    {
	for(int k = d<<1, i = 0; i < len; i += k)
	{
	    for(int j = 0; j < d; j++)
	    {
		int x = a[i+j], y = a[i+j+d];
		a[i+j] = 1ll*(x+y)*rev%mod, a[i+j+d] = (1ll*(x-y)*rev%mod+mod)%mod;
		//xor: a[i+j] = (x+y)/2, a[i+j+d] = (x-y)/2;
		//and: a[i+j] = x-y;
		//or:  a[i+j+d] = y-x;
	    }
	}
    }
}

void calc(int a[], int b[], int len)
{
    FWT(a, len); FWT(b, len);
    for(int i = 0; i < len; i++)
	a[i] = 1ll*a[i]*b[i]%mod;
    UFWT(a, len);
}

void dfs(int u, int fa)
{
    dp[u][val[u]] = 1;
    for(int i = 0; i < G[u].size(); i++)
    {
	int v = G[u][i];
	if(v == fa) continue;
	dfs(v, u);
	for(int i = 0; i < m; i++) temp[i] = dp[u][i];
	calc(dp[u], dp[v], m);
	for(int i = 0; i < m; i++) dp[u][i] = (dp[u][i]+temp[i])%mod;
    }
    for(int i = 0; i < m; i++) ans[i] = (ans[i]+dp[u][i])%mod;
}

int main()
{
    int T;
    scanf("%d", &T);
    while(T--)
    {
	//int n, m;
	scanf("%d%d", &n, &m);
	for(int i = 0; i <= n; i++) G[i].clear();
	for(int i = 1; i <= n; i++) scanf("%d", &val[i]);
	for(int i = 1; i < n; i++) 
	{
	    int u, v;
	    scanf("%d%d", &u, &v);
	    G[u].push_back(v);
	    G[v].push_back(u);
	}
	memset(dp, 0, sizeof(dp));
	memset(ans, 0, sizeof(ans));
	dfs(1, 0);
	for(int i = 0; i < m; i++)
	    printf("%d%c", ans[i], (i == m-1?'\n':' '));
    }
    return 0;
}
