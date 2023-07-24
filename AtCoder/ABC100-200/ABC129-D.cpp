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

// ABC129 https://atcoder.jp/contests/abc129

/*
 * まず前処理として、障害物の場所を、行ごと列ごとに管理しておく。 ⇒ wall_retu, wall_gyou
 * この際、マップの枠外の部分に番兵を入れておくと後の処理がやりやすい。
 * 
 * 各マスについて、何マス照らされるかを全探索する。
 * あるマス(i,j)において、
 *   横方向に見て、前後にある障害物をlower_bound()で求めると、照らされるマス数が分かる。
 *   縦方向に見た場合も同様。
 *   これらを加算して-1したものが、マス(i,j)に明かりを置いた場合に照らされるマス数となる。
 *   ※(i,j)部分が2回カウントされるので、-1が必要
 * この最大値が答。
 * 
 * ★最初、wall_retu, wall_gyouをvectorではなくsetで管理したところ、TLEした。
 *   HW=4*10^6なので、logが付くとしても大丈夫かと思っていたがダメだった。イテレータの操作も多かったからかな。
 *   vectorで管理できるならその方が良い、という教訓になった。
 */

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<string> s(H); for(i = 0; i < H; i++) {cin >> s[i];}
	vector<vector<int>> wall_retu(H), wall_gyou(W);  // retu[i]:i行目にある障害物の一覧, gyou[j]:j列目にある障害物の一覧

	// 障害物の一覧を構築
	for(i = 0; i < H; i++)
	{
		wall_retu[i].push_back(-1);  // 番兵
		for(j = 0; j < W; j++)
			if(s[i][j] == '#') wall_retu[i].push_back(j);
		wall_retu[i].push_back(W);  // 番兵
	}
	for(j = 0; j < W; j++)
	{
		wall_gyou[j].push_back(-1);
		for(i = 0; i < H; i++)
			if(s[i][j] == '#') wall_gyou[j].push_back(i);
		wall_gyou[j].push_back(H);
	}

	int ans = 0;
	// マス(i,j)に明かりを置いた場合に照らされるマスを計算
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int idx;
			// 自身の右側にある障害物をlower_bound()で求める
			// idx-1 が左側にある障害物となる
			idx = lower_bound(wall_retu[i].begin(), wall_retu[i].end(), j) - wall_retu[i].begin();
			int n1 = wall_retu[i][idx] - wall_retu[i][idx-1] - 1;

			idx = lower_bound(wall_gyou[j].begin(), wall_gyou[j].end(), i) - wall_gyou[j].begin();
			int n2 = wall_gyou[j][idx] - wall_gyou[j][idx-1] - 1;

			ans = max(ans, n1+n2-1);  // -1は交差した部分
		}
	}
	cout << ans << endl;

	return 0;
}
