#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC261 https://atcoder.jp/contests/abc261

/*
 * 連想配列を使って解く。
 * 以下コード及びコメント参照。
 */
int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<string> s(N);
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
	}
	map<string, int> mp;

	for(i = 0; i < N; i++)
	{
		if(mp.count(s[i]) == 0)
		{
			// 文字列が以前に存在しない
			cout << s[i] << endl;
			mp[s[i]]++;
		}
		else
		{
			// 文字列が以前に存在する
			int num = mp[s[i]];
			cout << s[i] << "(" << num << ")" << endl;
			mp[s[i]]++;
		}
	}

	return 0;
}
