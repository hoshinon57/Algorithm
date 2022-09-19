#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC268 https://atcoder.jp/contests/abc268

/*
 * ABC268-E-2.cppを、Twitterの解説を元にさらに改善した内容。
 *   https://twitter.com/hamamu_kyopro/status/1568603998109990912
 *   https://twitter.com/stat_learning/status/1568942612434010114
 * 
 * 周期Nのため、不満度の変化の山が途中で途切れてしまい、場合分けが煩雑になる。
 * よって周期を2倍の2Nにする、という考え方。
 * 
 * 以下はABC268-E-2.cppと同じ。
 * ＞不満度の変化量diff_fuman[]と、diff_fuman[]の変化量delta[]、2つのテーブルを持つ。
 * ＞xを、「人iの目の前に料理iが来るには(＝不満度0）、テーブルを何回回せば良いか」と定義すると、
 * ＞  x = (i-idx[i]+N)%N;
 * ＞で表すことができる。
 * 
 * (1)
 * 周期2Nにより山を途切れずに表現できるので、delta[]を以下のように事前計算する。
 *   delta[x] = +1;  // 山の始まり
 *   delta[x+N/2] = -1;
 *   delta[x+(N+1)/2] = -1;
 *   delta[x+N] = +1;  // 山の終わり
 * 
 * (2)
 * 後はテーブルを(2N-1)回回し、各タイミングでの不満度を求める。
 * あるタイミングt(0<=t<N)での不満度は、
 *   fuman[t]+fuman[t+N]
 * にて計算できるので、これの最小値を求めればよい。
 * 
 * 以上のメリットとして、以下2点が挙げられる。
 * ・主に(1)より、diff_fuman[0]を事前計算する必要はなくなる。
 * ・主に(2)より、初期状態での不満度を事前計算する必要はなくなる。(fuman[0+N]に保持されているので)
 */

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> p(N);
	vector<int> idx(N);  // idx[i]:料理iは、どの人の前に置かれているか
	for(i = 0; i < N; i++)
	{
		cin >> p[i];
		idx[p[i]] = i;
	}
	// ここまで入力

	vector<ll> fuman(N*2);
	vector<ll> diff_fuman(N*2);
	vector<ll> delta(N*2);
	for(i = 0; i < N; i++)
	{
		int x = (i-idx[i]+N)%N;  // 人iの目の前に料理iが来るには(＝不満度0）、テーブルを何回回せば良いか
		delta[x]++;  // 山の始まり
		delta[(x+N/2)]--;  // NとN+1を分けることで、偶奇を吸収する
		delta[(x+(N+1)/2)]--;
		delta[x+N]++;  // 山の終わり
	}

	for(i = 1; i < N*2; i++)
	{
		diff_fuman[i] = diff_fuman[i-1] + delta[i-1];
		fuman[i] = fuman[i-1] + diff_fuman[i];
	}

	ll answer = INF64;
	for(i = 0; i < N; i++)
	{
		// 周期を2Nにして不満度の変化を計算したので、
		// 最終的に、テーブルをi回回したときの不満度は fuman[i]+fuman[i+N] となる
		answer = min(answer, fuman[i]+fuman[i+N]);
	}
	cout << answer << endl;

	return 0;
}
