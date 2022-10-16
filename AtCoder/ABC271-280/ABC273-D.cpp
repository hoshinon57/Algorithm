#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

typedef set<pair<int,int>> spii;

// ABC273 https://atcoder.jp/contests/abc273

/*
 * まず、壁のデータの持ち方を考えたときに、
 *   bool wall[H][W]
 * としてマス目ごとに持たせるのは、H,Wが最大10^9のためメモリが足りない。
 * そこで set<pair<int,int>> として壁の集合で管理する。
 * このとき、(1){y,x}の順に登録するものと、(2){x,y}の順に登録するもの、2つのsetを用意する。
 * 
 * すると、現在の自分のマスをy,xとすると、
 * 例えば'R'については(1)の集合にて itr=upper_bound({y,x}) として、
 *   itr->first==y なら右方向に壁がある （よって、進めるのは壁まで）
 *   itr->first!=y なら右方向に壁は無い （よって、入力のlマス進める）
 * となる。
 * 'L'なら upper_bound() した後に itr-- したものと比較する。
 * また'D','U'は(2)の集合を使う。
 * 
 * 上記により、指示に対してO(logN)の計算量にて操作できる。
 * 
 * 補足：
 * ・upper_bound()やitr操作での判定を楽にするために、{0,0}および{H+1, W+1}に番兵を置いた。
 * ・set/pairの組み合わせについては以下を参考にした。
 *   https://note.com/momomo0214/n/n3990fbe5a4ec
 */

int main(void)
{
	int i;
	int H, W, y, x;
	cin >> H >> W >> y >> x;
	int N;
	cin >> N;
	spii s_yx, s_xy;  // s_yx:{y,x}の順に登録(Y方向優先) s_xy:{x,y}の順に登録(X方向優先)
	for(i = 0; i < N; i++)
	{
		int r, c;
		cin >> r >> c;
		s_yx.insert({r, c});  // Y方向が優先される
		s_xy.insert({c, r});  // X方向が優先される
	}
	s_yx.insert({0,0});  // itr--の番兵
	s_yx.insert({H+1, W+1});  // 今いるマスの後方側に壁が無かったとき用の番兵
	s_xy.insert({0,0});
	s_xy.insert({H+1, W+1});

	int Q;
	cin >> Q;
	for(i = 0; i < Q; i++)
	{
		char d;
		int l;
		cin >> d >> l;
		// s_yxには「yが小さい順」「yが同じなら、xが小さい順」に並んでいる
		if(d == 'L')  // s_yx
		{
			// Y方向が同じで、今いるマスより手前側にある壁を探す
			// 番兵{0,0}があるため、itr--しても範囲外アクセスにはならない
			auto itr = s_yx.upper_bound({y,x});
			itr--;
			if(itr->first == y)  // 左方向に壁がある
			{
				x -= min(x-(itr->second+1), l);  // 進めるのは壁まで or lマス分
			}
			else  // 左方向に壁は無い
			{
				x -= l;
			}
			if(x < 1) x = 1;
		}
		if(d == 'R')
		{
			// Y方向が同じで、今いるマスより後ろ側にある壁を探す
			// 番兵{H+1,W+1}があるため、upper_bound()にて範囲外アクセスにはならない(=s_yx.end()との比較は不要)
			auto itr = s_yx.upper_bound({y,x});
//			if(itr != s_yx.end() && itr->first == y)
			if(itr->first == y)  // 右方向に壁がある
			{
				x += min((itr->second-1)-x, l);  // 進めるのは壁まで or lマス分
			}
			else  // 右方向に壁は無い
			{
				x += l;
			}
			if(x > W) x = W;
		}
		// s_xyには「xが小さい順」「xが同じなら、yが小さい順」に並んでいる
		// 以下、U,Dの処理はL,Rとほぼ同じ。s_yxではなくs_xyを使うのと、x,yが逆になるのみ。
		if(d == 'U')  // s_xy
		{
			auto itr = s_xy.upper_bound({x,y});
			itr--;
			if(itr->first == x)  // 上方向に壁がある
			{
				y -= min(y-(itr->second+1), l);  // 進めるのは壁まで or lマス分
			}
			else  // 上方向に壁は無い
			{
				y -= l;
			}
			if(y < 1) y = 1;
		}
		if(d == 'D')
		{
			auto itr = s_xy.upper_bound({x,y});
			if(itr->first == x)  // 下方向に壁がある
			{
				y += min((itr->second-1)-y, l);  // 進めるのは壁まで or lマス分
			}
			else  // 下方向に壁は無い
			{
				y += l;
			}
			if(y > H) y = H;
		}
		cout << y << " " << x << endl;
	}

	return 0;
}
