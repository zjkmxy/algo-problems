#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<iostream>
#include<algorithm>
using namespace std;

#define MAXN 100
#define eps 1e-8
const double PI = 3.1415926535897932384626;

inline bool equal(const double a, const double b)
{
	return (fabs(a - b) <= eps);
}

double formal(double ang)
{
	while(ang > PI)
	{
		ang -= 2 * PI;
	}
	while(ang <= -PI)
	{
		ang += 2 * PI;
	}
	return ang;
}

struct DP
{
	double x, y;
};

struct POINT
{
	int x, y;
	double dist;
	double ang;
	int corr;	//对应点
	bool operator<(const POINT& other)const
	{
		if(dist != other.dist)
			return dist < other.dist;
		else
			return ang < other.ang;
	}
};

class SHAPE
{
private:
	int n;
	POINT pnt[MAXN];	//各个顶点

	bool domatch(SHAPE* other, double ang, double k)
	{
		int i, j;

		for(i=0;i<n;i++)
		{
			pnt[i].corr = -1;
			other->pnt[i].corr = -1;
		}
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if(other->pnt[j].corr >= 0)
					continue;
				if(!equal(pnt[i].dist * k, other->pnt[j].dist))
					continue;
				if(!equal(formal(pnt[i].ang + ang), other->pnt[j].ang))
					continue;
				other->pnt[j].corr = i;
				pnt[i].corr = j;
				break;
			}
			if(j >= n)
				return false;
		}
		return true;
	};

public:
	DP grav;			//重心

	SHAPE(int n)
	{
		this->n = n;
		while(n--)
		{
			cin >> pnt[n].x >> pnt[n].y;
		}
	};
	void rela_pos()
	{
		int i;
		double dx, dy;
		grav.x = 0.0;
		grav.y = 0.0;
		for(i=0;i<n;i++)
		{
			grav.x += pnt[i].x;
			grav.y += pnt[i].y;
		}
		grav.x /= n;
		grav.y /= n;
		for(i=0;i<n;i++)
		{
			dx = pnt[i].x - grav.x;
			dy = pnt[i].y - grav.y;
			pnt[i].dist = sqrt(dx * dx + dy * dy);
			pnt[i].ang = formal(atan2(dy, dx));
		}
		sort(pnt, pnt + n);
	};
	bool match(SHAPE* other, double &ang, double &k)
	{
		int i;
		k = other->pnt[0].dist / pnt[0].dist;
		if(!equal(pnt[n-1].dist * k, other->pnt[n-1].dist))
			return false;
		for(i=0;i<n;i++)
		{
			if(!equal(other->pnt[0].dist, other->pnt[i].dist))
				break;
			ang = formal(other->pnt[i].ang - pnt[0].ang);
			if(domatch(other, ang, k))
				return true;
		}
		return false;
	};
	void get_trans(SHAPE* other, double &x, double &y)
	{
		int p = pnt[0].corr;
		x = other->grav.x - grav.x;
		y = other->grav.y - grav.y;
	};
};
typedef SHAPE *PSHAPE;

inline double get_mat(double a11, double a12, double a21, double a22)
{
	return (a11 * a22 - a21 * a12);
}

void merge_trans(double ang, double k, double &dx, double &dy, DP grav)
{
	double p, q, c1, c2, z;

	p = 1 - k * cos(ang);
	q = k * sin(ang);
	c1 = grav.x * p + grav.y * q + dx;
	c2 = grav.y * p - grav.x * q + dy;
	z = get_mat(p, q, -q, p);
	dx = get_mat(c1, q, c2, p) / z;
	if(equal(dx, 0.0))
		dx = +0.0;
	dy = get_mat(p, c1, -q, c2) / z;
	if(equal(dy, 0.0))
		dy = +0.0;
}

PSHAPE sa, sb;
double ang, k;
double dx, dy;
int N;

int main()
{
	cin >> N;
	sa = new SHAPE(N);
	sb = new SHAPE(N);

	sa->rela_pos();
	sb->rela_pos();
	if(sa->match(sb, ang, k))
	{
		printf("Y\n");
		if(equal(k, 1.0) && equal(ang, 0.0))
		{
			//平移
			sa->get_trans(sb, dx, dy);
			printf("T(%.1lf,%.1lf)\n",dx,dy);
		}else{
			//旋转相似
			printf("R(%.1lf)S(%.1lf)",ang*180/PI,k);
			//一种做法是取一对匹配点AA',BB'，连线交于O
			//那么圆OAB,圆OA'B'的O以外的另一个交点就是中心
			//另一种做法是变换的合成
			sa->get_trans(sb, dx, dy);
			merge_trans(ang, k, dx, dy, sa->grav);
			printf("(%.1lf,%.1lf)\n",dx,dy);
		}
	}else{
		printf("N\n");
	}

	delete sa;
	delete sb;
	return 0;
}