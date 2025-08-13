#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC281 https://atcoder.jp/contests/abc281

// 実装を、2025/8に再解きしたソースコードに置き換えた。
// この類の問題では、以下の方針で実装するとやりやすい。
//   (1)着目するM個の範囲から外れた値を削除する
//   (2)(1)で削除した側「でない方」の集合に、値を追加する
//   (3)(2)で追加した集合から1つ最大(最小)を取り出し、もう片方に移す

/*
 * コンテスト中に解けず、解説を見て実装した。
 * KoD氏の公式解説、および以下ツイートを元にした実装。
 * https://twitter.com/kyopro_friends/status/1601577869792059392
 * 
 * 各iごとに対象となるM個をソートするのはTLEになる。
 * そこで、multisetを2つ(ms1, ms2)用意して、
 * M個の整数を昇順に並べたときの、先頭K個の集合をms1, 残りM-K個をms2とする。
 * 
 * それぞれの「独立な問題」にて、a[i-M]を取り除き、a[i]を追加することを考えると、
 * ・まずa[i-M]がms1, ms2のどちらにあるか調べ、それを削除する
 * ・追加するa[i]を、 a[i]<ms2の最小値 ならばms1へ、そうでなければms2へ追加する
 * ・ms1の要素数が K-1 or K+1 になるケースがあるので、
 *   ms1/ms2間で調整する
 * 上記操作を、総和を保持する変数をつど更新していくことで、
 * 各問題ごとに O(logN) にて計算ができる。
 * 
 * i=1である最初の問題のみ、ms1,ms2の構築がてら個別に計算する実装にした。
 * 
 * 計算量はO(NlogN).
 */

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	multiset<ll> ms1, ms2;
	ll tot = 0;
	vector<ll> ans;

	// 初回は愚直
	{
		vector<ll> tmp;
		for(i = 0; i < M; i++) tmp.push_back(a[i]);
		sort(tmp.begin(), tmp.end());

		for(i = 0; i < K; i++)
		{
			ms1.insert(tmp[i]);
			tot += tmp[i];
		}
		for(i = K; i < M; i++)
		{
			ms2.insert(tmp[i]);
		}
	}
	ans.push_back(tot);
	
	for(i = 1; i <= N-M; i++)
	{
		// a[i-1]を外す
		// a[i+M-1]を追加
		if(ms1.find(a[i-1]) != ms1.end())  // ms1側にa[i-1]がある
		{
			ms1.erase(ms1.find(a[i-1]));
			tot -= a[i-1];
			ms2.insert(a[i+M-1]);

			auto itr = ms2.begin();
			auto num = *itr;
			ms2.erase(itr);
			ms1.insert(num);
			tot += num;
		}
		else  // ms2側にa[i-1]がある
		{
			ms2.erase(ms2.find(a[i-1]));
			ms1.insert(a[i+M-1]);
			tot += a[i+M-1];

			auto itr = ms1.end();
			itr--;
			auto num = *itr;
			ms1.erase(itr);
			tot -= num;
			ms2.insert(num);
		}
		ans.push_back(tot);
	}

	int sz_ = (int)ans.size();
	// cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
