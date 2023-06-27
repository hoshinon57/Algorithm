#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC306 https://atcoder.jp/contests/abc306

/*
 * やりたいことは、数列Aの上位Kの和をQ回求める、ということ。
 * multisetを使って、数列Aのうち
 *   上位K個を保持する集合
 *   残りのN-K個を保持する集合
 * を用意し、クエリごとに集合から出し入れと総和の足し引きを行っていけばよい。
 * 
 * ABC281-E(Least Elements)が近い。
 * https://atcoder.jp/contests/abc281/tasks/abc281_e
 */
int main(void)
{
	// 数列Aの大きい方からK個の和を毎回求める
	int i;
	ll N, K, Q;
	cin >> N >> K >> Q;
	vector<ll> a(N);
	multiset<ll> list_gr, list_ls;  // それぞれ、大きい方K個、小さい方(N-K)個
	for(i = 0; i < K; i++) list_gr.insert(0);
	for(i = 0; i < N-K; i++) list_ls.insert(0);
	ll ans = 0;

	while(Q > 0)
	{
		ll x, y;
		cin >> x >> y;
		x--;  // 0-indexedに変換

		ll a_before = a[x];  // Yに変更する前の値 これを集合から消す必要がある
		a[x] = y;
		if(list_gr.count(a_before) > 0)  // 消すべきデータが、大きい側にある
		{
			list_gr.erase(list_gr.find(a_before));
			ans -= a_before;

			// とりあえずYを小さい側に入れて、最大値を大きい側に移す
			list_ls.insert(y);
			auto itr = list_ls.end();
			itr--;
			list_gr.insert(*itr);
			ans += *itr;
			list_ls.erase(itr);
		}
		else  // 消すべきデータが、小さい側にある
		{
			list_ls.erase(list_ls.find(a_before));

			// とりあえずYを大きい側に入れて、最小値を小さい側に移す
			list_gr.insert(y);
			ans += y;
			auto itr = list_gr.begin();
			ans -= *itr;
			list_ls.insert(*itr);
			list_gr.erase(itr);
		}
		cout << ans << endl;
		Q--;
	}

	return 0;
}
