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

// ABC334 https://atcoder.jp/contests/abc334

/*
 * 公式解説を参考に、両端から累積和で解く。
 * 以下、Kが奇数のときを考える。
 * (偶数のときは、末尾にINFを追加することで同じ処理とできる)
 * 
 * pre_sum[]を、Aの先頭から2個ずつ取ったときの差分、の累積和とする。
 * pre_sum[0]=0であり、pre_sum[1]には先頭2個の差分というインデックスとする。
 * またsuf_fux[]をAの末尾から2個ずつ取ったときの差分、の累積和とする。
 * 
 * pre_sum[]からi個、suf_sum[]から(K/2-1)個を各iについて計算していくことで、
 * 靴下を1つ捨てる全パターンを計算できる。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<int> a(K); for(i = 0; i < K; i++) {cin >> a[i];}
	if(K%2 ==0)  // 偶数なら末尾にINFを追加して奇数扱いにする
	{
		a.push_back(INF32);
		K++;
	}

	vector<int> pre_sum(K/2+1), suf_sum(K/2+1);  // pre[]:a[i*2+1]-a[i*2]のように2個ずつ取っていったときの累積和
	for(i = 0; i < K/2; i++)
	{
		pre_sum[i+1] = pre_sum[i] + a[i*2+1]-a[i*2];  // 先頭から2個ずつ の累積和
		suf_sum[i+1] = suf_sum[i] + a[K-i*2-1]-a[K-i*2-2];  // 末尾から2個ずつ の累積和
	}
	int ans = INF32;
	for(i = 0; i <= K/2; i++)
	{
		// 前からi個, 後ろからK/2-i個の計K個の和のうち、最小を探す
		chmin(ans, pre_sum[i] + suf_sum[K/2-i]);
	}
	cout << ans << endl;

	return 0;
}
