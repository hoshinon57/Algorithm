#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC259 https://atcoder.jp/contests/abc259

/*
 * グラフと幅優先探索(BFS)で解く。
 * 
 * 以下の3ステップで解く。
 * (1)全ての円どうしで交差または接するかの判定
 * (2)点s,tが円1～円Nのどの円周上にいるか判定
 * (3)点s->点tへのパスが存在するか、BFSで解く
 * 
 * (1)全ての円どうしで交差または接するかの判定
 * 2つの円i,jを全列挙し、それぞれについて交差判定を行う。
 * 条件はcheck_circle_cross()のコメントを参照。
 * 交差していれば、無向グラフにて頂点i,j同士に辺を貼る。
 * 
 * (2)点s,tが円1～円Nのどの円周上にいるか判定
 * 点S,Tについて、円iの円周上にいればiをそれぞれのリストに追加する。
 * また点SについてはBFSの開始にもなるため、キューに追加する。
 * 
 * (3)点s->点tへのパスが存在するか、BFSで解く
 * これはBFSのテンプレート的な書き方で。
 * 
 * 最後に、(2)で求めた点Tのリストについて、
 * 点sから到達可能なものがあればYes回答となる。
 */

ll sx, sy, tx, ty;
vector<ll> x, y, r;

// 円i,jが交差または接するかの判定
// 交差する条件は、円の中心間の距離をdとして、
//   |r-r'| <= d <= r+r'
// となる。
// 参考：https://examist.jp/mathematics/figure-circle/two-circle/
bool check_circle_cross(ll i, ll j)
{
	// 本問題ではACだったが、誤差の観点でsqrtは使わない方が良い。
	ll dx = x[i] - x[j];
	ll dy = y[i] - y[j];
	long double tmp = sqrtl(dx*dx+dy*dy);
	if((long double)(abs(r[i]-r[j])) <= tmp && tmp <= (long double)(r[i]+r[j]))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 点(x,y)が円i上にいるか判定
// 点と円iの中心の距離を求めて、それが半径rと等しいかで判定する
bool check_dot(ll dot_x, ll dot_y, ll i)
{
	// sqrtを使わず、2乗した状態で判定する
	ll dx = dot_x - x[i];
	ll dy = dot_y - y[i];
	ll tmp1 = dx*dx+dy*dy;
	ll tmp2 = r[i]*r[i];
	if(tmp1 == tmp2)
	{
		return true;
	}
	return false;
}

int main(void)
{
	// 0-indexed
	ll N;
	ll i, j;
	cin >> N;
	cin >> sx >> sy >> tx >> ty;
	x.resize(N);
	y.resize(N);
	r.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i] >> r[i];
	}

	// 円i,jが交差または接するかの全チェック
	vector<vector<ll>> circle_cross_list(N, vector<ll>());
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			if(check_circle_cross(i, j))
			{
				circle_cross_list[i].push_back(j);
				circle_cross_list[j].push_back(i);
			}
		}
	}

	queue<ll> que;
	vector<bool> seen(N, false);  // 各頂点について、点sから到達できればtrue
	// 点s,tが円1～Nのどの円周上にいるか
	vector<ll> s_list, t_list;
	for(i = 0; i < N; i++)
	{
		if(check_dot(sx, sy, i))
		{
			// 点sは円iの円周上にいるので、seenをtrueにしてキューに積む
			s_list.push_back(i);  // s_listは結局使わなかった
			seen[i] = true;
			que.push(i);
		}
		if(check_dot(tx, ty, i))
		{
			t_list.push_back(i);
		}
	}

	// 点s->点tへのパスが存在するか、BFSで解く
	while(!que.empty())
	{
		ll v = que.front();
		que.pop();
		// seen[v] = true;  // ★最初はこのタイミングでseen更新していた
		// 円vと交差している円をキューへ
		for(auto &e : circle_cross_list[v])
		{
			if(seen[e]) continue;  // 済ならスルー
			seen[e] = true;  // ★このタイミングでseen更新しないとTLEだった。キューに積む前に更新するのが大切らしい
			que.push(e);
		}
	}

	// 点tがいる円が、点sから到達可能か
	for(auto &e : t_list)
	{
		if(seen[e])
		{
			cout << "Yes" << endl;
			return 0;
		}
	}
	cout << "No" << endl;

	return 0;
}
