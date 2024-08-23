#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC330 https://atcoder.jp/contests/abc330

/*
 * evima氏の解説を元に実装した内容。
 *   https://atcoder.jp/contests/abc330/editorial/7774
 * 
 * 二分探索の判定関数cal()をO(N)で処理している。
 * a[]の各要素を幅lenの範囲に収めるとき、幅の左端は
 *   a[*]のどこか
 *   a[*]-len のどこか
 * となる。前者は幅lenの左端を、後者は右端を合わせるイメージ。
 * このN*2要素をソートした状態で持つと、最適な位置は中央値となる。
 * 
 * ではソートされたN*2要素をどう作るかだが、マージソートの要領でstd::merge()を使うことでO(N)で構築できる。
 * 詳細は実装を参照。
 */

int main(void)
{
	int i;
	ll N, K;  cin >> N >> K;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}
	sort(x.begin(), x.end());
	sort(y.begin(), y.end());

	// 各a[]を幅lenに収めるときの操作回数を返す
	// aはソート済みであること
	auto cal = [&](vector<ll> &a, ll len)
	{
		vector<ll> a2;
		for(auto &e : a) a2.push_back(e-len);
		// aはソート済みなので、a2もソート済み
		vector<ll> a_result;
		std::merge(a.begin(), a.end(), a2.begin(), a2.end(), std::back_inserter(a_result));

		// a_resultの要素数は2*N. Nor(N+1)番目の要素を幅lenの左端とするのが最善 -> [N-1],[N]
		ll ret = 0;
		for(auto &e : a)
		{
			if(e < a_result[N-1])
			{
				ret += a_result[N-1]-e;
			}
			else if(e > a_result[N-1]+len)
			{
				ret += e - (a_result[N-1]+len);
			}
		}
		return ret;
	};

	ll ng = -1, ok = INF32;  // ng:囲めない側, ok:囲める側
	while(ok - ng > 1)
	{
		ll mid = (ng+ok)/2;
		ll cnt = 0;
		cnt += cal(x, mid);
		cnt += cal(y, mid);
		if(cnt <= K) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}
