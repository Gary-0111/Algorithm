// Problem: SPOJ - 375
// Reference: 
//            http://blog.csdn.net/tgop_knight/article/details/44539555
//            http://www.cnblogs.com/kuangbin/p/3300217.html

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#define LL long long
#define INF 0x3f3f3f3f

using namespace std;

const int maxn = 1e4+5;

int ch[maxn][2], pre[maxn];
int Max[maxn], key[maxn];
bool rt[maxn];

void PushDown(int r)
{

}

void PushUp(int r)
{
    Max[r] = max(max(Max[ch[r][0]], Max[ch[r][1]]), key[r]);
}

void Rotate(int x)
{
    int y = pre[x], kind = (ch[y][1] == x);
    ch[y][kind] = ch[x][!kind];
    pre[ch[y][kind]] = y;
    pre[x] = pre[y];
    pre[y] = x;
    ch[x][!kind] = y;
    if(rt[y])
        rt[y] = false, rt[x] = true;
    else
        ch[pre[x]][ch[pre[x]][1] == y] = x;
    PushUp(y);
}

void Splay(int r)
{
    while(!rt[r])
    {
        int f = pre[r], ff = pre[f];
        if(rt[f])
            Rotate(r);
        else if((ch[ff][1] == f) == (ch[f][1] == r))
            Rotate(f), Rotate(r);
        else
            Rotate(r), Rotate(r);
    }
    PushUp(r);
}

int Access(int x)
{
    int y = 0;
    do
    {
        Splay(x);
        rt[ch[x][1]] = true, rt[ch[x][1] = y] = false;
        PushUp(x);
        x = pre[y = x];
    }while(x);
    return y;
}

void LCA(int &u, int &v)
{
    Access(v), v = 0;
    while(u)
    {
        Splay(u);
        if(!pre[u]) return;
        rt[ch[u][1]] = true;
        rt[ch[u][1] = v] = false;
        PushUp(u);
        u = pre[v = u];
    }
}

void Change(int u, int k)
{
    Access(u);
    key[u] = k;
    PushUp(u);
}

void Query(int u, int v)
{
    LCA(u, v);
    printf("%d\n", max(Max[v], Max[ch[u][1]]));
}

struct edge
{
    int to, val, index, next;
}E[maxn*2];
int head[maxn], tot;
int id[maxn];

void addedge(int u, int v, int val, int index)
{
    E[tot].to = v;
    E[tot].val = val;
    E[tot].index = index;
    E[tot].next = head[u];
    head[u] = tot++;
}

void dfs(int u)
{
    for(int i = head[u]; i != -1; i = E[i].next)
    {
        int v = E[i].to;
        if(pre[v]) continue;
        pre[v] = u;
        id[E[i].index] = v;
        key[v] = E[i].val;
        dfs(v);
    }
}

void init()
{
    tot = 0;
    memset(head, -1, sizeof(head));
}

int main()
{
	int T;
    scanf("%d", &T);
    while(T--)
    {
        int n;
        scanf("%d", &n);
        init();
        for(int i = 0; i <= n; i++)
        {
            pre[i] = 0;
            ch[i][0] = ch[i][1] = 0;
            rt[i] = true;
        }
        Max[0] = -1000000000;
        int u, v, w;
        for(int i = 1; i < n; i++)
        {
            scanf("%d%d%d", &u, &v, &w);
            addedge(u, v, w, i);
            addedge(v, u, w, i);
        }
        pre[1] = -1;
        dfs(1);
        pre[1] = 0;
        char op[10];
        while(~scanf("%s", op))
        {
            if(op[0] == 'D') break;
            scanf("%d%d", &u, &v);
            if(op[0] == 'C')
                Change(id[u], v);
            else Query(u, v);
        }
    }
	return 0;
}

