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

// ABC032 https://atcoder.jp/contests/abc032

/*
 * 尺取り法で解く。
 * 
 * lを左から順に動かしながら、条件を満たす間、rを右に動かしていく。
 * 半開区間[l,r)で考えると実装しやすかった。
 * 
 * sに0が含まれるときは処理が面倒になるので、Nを出力して終了させる。
 * 
 * 計算量はO(N).
 * 
 * 公式解説、及びけんちょんさんの記事が分かりやすい。
 *   https://img.atcoder.jp/abc032/editorial.pdf
 *   https://qiita.com/drken/items/ecd1a472d3a0e7db8dce
 */

int main(void)
{
	int i;
	int N, K;
	cin >> N >> K;
	vector<ll> s(N);
	for(i = 0; i < N; i++) cin >> s[i];

	// sに0が含まれれば、先頭から末尾までの積も0になるので、Nが答
	for(i = 0; i < N; i++)
	{
		if(s[i] == 0)
		{
			cout << N << endl;
			return 0;
		}
	}
	// 以降、s[i]に0は含まれない

	// 半開区間[l,r)で考える
	int l, r;
	ll mul = 1;
	int answer = 0;
	for(l = 0, r = 0; l < N; l++)
	{
		while(r < N && mul*s[r] <= K)  // 条件を満たす間、rを右に動かす
		{
			mul *= s[r];
			r++;
			// この時点の[l,r)は、積がK以下という条件を満たす
		}
		answer = max(answer, r-l);

		if(l == r)  // [1,1) のような場合、次にlを+1するのでrも+1しておく(追い越し防止)
		{
			r++;
		}
		else  // [1,3) のような場合、次にs[1]を積から除外するので、除算しておく
		{
			mul /= s[l];  // 事前の判定により s[l]!=0 が保証される
		}
	}

	cout << answer << endl;

	return 0;
}
