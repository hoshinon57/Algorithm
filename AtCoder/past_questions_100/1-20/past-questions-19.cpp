#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題19
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2009ho/tasks/joi2009ho_b

/*
 * 二分探索で解く。
 * 
 * 店舗の座標をソートして保持しておく。
 * 宅配先の座標「以上」で最も近い店舗を、lower_bound()で求める。
 * その1つ手前の店舗が、宅配先の座標「未満」で最も近い店舗となる。
 * この2店舗のうち近い方から届ける。
 * 
 * 問題は位置が円状になっていること。原点をまたぐ場合の処理が面倒になる。
 * そのため店舗の座標を3周ぶん保持しておき、
 * 宅配先の座標は「2周目」で計算する。
 * これにより、店舗の数や座標によらず、原点またぎを考慮せずに計算できる。
 * 
 * …としたが、距離dに本店に相当する番兵を置いておき、
 * lower_bound()の結果がbegin以外ならば手前の店舗とも比較する、
 * という実装なら判定も楽だし、1周で済ませることができた。
 * 参考：https://drken1215.hatenablog.com/entry/2020/12/22/011600
 * 
 * 計算量はO((n+m)log(n))となる。
 * ソートの部分がO(n*log(n))であり、
 * 全m件の注文について、1件あたりO(log(n))となる。
 */

int main(void)
{
	// 0-indexed
	int i;
	ll d;
	cin >> d;
	int n, m;  // 店舗の数、注文の数
	cin >> n >> m;
	vector<ll> store;  // 店舗の位置
	store.push_back(0);  // 本店の位置
	store.push_back(d);  // 2周目
	store.push_back(d+d);  // 3周目
	for(i = 0; i < n-1; i++)  // 本店は入力から省かれているためn-1まで
	{
		ll tmp;
		cin >> tmp;
		store.push_back(tmp);
		store.push_back(tmp+d);  // 2周目
		store.push_back(tmp+d+d);  // 3周目
	}
	sort(store.begin(), store.end());

	ll answer = 0;
	for(i = 0; i < m; i++)
	{
		ll k;  // 注文の座標
		cin >> k;
		k += d;  // 2周目で計算する	

		// 注文の位置から右側にあるものと左側にあるもののうち、近い方を選択する
		// （3周していることから、原点をまたぐ処理を省ける）
		auto itr = lower_bound(store.begin(), store.end(), k);
		answer += min(*itr - k, k - *(itr-1));
	}
	cout << answer << endl;

	return 0;
}
