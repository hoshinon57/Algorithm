#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC346 https://atcoder.jp/contests/abc346

/*
 * 方針は合っていたが、オーバーフローでWAを大量に出してしまった。
 * 二分探索で解く。
 * 
 * [ACまでの思考の流れ]
 * g(T,x)が部分列になるなら、g(T,x-1)も部分列になる。
 * 逆にg(T,x)が部分列にならないなら、g(T,x+1)も部分列にならない。
 * よって二分探索が候補に挙がる。
 * 
 * g(T,x)が部分列になるか？ の判定問題を考える。
 * これはSの文字ごとの累積和を求めておくことで解ける。
 * Tのある文字eについて、これがx個連続することから
 *   Sを何周するか(=loopぶん)
 *   S1周未満は何文字か(=zanぶん)
 * を計算することで、後者をlower_bound()で解くことができる。
 * つまり二分探索の二分探索。
 * 
 * 後は実装を参照。
 * Sの累積和は、Sを2周させておくことで実装が楽になる。
 * 
 * [反省点]
 * loop部分を加算するとき、オーバーフロー判定を入れずに大量のWAを出してしまった。
 * ※とはいえコンテスト結果を見るとWA率が相当高いので、皆似た感じでWAってたのかなあと。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s, t; cin >> s >> t;
	ll slen = (ll)s.size();
	ll tlen = (ll)t.size();

	s = s+s;  // 2周させる
	vector<vector<ll>> ssum(26, vector<ll>(slen*2+1, 0));  // ssum['a'][j]:文字aについての累積和
	for(char c = 'a'; c <= 'z'; c++)
	{
		for(i = 0; i < slen*2; i++)
		{
			int idx = c-'a';
			ssum[idx][i+1] = ssum[idx][i];
			if(s[i] == c) ssum[idx][i+1]++;
		}
	}

	// g(T,x)がf(S,N)の部分列になるならtrue
	auto check = [&](ll x) -> bool
	{
		ll pos = 0;  // f(S,N)のpos文字目まで使った
		for(auto &ee : t)
		{
			int e = ee-'a';
			if(ssum[e][slen] == 0) return false;  // Tに、Sに含まれていない文字があればfalse

			// 文字eがx個
			// S累積和の1周で割り、Sを何周するかとその余りを計算
			ll loop = x / ssum[e][slen];  // ssum[*][slen]:Sに含まれる*の文字数
			ll zan = x % ssum[e][slen];
			// 余り0のとき、ループ分を1つ持ってくると後の処理が楽
			if(zan == 0)
			{
				loop--;
				zan += ssum[e][slen];
			}

			// 残りの文字ぶんは、Sを2周させた累積和から移動量を求める
			ll idx = pos % slen;
			ll nxt_idx = lower_bound(ssum[e].begin(), ssum[e].end(), ssum[e][idx]+zan) - ssum[e].begin();
			pos += nxt_idx - idx;

			// ループぶんを加算			
			// pos+loop*slen > INF だとposがオーバーフローしてしまうため事前チェックが必要
			// 式変形して (INF-pos)/slen < loop でチェックする
			// (ここでめっちゃWA出してしまった)
			if((INF64-pos)/slen < loop) return false;
			pos += loop*slen;  // チェックしないと、ここでオーバーフローしてしまう
		}
		return pos <= slen*N;  // f(S,N)の文字数以下ならOK
	};

	ll ok = 0;
	// f(S,N)の長さは|S|*N<=10^17, g(T,x)の長さは|T|*x
	// |S|*N < |T|*x は必ずNG
	ll ng = 1e18 / tlen;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;  // 1e18は2倍してもオーバーフローしない
		if(check(mid)) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
