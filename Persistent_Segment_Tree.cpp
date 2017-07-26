// Problem: HDU 4348
// Reference: http://www.cnblogs.com/Empress/p/4652449.html
//            http://acm.hdu.edu.cn/discuss/problem/post/reply.php?postid=32913&messageid=1&deep=0


#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <queue>
#include <map>

#define lson l,m
#define rson m+1,r
#define LL long long

using namespace std;

const int maxn = 100005;

int L[maxn<<5], R[maxn<<5], T[maxn], tot, cur;
LL sum[maxn<<5], add[maxn<<2];
LL A[maxn];

int build(int l,int r)
{
    int rt = (++tot);
    add[rt] = 0;
    if(l == r)
    {
	sum[rt] = A[l];
	return rt;
    }
    int m = (l+r)>>1;
    L[rt] = build(lson);
    R[rt] = build(rson);
    sum[rt] = sum[L[rt]] + sum[R[rt]];
    return rt;
}

void PushUp(int rt, int m)
{
    sum[rt] = sum[L[rt]] + add[L[rt]]*(m-(m>>1)) + sum[R[rt]] + add[R[rt]]*(m>>1);
}

int update(int ql, int qr, int a, int l, int r, int pre)
{
    int rt = (++tot);
    L[rt] = L[pre];
    R[rt] = R[pre];
    add[rt] = add[pre];
    if(ql<=l && r<=qr)
    {
	add[rt] = (add[pre] + a);
	sum[rt] = (sum[pre]);
	//printf("add[%d,%d] = %lld  sum[%d,%d] = %lld\n",l,r,add[rt],l,r,sum[rt]);
	return rt;
    }
    int m = (l+r)>>1;
    if(ql<=m) L[rt] = update(ql,qr,a,lson,L[pre]);
    if(m<qr) R[rt] = update(ql,qr,a,rson,R[pre]);
    PushUp(rt, r-l+1);
    //printf("add[%d,%d] = %lld  sum[%d,%d] = %lld\n",l,r,add[rt],l,r,sum[rt]);
    return rt;
}

LL query(int ql, int qr, int val, int l, int r, int rt)
{
    if(ql<=l&&r<=qr)
    {
	//printf("sum[%d,%d] = %lld\n",l,r,sum[rt]+(val+add[rt])*(r-l+1));
	return sum[rt]+(val+add[rt])*(r-l+1);
    }
    int m = (l+r)>>1;
    LL ret = 0;
    if(ql<=m) ret += query(ql,qr,val+add[rt], lson,L[rt]);
    if(m<qr) ret += query(ql,qr,val+add[rt], rson,R[rt]);
    //printf("sum[%d,%d] = %lld\n",l,r,sum[rt]+(val+add[rt])*(r-l+1));
    return ret;
}

int main()
{
    int n, m;
    while(~scanf("%d%d",&n,&m))
    {
	tot = 0;
	cur = 0;
	for(int i = 1; i<=n; i++)
	    scanf("%lld",&A[i]);
	T[0] = build(1,n);
	T[1] = tot;
	char op[2];
	int l, r, x;
	while(m--)
	{
	    scanf("%s",op);
	    if(op[0]=='Q')
	    {
		scanf("%d%d",&l,&r);
		printf("%lld\n",query(l,r,0,1,n,T[cur]));
	    }
	    else if(op[0] == 'C')
	    {
		scanf("%d%d%d",&l,&r,&x);
		T[cur+1] = update(l,r,x,1,n,T[cur]);
		T[++cur+1] = tot;
	    }
	    else if(op[0] == 'H')
	    {
		scanf("%d%d%d",&l,&r,&x);
		printf("%lld\n",query(l,r,0,1,n,T[x]));
	    }
	    else
	    {
		scanf("%d",&x);
		cur = x;
		tot = T[cur+1];
	    }
	}
    }
    return 0;
}
