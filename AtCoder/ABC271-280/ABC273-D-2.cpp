#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC273 https://atcoder.jp/contests/abc273

/*
 * ABC273-D.cppでは set<pair<int,int>> の形で壁を管理した。
 * 本コードでは、公式解説のように行ごと、列ごとのmapにて壁を管理する。
 * 
 * map<int, set<int>> wall_y; について、
 *   wall_y[i]:上からi行目にある壁について、その列番号の集合
 * と定義する。
 * wall_xも同様。
 * 
 * upper_bound()やitr操作での判定を楽にするため、番兵を置く。
 * 各LRUDの操作はABC273-D.cppと同様のため、コメントは最小限にしている。
 * そちらのコード、コメントも参照。
 */

int main(void)
{
	int i;
	int H, W, y, x;
	cin >> H >> W >> y >> x;
	int N;
	cin >> N;
	// wall_y[i]:上からi行目にある壁について、列の一覧をsetにて管理
	// wall_xは列方向
	map<int, set<int>> wall_y, wall_x;
	for(i = 0; i < N; i++)
	{
		int r, c;
		cin >> r >> c;
		wall_y[r].insert(c);  // 上からr行目にある壁について、列の値をsetで管理
		wall_x[c].insert(r);  // 左からc列目にある壁について、行の値をsetで管理
	}
	// upper_bound()やitr操作での判定を楽にするため、番兵を置く
	for(auto &e : wall_y)
	{
		e.second.insert(0);
		e.second.insert(W+1);
	}
	for(auto &e : wall_x)
	{
		e.second.insert(0);
		e.second.insert(H+1);
	}

	int Q;
	cin >> Q;
	for(i = 0; i < Q; i++)
	{
		char d;
		int l;
		cin >> d >> l;
		if(d == 'L')
		{
			if(wall_y.count(y) > 0)  // y行目に壁がある場合
			{
				// upper_bound()で自分より右にある最初の壁が見つかるので、
				// itr--すると自分より左にある最初の壁になる
				auto itr = wall_y[y].upper_bound(x);
				itr--;
				x -= min(x-((*itr)+1), l);  // 進めるのは壁まで or lマス分
			}
			else  // y行目に壁が無い場合
			{
				x -= l;
			}
			if(x < 1) x = 1;
		}
		if(d == 'R')
		{
			if(wall_y.count(y) > 0)
			{
				// upper_bound()で自分より右にある最初の壁が見つかる
				auto itr = wall_y[y].upper_bound(x);
				x += min( (((*itr)-1)-x), l);
			}
			else
			{
				x += l;
			}
			if(x > W) x = W;
		}
		if(d == 'U')
		{
			if(wall_x.count(x) > 0)
			{
				auto itr = wall_x[x].upper_bound(y);
				itr--;
				y -= min(y-((*itr)+1), l);
			}
			else
			{
				y -= l;
			}
			if(y < 1) y = 1;
		}
		if(d == 'D')
		{
			if(wall_x.count(x) > 0)
			{
				auto itr = wall_x[x].upper_bound(y);
				y += min( (((*itr)-1)-y), l);
			}
			else
			{
				y += l;
			}
			if(y > H) y = H;
		}

		cout << y << " " << x << endl;
	}

	return 0;
}
