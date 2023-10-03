#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC136 https://atcoder.jp/contests/abc136

/*
 * 約数列挙で解く。
 * 
 * まず、操作によりAの総和は変化しないことに着目する。これをA_sumとする。
 * 色々いじってみると、A_sumの約数が答の候補になりそう、というのが分かる。
 * 「候補」止まりであるのは、操作回数の上限Kが決まっているから。
 * ※Kが無限であれば、A1=A_sumとし、A2以降は全て0とすれば約数の最大値が答になる。
 * ※「操作にて要素が負になってもよい」とあるが、
 *   負にしてまで嬉しいことは無さそうだな(＝正だけで考えてよさそうだな)、と無証明で判断した。
 * 
 * dをA_sumの約数として、「K回以内に可能か」を約数の大きい方から判定していく。
 * 判定方法は以下(貪欲法)。
 *   A_mod[] = Aの各要素 % 約数d とし、昇順ソートしておく。
 *   A_modの小さい方からは、「操作で-1を足してdの倍数にする」こととし、
 *   A_modの大きい方からは、「操作で+1を足してdの倍数にする」こととする。
 *   これをA_modの両端から計算していくと操作回数が求まるので、K以下なら成功。
 * ※ここも無証明で実装した
 * 
 * [参考記事]
 * 公式解説のPDF
 * https://algo-logic.info/abc136e/
 * https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2019/0804_abc136
 * 
 * [ACまでの思考の流れ]
 * ・操作を行う問題では、操作により不変な量に着目する。
 * ・色々試してみると、最終的に、Aの各要素がA_sumの約数の倍数になっている必要がありそう。
 * ・約数ごとに試すとしても、約数ってそれほど多く持てない感覚。1000個ぐらいか。
 *   ならN=500なので十分間に合いそう。
 */

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

// K回以下の操作で、全要素がdivで割り切れるような操作が可能か判定する
bool check(ll div, vector<ll> &a, ll K)
{
	// まず、Aの各要素をdivで割った余りをa_modとし、昇順ソートしておく
	vector<ll> a_mod = a;
	for(auto &e : a_mod)
	{
		e %= div;
	}
	sort(a_mod.begin(), a_mod.end());

	// a_modの小さい方からは「-1してdivの倍数にする」操作を、大きい方からは「+1してdivの倍数にする」操作を行っていく。
	// -1と+1の回数は一致するので、前者の回数をカウントしていく。
	ll l = 0, r = a_mod.size()-1;
	ll sum = 0;  // divの倍数にするために要素から取り除いた余剰分、的な
	ll cnt = 0;
	while(l <= r)
	{
		if(sum <= 0)  // 余剰分が無ければ、-1する操作の方から
		{
			sum += a_mod[l];
			cnt += a_mod[l];
			l++;
		}
		else  // 余剰分があれば、+1する操作の方から  分け与えるイメージ
		{
			sum -= div-a_mod[r];
			r--;
		}
	}
	return (cnt<=K);
}

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	ll a_sum = 0;
	vector<ll> a(N);
	for(i = 0; i < N; i++) {
		cin >> a[i];
		a_sum += a[i];
	}

	// Aの総和の約数を列挙する
	vector<ll> a_div;
	func_divisor(a_sum, a_div);
	reverse(a_div.begin(), a_div.end());

	for(auto &e : a_div)  // 約数eが条件を満たすか
	{
		if(check(e, a, K))
		{
			cout << e << endl;
			break;
		}
	}
	// e=1は必ず条件を満たす

	return 0;
}
