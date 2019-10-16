#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;

#define MX 3000000
#define FOR( A, B, C ) for ( register int A = B, _end_ = C; A <= _end_; A++ )
#define ROF( A, B, C ) for ( register int A = B, _end_ = C; A >= _end_; A-- )
#define ll long long

struct number {
#define MAX 2100
 int num[ MAX + 1 ], fir;
    number( int k = 0 ) { FOR ( i, 0, MAX ) { num[ i ] = 0; } for ( fir = MAX; k; num[ fir-- ] = k & 1, k >>= 1 ); fir++; fir = min( fir, MAX ); }
    int & operator [] ( const int &x ) { return num[ x ]; }
};

int w[ 2 ][ MX + 10 ], tmp[ MX + 10 ], P = 1004535809, K, invK, m, f[ MX + 10 ], invf[ MX + 10 ], E[ MX + 10 ], M0[ MX + 10 ], type, k, x1[ MX + 10 ], l, INP[ MX + 10 ], revf[ MX + 10 ], x2[ MX + 10 ], D[ MX + 10 ], R[ MX + 10 ], invB[ MAX ][ MAX ], B[ MAX ][ MAX ], d[ MAX ][ MAX ], r[ MAX ][ MAX ], tmpf[ MAX ], tmpM0[ MAX ], ttt[ MAX ], rec[ MAX ][ MAX ], TT1[ MAX + 10 ], TT2[ MAX + 10 ];

int fpm( int F, int FF, int P ) {
    int ans = 1;
    for ( ; FF; FF >>= 1, F = (ll)F * F % P )
        if ( FF & 1 ) ans = (ll)ans * F % P;
    return ans;
}

void MOD2( int &x ) { if ( x > 3000000 ) x = ( P - x ) & 1; else x = x & 1; }

inline void inc( int &x, int dat ) { x += dat; if ( x >= P ) x -= P; }

inline void dec( int &x, int dat ) { x -= dat; if ( x < 0 ) x += P; }

void FFT( int X[], int k, int v ) {
    for ( register int i = 0, j = 0; i < k; i++ ) {
        if ( i < j ) swap( X[ i ], X[ j ] );
        for ( register int l = k >> 1; ( j ^= l ) < l; l >>= 1 );
    }
    for ( register int i = 2; i <= k; i <<= 1 )
        for ( register int j = 0; j < k; j += i )
            for ( register int l = 0; l < i >> 1; l++ ) {
                int q = j + l + ( i >> 1 );
                int t = (ll)X[ q ] * w[ v ][ K / i * l ] % P;
                X[ q ] = X[ j + l ]; dec( X[ q ], t );
                inc( X[ j + l ], t );
            }
}

void GetInv( int A1[], int A[], int t ) {
    if ( t == 1 ) { A1[ 0 ] = fpm( A[ 0 ], P - 2, P ); return; }
    GetInv( A1, A, ( t + 1 ) >> 1 );
    int K = 1; for ( ; K < t + 3; K <<= 1 ); int invK = fpm( K, P - 2, P );
    FFT( A1, K, 0 );
    FOR ( i, 0, K - 1 ) { A1[ i ] = (ll)A1[ i ] * A1[ i ] % P; }
    FFT( A1, K, 1 );
    FOR ( i, 0, t - 1 ) { A1[ i ] = (ll)A1[ i ] * invK % P; MOD2( A1[ i ] ); } FOR ( i, t, K - 1 ) { A1[ i ] = 0; }
    K = 1; for ( ; K < 2 * t + 3; K <<= 1 ); invK = fpm( K, P - 2, P );
    memcpy( tmp, A, t << 2 ); memset( tmp + t, 0, ( K - t ) << 2 );
    FFT( A1, K, 0 ); FFT( tmp, K, 0 );
    FOR ( i, 0, K - 1 ) { A1[ i ] = (ll)A1[ i ] * tmp[ i ] % P; }
    FFT( A1, K, 1 );
    FOR ( i, 0, t - 1 ) { A1[ i ] = (ll)A1[ i ] * invK % P; MOD2( A1[ i ] ); } memset( A1 + t, 0, ( K - t ) << 2 );
}

