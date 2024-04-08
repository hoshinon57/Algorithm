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

// ABC085 https://atcoder.jp/contests/abc085

/*
 * 公式解説にあるように、「既に投げた刀を振ることができる」というルールに変えても問題ない。
 * 
 * priority_queueに以下の形でデータを持たせる。
 *   {振るダメージ, 0}
 *   {投げるダメージ, 1}
 * ダメージが大きい順に攻撃していけばよい。
 * 投げるダメージの場合は1回のみ攻撃可能、振るダメージは何回でも攻撃可能。
 * よって後者をキューから取り出したら、それで計算終了することになる。
 */

int main(void)
{
	int i;
	ll N, H; cin >> N >> H;
	using pll = pair<ll,ll>;  // {damage, 0=a 1=b}
	priority_queue<pll> que;
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		que.push({a, 0});
		que.push({b, 1});
	}
	ll ans = 0;
	while(!que.empty())
	{
		auto [d,k] = que.top();
		que.pop();
		if(k == 0)  // 何回でも可能 これで倒してしまう
		{
			int c = (H+d-1)/d;  // 切り上げ
			ans += c;
			H -= d*c;
		}
		else  // 1回のみ
		{
			ans++;
			H -= d;
		}
		if(H <= 0) break;
	}
	cout << ans << endl;

	return 0;
}
