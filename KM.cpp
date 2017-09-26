// Problem: HDU-2255
// Reference: http://blog.csdn.net/liguanxing/article/details/5665646

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
#define INF 0x3f3f3f3f

using namespace std;

const int maxn = 3e2+5;
const LL mod = 1e9+7;

bool x[maxn], y[maxn];
int a[maxn][maxn];
int my[maxn];
int lx[maxn], ly[maxn];
int slack[maxn];
int n;

bool dfs(int t)
{
    x[t] = true;
    for(int u = 1; u <= n; u++)
    {
        int wt = lx[t] + ly[u] - a[t][u];
        if(!y[u] && wt == 0)
        {
            y[u] = 1;
            if(my[u] == -1 || dfs(my[u]))
            {
                my[u] = t;
                return true;
            }
        }
        else if(slack[u] > wt) slack[u] = wt;
    }
    return false;
}

int KM()
{
    for(int i = 1; i <= n; i++)
    {
        my[i] = -1;
        lx[i] = -INF, ly[i] = 0;
        for(int j = 1; j <= n; j++)
            if(lx[i] < a[i][j])
                lx[i] = a[i][j];
    }
    for(int k = 1; k <= n; k++)
    {
        memset(x, 0, sizeof(x));
        memset(y, 0, sizeof(y));
        for(int i = 1; i <= n; i++) slack[i] = INF;
        while(!dfs(k))
        {
            int d = INF;
            for(int i = 1; i <= n; i++)
            {
                if(!y[i] && slack[i] < d)
                    d = slack[i];
            }
            for(int i = 1; i <= n; i++)
            {
                if(x[i])
                {
                    lx[i] = lx[i] - d;
                    x[i] = 0;
                }
                if(y[i])
                {
                    ly[i] = ly[i] + d;
                    y[i] = 0;
                }
            }
        }
    }
    int ans = 0;
    for(int i = 1; i <= n; i++)
        ans += (lx[i] + ly[i]);
    return ans;
}

int main()
{
    while(~scanf("%d", &n))
    {
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                scanf("%d", &a[i][j]);
        printf("%d\n", KM());
    }
	return 0;
}

