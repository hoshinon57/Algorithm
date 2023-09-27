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

// ABC189 https://atcoder.jp/contests/abc189

/*
 * クエリ先読みで解く。
 * 公式解説ではアフィン変換が主だが、自分はその手法は思いつかず。
 * 以下の記事が近い。
 * https://scrapbox.io/procon-kirokuyou/ABC189_E_-_Rotate_and_Flip_(500)
 * 
 * コマの座標(x,y)について、操作ごとの変化を考えると、
 * 操作1：(y, -x)に移動
 * 操作2：(-y, x)に移動
 * となる。
 * よって{kx,ky}={1,2}という変数を用意し、操作1,2ごとにswapや-1倍をしておくと、
 *   値が1：初期x座標を使う
 * 　値が2：初期y座標を使う
 * 　値が負：-1倍する
 * といった判定で移動後の座標が求まる。
 * 
 * また操作3は (2p-x, y) へ移動、操作4は (x, 2p-y) へ移動と考えられるので、
 * {dx,dy}という変数を用意し、これに2pを加算していく。
 * 操作1,2にてdx,dyもswapや-1倍が必要。
 * 
 * クエリを先読みしておき、操作ごとに上記の計算をしながら今答えるべきクエリの計算を行っていけばよい。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}
	ll M; cin >> M;
	vector<pair<ll,ll>> op(M+1);  // op[i] = {op番号, 付随する値}  op番号は1始まり
	for(i = 1; i <= M; i++)
	{
		cin >> op[i].first;
		if(op[i].first >= 3) cin >> op[i].second;
	}
	ll Q; cin >> Q;
	vector<vector<pair<ll,ll>>> qu(M+1);  // qu[i]:{i個目の操作を行った後に求める駒, クエリ番号} の列挙
	for(i = 0; i < Q; i++)  // クエリ先読み
	{
		ll a, b; cin >> a >> b;
		b--;
		qu[a].push_back({b, i});
	}

	ll dx = 0, dy = 0;  // op3,4にて加算される量
	ll kx = 1, ky = 2;  // 原点に対するコマの位置 1,2および正負にて操作でどう変化したかを判断できる
	vector<pair<ll,ll>> ans(Q);  // ans[i]:クエリiに対する回答
	for(i = 0; i <= M; i++)  // 操作i
	{
		// 操作は1回目から
		if(i != 0)
		{
			switch(op[i].first)
			{
			case 1:
				swap(kx, ky);
				ky = -ky;
				swap(dx, dy);
				dy = -dy;
				break;
			case 2:
				swap(kx, ky);
				kx = -kx;
				swap(dx, dy);
				dx = -dx;
				break;
			case 3:  // x:2p-x
				kx = -kx;
				dx = -dx;
				dx += op[i].second * 2;
				break;
			case 4:  // y:2p-y
				ky = -ky;
				dy = -dy;
				dy += op[i].second * 2;
				break;
			}
		}

		// クエリ回答
		for(auto &e : qu[i])
		{
			auto [b, q_idx] = e;
			ll ans_x = 0, ans_y = 0;
			if(abs(kx) == 1)  // xは初期x座標の方を使う
			{
				if(kx > 0) ans_x = x[b];
				else ans_x = -x[b];
				if(ky > 0) ans_y = y[b];
				else ans_y = -y[b];
			}
			else  // xは初期y座標の方を使う
			{
				if(kx > 0) ans_x = y[b];
				else ans_x = -y[b];
				if(ky > 0) ans_y = x[b];
				else ans_y = -x[b];
			}
			ans_x += dx;
			ans_y += dy;
			ans[q_idx]= {ans_x, ans_y};
		}
	}

	for(auto &e : ans)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
