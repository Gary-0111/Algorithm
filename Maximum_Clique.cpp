// Problem: ZOJ - 1492
// Reference: 红书 P104

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

const int maxn = 55;

int G[maxn][maxn];
int n;

int ans;
int len[maxn], list[maxn][maxn];
int mc[maxn];
bool found;
void dfs(int size)
{
    if(len[size] == 0)
    {
	if(size > ans)
	{
	    ans = size;
	    found = true;
	}
	return;
    }
    for(int k = 0; k < len[size] && !found; ++k)
    {
	if(size + len[size] - k <= ans) break;
	int i = list[size][k];
	if(size + mc[i] <= ans) break;
	int j;
	for(j = k+1, len[size+1] = 0; j < len[size]; ++j)
	    if(G[i][list[size][j]])
		list[size+1][len[size+1]++] = list[size][j];
	dfs(size+1);
    }
}

void max_clique()
{
    mc[n] = ans = 1;
    for(int i = n-1; i; --i)
    {
	found = false;
	len[1] = 0;
	for(int j = i+1; j <= n; ++j)
	    if(G[i][j])
		list[1][len[1]++] = j;
	dfs(1);
	mc[i] = ans;
    }
}

int main()
{
    while(~scanf("%d", &n) && n)
    {
	for(int i = 1; i <= n; ++i)
	    for(int j = 1; j <= n; ++j)
		scanf("%d", &G[i][j]);
	max_clique();
	printf("%d\n", ans);
    }
    return 0;
}
