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

// ABC369 https://atcoder.jp/contests/abc369

/*
 * 公式解説等の内容。累積xorで解く。
 *   https://atcoder.jp/contests/abc365/editorial/10607
 *   https://zenn.dev/yamasakit/articles/c4ac82f849ebcb
 * 
 * まず、bit演算なのでbitごとに分けて考えるのが典型。
 * また a[i]^a[i+1]^a[i+2]^...^a[j] の部分から、累積和ならぬ累積xorを考えてみる。
 * すると区間 [l,r) の累積xorは sum[r]^sum[l] となる。
 * 
 * 求める式を0-indexedにしたうえで変形していくと、
 * Σ{i=0～N-2}Σ{j=i+1～N-1}(Ai^Aj)   [i,j] -> [i,j+1)
 * = Σ{i=0～N-2}Σ{j=i+1～N-1}(sum[j+1]^sum[i])
 * = Σ{i=0～N-2}Σ{j=i+2～N}(sum[j]^sum[i])
 * = Σ{i=0～N-1}Σ{j=i+1～N}(sum[j]^sum[i]) - Σ(i=0～N-1)(sum[i]^sum[i+1])
 * = Σ{i=0～N-1}Σ{j=i+1～N}(sum[j]^sum[i]) - Σ(i=0～N-1)(a[i])
 * となる。
 * 左側の部分はsum[j]とsum[i]が異なる値、つまり(0,1)または(1,0)のときのみ1となる。
 * これは整理していくと累積xorについて (0の個数)*(1の個数) になることが分かる。
 * ※0,1異なる値に無向辺を結び、i<jなので右方向に矢印を作るイメージ。
 * 
 * 実装では、左側の部分はbitごとに計算し、
 * 右側の部分は最後にまとめて減算した。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ※コンテスト時は別解法で解いた。
 * ・a[i]^a[i+1]^a[i+2]^...^a[j] は累積xorを考えてみる。
 * ・2種の値があり、異なる値どうしをペアにする組み合わせの数は、 (種Aの個数)*(種Bの個数) となる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> a_xor(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {a_xor[kk+1] = a_xor[kk] ^ a[kk];}
	
	ll ans = 0;
	for(i = 0; i < 30; i++)  // iビット目
	{
		ll cnt_0=0, cnt_1=0;
		for(auto &e : a_xor)
		{
			if((e>>i)&1) cnt_1++;
			else cnt_0++;
		}
		ans += cnt_0*cnt_1*(1<<i);
	}
	for(i = 0; i < N; i++) ans -= a[i];
	cout << ans << endl;

	return 0;
}
