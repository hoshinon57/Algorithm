#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC411 https://atcoder.jp/contests/abc411

/*
 * コンテスト時に解けず、終了後に思いついてAC.
 * 永続データ構造に近い？ gitっぽい？感じで解く。
 *   https://x.com/kyopro_friends/status/1936428711584346216
 * 
 * サーバ/PCが現在どの文字列を参照しているかのidxを持つ。
 * クエリ2の文字列追加は、PC[p]のノードに{親=p,文字列=s}の子を追加する。
 * 
 * 全クエリを処理したら、サーバが指しているidxから根の方向へ、文字列を取得していったものが答。
 * ただし根から文字列を取っていくので、うまいこと反転処理が必要。
**/

struct dt_
{
	ll par;  // 親のidx -1なら根
	string s;  // そのノードが持つ文字列
};

int main(void)
{
	// 0:サーバ, 1～N:PC
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<dt_> d;
	d.push_back({-1, ""});  // idx=0
	vector<ll> idx(N+1);  // その機器が指すノード番号
	for(i = 0; i < N+1; i++)
	{
		idx[i] = 0;  // d[0]を指す
	}

	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		int p; cin >> p;
		if(ki == 1)
		{
			idx[p] = idx[0];
		}
		if(ki == 2)
		{
			string t; cin >> t;
			ll sz = (int)d.size();
			d.push_back({idx[p], t});  // PCpに文字列を追加するので、親はidx[p]
			idx[p] = sz;
		}
		if(ki == 3)
		{
			idx[0] = idx[p];
		}
	}
	ll n = idx[0];
	string ans;
	while(d[n].par != -1)  // 根へたどっていく
	{
		auto tmp = d[n].s;
		reverse(tmp.begin(), tmp.end());
		ans += tmp;
		n = d[n].par;
	}
	reverse(ans.begin(), ans.end());
	cout << ans << endl;

	return 0;
}
