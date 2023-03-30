
// ダブリングのメモ

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