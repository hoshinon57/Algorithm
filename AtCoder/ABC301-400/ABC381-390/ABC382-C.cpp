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

// ABC382 https://atcoder.jp/contests/abc382

/*
 * man[b]を、おいしさBの寿司を誰が食べるか(-1ならだれも食べない)と定義する。
 * Bの最大値をBMAXとし、要素数をman[BMAX]まで用意する。
 * 
 * 人iについて、美味しさAi以上の寿司は全てこの人が食べるので、
 * man[Ai]から大きい方へ順に、man[]!=-1である限りman[Ai]=iとしていく。
 * この処理はO(N*BMAX)とかになりそうに見えるが、man[]は高々1回しか更新されないため、O(BMAX)で済む。
 * 
 * 後は各Biに対してman[Bi]を出力すれば良い。
 */

int main(void)
{
	// 0-indexed
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	const ll MX = 200000;  // Bの最大値
	vector<ll> man(MX+5, -1);  // man[b]:おいしさBの寿司を誰が食べるか -1ならだれも食べない
	for(i = 0; i < N; i++)  // 人i
	{
		for(j = a[i]; j <= MX; j++)
		{
			if(man[j] != -1) break;
			man[j] = i+1;
		}
	}
	for(i = 0; i < M; i++)
	{
		cout << man[b[i]] << endl;
	}

	return 0;
}
