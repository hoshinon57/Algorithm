#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題23
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008ho/tasks/joi2008ho_c

/*
 * 二分探索で解く。
 * 
 * 点数P1～PNから4つ選ぶ組み合わせを全て計算するのは、O(N^4)となりTLEとなる。
 * よって、
 * ・2本投げたときのスコアを全て計算する (これはO(N^2)であり間に合う)
 * ・上記スコアを「2つ選ぶ」
 * ことで、4つ選ぶ操作を実現する。
 * 
 * まず入力の点数p[]の各要素に対して、2つ取り出した和をp_2[]に登録する。
 * これは2本投げたときのスコアに相当する。
 * 
 * 次にp_2[]の各要素に対して、値をeとしたときに
 *  e+X <= M
 * となる最大のXを、p_2[]の中から探す。ここを二分探索で求める。
 * 
 * 問題文の「1本も投げなくても構わない」をそのまま実装するのは面倒なので、
 * p[]に点数0を事前に登録しておけばよい。
 * 
 * 計算量はO(N^2*logN).
 * p_2[]の要素数がN^2であり、これに対してソート操作やupper_bound()を行うため。
 */

int main(void)
{
	int i;
	ll N, M;
	cin >> N >> M;
	vector<ll> p(N);
	for(i = 0; i < N; i++) cin >> p[i];
	p.push_back(0);  // 「投げない」をスコア0として登録

	// p[]の各要素から2つ取り出した和をp_2[]に登録する
	// これは2本投げたときのスコアに相当する
	vector<ll> p_2;
	for(auto &e1 : p)
	{
		for(auto &e2 : p)
		{
			p_2.push_back(e1+e2);
		}
	}
	sort(p_2.begin(), p_2.end());

	// p_2[]の各要素から2つ取り出した和が、4本投げたときのスコアになる
	// このうちM以下の最大の整数を求めたい
	ll answer = -1;
	for(auto &e : p_2)
	{
		if(e > M) continue;  // 2本投げた時点でMを超えていたら、そもそもNG
		// 以下、e<=M, つまりM-e>=0が保証される

		// e+X <= M である最大のXを、p_2[]から求める
		// X <= M-e
		// upper_bound()でM-eより大きい要素を求めて、その1つ前が求めるXとなる
		auto itr = upper_bound(p_2.begin(), p_2.end(), M-e);
		// M-e>=0より、itrが指す要素の値は0より大きい値となる。
		// よってp_2内に0が存在するため、itr!=p_2.begin()が保証される。
		// つまり無条件でデクリメントして良い。
		itr--;
		answer = max(answer, e+(*itr));
	}
	cout << answer << endl;

	return 0;
}
