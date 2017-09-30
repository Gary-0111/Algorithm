// Problem: SPOJ - QTREE2
// Reference: 

#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#define LL long long

using namespace std;

const int INF = 0x3f3f3f3f;
const int maxn = 1e4+5;

int ch[maxn][2], pre[maxn];
int Sum[maxn], Size[maxn], key[maxn];
bool rt[maxn];

void PushDown(int r)
{

}

void PushUp(int r)
{
    Sum[r] = Sum[ch[r][0]] + Sum[ch[r][1]] + key[r];
    Size[r] = Size[ch[r][0]] + Size[ch[r][1]] + 1;
}

void Rotate(int x)
{
    int y = pre[x], kind = (ch[y][1] == x);
    ch[y][kind] = ch[x][!kind];
    pre[ch[y][kind]] = y;
    pre[x] = pre[y];
    pre[y] = x;
    ch[x][!kind] = y;
    if(rt[y]) rt[y] = false, rt[x] = true;
    else ch[pre[x]][ch[pre[x]][1] == y] = x;
    PushUp(y);
}

void Splay(int r)
{
    while(!rt[r])
    {
        int f = pre[r], ff = pre[f];
        if(rt[f]) Rotate(r);
        else if((ch[ff][1] == f) == (ch[f][1] == r)) 
            Rotate(f), Rotate(r);
        else Rotate(r), Rotate(r);
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
// 调用之后 u 为原来 u,v 的 LCA
// 调用之后 v 和 ch[u][1] 为 LCA 的两个儿子
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

void Query(int u, int v)
{
    LCA(u, v);
    printf("%d\n", Sum[v] + Sum[ch[u][1]]);
}
// 
int get_kth(int u, int k)
{
    int tmp = 0;
    int t = 0;
    Splay(u);
    for(t = u; ;)
    {
        tmp = Size[ch[t][0]];
        if(k == tmp + 1) break;
        if(k <= tmp) t = ch[t][0];
        else k -= tmp+1, t = ch[t][1];
    }
    Splay(t);
    return t;
}
// The k-th node on the path from u to v
void Query(int u, int v, int k)
{
    Access(u);
    Access(v), v = 0;
    while(u)
    {
        Splay(u);
        if(!pre[u]) break;
        int tmp = Size[ch[u][0]] + 1;
        if(k <= tmp) 
        { 
            printf("%d\n", get_kth(u, tmp - k + 1));
            return;
        }
        rt[ch[u][1]] = true;
        rt[ch[u][1] = v] = false;
        PushUp(u);
        u = pre[v = u];
        k -= tmp;
    }
    printf("%d\n", get_kth(u, k + Size[ch[u][0]]));
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
        Sum[0] = 0;
        Size[0] = 0;
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
            if(op[1] == 'O') break;
            scanf("%d%d", &u, &v);
            if(op[0] == 'D') Query(u, v);
            else
            {
                scanf("%d", &w);
                Query(u, v, w);
            } 
        }
    }
	return 0;
}

