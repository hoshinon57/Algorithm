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

// ABC388 https://atcoder.jp/contests/abc388

/*
 * コンテスト時、嘘の貪欲法でハマってしまった。
 * YouTube公式解説がとても丁寧。必聴。
 *   https://www.youtube.com/live/UWoRBhN2s6Y
 * 
 * YouTube公式解説より、N=8で順に
 *   上下×上上下×下
 * のように使うとき、上下を入れ替えて
 *   上上×上下下×下
 * としても損しない。(作れる数は減らない)
 * また使わない×についても移動させて
 *   上上上××下下下
 * としても損しない。
 * ここから、「上は小さいほど、下は大きいほど良い」という予想が得られる。
 * (使う個数を決め打ちすると、上下の割り当ては確定する ここから二分探索にて解くこともできる)
 * 
 * ここから本ソースコードのO(N)は、YouTube解説の1:52:00辺りからを参照。
 * 
 * 【反省】
 * コンテスト時、「前から貪欲に上下作っていく」とした。
 * これは {1,2,4,4} でダメ。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = 0;
	ll li = 0, ri = N-(N/2);  // モチの上側のidxと下側のidx 最大でN/2個作れる
	while(ri < N)
	{
		if(a[li] <= a[ri]/2)
		{
			li++; ri++;
			ans++;
		}
		else
		{
			ri++;
		}
	}
	cout << ans << endl;

	return 0;
}