void Init() {
    for ( K = 1; K < 2 * m; K <<= 1 ); invK = fpm( K, P - 2, P );
    w[ 0 ][ 0 ] = 1; int G = fpm( 3, ( P - 1 ) / K, P );
    FOR ( i, 1, K ) { w[ 0 ][ i ] = (ll)w[ 0 ][ i - 1 ] * G % P; }
    FOR ( i, 0, K ) { w[ 1 ][ i ] = w[ 0 ][ K - i ]; }
    
    FOR ( i, 0, m ) { revf[ i ] = f[ m - i ]; }
    GetInv( invf, revf, m + 1 );
    FOR ( i, 0, min( m, MAX - 10 ) ) { tmpf[ i ] = f[ i ]; }
    FFT( f, K, 0 ); FFT( invf, K, 0 );
}

void Div( int X[], int D[], int R[], int inv[], int B[], int N, int M, int v ) {
    FOR ( i, 0, N - M ) { D[ i ] = X[ N - i ]; } FOR ( i, N - M + 1, K - 1 ) { D[ i ] = 0; }
    int K; if ( v == 0 ) for ( K = 1; K < N + 1; K <<= 1 ); else for ( K = 1; K < 2 * N; K <<= 1 );
    int invK = fpm( K, P - 2, P );
    FFT( D, K, 0 );
    FOR ( i, 0, K - 1 ) { D[ i ] = (ll)D[ i ] * inv[ i ] % P; }
    FFT( D, K, 1 );
    FOR ( i, 0, N - M ) { D[ i ] = (ll)D[ i ] * invK % P; } FOR ( i, N - M + 1, K - 1 ) { D[ i ] = 0; }
    FOR ( i, 0, ( N - M ) / 2 ) { swap( D[ i ], D[ N - M - i ] ); }
    FOR ( i, 0, N - M ) { MOD2( D[ i ] ); tmp[ i ] = D[ i ]; } FOR ( i, N - M + 1, K - 1 ) { tmp[ i ] = 0; }
    FFT( tmp, K, 0 );
    FOR ( i, 0, K - 1 ) { tmp[ i ] = (ll)tmp[ i ] * B[ i ] % P; }
    FFT( tmp, K, 1 );
    FOR ( i, 0, M - 1 ) { tmp[ i ] = (ll)tmp[ i ] * invK % P; } FOR ( i, M, K - 1 ) { tmp[ i ] = 0; }
    FOR ( i, 0, M - 1 ) { R[ i ] = ( X[ i ] - tmp[ i ] ) & 1; } FOR ( i, M, K - 1 ) { R[ i ] = 0; }
}

void Mul( int X[], int Y[], int k = 0 ) {
    if ( !k ) {
        FOR ( i, 0, K - 1 ) { tmp[ i ] = Y[ i ]; }
        FFT( X, K, 0 ); FFT( tmp, K, 0 );
        FOR ( i, 0, K - 1 ) { X[ i ] = (ll)X[ i ] * tmp[ i ] % P; }
        FFT ( X, K, 1 );
        FOR ( i, 0, m + m - 1 ) { X[ i ] = (ll)X[ i ] * invK % P; MOD2( X[ i ] ); }
    }
    else {
        FOR ( i, 0, k - 1 ) { X[ i ] = 0; }
        FOR ( i, 0, m - 1 ) { X[ k + i ] = Y[ i ]; }
    }
    Div( X, D, R, invf, f, m + m - 1, m, 0 );
    FOR ( i, 0, K - 1 ) { X[ i ] = R[ i ]; }
}

