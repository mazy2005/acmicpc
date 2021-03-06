#include <set>
#include <vector>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

const int maxn = 200000 + 5;
int N, S;
vector<pair<int, int> > now, ans;
set<pair<int, int> > myset;

int main() {
	freopen("in.txt", "r", stdin);
	cin >> N >> S;
	for (int i = 0; i < N; i++) {
		int k;
		cin >> k;
		myset.insert(make_pair(k, i));
	}

	int error = 0;
	while (!myset.empty()) {
		set<pair<int, int> >::iterator it = myset.end();
		it--;
		pair<int, int> tmp = *it;
		now.clear();
		while (tmp.first) {
			if (it == myset.begin()) break;
			it--;
			if (it->first == 0) break;
			now.push_back(make_pair(it->first - 1, it->second));
			ans.push_back(make_pair(it->second, tmp.second));
			tmp.first--;
		}
		if (tmp.first) {
			error = 1;
			break;
		}
		myset.erase(it, myset.end());
		myset.insert(now.begin(), now.end());
	}
	if (error) cout << "No" << endl;
	else {
		cout << "Yes" << endl;
		cout << ans.size() << endl;
		for (int i = 0; i < ans.size(); i++)
			printf("%d %d\n", ans[i].first + 1, ans[i].second + 1);
	}
	return 0;
}
