#include <cstdio>

int a, b;

int gcd(int a, int b)
{
	if (b == 0) return a;
	return gcd(b, a % b);
}

int main()
{
	scanf("%d%d", &a, &b);
	int g = gcd(a, b);
	a /= g; b /= g;
	if (a - b == 1 || b - a == 1) printf("Equal\n");
	else if (a < b) printf("Dasha\n");
	else printf("Masha\n");
	return 0;
}
