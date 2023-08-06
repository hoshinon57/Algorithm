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

// ABC313 https://atcoder.jp/contests/abc313

/*
 * 制約「Kは奇数」に着目する。
 * N=4,K=3のときにどうやれば解けるかを考えると、
 *   A2+A3+A4
 *   A1+A3+A4
 *   A1+A2+A4
 *   A1+A2+A3
 * をまず質問する。これらを足すと
 *   (A1+A2+A3+A4)*K(=3)
 * となり、Kは奇数であることから上記の和とA1～A4の和は偶奇が一致する。(Kが奇数という制約が効いてくる)
 * するとA2+A3+A4の偶奇と比較することで、A1が求まる。
 * A2,A3,A4も同様に求まる。
 * 
 * では次にNが4(=K+1)より大きい場合を考えるが、これも同じ考え方で解ける。
 * ここまででA1+A2の偶奇が分かるので、
 *   A5：A1+A2+A5を質問すれば解ける
 *   Ax：A1+A2+Axを質問すれば解ける
 * となる。
 */

int main(void)
{
	// 1-indexed
	int i, j;
	int N, K; cin >> N >> K;
	ll tot = 0;
	vector<int> exc(N+1);  // [i]:Aiを選ばない場合の、A1～A(K+1)の和の偶奇
	// まず、A1～A(K+1)の和の偶奇を求める
	for(i = 1; i <= K+1; i++)  // Aiを選ばない場合
	{
		// A1+A2+...+AK+A(K+1) (ただしAiは選ばない) の和を質問する
		string str = "?";
		for(j = 1; j <= K+1; j++)
		{
			if(i == j) continue;  // Aiは選ばない
			str += " " + to_string(j);
		}
		cout << str << endl;
		int tmp; cin >> tmp;
		exc[i] = tmp;
		tot += tmp;
	}
	tot %= 2;  // tot:A1～A(K+1)の偶奇と一致する

	// 次に、A1～A(K+1)を個別に求める
	vector<int> ans(N+1, 0);
	for(i = 1; i <= K+1; i++)
	{
		if(tot != exc[i]) ans[i] = 1;
	}

	// 最後に、A(K+2)～ANを求める
	tot = 0;
	for(i = 1; i <= K-1; i++)
	{
		tot += ans[i];
	}
	tot %= 2;  // A1～A(K-1)までのの和の偶奇
	for(i = K+2; i <= N; i++)
	{
		// A1～A(K-1)と、Aiの和を質問する
		string str = "?";
		for(j = 1; j <= K-1; j++)
		{
			str += " " + to_string(j);
		}
		str += " " + to_string(i);
		cout << str << endl;
		int tmp; cin >> tmp;
		if(tot != tmp) ans[i] = 1;
	}

	string ans_str = "!";
	for(i = 1; i <= N; i++)
	{
		ans_str += " " + to_string(ans[i]);
	}
	cout << ans_str << endl;

	return 0;
}
