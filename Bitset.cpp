// Problem: HDU 6085
// Reference: http://bestcoder.hdu.edu.cn/blog/2017-multi-university-training-contest-5-solutions-by-%e5%90%89%e5%a6%82%e4%b8%80/

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
#include <bitset>
#define LL long long
#define ULL unsigned long long
using namespace std;
const int maxn = 50005;
int N, M, K;

struct Bitset
{
    ULL bits[(maxn>>6) + 5];
    void reset()
    {
    memset(bits, 0, sizeof(bits));
    }
    void flip(int pos)
    {
    bits[pos>>6] ^= (1llu<<(pos&63));
    }
    bool get(int pos)
    {
    return bits[pos>>6] & (1llu<<(pos&63));
    }
    void add(Bitset &a, int l, int r)
    {
    int len = (r-l);
    int b = len >> 6, j = l >> 6, i;
    for(i = 0; i < b; j++, i++)
        bits[i] ^= a.bits[j];
    int t = (((b+1)<<6) - len)&63;
    if(t) bits[i] ^= ((a.bits[j]<<t)>>t);
    }

}A[64], B, ans;

void solve()
{
    for(int j = 1; j <= M; j++)
    {
    if(B.get(j))
    {
        for(int i = 0; i <= N; i += j)
        {
        int pos = (64-(i&63))&63;
        ans.add(A[pos], i+pos, min(N+pos+1, i+pos+j));
        }
    }
    }
}

int main()
{
    int T;
    scanf("%d", &T);
    while(T--)
    {
    for(int i = 0; i < 64; i++)
        A[i].reset();
    B.reset();
    ans.reset();
    int n, m, q, x;
    N = M = 0;
    scanf("%d%d%d", &n, &m, &q);
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &x);
        for(int i = 0; i < 64; i++)
        A[i].flip(x+i);
        N = max(N, x);
    }
    for(int i = 0; i < m; i++)
    {
        scanf("%d", &x);
        B.flip(x);
        M = max(M, x);
    }
    solve();
    while(q--)
    {
        scanf("%d", &x);
        printf("%d\n", ans.get(x));
    }
    }
    return 0;
}
