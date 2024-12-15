#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC384 https://atcoder.jp/contests/abc384

/*
 * コンテスト時に解けず、解説を見た。
 *   https://atcoder.jp/contests/abc384/editorial/11568
 * 
 * 基本的に公式解説の通り。
 * Ai+Ajが「2でちょうど何回割れるか」ごとに計算していく。
 * 
 * d[k]を、
 *   Ai+Ajが2^kの倍数、となるAi+Ajの総和
 * として定義する。(i<=j)
 * すると「2でちょうどk回割れる」Ai+Ajの総和は d[k]-d[k+1] で表せることから、答は
 *   Σ(d[k]-d[k+1])/2^k
 * となる。
 * kは0以上、上限は30もあれば十分。
 * 
 * d[k]について、Aiを固定したときにAi+Ajが2^kの倍数であることから、
 *   Aj≡-Ai (mod 2^k)
 * となる。
 * ※実装ではsmod(-Ai,2^k)としている
 * mapを使って mp[A]={Aj=AとなるAの総和S, その個数n} とすると S+Ai*n をd[k]に加算していけばよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・いやームズい。どうやったら解けるようになるんだ…
 * ・「2で割れるだけ割る」なので、2^kごとに考えるのが典型か。
 *   そこからAi+Ajが2^kの倍数というのを考えて、重複があるので「ちょうどk回割れる」はd[k]-d[k+1]で求められる…感じかなあ。
 */

// n%mを計算し、非負つまり[0,m)の範囲で返す
// 参考：https://atcoder.jp/contests/abc376/submissions/58995270 (ecottea氏のテンプレート)
template<typename T>
T smod(T n, T m) {
	n %= m;
	if(n < 0) n += m;
	return n;
}

int main(void)
{
	ll i, k;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	const ll dmx = 29;  // 2^d の最大
	vector<ll> d(dmx+1);
	for(k = 0; k < dmx+1; k++)
	{
		ll p2 = (1ll<<k);  // 2^k
		map<ll, pair<ll,ll>> mp;  // mp[A] : (mod 2^k)において、Ai=AであるようなAiの{総和S, 個数num}
		for(i = 0; i < N; i++)
		{
			// 二重シグマの条件がi<=jなので、まずはa[i]をmpに加える
			ll tmp = a[i]%p2;
			auto [s,n] = mp[tmp];
			s += a[i];
			n++;
			mp[tmp] = {s,n};

			tmp = smod(-a[i], p2);  // -a[i] mod 2^k
			if(mp.count(tmp) == 0) continue;
			auto [s2, n2] = mp[tmp];
			d[k] += s2 + a[i]*n2;
		}
	}
	
	ll ans = 0;
	for(k = 0; k < dmx; k++)
	{
		ll p2 = (1ll<<k);  // 2^k
		ans += (d[k] - d[k+1]) / p2;
	}
	cout << ans << endl;

	return 0;
}
