#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC203 https://atcoder.jp/contests/abc203

/*
 * 自力で解けず、解説を見た。
 * 二分探索と二次元累積和で解く。
 *   https://blog.hamayanhamayan.com/entry/2021/05/30/225914
 * 
 * 「どの区画も、中央値がX以上か？」という判定問題を考える。
 * Xを大きくするにつれ、 true,true,...,true,false,false,... とどこかで変化する。
 * trueの右端が答となる。
 * 
 * 次に判定問題について考える。
 * どのK*K区画についても、Aの値がX以上となる要素数が (K*K/2)+1 個以上あればよい。
 * このときX以上or未満しか値に意味は無くなるので、Aの値をX以上なら1, 未満なら0に置き換え、
 * 区間の総和を二次元累積和で求めれば高速に判定できる。
 * 
 * よって全てのK*K区画について総和が (K*K/2)+1 以上ならOK, 未満の区画が1つでもあればNG.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「最大値の最小化」は二分探索で解けることが多いが、
 *   「中央値の最小化」も同じ考えで解けることがある。
 * ・「中央値がX以上か？」は、要素がX以上か未満かしか関係ない。
 *   以上/未満を1/0に割り当てることで、X以上の個数を二次元累積和で高速に求めることができる。
 */

template <typename T>
struct PrefixSum_2D
{
public:
	int H, W;
	vector<vector<T>> n;
private:
	vector<vector<T>> sum;  // 累積和

public:
	// H:高さ W:幅
	PrefixSum_2D(int H_, int W_) : H(H_), W(W_)
	{
		Init();
	}
	PrefixSum_2D(void) {}

	void Init(void)
	{
		n.assign(H, vector<T>(W, 0));
		sum.assign(H+1, vector<T>(W+1, 0));
	}

	// 累積和を計算する
	// n[][]へは事前に設定しておくこと
	void Build(void)
	{
		int i, j;
		for(i = 0; i < H; i++)
		{
			for(j = 0; j < W; j++)
			{
				sum[i+1][j+1] = sum[i+1][j] + sum[i][j+1] - sum[i][j] + n[i][j];
			}
		}
	}

	// nについて、[y1,y2) x [x1,x2) の半開区間の累積和を返す
	T Sum(int y1, int y2, int x1, int x2)
	{
		return sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1];
	}
};

int main(void)
{
	int i, j;
	ll N, K; cin >> N >> K;
	vector<vector<ll>> a(N, vector<ll>(N, 0));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}

	auto check = [&](ll m) -> bool
	{
		PrefixSum_2D<ll> sum(N,N);
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(a[i][j] >= m) sum.n[i][j] = 1;
			}
		}
		sum.Build();
		for(i = 0; i < N-K+1; i++)
		{
			for(j = 0; j < N-K+1; j++)
			{
				ll s = sum.Sum(i, i+K, j, j+K);
				if(s < K*K/2+1) return false;
			}
		}
		return true;
	};
	
	ll l = -1, r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	cout << l << endl;

	return 0;
}
