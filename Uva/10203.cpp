#include <map>
#include <set>
#include <list>
#include <cmath>
#include <queue>
#include <stack>
#include <bitset>
#include <vector>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

double dist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int main()
{
	freopen("a", "r", stdin);
	int Test;
	scanf("%d\n", &Test);
	bool blank = false;
	while (Test--)
	{
		if (blank) puts("");
		blank = true;
		double x, y;
		scanf("%lf%lf", &x, &y);
		char str[100];
		double sum = 0.0;
		gets(str);
		while (gets(str) && strlen(str) > 0)
		{
			double x1, y1, x2, y2;
			sscanf(str, "%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
			sum += dist(x1, y1, x2, y2);
		}
		int t = (int)(sum * 60.0 / 10000.0 + 0.5);
		int hour = t / 60, minute = t % 60;
		if (minute < 10) printf("%d:0%d\n", hour, minute);
		else printf("%d:%d\n", hour, minute);
	}
	return 0;
}
