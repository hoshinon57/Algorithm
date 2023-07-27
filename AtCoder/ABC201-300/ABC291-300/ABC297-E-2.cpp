#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC297 https://atcoder.jp/contests/abc297

/*
 * ABC297-E.cppのpriority_queueをset使用に変えただけのもの。
 * 考え方やアルゴリズムは同じ。
 */

int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}

	ll v;
	set<ll> st;
	st.insert(0);
	K++;  // 初回の0円のぶん
	while(K > 0)
	{
		v = *st.begin();  // setの先頭、つまり最小値を取得
		st.erase(v);

		// 集合の最小値に、各Aiを加算したものを追加する
		// すると、次の最小値は必ず集合に存在する
		for(i = 0; i < N; i++)
		{
			st.insert(v + a[i]);
		}

		K--;
	}
	cout << v << endl;

	return 0;
}
