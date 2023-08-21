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

// ABC245 https://atcoder.jp/contests/abc245

/*
 * multisetを使う。
 * ※けんちょん氏の解説とほぼ同じ実装だった。
 *   https://drken1215.hatenablog.com/entry/2022/03/30/003800
 * 
 * チョコ、箱ともに縦のサイズで昇順ソートしておく。
 * 箱を先頭から1つ取り、以下の手順を行う。
 *   この箱iの縦以下となるチョコについて、横幅をmultisetに入れる。
 *   箱iの横幅について、それ以下でなるべく大きいチョコを、upper_bound()を用いて先ほどのmultisetから取得する。
 *   取り出したチョコと箱iがペアとなる。
 * 
 * まず縦幅について見ておくことで、
 * 後に横幅をチェックする際は縦のことを考える必要はない、というイメージ。
 * 
 * ペアを作った回数がNであればOK.
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<pair<int,int>> choco(N), box(M);
	for(i = 0; i < N; i++) {cin >> choco[i].first;}
	for(i = 0; i < N; i++) {cin >> choco[i].second;}
	for(i = 0; i < M; i++) {cin >> box[i].first;}
	for(i = 0; i < M; i++) {cin >> box[i].second;}
	// 縦で昇順ソート
	sort(choco.begin(), choco.end());
	sort(box.begin(), box.end());

	int cnt = 0;  // 箱に詰められたチョコの数
	int c_idx = 0;
	multiset<int> choco_b;  // チョコの横幅
	choco_b.insert(-INF32);  // 番兵
	for(i = 0; i < M; i++)  // 箱i
	{
		// 今見ている箱iについて、縦が入るチョコの横幅をmultisetに入れる
		for(j = c_idx; j < N; j++)  // チョコj
		{
			if(choco[j].first <= box[i].first)
			{
				choco_b.insert(choco[j].second);
			}
			else break;
		}
		c_idx = j;

		// 今見ている箱iの横について、そこに入るなるべく大きい横幅のチョコを取得
		auto itr = choco_b.upper_bound(box[i].second);
		itr--;
		if(*itr == -INF32) continue;  // 入るチョコは無い
		choco_b.erase(itr);
		cnt++;
	}

	string ans = "No";
	if(cnt == N) ans = "Yes";
	cout << ans << endl;

	return 0;
}
