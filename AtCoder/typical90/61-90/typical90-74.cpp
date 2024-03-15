#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:74 https://atcoder.jp/contests/typical90/tasks/typical90_bv

/*
 * 自力で解けず、解説の「不変量を考える」のフレーズだけ見てAC.
 *   https://twitter.com/e869120/status/1407833847413178372
 * ※少しは机上で手計算して法則を探すべきだった、反省。
 * 
 * 基本は公式解説の通り。
 * 操作にてc->aとなるのはもったいないので、そうならないよう左側から処理していくのが最善というイメージか。
 * 
 * baaa:1回
 * abaa:2回
 * aaba:4回
 * aaca:8回(=4+4)
 * というのを手計算で考えると、左からi文字目(0-indexed)が(2^i)回に対応してそう、と分かる。
 * 文字がcならその2倍。
 * 制約のN<=60から2^60はll型の範囲に収まるので、そこからも推測できる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・N<=60など、特殊な制約に着目する
 * ・操作にて数列の値が変わる場合、不変量に着目する。
 *   「必ず1減る」「操作によって変わらない」など。
 * ・手計算で法則を探してみる。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	string s; cin >> s;
	ll p = 1;
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ll d = s[i] - 'a';
		ans += d*p;
		p *= 2;
	}
	cout << ans << endl;

	return 0;
}
