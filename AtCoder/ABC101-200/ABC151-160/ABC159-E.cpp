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

// ABC159 https://atcoder.jp/contests/abc159

/*
 * bit全探索で解く。
 * 
 * H=10と小さいため、bit全探索を用いて横方向の切り方全てを調べることができる。
 * 横方向の各切り方について、Kを超えないように縦方向を切っていく。
 * 各列が何番目のグループになるかは、切り方のbitを見てidx[]で管理した。
 * 
 * 横の切り方によっては、縦方向を1列ずつ切っても制約Kを満たさないケースがある。
 * その場合のチェックが必要。
 * 
 * 計算量はO(2^H*HW).
 */

int H, W, K;
vector<vector<int>> s;

// divで示されたカット方法で行を切るときに、列カット操作の最小回数を返す
// 1列単位でカットしても白チョコがK個overになるときは、INFを返す
int cut(int div)
{
	int i, j, k;

	vector<int> idx(H);  // idx[i]:行単位で見て、i行目が何番目のブロックになるか
	int cnt = 0;
	for(i = 0; i < H; i++)
	{
		idx[i] = cnt;
		if((div>>i)&1) cnt++;  // i,i+1行目の間でカットされる
	}
	int idx_max = cnt;

	vector<int> wht(H);
	cnt = 0;
	for(j = 0; j < W; j++)
	{
		vector<int> cur(H);
		for(i = 0; i < H; i++)
		{
			if(s[i][j])
			{
				// 白チョコなら、今見ている区分を+1
				// curだけでKを超えるならINFを返す
				if(++cur[idx[i]] > K) return INF32;
			}
		}
		// curの手前で列を切る必要があるか判定
		bool over = false;
		for(k = 0; k <= idx_max; k++)
		{
			if(wht[k] + cur[k] > K) over = true;  // "curの前までの列"+cur にてKを超えるので、カットする必要がある
		}

		if(over)  // curの手前で列を切る
		{
			cnt++;
			wht = cur;
		}
		else
		{
			for(k = 0; k <= idx_max; k++)
			{
				wht[k] += cur[k];
			}
		}
	}
	return cnt;
}

int main(void)
{
	int i, j;
	cin >> H >> W >> K;
	s.resize(H, vector<int>(W, 0));
	for(i = 0; i < H; i++)
	{
		string s_; cin >> s_;
		for(j = 0; j < W; j++)
		{
			if(s_[j] == '1') s[i][j] = 1;
		}
	}

	int ans = INF32;
	for(i = 0; i < (1<<(H-1)); i++)  // 行のカット方法
	{
		int retu_cut, gyou_cut;
		retu_cut = cut(i);
		gyou_cut = __builtin_popcount(i);
		ans = min(ans, retu_cut+gyou_cut);
	}
	cout << ans << endl;

	return 0;
}
