#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC193 https://atcoder.jp/contests/abc193

/*
 * 2人の最後のカードについて全探索する。
 * 
 * zan[i]を数値iのカードの残枚数とし、
 * 高橋くんの最後のカードをCi, 青木くんのそれをCjとすると、そうなる場合の数は
 *   Ci =Cjのとき：zan[Ci]*(zan[Ci]-1) 通り
 *   Ci!=Cjのとき：zan[Ci]*zan[Cj] 通り
 * となる。
 * これを全組み合わせについて計算していく。
 */

int main(void)
{
	int i, j;
	int K; cin >> K;
	vector<ll> zan(10, K);  // [i]:数値iの残枚数
	vector<vector<int>> card(2, vector<int>(10, 0));  // [i][j]:i=0なら高橋くん、i=1なら青木くんが持つ、数値jのカードの枚数
	for(i = 0; i < 2; i++)
	{
		string s; cin >> s;
		for(j = 0; j < 4; j++)
		{
			int n = s[j]-'0';
			zan[n]--;
			card[i][n]++;
		}
	}

	// 10^nを返す
	auto pow10 = [](int n) -> ll
	{
		ll ans = 1;
		while(n > 0)
		{
			ans *= 10;
			n--;
		}
		return ans;
	};
	// c:表の4枚, last:最後の1枚 のときのスコアを返す
	auto calc = [&](vector<int> c, int last) -> ll
	{
		ll ans = 0;
		c[last]++;
		for(int k = 1; k <= 9; k++)
		{
			ans += k*pow10(c[k]);
		}
		return ans;
	};
	ll win = 0;  // 高橋くんが勝つケース数
	ll tot = 0;  // 全組み合わせ数
	for(i = 1; i <= 9; i++)
	{
		for(j = 1; j <= 9; j++)
		{
			ll c;
			if(i == j) c = zan[i] * (zan[i]-1);
			else c = zan[i] * zan[j];
			tot += c;
			if(calc(card[0], i) > calc(card[1], j)) win += c;  // 高橋くんが勝つなら、勝つケース数に加算
		}
	}
	cout << std::fixed << std::setprecision(15);
	cout << (double)win / tot << endl;

	return 0;
}
