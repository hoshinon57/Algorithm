#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC240 https://atcoder.jp/contests/abc240

/*
 * Aがどういうグラフになるかを考えてみると、
 *   C>0のとき、下に凸の二次関数 の一部
 *   C<0のとき、上に凸の二次関数 の一部
 * となる。
 * ※"一部"なので、凸部分が出てこないケースもある点に注意。
 * physics0523氏の解説の通り、Cが加速度、Bが速度とみなすことができる。
 * 
 * ではAの最大値はどこが候補になるかを考えると、各x,yについて、凸の上下は無視して
 *   初項 および 末項
 * が候補になると分かる。
 * また上に凸のとき、凸部分も候補になる。これはBが+から-に切り替わる点となる。
 * 以上の3候補をx,yごとに計算し、最大値が答。
 * 
 * 実装では各x,yごとに「BおよびAのno番目を返す」関数を実装している。
 * 
 * ※最初、初項は「前回の末項」を用いることで計算を省いており(ロジックとしてはこれでもACできる)、
 *   入力全体の初項が最大値となるケースにてWAとなっていた。
 */

void solve(void)
{
	int i;
	ll N, M; cin >> N >> M;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];

	ll ans = -INF64;
	ll bpre = 0, apre = 0;
	// x,yのidx個目について、no番目のBの値を返す (no:1-indexed)
	auto cal_b = [&](int idx, ll no) -> ll
	{
		return bpre + x[idx]*no;
	};
	auto cal_a = [&](int idx, ll no) -> ll
	{
		// 初項がB[1], 末項がB[no]の等差数列の和
		ll tmp_1 = cal_b(idx, 1);
		ll tmp_no = cal_b(idx, no);
		return apre + (tmp_1 + tmp_no) * no / 2;
	};
	for(i = 0; i < N; i++)
	{
		// 先頭と末尾が候補
		chmax(ans, cal_a(i, 1));
		chmax(ans, cal_a(i, y[i]));

		// C<0のとき、Bが+から-に切り替わる点も候補
		if(x[i] < 0)
		{
			int pmidx = bpre / (-x[i]);  // pmidx番目が+の最後
			if(0 < pmidx && pmidx <= y[i])  // 切り替わりの点が含まれているか
			{
				chmax(ans, cal_a(i, pmidx));
			}
		}

		// 末項を次のために保存 Aから計算必要
		apre = cal_a(i, y[i]);
		bpre = cal_b(i, y[i]);
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
