#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code7.2の写経

bool cmp(pair<int,int> &a, pair<int,int> &b)
{
	return a.second < b.second;
}

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<pair<int,int>> kukan(N);
	for(i = 0; i < N; i++)
	{
		cin >> kukan[i].first >> kukan[i].second;
	}
	// 終端時刻が早い順にソート
	sort(kukan.begin(), kukan.end(), cmp);

	int count = 0;
	int now_end_time = 0;  // 最後に選んだ区間の終端時刻
	for(i = 0; i < N; i++)
	{
		// 最後に選んだものより前に開始するものは、選べない
		if(kukan[i].first < now_end_time) continue;

		count++;
		now_end_time = kukan[i].second;
	}

	cout << count << endl;

	return 0;
}
