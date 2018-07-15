/*
 *
 */
#include <cctype>
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>
#define N (0x200)
using namespace std;
enum { BLACK, WHITE, UNDEF };
#define vc(x,y) (0<=(x)&&(x)<m&&0<=(y)&&(y)<n)

int m,n,up[N][N],down[N][N],c[N];
char g[N][N];

struct chessboard {
	pair<int,int> coords;
	int size;
	bool operator < ( const chessboard &a ) const { 
		if ( size == a.size ) 
			return coords < a.coords;
		return size > a.size;
	}
	chessboard() { size= 0; }
	chessboard( int x, int y, int s ) {
		coords= {x,y}, size= s;
	}
};

void occupy( int x, int y, int sz ) {
	for ( int i= x; i <= x+sz-1; ++i )
		for ( int j= y; j <= y+sz-1; g[i][j++]= UNDEF ) ;
}

bool opposites( char x, char y ) {
	if ( x == UNDEF || y == UNDEF )
		return false ;
	return x != y;
}

void calc_dp() {
	int i,j,k;
	for ( i= m-1; i >= 0; --i )
		for ( j= n-1; j >= 0; --j ) {
			if ( g[i][j] == UNDEF ) {
				down[i][j]= 0;
				continue ;
			}
			down[i][j]= 1;
			if ( vc(i,j+1) && vc(i+1,j) ) {
				if ( opposites(g[i][j],g[i+1][j]) && opposites(g[i][j],g[i][j+1]) )
					down[i][j]= max(down[i][j],1+min(down[i+1][j],down[i][j+1]));
			}
		}
	for ( i= 0; i < m; ++i )
		for ( j= 0; j < n; ++j ) {
			if ( g[i][j] == UNDEF ) {
				up[i][j]= 0;
				continue ;
			}
			up[i][j]= 1;
			if ( vc(i-1,j) && vc(i,j-1) && opposites(g[i][j],g[i-1][j]) && opposites(g[i][j],g[i][j-1]) )
				up[i][j]= max(up[i][j],1+min(up[i-1][j],up[i][j-1]));
		}
}

int main() {
	int i,j,k,cs= 0,ts,t,area,ni,nj,sz,ans,l;
	for ( cin >> ts; ts--; ) {
		cin >> m >> n;
		assert( !(n&3) );
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; g[i][j++]= UNDEF ) ;
		for ( i= 0; i < m; ++i ) {
			string tmp;
			cin >> tmp;
			assert( tmp.size() == n/4 );
			for ( j= 0; j < (n/4); ++j ) 
				for ( k= isdigit(tmp[j])?tmp[j]-'0':tmp[j]-'A'+10, t= 0; t < 4; ++t ) 
					g[i][4*j+t]= '0'+((k>>(3-t))&1);
		}
		/*
		for ( i= 0; i < m; ++i ) {
			g[i][n]= '\0';
			puts(g[i]);
		}
		*/
		for ( i= 0; i < m; ++i )
			for ( j= 0; j < n; ++j )
				assert( g[i][j] != UNDEF );
		int iteration= 0;
		for ( memset(c,0,sizeof c), calc_dp(), area= m*n; area; ++iteration ) {
			chessboard candidate;
			for ( i= 0; i < m && !candidate.size; ++i )
				for ( j= 0; j < n && !candidate.size; ++j )
					if ( g[i][j] != UNDEF )
						candidate= chessboard(i,j,1);
			assert( candidate.size ) ;
			for ( i= 0; i < m; ++i )
				for ( j= 0; j < n; ++j ) {
					if ( g[i][j] == UNDEF ) continue ;
					k= down[i][j];
					assert( k >= 1 );
					if ( k == 1 ) continue ;
					if ( k&1 ) {
						for ( sz= ((k+1)>>1), t= sz+1, ni= i+((k+1)>>1), nj= j+((k+1)>>1); vc(ni,nj) && g[ni][nj] == g[i][j] && up[ni][nj] >= t && (ni-i+1) <= k; ++ni, ++nj, ++sz, ++t ) ;
					}
					else {
						for ( sz= (k>>1), t= sz+1, ni= i+(k>>1), nj= j+(k>>1); vc(ni,nj) && g[ni][nj] == g[i][j] && up[ni][nj] >= t && (ni-i+1) <= k; ++ni, ++nj, ++sz, ++t ) ;
					}
					chessboard cnd= chessboard(i,j,sz);
					if ( cnd < candidate )
						candidate= cnd;
				}
			occupy(candidate.coords.first,candidate.coords.second,candidate.size);
			++c[candidate.size], calc_dp(), area-= candidate.size*candidate.size;
		}
		for ( ans= 0, i= 1; i <= m && i <= n; ++i )
			if ( c[i] ) ++ans;
		printf("Case #%d: %d\n",++cs,ans);
		for ( i= min(m,n); i >= 1; --i )
			if ( c[i] )
				printf("%d %d\n",i,c[i]);
	}
	return 0;
}

