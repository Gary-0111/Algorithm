// Problem: HDU - 3068
// Reference: https://segmentfault.com/a/1190000003914228

#include<vector>
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;

const int maxn = 250000;
int n, p[maxn];
char s[maxn], str[maxn];

void init()
{
    memset(str, 0, sizeof(str));
    str[0] = '$';
    str[1] = '#';
    for (int i = 0; i < n; i++)
    {
	str[i * 2 + 2] = s[i];
	str[i * 2 + 3] = '#';
    }
    n = n * 2 + 2;
    str[n] = '\0';
}

void manacher()
{
    init();
    int id, mx = 0;
    for (int i = 1; i < n; i++)
    {
	if (mx > i) p[i] = min(p[2 * id - i], p[id] + id - i);
	else p[i] = 1;
	for (; str[i + p[i]] == str[i - p[i]]; p[i]++) ;
	if (p[i] + i > mx)
	{
	    mx = p[i] + i;
	    id = i;
	}
    }
}

int main()
{
    while (~scanf("%s", s))
    {
	int ans = 0;
	n = strlen(s);
	manacher();
	for (int i = 0; i < n; i++)
	    if (p[i]>ans)
		    ans = p[i];
	printf("%d\n", ans - 1);
    }
    return 0;
}
