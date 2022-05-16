#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC245 https://atcoder.jp/contests/abc245

/*
 * B(x)について大きい次数から解いていく。
 * 以下、x^ の記載を省略して、
 * (1) C(N+M)：A(N)*B(M)
 * (2) C(N+M-1)：A(N-1)*B(M)+A(N)*B(M-1)
 * (3) C(N+M-2)：A(N-2)*B(M)+A(N-1)*B(M-1)+A(N)*B(M-2)
 * となる。
 * (1)にてB(M)はそのまま解ける。
 * (2)にてB(M)が分かっているので、B(M-1)は解ける。
 * (3)にてB(M), B(M-1)が分かっているので、B(M-2)は解ける。
 * 
 * よってCを上の次数から順に、Aを下の次数からループさせて、
 * Aの次数がN以外の場合、Aとの積をCから減算し、
 * Aの次数がNの場合(ループの最後)、減算されたCをAで除算したものがB
 * となる。
 */
int main(void)
{
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<int> A(N+1);
	vector<int> B(M+1);
	vector<int> C(N+M+1);
	for(i = 0; i <= N; i++) cin >> A[i];
	for(i = 0; i <= N+M; i++) cin >> C[i];

	int sum;
	for(i = N+M; i >= N+M-M; i--)  // i:C
	{
		sum = C[i];
		for(j = 0; j <= N; j++)  // j:A
		{
			// C[i], A[j]に対応するのはB[i-j]
			if(i-j > M) continue;  // 対応するB[]が存在しない
			if(j != N)
			{
				sum -= A[j]*B[i-j];
			}
			else
			{
				B[i-j] = sum/A[j];
			}
		}
	}

	for(i = 0; i <= M; i++)
	{
		cout << B[i];
		if(i != M) cout << " ";
	}
	cout << endl;

	return 0;
}
