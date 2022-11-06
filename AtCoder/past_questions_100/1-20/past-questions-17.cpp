#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題17
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_13_A&lang=ja

/*
 * 順列全探索で解く。
 * 
 * 配列a[]を「i行目におくクイーンの列番号」とし、初期値で0,1,2,...,7まで順に設定しておく。
 * これをnext_permutation()で更新していくことで、
 * まずは縦横に矛盾が無い置き方を順に取得できる。
 * 
 * 初期状態で置かれているクイーンがあるので、それとa[]が矛盾していないかの判定が必要。
 * また斜め方向については、
 *   行番号+列番号が同一のクイーンがあればNG (右上、左下方向にクイーンがいる)
 *   行番号-列番号が同一のクイーンがあればNG (右下、左上方向にクイーンがいる)
 * にて判定する。
 * 
 * 以上に矛盾が起きないようなa[]があれば、それが答となる。
 */
int main(void)
{
	int i, j;
	int k;
	cin >> k;
	// def[i]:i行目に初期状態で置かれているクイーンの列番号 置かれていない場合は-1
	vector<int> def(8, -1);
	for(i = 0; i < k; i++)
	{
		int r, c;
		cin >> r >> c;
		def[r] = c;  // r行目には、c列目に初期状態で置かれている
	}

	vector<int> a = {0, 1, 2, 3, 4, 5, 6, 7};  // a[i]:i行目におくクイーンの列番号
	do
	{
		bool ok = true;

		// 初期状態で置かれているクイーンと矛盾が無いか調べる
		for(i = 0; i < 8; i++)
		{
			if(def[i] != -1 && def[i] != a[i]) ok = false;
		}
		if(!ok) continue;

		// 斜め方向に矛盾が無ければtrue, 矛盾ありならfalse
		auto func = [&](void) -> bool
		{
			for(i = 0; i < 8; i++)
			{
				for(j = i+1; j < 8; j++)
				{
					if(i+a[i] == j+a[j]) return false;  // 右上、左下方向にクイーンがいる
					if(i-a[i] == j-a[j]) return false;  // 右下、左上方向にクイーンがいる
				}
			}
			return true;
		};

		// ここまで来たら、縦横は被っていない
		// 斜め方向に被っていないか調べる
		if(func()) break;  // 矛盾無しなら探索終了
	} while(next_permutation(a.begin(), a.end()));

	// 結果出力
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(a[i] == j) cout << 'Q';
			else cout << '.';
		}
		cout << endl;
	}

	return 0;
}
