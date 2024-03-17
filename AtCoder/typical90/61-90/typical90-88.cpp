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

// 競プロ典型90問:88 https://atcoder.jp/contests/typical90/tasks/typical90_cj

/*
 * 鳩の巣原理を用いて解く。基本は公式解説の通り。
 *   https://twitter.com/e869120/status/1413629046282280963
 * 
 * 制約より、好きにカードを選んだときの総和は0～8888の範囲で8889通り。
 * よって条件2に違反しないようカードを選んでいくと、8889+1=8890回目には必ず総和が一致するものが登場する。(鳩の巣原理)
 * 
 * DFSを用いて各カードを選ぶ/選ばないの組み合わせを探索していく。
 */

int N, Q;
vector<int> a;
vector<vector<bool>> ng;  // ng[i][j]=trueならカードi,jの組み合わせはNG
vector<vector<int>> card;  // card[sum]:和がsumになるカードの組み合わせ  card[sum] = {0, 1, 5, 7, ...}

void prnt(vector<int> &list)
{
	int i;
	cout << list.size() << endl;
	for(i = 0; i < (int)list.size(); i++)
	{
		cout << list[i]+1;
		if(i != (int)list.size()-1) cout << " ";
	}
	cout << endl;
}

// idx番目のカードについて選ぶ/選ばないを考える
// list:これまでに選んだカードの一覧
// 両者のカードの選び方が見つかればtrueを返す
bool dfs(int idx, vector<int> &list)
{
	if(idx == N)  // 最後まで選んだ
	{
		int sum = 0;
		for(auto &e : list) sum += a[e];
		if(sum == 0) return false;  // 1枚も選んでないのはNG
		if(card[sum].size() > 0)  // 既に同じ和の組み合わせがある
		{
			prnt(card[sum]);
			prnt(list);
			return true;
		}
		else  // この和の組み合わせは1回目
		{
			card[sum] = list;
			return false;
		}
	}

	// カードを選ばない
	if(dfs(idx+1, list)) return true;

	// カードを選ぶ
	auto check = [&](void) -> bool  // idx番目のカードを選んでも大丈夫か
	{
		for(auto &e : list)
		{
			if(ng[idx][e]) return false;
		}
		return true;
	};
	if(check())
	{
		list.push_back(idx);
		if(dfs(idx+1, list)) return true;
		list.pop_back();
	}

	return false;
}

int main(void)
{
	int i;
	cin >> N >> Q;
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	ng.resize(N, vector<bool>(N, false));
	for(i = 0; i < Q; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		ng[x][y] = ng[y][x] = true;  // 制約によりx<yなので逆側は不要っぽいけど
	}
	card.resize(8890);  // 総和は8888まで
	vector<int> list;
	dfs(0, list);

	return 0;
}
