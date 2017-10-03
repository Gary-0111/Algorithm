// Problem: HDU - 4347
// Reference: http://blog.csdn.net/acdreamers/article/details/44664645
//            http://blog.csdn.net/v_july_v/article/details/8203674

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

using namespace std;

#define lson rt<<1
#define rson rt<<1|1
#define sqr(x) (x)*(x)

typedef long long LL;

const int INF = 0x3f3f3f3f;
const int maxn = 5e4+5;
const LL mod = 1e9+7;

int n, k, idx;

struct node
{
    int feature[5];
    int dim;
    bool operator < (const node &u) const
    {
        return feature[idx] < u.feature[idx];
    }
}_data[maxn];

typedef pair<double, node> Pair;

priority_queue<Pair> Q;

class KDTree
{
    public:
        void Build(int l, int r, int rt)
        {
            if(l > r) return;
            flag[rt] = 1;
            flag[lson] = flag[rson] = -1;
            idx = GetDim(l, r);
            int mid = (l + r) >> 1;
            nth_element(_data + l, _data + mid, _data + r + 1);
            data[rt] = _data[mid];
            data[rt].dim = idx;
            Build(l, mid - 1, lson);
            Build(mid + 1, r, rson);
        }
        void Query(node p, int m, int rt)
        {
            if(flag[rt] == -1) return;
            Pair cur(0, data[rt]);
            for(int i = 0; i < k; i++)
                cur.first += sqr(cur.second.feature[i] - p.feature[i]);
            int dim = data[rt].dim;
            bool fg = false;
            int x = lson, y = rson;
            if(p.feature[dim] >= data[rt].feature[dim]) swap(x, y);
            if(~flag[x]) Query(p, m, x);

            if(Q.size() < m)
            {
                Q.push(cur);
                fg = true;
            }
            else
            {
                if(cur.first < Q.top().first)
                {
                    Q.pop();
                    Q.push(cur);
                }
                if(sqr(p.feature[dim] - data[rt].feature[dim]) < Q.top().first) fg = true;
            }
            if(~flag[y] && fg)
                Query(p, m, y);
        }
    private:
        int GetDim(int l, int r)
        {
            int dim = 0, num = r - l + 1;
            double mxvar = -INF;
            for(int i = 0; i < k; i++)
            {
                double aver = 0.0;
                for(int j = l; j <= r; j++)
                    aver += _data[j].feature[i];
                aver /= num;
                double var = 0.0;
                for(int j = l; j <= r; j++)
                    var += sqr(_data[j].feature[i] - aver);
                var /= num;
                if(var > mxvar)
                {
                    mxvar = var;
                    dim = i;
                }
            }
            return dim;
        }
        node data[maxn<<2];
        int flag[maxn<<2];
}kd;

void Print(node data)
{
    for(int i = 0; i < k; i++)
        printf("%d%c", data.feature[i], i == k - 1 ? '\n' : ' ');
}

int main()
{
	while(~scanf("%d%d", &n, &k))
    {
        for(int i = 0; i < n; i++)
            for(int j = 0; j < k; j++)
                scanf("%d", &_data[i].feature[j]);
        kd.Build(0, n - 1, 1);
        int t, m;
        scanf("%d", &t);
        while(t--)
        {
            node p;
            for(int i = 0; i < k; i++)
                scanf("%d", &p.feature[i]);
            scanf("%d", &m);
            while(!Q.empty()) Q.pop();
            kd.Query(p, m, 1);
            printf("the closest %d points are:\n", m);
            node tmp[25];
            for(int i = 0; !Q.empty(); i++)
            {
                tmp[i] = Q.top().second;
                Q.pop();
            }
            for(int i = m - 1; i >= 0; i--)
                Print(tmp[i]);
        }
    }
	return 0;
}

