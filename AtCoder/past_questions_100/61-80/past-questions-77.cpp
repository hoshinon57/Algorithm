#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題77
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2010ho/tasks/joi2010ho_a

/*
 * 累積和で解く。
 * 
 * 入力である宿場町間の距離について、累積和を計算しておく。
 * 今いる宿場町nowからnow+aへ移動することを考えると、
 * d[now] + d[now+1] + ... + d[now+a-1] のa個の和が移動距離となる。
 * これを[now, now+a)の半開区間で考えると、d_sum[now+a] - d_sum[now]となる。
 * 
 * aが負の場合も同じように計算し、絶対値を取ればよい。
 * 
 * 計算量はO(n+m).
 */
int main(void)
{
	// 0-indexed
	int i;
	int n, m;  // 宿場町の数, 旅の日数
	cin >> n >> m;
	vector<int> d(n-1);  // d[i]:宿場町iとi+1の間の距離
	vector<int> d_sum(n-1+1);  // d[]の累積和
	for(i = 0; i < n-1; i++)
	{
		cin >> d[i];
		d_sum[i+1] = d_sum[i] + d[i];
	}

	ll answer = 0;
	int now = 0;  // 今いる宿場町の番号
	for(i = 0; i < m; i++)
	{
		int a;
		cin >> a;
		// aが負の場合もあるので、abs()で絶対値を取る
		// ※過去問精選の71番"散歩"と違い、d_sum[]はMODを取っていないので単純に絶対値を取って良い
		answer += abs(d_sum[now+a] - d_sum[now]);
		answer %= MOD;
		now += a;  // 宿場町を移動
	}
	cout << answer << endl;

	return 0;
}
