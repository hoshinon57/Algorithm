#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC325 https://atcoder.jp/contests/abc325

/*
 * 商品{T,D}をTでソートしておく。
 * 商品を先頭から見ていき、
 *   既に入ってきた商品について、Tの手前まで印字処理。
 *   印字は範囲から出るタイミングが早い順。
 * のように処理をしていく。
 * 
 * ・商品が無い区間は、時刻を次の商品まで進める。
 * ・商品の末尾に番兵を入れて実装を簡易にする。
 * といった対応を行っている。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<pair<ll,ll>> el(N);  // {t,d}
	for(i = 0; i < N; i++)
	{
		cin >> el[i].first >> el[i].second;
	}
	el.push_back({INF64, 0});  // 番兵
	sort(el.begin(), el.end());  // Tでソート
	ll now = 0;  // ここの手前まで印字機を動かした(時刻=nowに出てくる商品には印字しない)
	int ans = 0;
	priority_queue<ll, vector<ll>, greater<ll>> que;
	for(auto &e : el)
	{
		while(now < e.first)
		{
			if(que.empty()) break;
			ll q = que.top();
			que.pop();
			if(now <= q)  // 印字できる
			{
				ans++;
				now++;
			}
			else {;}  // 印字できない場合は捨てることになるので、nowも更新しない
		}
		now = e.first;  // 商品がずっと無い区間は、この処理ですっ飛ばすイメージ
		if(e.first == INF64) break;  // 番兵を入れたことで、最後の商品の判定が楽になる
		que.push(e.first+e.second);
	}
	cout << ans << endl;

	return 0;
}