void exgcd( int v, int a[], int b[], int x[], int y[] ) {
    int lasa = m; for ( ; lasa >= 0  &&  !a[ lasa ]; lasa-- );
    int lasb = m; for ( ; lasb >= 0  &&  !b[ lasb ]; lasb-- );
    if ( lasb < 0 ) { x[ 0 ] = 1; return; }
    if ( lasa < lasb ) { exgcd( v + 1, b, a, y, x ); return; } int cnt;
    for ( cnt = 0; !b[ cnt ]; rec[ v ][ cnt ] = a[ cnt ], cnt++ ); cnt--;
    FOR ( i, 0, lasa - cnt - 1 ) { a[ i ] = a[ i + cnt + 1 ]; } FOR ( i, lasa - cnt, lasa ) { a[ i ] = 0; } lasa = lasa - cnt - 1;
    FOR ( i, 0, lasb - cnt - 1 ) { b[ i ] = b[ i + cnt + 1 ]; } FOR ( i, lasb - cnt, lasb ) { b[ i ] = 0; } lasb = lasb - cnt - 1;
    FOR ( i, 0, lasb / 2 ) { swap( b[ i ], b[ lasb - i ] ); }
    GetInv( invB[ v ], b, lasa - lasb + 1 );
    FOR ( i, 0, lasb / 2 ) { swap( b[ i ], b[ lasb - i ] ); }
    FOR ( i, 0, lasb ) { ttt[ i ] = b[ i ]; }
    int K; for ( K = 1; K < 2 * lasa; K <<= 1 ); FOR ( i, lasb + 1, K ) { ttt[ i ] = 0; } int invK = fpm( K, P - 2, P );
    FFT( ttt, K, 0 ); FFT( invB[ v ], K, 0 );
    Div( a, d[ v ], r[ v ], invB[ v ], ttt, lasa, lasb, 1 );
    ROF ( i, lasb - 1, 0 ) { r[ v ][ i + cnt + 1 ] = r[ v ][ i ]; }
    FOR ( i, 0, cnt ) { r[ v ][ i ] = rec[ v ][ i ]; }
    ROF ( i, lasa + cnt + 1, cnt + 1 ) { a[ i ] = a[ i - cnt - 1 ]; } FOR ( i, 0, cnt ) { a[ i ] = rec[ v ][ i ]; } lasa = lasa + cnt + 1;   
    ROF ( i, lasb + cnt + 1, cnt + 1 ) { b[ i ] = b[ i - cnt - 1 ]; } FOR ( i, 0, cnt ) { b[ i ] = 0; } lasb = lasb + cnt + 1;
    exgcd( v + 1, b, r[ v ], y, x );
    for ( K = 1; K < lasa + cnt + 2; K <<= 1 ); invK = fpm( K, P - 2, P );
    FOR ( i, 0, K ) { tmp[ i ] = x[ i ]; }
    FFT( d[ v ], K, 0 ); FFT( tmp, K, 0 );
    FOR ( i, 0, K - 1 ) { tmp[ i ] = (ll)d[ v ][ i ] * tmp[ i ] % P; }
    FFT( tmp, K, 1 );
    FOR ( i, 0, K ) { tmp[ i ] = (ll)tmp[ i ] * invK % P; MOD2( tmp[ i ] ); }
    FOR ( i, 0, K ) { y[ i ] -= tmp[ i ]; MOD2( y[ i ] ); }
}

void fpm( int X[], number &e ) {
    FOR ( i, 0, m ) { X[ i ] = E[ i ] = 0; }
    int flag = 1, gt = 0;
    ROF ( i, MAX, e.fir ) {
        if ( !flag ) {
            if ( e[ i ] ) Mul( X, E );
            if ( i != e.fir ) Mul( E, E );
        }
        if ( flag ) {
            gt += e[ i ] * ( 1 << ( MAX - i ) );
            if ( ( 1 << ( MAX - i + 1 ) ) >= m ) {
                flag = 0; X[ gt ] = 1; E[ 1 << ( MAX - i ) ] = 1;
                if ( i != e.fir ) Mul( E, E );
            }
        }
    }
}

void fpm_2( int X[], number &e ) {
    FOR ( i, 0, m ) { E[ i ] = X[ i ]; X[ i ] = 0; } X[ 0 ] = 1;
    ROF ( i, MAX, e.fir ) {
        if ( e[ i ] ) Mul( X, E );
        if ( i != e.fir ) Mul( E, E );
    }
}
