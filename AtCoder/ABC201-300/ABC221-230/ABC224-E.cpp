#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC224 https://atcoder.jp/contests/abc224

/*
 * いくつか試行錯誤してみると、整数aが大きい順に見ていけばよいことが分かる。
 * あるマス{r,c}について、そこから移動できる回数は
 *   その行方向に見たときに、移動できる回数の最大値 -> max_gyou[r]
 *   その列方向に見たときに、移動できる回数の最大値 -> max_retu[c]
 * のうち大きい方となる。
 * その後、max_gyou[r]とmax_retu[c]は+1する。
 * 
 * よって入力の{r,c,a}について、aが大きい順に
 *   (1)max_gyou[r], max_retu[c]からこのマスにおける解を求める
 *   (2)max_gyou[r], max_retu[c]を更新する
 * を順に行えばよい。
 * ただし注意点として、移動できるのは「真に大きい」場合であるため、
 * (2)は同じ値でまとめて更新する必要がある。
 */

int main(void)
{
	int i;
	int H, W, N; cin >> H >> W >> N;
	using pipii = pair<int, pair<int,int>>;
	vector<pipii> masu(N);  // {値, {r, c}}
	map<pair<int,int>,int> idx;  // idx[{r,c}] = masuの番号
	for(i = 0; i < N; i++)
	{
		int r, c, a; cin >> r >> c >> a;
		r--; c--;
		masu[i] = {a, {r, c}};
		idx[{r, c}] = i;
	}
	sort(masu.begin(), masu.end(), greater<pipii>());

	vector<int> max_gyou(H), max_retu(W);  // max_gyou[i]:i行目にコマがあるとき、移動できる回数の最大値
	vector<int> ans(N);
	vector<pipii> masu_list;  // 同じ値をまとめて更新する用
	for(i = 0; i < N; i++)
	{
		auto [r_, c_] = masu[i].second;

		int num = max(max_gyou[r_], max_retu[c_]);
		ans[idx[{r_, c_}]] = num;
		masu_list.push_back({num+1, {r_, c_}});

		if(i == N-1 || masu[i].first != masu[i+1].first)  // 次が違う値、もしくはこれが最後の要素なら max_*** を更新
		{
			for(auto &e : masu_list)
			{
				max_gyou[e.second.first] = max(max_gyou[e.second.first], e.first);
				max_retu[e.second.second] = max(max_retu[e.second.second], e.first);
			}
			masu_list.clear();
		}
	}

	for(auto &e : ans)
	{
		cout << e << endl;
	}

	return 0;
}
