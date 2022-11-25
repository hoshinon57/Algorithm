#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題73
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc145/tasks/abc145_d

/*
 * 移動方向が{+1,+2}と{+2,+1}であり、どちらも計3マス進む。
 * よって進み方はどうあれ、(X+Y)/3が動かす回数になる。
 * 
 * この動かす回数を、{+1,+2}に何回割り振れば(X,Y)に到達するかをまず全探索する。
 * (ちょっと考えれば連立方程式でO(1)で計算できる…反省)
 * ここで(X,Y)に到達できない場合は0を出力して終了する(0通り)。
 * 
 * 全部で(X+Y)/3回動かすうち、{+1,+2}方向に進むのを何回目に割り振るかという問題になるので、
 * combinationの ((X+Y)/3) C ({+1,+2}方向の移動回数) が答となる。
 */

const ll MOD = 1000000007;
const ll NCK_SIZE = 2000001;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	int i;
	int X, Y;
	cin >> X >> Y;
	int move_num = (X+Y)/3;  // 駒を移動する回数  端数は「移動できない」ケースに遷移するので気にしなくてよい

	int pattern_A, pattern_B;  // それぞれ、{+1,+2}および{+2,+1}方向に移動する回数
	// 駒を移動する回数のうち、2パターンの移動を何回行えば(X,Y)に到達できるかを全探索する
	for(i = 0; i <= move_num; i++)
	{
		pattern_A = i;
		pattern_B = move_num - i;
		if((pattern_A + pattern_B*2 == X) && (pattern_A*2 + pattern_B == Y)) break;
	}
	if(i == move_num+1)  // (X,Y)にはどうやっても移動できない
	{
		cout << 0 << endl;
		return 0;
	}

	nCk_init();
	// ((X+Y)/3) C ({+1,+2}方向の移動回数) を求める
	cout << nCk(move_num, pattern_A) << endl;

	return 0;
}
