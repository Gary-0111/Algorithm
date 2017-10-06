// Problem: HDU - 1348
// Reference: http://blog.csdn.net/fivedoumi/article/details/7653128
//            http://blog.csdn.net/u013532224/article/details/37533731
//            http://blog.csdn.net/bone_ace/article/details/46239187

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
#define PI acos(-1.0)

using namespace std;

typedef long long LL;

const int INF = 0x3f3f3f3f;
const int maxn = 1e3+5;
const LL mod = 1e9+7;
int n, L;

struct Point
{
    double x, y;
}p[maxn];

double Dis(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double CrossProduct(Point p0, Point p1, Point p2) // 计算 p0p1 与 p0p2 的向量积, 如果大于0, 则 p2 在 p1 绕 p0 的逆时针方向(向左转)
{
    return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool cmp(const Point &p1, const Point &p2)
{
    int tmp = CrossProduct(p[0], p1, p2);
    if(tmp > 0) return true;
    else if(tmp == 0 && Dis(p[0], p1) < Dis(p[0], p2)) return true;
    else return false;
}

void Init()
{
    int k = 0;
    scanf("%d%d", &n, &L);
    for(int i = 0; i < n; i++)
    {
        scanf("%lf%lf", &p[i].x, &p[i].y);
        if((p[i].y < p[k].y) || (p[i].y == p[k].y && p[i].x < p[k].x))
            k = i;
    }
    Point tmp = p[0];
    p[0] = p[k];
    p[k] = tmp;
    sort(p+1, p+n, cmp);
}

int stack[maxn], top;
void Graham()
{
    if(n == 1) {stack[top = 0] = 0;}
    else 
    {
        top = 1;
        stack[0] = 0, stack[1] = 1;
        for(int i = 2; i < n; i++)
        {
            while(top > 0 && CrossProduct(p[stack[top-1]], p[stack[top]], p[i]) <= 0) top--; // 不满足向左转
            top++;
            stack[top] = i;
        }
    }
}

double sumlen()
{
    double sum = 0.0;
    for(int i = 1; i <= top; i++)
        sum += Dis(p[stack[i-1]], p[stack[i]]);
    sum += Dis(p[stack[0]], p[stack[top]]);
    return sum;
}

int main()
{
	int T;
    scanf("%d", &T);
    while(T--)
    {
        Init();
        Graham();
        printf("%.0lf\n", sumlen()+2.0*PI*L);
        if(T) printf("\n");
    }
	return 0;
}

