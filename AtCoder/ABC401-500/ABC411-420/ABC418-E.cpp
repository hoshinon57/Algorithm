#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

#if 0
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#endif

// ABC418 https://atcoder.jp/contests/abc418

/*
 * コンテスト時に解き切れなかった。
 * 
 * まず、辺を2つ取ったときに平行になる組み合わせの数を求めたい気持ちになる。
 * 
 * 全ての2点の組み合わせについて {dy,dx} を「正規化して」求め、mapでその数をカウントする。
 * {2,4}と{3,6},{-4,-8}などを同じくくりにしたいので、gcdで割り、dyが負なら-1倍するなどする。
 * dx/dyを約分してdy>0にするイメージ。
 * これだけでは{0,-1}と{0,1}の区別がされないので、特出しで処理する。
 * 以上、詳しくは実装を参照。
 * 
 * これで台形の数が求められる…と思いきや、平行四辺形についてダブリカウントしてしまっている。
 * そのため平行四辺形の数をカウントする必要がある。
 * 公式解説では辺の中点としていたが、
 * 平行四辺形は「平行である2辺の長さが同じ」なので、自分は辺の長さ(の2乗)lenを追加した {dy,dx,len} をmapでカウントし、
 * 同じ要素について引く形とした。
 * ⇒このとき、平行四辺形もこれまたダブリカウントさるので、2で割る必要がる。この辺りややこしい。
 * 
 * …と、ここまでやってTLE.
 * 制限時間4sだが、 O(N^2*logN)は無理みたい。まあmap2つ使うし。
 * pragma最適化を追加すると、ギリギリでACした。(ランダム要素あるが、3900msとか)
 * これが実装の#if 0の部分。
 * 
 * mapを使わず実行時間に余裕を持たせた実装として、mapではなくvector＆ソートの手法がある。
 * 以下の実装で#if 1側の部分。
 * カウントをソートし、ランレングス圧縮すると同じ要素の数を楽にカウントできる。
**/

using pll = pair<ll,ll>;
using ar3 = array<ll,3>;

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

// 2点(x1,y1),(x2,y2)の距離の "2乗" を返す
// 実距離を求めたい場合は呼び出し元で sqrt(dist_squared()) など行うこと
// 引数はlong longにしているが、2乗の関係で引数が10^9を超えるとオーバーフローするため注意 (10^9を超える問題はあまり出ないと思うが)
ll dist_squared(ll x1, ll y1, ll x2, ll y2) {
	ll dx = x1-x2, dy = y1-y2;
	return dx*dx+dy*dy;
}

// 文字列strをランレングス圧縮して {文字,長さ} のpairの列挙で返す
// [補足]配列に対して扱いたい場合、引数をstring->vector<T>に変えるだけで良い
//       "長さ"は呼び出し元で乗算する可能性があるため、オーバーフロー防止でll型としている (ABC369-C)
// auto enc = rle<char>(s); といった形で呼び出す
// for(auto [c,len] : enc) が使い方の一例
// [参考]https://algo-logic.info/run-length/  [verify]ABC019-B,ABC380-B
template <typename T> vector<pair<T,ll>> rle(vector<T> &str) {
	vector<pair<T,ll>> ret;
	int n = str.size();
	for(int l = 0, r = 0; l < n; ) {  // 尺取り法っぽく [l,r)が条件を満たす
		while(r < n && str[l] == str[r]) r++;
		ret.push_back({str[l], r-l});
		l = r;
	}
	return ret;
}

int main(void)
{
#if 1
	ll i, j;
	ll N; cin >> N;
	vector<pll> cnt1;  // 傾き {dy,dx} の個数
	vector<ar3> cnt2;  // 傾きと長さの2乗 {dy,dx,len} の個数
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	ll ans = 0;
	ll tmp = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // 2点i,j
		{
			ll dy = y[j] - y[i];
			ll dx = x[j] - x[i];
			ll g = gcd(abs(dy), abs(dx));
			dy /= g;
			dx /= g;
			if(dy < 0)
			{
				dy = -dy;
				dx = -dx;
			}
			if(dy == 0) dx = 1;  // {0,-1}が{0,1}になる

			cnt1.push_back({dy,dx});

			ll di = dist_squared(x[i], y[i], x[j], y[j]);
			cnt2.push_back({dy, dx, di});
		}
	}

	// 台形のカウント
	// ソートし、ランレングス圧縮で同じ傾きをまとめる
	{
		sort(cnt1.begin(), cnt1.end());
		auto enc = rle<pll>(cnt1);
		for(auto [c,len] : enc)
		{
			ans += len*(len-1)/2;
		}
	}
	// 平行四辺形のカウント
	{
		sort(cnt2.begin(), cnt2.end());
		auto enc = rle<ar3>(cnt2);
		for(auto [c,len] : enc)
		{
			tmp += len*(len-1)/2;
		}
		tmp /= 2;  // ある平行四辺形について、2回カウントされるため
	}

	ans -= tmp;
	cout << ans << endl;

#else
	ll i, j;
	ll N; cin >> N;
	map<pll,ll> cnt1;  // 傾き {dy,dx} の個数
	map<ar3,ll> cnt2;  // 傾きと長さの2乗 {dy,dx,len} の個数
	vector<ll> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	ll ans = 0;
	ll tmp = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // 2点i,j
		{
			ll dy = y[j] - y[i];
			ll dx = x[j] - x[i];
			ll g = gcd(abs(dy), abs(dx));
			dy /= g;
			dx /= g;
			if(dy < 0)
			{
				dy = -dy;
				dx = -dx;
			}
			if(dy == 0) dx = 1;  // {0,-1}が{0,1}になる

			ans += cnt1[{dy,dx}];
			cnt1[{dy,dx}]++;

			ll di = dist_squared(x[i], y[i], x[j], y[j]);
			tmp += cnt2[{dy, dx, di}];
			cnt2[{dy, dx, di}]++;
		}
	}
	ans = ans - tmp/2;
	cout << ans << endl;
#endif

	return 0;
}
