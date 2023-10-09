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

// ABC170 https://atcoder.jp/contests/abc170

/*
 * シミュレーションで解く。
 * 
 * クエリごとに各幼稚園の最大レートを求めれば正しい答が得られるが、それではTLEする。
 * クエリごとに園児の所属はほぼ変化しないことから、
 * 差分に着目して高速に答を求めることを考える。
 * 
 * 以下の変数を用意する。
 *   idx[i]:幼児iが所属する園
 *   cls[x]:園xに所属する園児の、レート一覧(multiset)
 *   total:園ごとの最大レート  これの最小値が「平等さ」となる
 * 
 * クエリ(c,d)にて、園idx[c]にいた園児cを園dに移すことになる。
 * 以下の方針で更新する。
 * (1)元いた園の更新
 *   cls[idx[c]]の最大値を、totalから削除しておく。(判定の簡易化のため)
 *   cls[idx[c]]から園児cのレートを削除
 *   cls[idx[c]]が空でなければ、最大値をtotalに登録
 * (2)移行先の園の更新
 *   cls[d]が空でなければ、最大値をtotalから削除
 *   cls[d]に園児cのレートを登録
 *   cls[d]の最大値をtotalに登録
 *   idx[c]を更新
 * (3)出力
 *   totalの最小値を出力
 * 
 * [ACまでの思考の流れ]
 * ・シミュレーションっぽいが、クエリごとに愚直にレート計算するとTLEしそう。
 * ・クエリごとに園児の所属はほとんど変化しないので、
 *   差分に絞って計算できないか。
 */

int main(void)
{
	// 0-indexed
	const ll GAR_MAX = 200000;  // 幼稚園数
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> a(N), b(N), idx(N);
	vector<multiset<ll>> cls(GAR_MAX+5);  // cls[i]:幼稚園iのレート一覧
	multiset<ll> total;  // 園ごとの最大レート これの最小値が「平等さ」
	for(i = 0; i < N; i++)
	{
		cin >> a[i] >> b[i];
		b[i]--;
		idx[i] = b[i];
		cls[b[i]].insert(a[i]);
	}
	// 初期設定
	for(i = 0; i < GAR_MAX; i++)
	{
		if(cls[i].empty()) continue;
		total.insert(*cls[i].rbegin());
	}

	while(Q > 0)
	{
		ll c, d; cin >> c >> d;
		c--; d--;

		// 元いた園のレート更新
		auto &cls_c = cls[idx[c]];
		total.erase(total.find(*cls_c.rbegin()));
		cls_c.erase(cls_c.find(a[c]));
		if(!cls_c.empty())
		{
			total.insert(*cls_c.rbegin());  // 最大値をtotalに登録
		}

		// 移行先の園の更新
		if(!cls[d].empty())  // まず、転園後の最大値を削除しておく
		{
			total.erase(total.find(*cls[d].rbegin()));
		}
		cls[d].insert(a[c]);
		total.insert(*cls[d].rbegin());
		idx[c] = d;  // idx更新

		cout << *total.begin() << endl;
		Q--;
	}

	return 0;
}
