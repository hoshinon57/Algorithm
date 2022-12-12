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
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	vector<ll> a(N), a_2;  // a_2:最初のM個
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(i < M) a_2.push_back(a[i]);
	}

	multiset<ll> ms1, ms2;  // M個の整数を昇順に並べたときの、先頭K個がms1, 残りM-K個がms2
	ll sum = 0;
	// 1つ目は手作業で構築する
	sort(a_2.begin(), a_2.end());  // 最初のM個を昇順ソート
	for(i = 0; i < K; i++)  // 先頭K個:ms1
	{
		ms1.insert(a_2[i]);
		sum += a_2[i];  // 和の計算対象
	}
	for(i = K; i < M; i++)  // 残りM-K個:ms2
	{
		ms2.insert(a_2[i]);
		// 和の計算からは対象外
	}
	cout << sum;

	// 2個目以降
	for(i = M; i < N; i++)  // a[i-M]を取り除き、a[i]を追加する
	{
		// 取り除くa[i-M]がms1, ms2のどちらにあるか
		if(ms1.find(a[i-M]) != ms1.end())  // ms1側にある
		{
			ms1.erase(ms1.find(a[i-M]));
			sum -= a[i-M];
		}
		else  // ms2側にある
		{
			ms2.erase(ms2.find(a[i-M]));
		}

		// a[i]<ms2.begin()の値 ならばms1へ、そうでなければms2へ、とりあえず入れる
		if(a[i] < *(ms2.begin()))
		{
			ms1.insert(a[i]);
			sum += a[i];
		}
		else
		{
			ms2.insert(a[i]);
		}

		// ms1, ms2の個数調整
		// ms1の個数がK個になるようにする
		if((int)ms1.size() > K)
		{
			// ms1終端をms2へ1つ移動
			auto itr = ms1.end();
			itr--;
			int num = *itr;
			ms1.erase(itr);
			ms2.insert(num);
			sum -= num;
		}
		else if((int)ms1.size() < K)
		{
			// ms2先頭をms1へ1つ移動
			auto itr = ms2.begin();
			int num = *itr;
			ms2.erase(itr);
			ms1.insert(num);
			sum += num;
		}
		// ==K なら処置不要

		cout << " " << sum;
	}
	cout << endl;

	return 0;
}
