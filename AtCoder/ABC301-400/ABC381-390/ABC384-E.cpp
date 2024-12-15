#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC384 https://atcoder.jp/contests/abc384

/*
 * 優先度付きキューで解く。
 * 
 * 基本的には公式解説の通りで、
 * ・スライムを吸収するデメリットは無い
 * ・現在吸収できるスライムがいるならば、今でも後でも吸収できる
 * といった観点から、現在接しているスライムのうち、最弱のスライムを吸収していく方針で上手くいく。
 * 
 * 上記は優先度付きキューで {敵の強さ,y座標,x座標} を管理すればよい。
 * 同じマスを2回処理しないようusedを用いているが、このチェックタイミングを
 * ・キューから取り出したときに行う
 * ・キューに積むときに行う
 * どちらでも正しく動作するはず。
 * （ダイクストラ法のように、キューに距離とか高橋君"自身"の強さとか、そういうものを登録しないため）
 * （このソースコードでは後者だが、コンテスト時は前者で実装していた）
 */

int main(void)
{
	ll i, j;
	ll H, W, X; cin >> H >> W >> X;
	ll p, q; cin >> p >> q;  // (p,q)
	p--; q--;
	vector<vector<ll>> s(H, vector<ll>(W));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> s[i][j];
	}

	// array<ll,3> : {強さs, y, x}
	priority_queue<array<ll,3>, vector<array<ll,3>>, greater<array<ll,3>>> que;  // 強さsで昇順
	vector<vector<bool>> used(H, vector<bool>(W, false));  // used[H][W] キューに積み済ならtrue

	auto enq = [&](ll yy, ll xx) -> void  // [yy,xx]の周囲をキューに入れる
	{
		const int DIR = 4;
		int dx[DIR] = {1, -1, 0, 0};
		int dy[DIR] = {0, 0, 1, -1};
		for(int k = 0; k < DIR; k++)
		{
			int ny = yy + dy[k];
			int nx = xx + dx[k];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			if(used[ny][nx]) continue;

			que.push({s[ny][nx], ny, nx});
			used[ny][nx] = true;
		}	
	};

	used[p][q] = true;
	enq(p, q);

	ll ans = s[p][q];
	while(!que.empty())
	{
		auto [ss, y, x] = que.top();
		que.pop();

		if(ss <= (ans-1)/X)  // X倍未満なので、-1してから割る
		{
			ans += ss;
			used[y][x] = true;
			enq(y, x);
		}
	}
	cout << ans << endl;

	return 0;
}
