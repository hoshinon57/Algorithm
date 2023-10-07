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

// ABC211 https://atcoder.jp/contests/abc211

/*
 * 自力で解けず、解説を見た。
 * 深さ優先探索を用いて全探索する。
 * 
 * 現在の盤面を何らかの形(後述)で保持しておき、
 * そこから「赤に塗られているマスを1つ選び、上下左右の1マスを赤く塗る」を全探索する。
 * ※計算量については、入力例3の数値や「たぶんこの方法ぐらいしか無い」というエスパー要素もあるが、
 *   公式解説動画が分かりやすかった。
 *   https://www.youtube.com/playlist?list=PLLeJZg4opYKb5AsMhc2t62lNBHCgb447B
 * 
 * 盤面をどう保持するかは、マス数が最大64であることから、64bit型整数で0/1で表すこととした。
 * それをsetで管理していく。
 * 
 * シフト演算にてullへのキャストを忘れたことで1WA.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・N,Kの制約が小さいので、全探索で考えてみる。
 * ・「塗るマスが連結であること」は
 *   「塗り済みのマスから1マス拡張する、を繰り返す」方法が最適なのかなと。
 */

using ull = unsigned long long;
int N, K;
vector<string> mp;
set<ull> used;  // 探索済みの盤面
int ans;

// 現在の盤面がbiのとき、(x,y)を起点にdfs
void dfs(int y, int x, ull bi)
{
	// 範囲外などチェック
	if(y < 0 || y >= N || x < 0 || x >= N) return;
	if(mp[y][x] == '#') return;

	// (x,y)に色を塗った盤面がチェック済か
	bi |= 1ull<<(y*N+x);
	if(used.count(bi) > 0) return;
	used.insert(bi);

	// Kマス塗ったら完了
	if(__builtin_popcountll(bi) == K)
	{
		ans++;
		return;
	}

	// 塗られているマスを起点に周辺探索
	int i, j, k;
	int di[4] = {1, -1, 0, 0};
	int dj[4] = {0, 0, 1, -1};
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(((bi>>(i*N+j))&1) == 0) continue;  // マスが塗られていない
			for(k = 0; k < 4; k++)
			{
				dfs(i+di[k], j+dj[k], bi);
			}
		}
	}
}

int main(void)
{
	int i, j;
	cin >> N >> K;
	mp.resize(N);
	for(i = 0; i < N; i++) cin >> mp[i];
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dfs(i, j, 0);
		}
	}
	cout << ans << endl;

	return 0;
}
