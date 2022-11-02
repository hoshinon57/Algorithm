#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題27
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2009yo/tasks/joi2009yo_d

/*
 * 深さ優先探索で解く。
 * 
 * 各マスが「薄氷か」「探索済みか」の情報を持つ変数をそれぞれ用意しておく。
 * 
 * 関数dfs(y,x,count)を定義し、以下の処理を行う。
 *   地点y,xを起点として、上下左右へ深さ優先探索を行っていく。
 *   その際、通ったマスを「探索済み」にし、かつ通ったマス数をインクリメントする。
 *   通ったマスは最大値を保持しておく(max_count)。
 *   「探索済み」と通ったマス数は、dfs()の帰りがけ時に戻す。
 * 
 * あとはmain関数より、各マスをスタート地点としてdfs()を呼び出す。
 * するとmax_countに通ったマスの最大数が保持されているので、これが答となる。
 * 
 * 計算量は、
 *   ある地点をスタートとしたときの深さ優先探索にO(mn).
 *   各マスをスタート地点としてループする部分がO(mn).
 * より、O((mn)^2)かな…と思う。
 * ちなみに問題文より「移動方法は20万通りを超えない」とのこと。
 */

using vbb = vector<vector<bool>>;
vbb walk;  // walk[y][x]:薄氷ならtrue
vbb seen;  // seen[y][x]:移動済ならtrue
int m, n;  // m:x, n:y
int max_count = -1;

// 地点y,xを起点として深さ優先探索を行う
// count:これまで割った薄氷の数
void dfs(int y, int x, int count)
{
	// 範囲外、薄氷が無い、または探索済みならスルー
	if(y < 0 || y >= n || x < 0 || x >= m) return;
	if(!walk[y][x]) return;
	if(seen[y][x]) return;

	// 今いる地点を「探索済み」にし、割った薄氷の数をインクリメントする
	seen[y][x] = true;
	count++;
	max_count = max(max_count, count);

	// 上下左右4方向について探索
	int dy[] = {-1, 1,  0, 0};
	int dx[] = { 0, 0, -1, 1};
	for(int i = 0; i < 4; i++)
	{
		dfs(y+dy[i], x+dx[i], count);
	}

	// 戻りがけのタイミングで、countとseenを戻す
	seen[y][x] = false;
	count--;
}

int main(void)
{
	int i, j;
	cin >> m >> n;
	walk.resize(n, vector<bool>(m, false));
	seen.resize(n, vector<bool>(m, false));
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < m; j++)
		{
			int tmp;
			cin >> tmp;
			if(tmp == 1) walk[i][j] = true;
		}
	}

	// 各地点をスタート地点として深さ優先探索を行う
	// seen[]は、関数内の帰りがけ時に元に戻しているため、データを呼び出しごとに初期化する必要はない
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < m; j++)
		{
			dfs(i, j, 0);  // スタート時は薄氷を割った個数は0
		}
	}
	cout << max_count << endl;

	return 0;
}
