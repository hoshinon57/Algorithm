#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題48
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1611&lang=jp

/*
 * 自力で解けず、各所の解説を見た。
 *   https://ferin-tech.hatenablog.com/entry/2017/05/17/123005
 *   https://qiita.com/mizoono/items/18cda7b6333bf35fb9ad
 *   https://drken1215.hatenablog.com/entry/2020/03/10/160500
 * 区間DPで解く。
 * 
 * dp[l][r]を以下のように定義する。
 *   半開区間[l, r)にて消せるだるまの最大数
 *   l:0～N-1
 *   r:1～N
 * 
 * dpの遷移は以下のようになる。
 * (1)r-l=2, つまりだるま2個の場合
 *   重さの差が2以下であれば dp[l][r]=2, つまり消せる。
 * (2)r-l>2 の場合
 *   (a)区間[l,r)の中で区切りkを1つずつ動かしていったときに、
 *      dp[l][k] + dp[k][r]
 *      の最大値
 *   (b)区間[l,r)のうち両端を除く[l+1,r-1)が全て消せる、という前提で、
 *      両端のl,r-1のペアが
 *        消せる場合
 *        消せない場合
 *      のそれぞれのケース
 *   (a)(b)の最大値がdp[l][r]となる。
 * 以上をメモ化再帰にて実装した。
 * 
 * 計算量は、以下よりO(N^3)となる。
 *   (1)dpの状態がN*N通りなので、O(N^2)
 *   (2)各dpの状態において、区切りをずらしながら最大値を探す部分がO(N)
 */

const int MAX_N = 300;
int N;
vector<int> w;

// dp[l][r]
//   半開区間[l, r)にて消せるだるまの最大数
//   l:0～N-1
//   r:1～N
vector<vector<int>> dp;

// 再帰を用いてdp[l][r]を返す
int func(int l, int r)
{
	int &d = dp[l][r];  // 記述簡略化

	if(d != -1) return d;  // メモ化済み

	if(r - l == 1)  // 1個だけのだるまは消せない
	{
		d = 0;
		return d;
	}
	else if(r - l == 2)  // 2個
	{
		// 差が1以下なら消せる
		if(abs(w[l]-w[l+1]) <= 1) d = 2;
		else d = 0;
		return d;
	}

	int answer = -1;
	// 区間[l,r)の中で区切りkを1つずつ動かしていったときに、
	//   dp[l][k] + dp[k][r]
	// の最大値
	int k;  // [l,r)間の区切り
	for(k = l+1; k <= r-1; k++)
	{
		int tmp = func(l, k) + func(k, r);
		answer = max(answer, tmp);
	}
	// 区間[l,r)のうち両端を除く[l+1,r-1)が全て消せる、という前提で、
	// 両端のl,r-1のペアが
	//   消せる場合
	//   消せない場合
	// のそれぞれのケース
	if(func(l+1, r-1) == (r-1)-(l+1))  // [l+1,r-1)が消せる場合
	{
		int tmp;
		if(abs(w[l]-w[r-1]) <= 1)  // 両端のペアで消せるか
		{
			tmp = r-l;  // 全て消せる
		}
		else
		{
			tmp = r-l-2;  // 両端の2個だけ残る
		}
		answer = max(answer, tmp);
	}
	d = answer;

	return d;
}

int main(void)
{
	// 0-indexed
	while(true)
	{
		cin >> N;
		if(N == 0) break;
		w.resize(N);
		for(int i = 0; i < N; i++) cin >> w[i];
		dp.assign(N+1, vector<int>(N+1, -1));  // -1で初期化 以前のデータを消したいので、resize()ではダメ

		cout << func(0, N) << endl;
	}
	
	return 0;
}
