// Problem: HDU 4656
// Reference: 
//            NTT:
//            1. http://blog.miskcoo.com/2015/04/polynomial-multiplication-and-fast-fourier-transform#i-15
//            2. http://blog.csdn.net/whzzt/article/details/70880091
//            3. http://www.cnblogs.com/candy99/p/6754278.html
//            CRT: 
//            1. http://blog.miskcoo.com/2014/09/chinese-remainder-theorem
//            2. Introduction to Algorithm. ch.31
//            以及VJ上的各份公开代码。


#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#define Pi acos(-1.0)
#define CLR(x) memset(x, 0, sizeof(x))
#define LL long long
using namespace std;

const LL n1 = 998244353;	    //119 * 2^23 + 1 
const LL n2 = 1004535809;	    //479 * 2^21 + 1 
const LL c1 = 334257240187163831;   
const LL c2 = 668514958533372747;   
const LL P = n1*n2;
const LL G = 3;
const LL MOD = 1e6+3;
const int maxn = 1<<19;

LL A[maxn], B[maxn], C[maxn], w1[maxn], w2[maxn], w3[maxn], w4[maxn], cc[maxn];
LL fac[maxn/4], a[maxn/4], inv[MOD+5];

LL qpow(LL a, LL b, LL m)
{
    LL tmp = 1;
    while(b)
    {
	if(b&1) tmp = tmp*a % m;
	a = a*a % m;
	b>>=1;
    }
    return tmp;
}

LL mul_mod(LL x, LL y, LL m)
{
    return (x * y - (LL)((long double)x / m * y + 1e-3) * m + m) % m;
}

LL CRT(LL r1, LL r2)
{
    return (mul_mod(r1, c1, P) + mul_mod(r2, c2, P)) % P % MOD;
}

void BRC(LL *y, int len)	//rader algorithm
{
    for(int i = 1, j = len/2, k; i < len-1; i++)
    {
	if(i<j) swap(y[i], y[j]);
	k = len/2;
	while(j >= k)
	{
	    j -= k;
	    k /= 2;
	}
	j += k;
    }
}

void NTT(LL *y, int len, double on, LL m)	    //Number-Theoretic Transform
{
    BRC(y, len);
    for(int h = 2, id = 0; h <= len; h<<=1)
    {
	id++;
	LL wh = qpow(G, (m-1)>>id, m);
	if(on < 0) wh = qpow(wh, m-2, m);
	for(int j = 0; j < len; j+=h)
	{
	    LL w = 1;
	    for(int k = j; k < j+h/2; k++)
	    {
		LL tmp = k + h/2;
		LL u = y[k];
		LL t = w*y[tmp] % m;
		y[k] = u+t;
		if(y[k] >= m) y[k] -= m;
		y[tmp] = u-t;
		if(y[tmp] < 0) y[tmp] += m;
		w = w*wh % m;
	    }
	}
    }
    if(on == -1)
    {
	LL Inv = qpow((LL)len, m-2, m);
	for(int i = 0; i < len; i++)
	    y[i] = y[i] * Inv % m;
    }
}

void init()
{
    fac[0] = 1;
    for(int i = 1; i < maxn/4; i++)
	fac[i] = fac[i-1]*i % MOD;
    inv[0] = 0;
    inv[1] = 1;
    for(int i = 2; i < MOD; i++)
	inv[i] = inv[MOD%i] * (MOD-MOD/i) % MOD;
}

void convol(LL *x, LL *y, LL *z, int len)
{
    memcpy(w1, x, len<<2);
    memcpy(w2, y, len<<2);
    NTT(w1, len, 1, n1);
    NTT(w2, len, 1, n1);
    for(int i = 0; i < len; i++)
	w1[i] = (w1[i]*w2[i]) % n1;
    NTT(w1, len, -1, n1);

    NTT(x, len, 1, n2);
    NTT(y, len, 1, n2);
    for(int i = 0; i < len; i++)
	x[i] = (x[i]*y[i]) % n2;
    NTT(x, len, -1, n2);
    for(int i = 0; i < len; i++)
	z[i] = CRT(w1[i], x[i]);
}

int main()
{
    init();
    int n, b, c, d;
    scanf("%d%d%d%d",&n, &b, &c, &d);
    for(int i = 0; i < n; i++)
	scanf("%lld", &a[i]);
    LL dd = 1;
    for(int i = 0; i < n; i++, dd = dd*(LL)d % MOD)
    {	
	A[i] = (LL)a[n-i-1] * fac[n-i-1] % MOD;
	B[i] = dd * inv[fac[i]] % MOD;
    }
    int len = 1;
    while(len <= 2*n) len<<=1;
    for(int i = n; i < len; i++)
	A[i] = B[i] = 0;
    convol(A, B, C, len);
    for(int i = 0; i < n/2; i++)
	swap(C[i], C[n-i-1]);

    LL bb = 1;
    for(int i = 0; i < n; i++, bb = (LL)b * bb % MOD)
    {
	cc[i] = qpow(c, (LL)i*i, MOD);
	B[i] = bb * cc[i] % MOD * C[i] % MOD * inv[fac[i]] % MOD;
	A[n-i-1] = A[n-1+i] = inv[cc[i]];
    }
    len<<=1;
    for(int i = n; i < len; i++) B[i] = 0;
    for(int i = 2*n-1; i < len; i++) A[i] = 0;

    convol(A, B, C, len);
    for(int i = 0; i < n; i++)
	printf("%lld\n", C[i+n-1] * cc[i] % MOD);

    return 0;
}
