#include <iostream>
#include <vector>
#include <algorithm>

// ダブリングのメモ
// 周期性を用いた考え方についても記載する

/*
 * [参考資料]
 * https://satanic0258.hatenablog.com/entry/2017/02/23/222647
 * https://algo-logic.info/doubling/
 * 
 * [関連する問題]
 * ABC136-D
 * ABC167-D
 * ABC179-E  これ自力ACできなかったのは反省
 * ABC241-E
 * ABC258-E  難問だった
 * ABC293-E
 * ARC060-E
 */

// ダブリングの基本コード
// https://satanic0258.hatenablog.com/entry/2017/02/23/222647
// をほぼそのまま持ってきている
void doubling(void)
{
// 疑似コードのためコメント内に記述する
/*
	ll N;  // 要素数
	const int LOG_K = xx;  // Kの最大が10^18なら <2^60 なので60など
	vector<vector<ll>> db(LOG_K+1, vector<ll>(N));

	// db[0][*]を計算
	for(i = 0; i < N; i++)
	{
		db[0][i] = (iの次の要素);
	}
	// db[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			if(db[k-1][i] == -1)
			{
				// 2^(k-1)個先に要素が無い場合、2^k個先にも無し
				db[k][i] = -1;
			}
			else
			{
				// iの2^k個先は、
				// "iの2^(k-1)個先" の要素からさらに "2^(k-1)個先" の要素となる
				db[k][i] = db[k-1][db[k-1][i]];
			}
		}
	}
	// ここまでが事前計算

	// クエリ
	ll Q, p;  // pからQ個先の要素を求める
	p = 0;
	for(i = 0; i <= LOG_K; i++)
	{
		if(p == -1) break;  // 存在しない要素を指している

		// Qのビットが立っている箇所について計算していく
		if((Q&1) == 1)
		{
			p = db[i][p];
		}
		Q >>= 1;
	}
*/
}

// 周期性を用いた考え方
// 参考：https://algo-logic.info/abc167d/
// ABC167-Dの入力を前提としたコード (https://atcoder.jp/contests/abc167/tasks/abc167_d)
void solution_loop(void)
{
	using ll = long long;
	using namespace std;

	// 0-indexed
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i]--;
	}

	// まずは先頭から1つずつ探索していき、ループを見つける
	// path[]には要素番号を入れていくのが良さそう
	// ->例えば、追加するアメの数(ABC241-E)や箱に入れるじゃがいもの数(ABC258-E)では、うまく行かない気がする
	vector<bool> used(N, false);  // 探索済みか
	vector<int> path;  // スタートから辿る町を順に格納する
	int cur = 0;  // 町1スタート
	while(!used[cur])
	{
		path.push_back(cur);
		// 探索済みにして次の町へ
		used[cur] = true;
		cur = a[cur];
	}
	// curにはループの最初の町が入っている
	// 0 -> 1 -> 2 -> 3 -> 4 -> (2) という経路であれば、
	// path[]には0～4までの5要素が、curには2が入っている

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();  // ループに入るまでに辿る町の数
	ll loop_num = path.size() - bef_loop;  // ループの周期
	ll answer;
	if(K < bef_loop)  // ループに入る前
	{
		answer = path[K];
	}
	else
	{
		// ループに入る前の余分な要素を引いて、それを周期で割った余り -> ループ中の何番目か
		// それとbef_loopを加算すれば、何番目の要素かが分かる (<N)
		ll tmp = ((K - bef_loop) % loop_num) + bef_loop;
		answer = path[tmp];
	}
	cout << answer+1 << endl;  // 1-indexedに戻すために+1
}
