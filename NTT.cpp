// Problem: HDU 6061
// Reference: 1. http://blog.miskcoo.com/2015/04/polynomial-multiplication-and-fast-fourier-transform#i-15
//            2. http://blog.csdn.net/acdreamers/article/details/39026505
//            3. http://blog.csdn.net/ZLH_HHHH/article/details/76576544?locationNum=1&fps=1
//            4. http://blog.csdn.net/qq_32570675/article/details/76571666

#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#define Pi acos(-1.0)
#define CLR(x) memset(x, 0, sizeof(x))
#define LL long long
using namespace std;

const LL MOD = 998244353;	    //
const LL G = 3;
const int maxn = 1<<18;

LL A[maxn], C[maxn];
LL fac[maxn/2], inv[maxn/2], wn[30];


LL qpow(LL a, LL b)
{
    LL tmp = 1;
    while(b)
    {
	if(b&1) tmp = tmp*a % MOD;
	a = a*a % MOD;
	b>>=1;
    }
    return tmp;
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

void NTT(LL *y, int len, double on)	    //Number-Theoretic Transform
{
    BRC(y, len);
    for(int h = 2, id = 0; h <= len; h<<=1)
    {
	id++;
	LL wh = wn[id];
	if(on < 0) wh = qpow(wh, MOD-2);
	for(int j = 0; j < len; j+=h)
	{
	    LL w = 1;
	    for(int k = j; k < j+h/2; k++)
	    {
		LL tmp = k + h/2;
		LL u = y[k];
		LL t = w*y[tmp] % MOD;
		y[k] = u+t;
		if(y[k] >= MOD) y[k] -= MOD;
		y[tmp] = u-t;
		if(y[tmp] < 0) y[tmp] += MOD;
		w = w*wh % MOD;
	    }
	}
    }
    if(on == -1)
    {
	LL Inv = qpow((LL)len, MOD-2);
	for(int i = 0; i < len; i++)
	    y[i] = y[i] * Inv % MOD;
    }
}


void init()
{
    for(int i = 0; i < 22; i++)
	wn[i] = qpow(G, (MOD-1)/(1<<i));
    fac[0] = 1;
    for(int i = 1; i < maxn/2; i++)
	fac[i] = fac[i-1]*i % MOD;
    inv[maxn/2-1] = qpow(fac[maxn/2-1], MOD-2);
    for(int i = maxn/2-1; i; i--)
	inv[i-1] = inv[i]*i % MOD;
}

int main()
{
    init();
    int n;
    while(~scanf("%d",&n)&&n)
    {
	n++;
	for(int i = 0; i < n; i++)
	    scanf("%lld", &C[n-i-1]);

	for(int i = 0; i < n; i++)
	    C[i] = C[i]*fac[n-i-1] % MOD;
	
	int m;
	LL sum = 0, x;
	scanf("%d", &m);
	while(m--)
	{
	    scanf("%lld", &x);
	    sum += x;
	    if(sum >= MOD) sum -= MOD;
	}
	sum = MOD - sum;
	A[0] = 1;
	for(int i = 1; i < n; i++)
	    A[i] = sum * A[i-1] % MOD;
	for(int i = 0; i < n; i++)
	    A[i] = A[i] * inv[i] % MOD;

	int len = 1;
	while(len <= 2*n) len<<=1;
	for(int i = n; i < len; i++)
	    A[i] = C[i] = 0;

	NTT(A, len, 1);
	NTT(C, len, 1);
	for(int i = 0; i < len; i++)
	    A[i] = A[i]*C[i] % MOD;
	NTT(A, len, -1);

	for(int i = n - 1; i >= 0 ; i--)
	    printf("%lld ", (A[i] % MOD + MOD) * inv[n-i-1] % MOD);
	printf("\n");
    }
    return 0;
}
