//Problem: UVA - 12633
//Reference: 1. Introduction to Algorithm ch.30
//           2. 叉姐的FFT讲义
//           3. http://blog.csdn.net/u013368721/article/details/45367249

#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#define Pi acos(-1.0)
#define CLR(x) memset(x, 0, sizeof(x))
#define LL long long
using namespace std;
const int maxn = 1<<18;

struct complex
{
    double re, im;
    complex(double r = 0.0, double i = 0.0):re(r), im(i) {}
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
}x1[maxn], x2[maxn];

void BRC(complex *y, int len)	//rader algorithm
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

void FFT(complex *y, int len, double inv)
{
    BRC(y, len);
    for(int h = 2; h <= len; h<<=1)
    {
	complex wn(cos(inv*2*Pi/h), sin(inv*2*Pi/h));
	for(int j = 0; j < len; j+=h)
	{
	    complex w(1, 0);
	    for(int k = j; k < j+h/2; k++)
	    {
		complex u = y[k];
		complex t = w*y[k+h/2];
		y[k] = u+t;
		y[k+h/2] = u-t;
		w = w*wn;
	    }
	}
    }
    if(inv == -1)
	for(int i = 0; i < len; i++)
	    y[i].re /= len;
}

bool visx[maxn/2], visy[maxn/2], visd[maxn/2];

int main()
{
    int T, cn = 1;
    scanf("%d",&T);
    while(T--)
    {
	CLR(visx);
	CLR(visy);
	CLR(visd);
	int r, c, n, x, y;
	scanf("%d%d%d",&r,&c,&n);
	for(int i = 0; i < n; i++)
	{
	    scanf("%d%d", &x, &y);
	    visx[x] = true;
	    visy[y] = true;
	    visd[x-y+c] = true;
	}
	
	int len = 1;
	while(len<=(r+c)) len <<= 1;

	x1[0] = complex(0.0, 0.0);
	for(int i = 1; i <= r; i++) x1[i] = complex(visx[i]? 0.0 : 1.0, 0.0);
	for(int i = r+1; i < len; i++) x1[i] = complex(0.0, 0.0);
	for(int i = 1; i <= c; i++) x2[c-i] = complex(visy[i]? 0.0 : 1.0, 0.0);
	for(int i = c; i < len; i++) x2[i] = complex(0.0, 0.0);
	
	FFT(x1, len, 1);
	FFT(x2, len, 1);
	for(int i = 0; i < len; i++)
	    x1[i] = x1[i]*x2[i];
	FFT(x1, len, -1);

	LL ans = 0;
	for(int i = 0; i < len; i++)
	    ans += (visd[i]? 0 : (LL)(x1[i].re+0.5));
	printf("Case %d: %lld\n", cn++, ans);
    }
    return 0;
}
