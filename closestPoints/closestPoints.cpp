#include <iostream>
#include <algorithm>
#include<stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

class Point {
public:
	double x;
	double y;
	Point() {
		x = 0;
		y = 0;
	}

	void print_point() {
		cout << '(' << x << ',' << y << ')';
	}

	void operator=(const Point &p) {
		x = p.x;
		y = p.y;
	}

	double distance(Point p) {
		return (p.x - x)*(p.x - x) + (p.y - y)*(p.y - y);
	}

	bool operator <(const Point& p)  //使用自带sort函数时需要用到重载符
	{
		if (x < p.x)
			return true;
		return false;
	}
};

class Points {
	Point *points;            //点集数组
	double closest_distance;  //记录最近距离 
	double closest_distance1;  //暴力算法，用来验证
	int n;                    //数组规模 
	Point *x;                 //按x排序的点集
	Point *y;                 //按y排序的点集 

public:
	Points(int n0) {
		n = n0;
		closest_distance = n;	  //n 相当于无穷值 
		closest_distance1 = closest_distance;
		points = new Point[n];
		x = new Point[n];
		y = new Point[n];
	}

	void init() {  //初始化点集
		srand((unsigned)time(NULL));  //时间种子
		for (int i = 0; i < n; i++) {
			points[i].x = rand() % n + rand() / double(RAND_MAX);
			points[i].y = rand() % n + rand() / double(RAND_MAX);
			x[i] = points[i];     
		}
	}

	void sort_x() {
		sort(x, x + n);
		for (int i = 0; i < n; i++)
		{
			y[i] = x[i];
		}
	}

	double find_closest_distance(int low, int high) {
		//只有一个元素直接返回无穷大 
		if (low == high) {
			return n;
		} 
		//只有两个元素返回两者之间的距离 
		if (low + 1 == high) {
			merge_y(low, high); //元素超过1个，对数组y进行预排序（归并排序）
			return x[low].distance(x[high]);
		}

		double left_min;
		double right_min;
		double closest_distance;
		int mid = (low + high) / 2;
		left_min = find_closest_distance(low, mid);      //递归求左边最小值 
		right_min = find_closest_distance(mid + 1, high);  //递归求右边最小值 
		closest_distance = left_min < right_min ? left_min : right_min;
		merge_y(low, high); //对数组y进行预排序（归并排序）
		closest_distance = merge(low, high, closest_distance);  //合并时找边界最小值并比较 
		return closest_distance;
	}

	void merge_y(int low, int high) {      //对数组y进行归并排序 
		Point *p = new Point[high - low + 1]; //临时数组，归并排序用 
		int mid = (low + high) / 2;
		int index = 0;
		int i = low;
		int j = mid + 1;

		while (i <= mid && j <= high) {
			if (y[i].y < y[j].y) {
				p[index] = y[i];
				index++;
				i++;
			}
			else {
				p[index] = y[j];
				index++;
				j++;
			}
		}
		while (i <= mid) {
			p[index] = y[i];
			index++;
			i++;
		}
		while (j <= high) {
			p[index] = y[j];
			index++;
			j++;
		}
		for (int k = 0; k < index; k++) {  //排好序复制回y数组 
			y[low + k] = p[k];
			//y[low + k].print_point();   //这是递归排序树的输出
		}
		//cout << endl;
		delete p;
	}

	double merge(int low, int high, double closest_distance) { //合并时找边界的最小值
		Point *p = new Point[high-low+1];     //临时数组
		int n = 0;
		int mid = (low + high) / 2;
		for (int i = low; i <= high; i++)  //寻找边界附近的点放在临时数组中
		{
			if (abs(x[mid].x-y[i].x) < closest_distance)
			{
				p[n] = y[i];
				n++;
			}
		}
		for (int i = 0; i < n-1; i++) {
			for (int j = i + 1; j < n && j < i + 7; j++) {
				if (p[i].distance(p[j]) < closest_distance) {
					closest_distance = p[i].distance(p[j]);
				}
			}
		}
		delete p;
		return closest_distance;
	}

	~Points() {
		delete points;
		delete x;
		delete y;
	}
	double brute_force() {
		for (int i = 0; i < n; i++) {
			for (int j = i+1; j < n; j++) {
				if (points[i].distance(points[j]) < closest_distance1)
				{
					closest_distance1 = points[i].distance(points[j]);
				}
			}
		}
		return closest_distance1;
	}
};

int main() {
	int n = 90000;
	double time = 0;
	Points points(n);  //传入的数字是规模 
	cout << "规模为" << n << "次的结果如下" << endl;
	for (int i = 0; i < 5; i++)
	{
		points.init();
		clock_t start = clock();
		//points.sort_x();
		//double closest_distance = points.find_closest_distance(0, n - 1);
		double closest_distance = points.brute_force();
		clock_t end = clock();
		time += (double)(end - start);
		cout << "第" << i + 1 << "次分治法最短距离为" << closest_distance << "   ";
		cout << "第"<<i+1<< "次运行时间为：" << (double)(end - start) << "ms" << endl;
	}
	cout << "平均时间为：" << time / 5 <<"ms"<< endl;
	getchar();
	return 0;
}

/*
心得
1. 通过对数组y预排序降低每一次的递归树的时间复杂度
2. 数组y本身是和x一样的，从最底层开始归并，为了就是在找边界值之前有序
3. C++的对象不同于JAVA，可以使用指针，但是如果没有严密的思路和强大的编辑器，出错很难找bu
   当出现访问异常的时候有以下情况
   ① 数组越界
   ② 野指针
4. 自己定义的类，如果要用到sort函数，需要自己重载 < 。
5. 抽屉原理
6. 利用暴力算法进行验证，使得分治法不容易出错。
7. C++只有生成随机整数，小数可以通过除法实现，范围可以通过取余实现。
8. 递归需要注意以下几种情况
   ① 返回条件
   ② 压栈顺序
   特别是压栈顺序的不同，带来的结果全然不同，甚至导致出错，可以参照遍历树。
9. 结果出现负数, 我考虑了很久，怎么就出现了负数呢，计算机出现负数，当你排除所有可能的时候，只有一个真相
   那就是溢出。当规模为1000000时，我定义n*n为最大值，自然就溢出了，所以以后最大值就是最大值，别老想其他的。
*/