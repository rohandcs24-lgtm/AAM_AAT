#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005

static int n, sz;
static int h[MAXN], L[MAXN], R[MAXN], st[MAXN], idx[MAXN];
static int tree[2 * 262144];

static void update(int pos, int val) {
    pos += sz;
    tree[pos] = val;
    for (pos >>= 1; pos >= 1; pos >>= 1) {
        int a = tree[2 * pos], b = tree[2 * pos + 1];
        tree[pos] = a > b ? a : b;
    }
}

/* max over positions [l, r], inclusive */
static int query(int l, int r) {
    int res = 0;
    for (l += sz, r += sz + 1; l < r; l >>= 1, r >>= 1) {
        if (l & 1) { if (tree[l] > res) res = tree[l]; l++; }
        if (r & 1) { --r; if (tree[r] > res) res = tree[r]; }
    }
    return res;
}

static int cmp(const void *x, const void *y) {
    int a = *(const int *)x, b = *(const int *)y;
    return (h[a] > h[b]) - (h[a] < h[b]);
}

int main(void) {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) scanf("%d", &h[i]);

    /* L[i] = nearest index left of i with h >= h[i], or -1 */
    int top = 0;
    for (int i = 0; i < n; i++) {
        while (top > 0 && h[st[top - 1]] < h[i]) top--;
        L[i] = top ? st[top - 1] : -1;
        st[top++] = i;
    }

    /* R[i] = nearest index right of i with h >= h[i], or n */
    top = 0;
    for (int i = n - 1; i >= 0; i--) {
        while (top > 0 && h[st[top - 1]] < h[i]) top--;
        R[i] = top ? st[top - 1] : n;
        st[top++] = i;
    }

    sz = 1;
    while (sz < n) sz <<= 1;

    for (int i = 0; i < n; i++) idx[i] = i;
    qsort(idx, n, sizeof(int), cmp);

    int ans = 0;
    for (int k = 0; k < n; k++) {
        int i = idx[k];
        int lo = L[i] + 1, hi = R[i] - 1;
        int best = (lo <= hi) ? query(lo, hi) : 0;
        int dp = best + 1;
        if (dp > ans) ans = dp;
        update(i, dp);
    }

    printf("%d\n", ans);
    return 0;
}