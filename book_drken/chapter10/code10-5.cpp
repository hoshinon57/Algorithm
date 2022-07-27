#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code10.5の写経

struct Heap
{
	vector<int> heap;
	Heap() {}

	void push(int x)
	{
		heap.push_back(x);
		int index = heap.size()-1;  // 挿入された要素の番号
		while(index > 0)
		{
			int p = (index-1)/2;
			if(heap[p] >= x) break;  // 親>=子なのでヒープ成立
			// 子の部分に親の値を入れ、要素番号は親のものに変更
			heap[index] = heap[p];
			index = p;
		}
		heap[index] = x;
	}

	int top(void)
	{
		if(heap.empty())
		{
			cout << "error top():heap size is zero." << endl;
			return -1;
		}
		return heap[0];
	}

	void pop(void)
	{
		if(heap.empty())
		{
			cout << "error pop():heap size is zero." << endl;
			return;
		}
		int x = heap[heap.size()-1];  // 末尾の値
		heap.pop_back();
		int index = 0;
		while(true)
		{
			if(heap.size() <= index*2+1) break;  // 子がいないので終了

			int c1 = index*2+1;
			int c2 = index*2+2;
			// c1と比較するよう調整する
			if(heap.size() < c2+1)  // 右側の子(c2)がいない場合、左側の子(c1)と比較
			{
				;
			}
			else
			{
				if(heap[c2] > heap[c1]) c1 = c2;
			}

			if(x < heap[c1])  // 子の方が大きい場合
			{
				// 親は子の値に書き換え、要素番号は子にする
				heap[index] = heap[c1];
				index = c1;
			}
			else break;  // 親の方が大きいので終了
		}
		heap[index] = x;
	}
};

int main(void)
{
	Heap heap;
	
	heap.push(1);
	cout << heap.top() << endl;
	heap.push(3);
	cout << heap.top() << endl;
	heap.push(5);
	cout << heap.top() << endl;
	heap.push(4);  // {1,3,4,5}
	cout << heap.top() << endl;

	heap.pop();  // {1,3,4}
	cout << heap.top() << endl;
	heap.pop();  // {1,3}
	cout << heap.top() << endl;
	heap.push(10);  // {1,3,10}
	cout << heap.top() << endl;
	heap.pop();  // {1,3}
	cout << heap.top() << endl;
	heap.pop();  // {1}
	cout << heap.top() << endl;
	heap.pop();  // {}
	cout << heap.top() << endl;  // error

	return 0;
}
