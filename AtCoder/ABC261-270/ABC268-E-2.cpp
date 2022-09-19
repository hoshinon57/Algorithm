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
 * ABC268-E.cpp の実装が乱雑になったため、公式解説やTwitterを見ながら改善を実施。
 * いもす法…ぽい方法で解く。
 * 
 * 不満度の変化量diff_fuman[]と、diff_fuman[]の変化量delta[]、2つのテーブルを持つ。
 * 
 * まずdelta[]を人ごとに事前計算する。
 * xを、「人iの目の前に料理iが来るには(＝不満度0）、テーブルを何回回せば良いか」と定義すると、
 *   x = (i-idx[i]+N)%N;
 * で表すことができる。
 * 自分の料理が目の前を通り過ぎるとき、diff_fumanは-1から+1へ変化するので、delta[x]=+2となる。
 * 一方で料理が正面を通り過ぎるときは、+1から-1へ変化するので、delta[x+N/2]=-2となる。
 * ただしNが奇数時はdiff_fumanが+1,0,-1と変化するので、
 *   delta[x+N/2]=-1;
 *   delta[x+(N+1)/2]=-1;
 * とすることで、偶奇を吸収して同じ処理にできる。
 * 
 * 次にdiff_fuman[0]を事前計算する必要がある。
 * これは初期状態で料理が近づいてくる場合、遠ざかっていく場合とで、それぞれ+1, -1となる。
 * またNが奇数時に料理が正面を通過する場合は不満度が変化しないので、diff_fuman[0]=0となる。
 * 
 * 以上により、変化量を事前に計算する部分が簡易化できた。
 * 
 * 後は、
 * ・テーブル初期配置での不満度を事前計算
 * ・テーブルを(N-1)回回し、それぞれの不満度を計算して、その最小値を出力
 * する処理を行う。
 * これはABC268-E.cppとおおよそ同じため、コメントは省略する。
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

	vector<ll> diff_fuman(N);
	vector<ll> delta(N);
	for(i = 0; i < N; i++)  // 各人ごとにdelta[]とdiff_fuman[0]を事前計算する
	{
		int x = (i-idx[i]+N)%N;  // 人iの目の前に料理iが来るには(＝不満度0）、テーブルを何回回せば良いか
		delta[x] += 2;
		delta[(x+N/2)%N]--;  // NとN+1を分けることで、偶奇を吸収する
		delta[(x+(N+1)/2)%N]--;

		// diff_fuman[0]の計算
		if(x < N/2)
		{
			diff_fuman[0]--;
		}
		else if(x >= (N+1)/2)
		{
			diff_fuman[0]++;
		}
		else {;}  // 奇数で料理が正面を通過、つまり不満度が変化しないとき
	}

	// テーブル初期配置での不満度を事前計算
	ll fuman = 0;
	for(i = 0; i < N; i++)
	{
		int x = (i-idx[i]+N)%N;  // 人iの目の前に料理iが来るには(＝不満度0）、テーブルを何回回せば良いか
		fuman += min(x, N-x);  // 不満度はxかN-xのうち小さい方
	}

	ll answer = fuman;
	for(i = 1; i < N; i++)
	{
		diff_fuman[i] = diff_fuman[i-1] + delta[i-1];
		fuman += diff_fuman[i];
		answer = min(answer, fuman);
	}
	cout << answer << endl;

	return 0;
}
