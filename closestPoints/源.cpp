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

	bool operator <(const Point& p)  //ʹ���Դ�sort����ʱ��Ҫ�õ����ط�
	{
		if (x < p.x)
			return true;
		return false;
	}
};

class Points {
	Point *points;            //�㼯����
	double closest_distance;  //��¼������� 
	double closest_distance1;  //�����㷨��������֤
	int n;                    //�����ģ 
	Point *x;                 //��x����ĵ㼯
	Point *y;                 //��y����ĵ㼯 

public:
	Points(int n0) {
		n = n0;
		closest_distance = n * n;	  //n*n �൱������ֵ 
		closest_distance1 = closest_distance;
		points = new Point[n];
		x = new Point[n];
		y = new Point[n];
	}

	void init() {  //��ʼ���㼯
		srand((unsigned)time(NULL));  //ʱ������
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
		//ֻ��һ��Ԫ��ֱ�ӷ�������� 
		if (low == high) {
			return n * n;
		} 
		//ֻ������Ԫ�ط�������֮��ľ��� 
		if (low + 1 == high) {
			merge_y(low, high); //Ԫ�س���1����������y����Ԥ���򣨹鲢����
			return x[low].distance(x[high]);
		}

		double left_min;
		double right_min;
		double closest_distance;
		int mid = (low + high) / 2;
		left_min = find_closest_distance(low, mid);      //�ݹ��������Сֵ 
		right_min = find_closest_distance(mid + 1, high);  //�ݹ����ұ���Сֵ 
		closest_distance = left_min < right_min ? left_min : right_min;
		merge_y(low, high); //������y����Ԥ���򣨹鲢����
		closest_distance = merge(low, high, closest_distance);  //�ϲ�ʱ�ұ߽���Сֵ���Ƚ� 
		return closest_distance;
	}

	void merge_y(int low, int high) {      //������y���й鲢���� 
		Point *p = new Point[high - low + 1]; //��ʱ���飬�鲢������ 
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
		for (int k = 0; k < index; k++) {  //�ź����ƻ�y���� 
			y[low + k] = p[k];
			//y[low + k].print_point();   //���ǵݹ������������
		}
		//cout << endl;
		delete p;
	}

	double merge(int low, int high, double closest_distance) { //�ϲ�ʱ�ұ߽����Сֵ
		Point *p = new Point[high-low+1];     //��ʱ����
		int n = 0;
		int mid = (low + high) / 2;
		for (int i = low; i <= high; i++)  //Ѱ�ұ߽總���ĵ������ʱ������
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
	void brute_force() {
		for (int i = 0; i < n; i++) {
			for (int j = i+1; j < n; j++) {
				if (points[i].distance(points[j]) < closest_distance1)
				{
					closest_distance1 = points[i].distance(points[j]);
				}
			}
		}
		cout << "��������̾���Ϊ��" << closest_distance1 << endl;
	}
};

int main() {
	int n = 1000;
	Points points(n);  //����������ǹ�ģ 
	points.init();
	points.sort_x();
	cout << "���η���̾���Ϊ" << points.find_closest_distance(0,n-1);
	points.brute_force();
	int x;
	cin >> x;
	return 0;
}

/*
�ĵ�
1. ͨ��������yԤ���򽵵�ÿһ�εĵݹ�����ʱ�临�Ӷ�
2. ����y�����Ǻ�xһ���ģ�����ײ㿪ʼ�鲢��Ϊ�˾������ұ߽�ֵ֮ǰ����
3. C++�Ķ���ͬ��JAVA������ʹ��ָ�룬�������û�����ܵ�˼·��ǿ��ı༭�������������bu
   �����ַ����쳣��ʱ�����������
   �� ����Խ��
   �� Ұָ��
4. �Լ�������࣬���Ҫ�õ�sort��������Ҫ�Լ����� < ��
5. ����ԭ��
6. ���ñ����㷨������֤��ʹ�÷��η������׳���
7. C++ֻ���������������С������ͨ������ʵ�֣���Χ����ͨ��ȡ��ʵ�֡�
8. �ݹ���Ҫע�����¼������
   �� ��������
   �� ѹջ˳��
   �ر���ѹջ˳��Ĳ�ͬ�������Ľ��ȫȻ��ͬ���������³������Բ��ձ�������
*/