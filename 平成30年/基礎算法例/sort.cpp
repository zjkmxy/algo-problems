// 整列
#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<iostream>
#include<functional>
using namespace std;
using placeholders::_1;
using placeholders::_2;

typedef unsigned long ul;

// 挿入ソート
// 計算量は平均O(N^2)
// ただし、大体に順序ある配列に対する計算量はO(N)
// バブルソートと似てるが各方面より優れている
void InsertSort(ul *st, ul *ed) {
    ul *p, *q;
    for (p = st; p < ed; p++) {
        ul cur = *p;
        // st..p-1 の要素は既に整列されてると仮設し、
        // p番目の要素をその中に挿入する
        for (q = p - 1; q >= st && *q > cur; q--)
            q[1] = *q;
        q[1] = cur;
    }
}

// 基数ソート
// ここでバケットの代わり、計数ソートを基づいて実装した
// 計算量 max(N, 65536)
// N>10^6の場合はかなり早い
void RadixSort(ul arr[], const int N) {
    int i;
    static int cnt[0x10000][2];
    ul *arr2 = new ul[N];
    
    // cnt[x] = 値xの数
    // ここで二つのラウンドを一緒に書いたが、別々書くも良い
    memset(cnt, 0, sizeof(cnt));
    for (i = 0; i < N; i++) {
        //ラウンド１
        cnt[arr[i] & 0xFFFF][0] ++;
        //ラウンド２
        cnt[arr[i] >> 16][1] ++;
    }
    // cnt[x] = 値x以下の数
    for (i = 0; i < 0xFFFF; i++) {
        //ラウンド１
        cnt[i + 1][0] += cnt[i][0];
        //ラウンド２
        cnt[i + 1][1] += cnt[i][1];
    }
    
    //ここで、cnt[x] = 整列した後xの最後の位置+1
    
    //ラウンド１
    for (i = N - 1; i >= 0; i--)
        arr2[--cnt[arr[i] & 0xFFFF][0]] = arr[i];
    //ラウンド２
    for (i = N - 1; i >= 0; i--)
        arr[--cnt[arr2[i] >> 16][1]] = arr2[i];
    
    delete[] arr2;
}

// ヒップソート (heapは最大ヒップ)
// 実装方法は二種類ある
struct Heap {
    // ヒップを作るベース空間arr[1..len]
    ul *arr;
    
    // ヒップのサイズ、空間arrのサイズ
    size_t siz, len;
    
    //左の子
    // 根は１
    static inline size_t Left(size_t x) {
        return x << 1;
    }
    
    //右の子
    static inline size_t Right(size_t x) {
        return (x << 1) | 1;
    }
    
    //親
    static inline size_t Parent(size_t x) {
        return x >> 1;
    }
    
    Heap(ul *base, size_t len) :arr(base - 1), siz(1), len(len) {}
    
    // iを根とした部分木で、i以外全部ヒップ性質を満たす場合
    // 部分木全体が性質を満たすようにする関数
    void MaxHeapify(size_t i) {
        size_t large;
        while (i > 0 || i <= siz) {
            large = i;
            if (Left(i) <= siz && arr[Left(i)] > arr[large])
                large = Left(i);
            if (Right(i) <= siz && arr[Right(i)] > arr[large])
                large = Right(i);
            if (large != i) {
                swap(arr[i], arr[large]);
                i = large;
            }
            else
                break;
        }
    }
    
    // 整列バージョン一
    void HeapSortVer1() {
        // arrを基づいたヒップ建立 O(N)
        siz = len;
        for (size_t i = len / 2; i >= 1; i--)
            MaxHeapify(i);
        
        // 大きい順から整列 O(NlogN)
        for (size_t i = len; i >= 1; i--) {
            swap(arr[i], arr[1]);
            siz--;
            MaxHeapify(1);
        }
    }
    
    inline ul PopMax() {
        swap(arr[1], arr[siz]);
        siz--;
        MaxHeapify(1);
        return arr[siz + 1];
    }
    
    // arr[i]をvalと修正する関数
    // 特にvalはarr[i]より大きい場合
    void Modify(size_t i, ul val) {
        if (val > arr[i]) {
            for (; i > 1 && arr[Parent(i)] < val; i = Parent(i))
                arr[i] = arr[Parent(i)];
            arr[i] = val;
        }
        else if (val < arr[i]) {
            arr[i] = val;
            MaxHeapify(i);
        }
    }
    
    void HeapSortVer2() {
        // 一個づつ空っぽなヒップに挿入 O(N)
        for (siz = 2; siz <= len; siz++) {
            ul cur = arr[siz];
            arr[siz] = 0;
            Modify(siz, cur);
        }
        siz = len;
        
        // 整列の部分は一緒 O(NlogN)
        while (siz > 1)
            PopMax();
    }
};

void HeapSort(ul arr[], size_t N, bool ver1) {
    Heap h(arr, N);
    if (ver1)
        h.HeapSortVer1();
    else
        h.HeapSortVer2();
}

// 快速整列
// いくつかのバージョンがある。こちらのはテキストより少々調整したバージョン。
// DualPivotQuickSortの方は更に早いそうだ。
void QuickSort(ul *st, ul *ed) {
    size_t len = ed - st;
    if (len <= 2){
        if(len == 2 && st[0] > st[1]){
            swap(st[0], st[1]);
        }
        return;
    }
    
    // ピボット(中枢軸)は三つの値から中央値を選ぶとバランスがよい
    ul v1 = st[0], v2 = st[len - 1], v3 = st[len >> 1];
    ul pivot;
    if (v1 > v2) {
        pivot = (v2 > v3 ? v2 : min(v1, v3));
    }
    else {
        pivot = (v1 > v3 ? v1 : min(v2, v3));
    }
    
    // ==pivot, <pivot, >pivot三つの部分に分ける
    ul *p = st, *q = ed - 1, *r = st;
    while (p < q) {
        for (; p < q && *p <= pivot; p++) {
            if (*p == pivot) {
                swap(*p, *r);
                r++;
            }
        }
        for (; p < q && *q > pivot; q--);
        if (p < q)
            swap(*p, *q);
        // もし*q == pivotならば、次のpの反復文で調整する。
    }
    
    // <pivot, ==pivot, >pivotようにする
    if (*p > pivot)
        p--;
    else
        q++;
    for (r--; r >= st; r--) {
        swap(*p, *r);
        p--;
    }
    
    // <pivotと>pivotのグループのみに再帰的に適用
    QuickSort(st, p + 1);
    QuickSort(q, ed);
}

int main() {
    const int N = 20;
    ul arr[20];
    function<void(ul*, size_t)>  procs[] = {
        [](ul arr[], size_t N) {},
        [](ul arr[], size_t N) { InsertSort(arr, arr + N); },
        RadixSort,
        bind(HeapSort, _1, _2, true),
        bind(HeapSort, _1, _2, false),
        [](ul arr[], size_t N) { QuickSort(arr, arr + N); },
    };
    
    for (const auto& func : procs) {
        srand(13);
        for (ul i = 0; i < N; i++) {
            arr[i] = rand() % 100;
        }
        func(arr, N);
        for (ul i = 0; i < N; i++) {
            printf("%lu ", arr[i]);
        }
        printf("\n");
    }
    
    return 0;
}
