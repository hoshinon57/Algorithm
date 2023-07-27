#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC228 https://atcoder.jp/contests/abc228

/*
 * 可能性の話であるので、4日目が自分だけ満点、他は0点で考えてよい。
 * 3日目までの合計点数でソートし、K位の点数をborderとすると、
 * (自分の点数)+300 >= border であればYes回答となる。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<int> score(N);
	for(i = 0; i < N; i++)
	{
		int p1, p2, p3; cin >> p1 >> p2 >> p3;
		score[i] = p1+p2+p3;
	}

	auto score_sorted = score;
	sort(score_sorted.begin(), score_sorted.end(), greater<int>());  // 降順
	int border = score_sorted[K-1];  // K位の点数

	for(i = 0; i < N; i++)
	{
		string ans = "No";
		if(score[i]+300 >= border) ans = "Yes";
		cout << ans << endl;
	}

	return 0;
}
