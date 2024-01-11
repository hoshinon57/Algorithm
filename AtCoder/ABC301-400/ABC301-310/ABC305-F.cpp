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

// ABC305 https://atcoder.jp/contests/abc305

// DFSの気持ちになって解く。
// 実装は1-indexed.

int N, M;
vector<bool> seen;

vector<int> in(void)
{
	int i;
	int k; cin >> k;
	vector<int> list(k);
	for(i = 0; i < k; i++) cin >> list[i];
	return list;
}

void in_ok(void)
{
	string ok; cin >> ok;
}

void out(int v)
{
	cout << v << endl;
}

void dfs(int v)
{
	seen[v] = true;
	if(v == N)  // ゴールに到達
	{
		in_ok();
		exit(0);  // 終わっちゃえ
	}

	auto v_list = in();
	for(auto &e : v_list)
	{
		if(seen[e]) continue;
		out(e);  // eへ移動
		dfs(e);
		out(v);  // vに戻ってきた
		in();  // v_listと同じなので、入力は捨てる
	}
}

int main(void)
{
	cin >> N >> M;
	seen.resize(N+1, false);
	dfs(1);

	return 0;
}
