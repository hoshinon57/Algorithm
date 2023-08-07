#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC182 https://atcoder.jp/contests/abc182

/*
 * マスごとに「このマスが照らされるか否か」を判定していく。
 * 
 * まず、「i行目にあるオブジェクト(電球orブロック)のx座標」をobj_gyouにて管理する。
 * (2,3),(2,8),(2,11)にオブジェクトがある場合、obj_gyou[2] = {3,8,11}となる。
 * obj_retuも同じように管理する。
 * 
 * 上記は電球とブロックの区別をしていないので、obj[y][x]にてオブジェクトの種別を管理する。
 * 値が1なら電球、2はブロックとする。
 * 
 * マス(i,j)について上下左右をそれぞれ見ていったときに、最初に見つかるオブジェクトをlower_bound()で探索する。
 * いずれかにライトがあれば照らされていると判定できる。
 * (マス外に番兵としてブロックを入れておくと実装が楽)
 * 
 * 計算量はO(HW*log(N+M))かO(HW*log(max(H,W))) とか、か。
 * 実行時間は410ms程度で十分に間に合ったが、
 * obj[y][x]をmapで管理した場合、2500msとギリギリACだった。
 * (ジャッジシステムの仕組みにより、お情けでACになったかもしれない…笑)
 */

int obj[1505][1505];  // 1:電球,2:ブロック

int main(void)
{
	// 1-indexed
	int i, j;
	int H, W, N, M; cin >> H >> W >> N >> M;
	vector<vector<int>> obj_gyou(H+5);  // [i]:i行目にあるオブジェクト(電球orブロック)のx座標
	vector<vector<int>> obj_retu(W+5);  // [i]:i列目にあるオブジェクト(電球orブロック)のy座標
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		obj_gyou[a].push_back(b);
		obj_retu[b].push_back(a);
		obj[a][b] = 1;
	}
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		obj_gyou[a].push_back(b);
		obj_retu[b].push_back(a);
		obj[a][b] = 2;
	}
	for(i = 1; i <= H; i++)  // 番兵を入れる
	{
		obj_gyou[i].push_back(0);
		obj_gyou[i].push_back(W+1);
		obj[i][0] = obj[i][W+1] = 2;
		sort(obj_gyou[i].begin(), obj_gyou[i].end());
		
	}
	for(i = 1; i <= W; i++)  // 番兵を入れる
	{
		obj_retu[i].push_back(0);
		obj_retu[i].push_back(H+1);
		obj[0][i] = obj[H+1][i] = 2;
		sort(obj_retu[i].begin(), obj_retu[i].end());
	}

	ll ans = 0;
	for(i = 1; i <= H; i++)
	{
		for(j = 1; j <= W; j++)
		{
			if(obj[i][j] == 2) continue;
			int o1, o2, o3, o4;
			auto itr = lower_bound(obj_gyou[i].begin(), obj_gyou[i].end(), j);
			o1 = obj[i][*itr];
			itr--;
			o2 = obj[i][*itr];

			itr = lower_bound(obj_retu[j].begin(), obj_retu[j].end(), i);
			o3 = obj[*itr][j];
			itr--;
			o4 = obj[*itr][j];

			if(o1 == 1 || o2 == 1 || o3 == 1 || o4 == 1) ans++;  // 上下左右のいずれかに電球があれば照らされている
		}
	}
	cout << ans << endl;

	return 0;
}
