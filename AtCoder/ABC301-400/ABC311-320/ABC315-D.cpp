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
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC315 https://atcoder.jp/contests/abc315

/*
 * 実装問題…だと思う。
 * 基本的には手順通り処理していく。
 * 
 * 1回の操作ごとに、各行/列にあるクッキーの枚数を数えるのは、1操作あたりO(HW)とかかかってしまう。
 * 操作は最大で(H+W)回になりそう(※)なので、全体でO((H+W)HW)とかになりTLEする。
 * (※)1操作あたり行or列のどちらか1ラインのみ消えていくケース
 * 
 * このソースコードでは、
 * ・行ごとに、種類ごとのクッキーの枚数と、1枚以上残っているクッキーの集合を管理。
 *   列も同様。
 * ・行ごと(列ごと)に、残っているクッキーの種類が1種、かつ2枚以上残っていればチェックをつける。
 * ・チェックが付かなかったら終了。
 * ・チェックが付いた行(列)について、
 *   その行の対象クッキーの枚数を0に、また集合から削除する。
 * として実装した。
 */

int main(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<vector<int>> num_gyou(H, vector<int>(26, 0));  // [i][j]:i行目にある種類jの枚数
	vector<set<int>> list_gyou(H);  // [i]:i行目にあるクッキーの種類
	vector<vector<int>> num_retu(W, vector<int>(26, 0));
	vector<set<int>> list_retu(W);
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		for(j = 0; j < W; j++)
		{
			int c = s[j]-'a';
			num_gyou[i][c]++;
			list_gyou[i].insert(c);
			num_retu[j][c]++;
			list_retu[j].insert(c);
		}
	}

	while(true)
	{
		// チェックを付ける
		vector<int> chk_gyou,chk_retu;
		for(i = 0; i < H; i++)
		{
			if(list_gyou[i].size() == 1 && num_gyou[i][*list_gyou[i].begin()] > 1) chk_gyou.push_back(i);
		}
		for(i = 0; i < W; i++)
		{
			if(list_retu[i].size() == 1 && num_retu[i][*list_retu[i].begin()] > 1) chk_retu.push_back(i);
		}
		// チェックが付かなければ終了
		if(chk_gyou.empty() && chk_retu.empty()) break;

		// チェックが付いた行、列について処理
		for(auto &e : chk_gyou)
		{
			int kind = *list_gyou[e].begin();
			num_gyou[e][kind] = 0;
			list_gyou[e].erase(kind);
			for(i = 0; i < W; i++)  // 各列、kindを1つ削除
			{
				if(--num_retu[i][kind] == 0)
				{
					list_retu[i].erase(kind);
				}
			}
		}
		for(auto &e : chk_retu)
		{
			int kind = *list_retu[e].begin();
			num_retu[e][kind] = 0;
			list_retu[e].erase(kind);
			for(i = 0; i < H; i++)  // 各行、kindを1つ削除
			{
				// 後から処理する方は、先に処理した時点でnumが0になっている可能性があるため、判定を追加
				if(num_gyou[i][kind] > 0)
				{
					if(--num_gyou[i][kind] == 0)
					{
						list_gyou[i].erase(kind);
					}
				}
			}
		}
	}

	int ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < 26; j++)
		{
			ans += num_gyou[i][j];
		}
	}
	cout << ans << endl;

	return 0;
}
