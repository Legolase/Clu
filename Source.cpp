#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <windows.h>

struct pt {
	int x, y;

	pt() : x(0), y(0) {}
	pt(const int a, const int b) : x(a), y(b) {}
	pt operator+(const pt& a) const {
		return pt(this->x + a.x, this->y + a.y);
	}
	pt operator-(const pt& a) const {
		return pt(this->x - a.x, this->y - a.y);
	}
	void operator=(const pt& a) {
		this->x = a.x;
		this->y = a.y;
	}
	bool operator==(const pt& a) const {
		return ((this->x == a.x) && (this->y == a.y)) ? true : false;
	}
	bool operator!=(const pt& a) const {
		return ((this->x != a.x) || (this->y != a.y)) ? true : false;
	}
};

const int length = 80, height = 80;
const pt sides[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

int rand(const int& a, const int& b) {
	std::mt19937 gen{ std::random_device()() };
	std::uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}

std::mt19937 gen{ std::random_device()() };
std::uniform_int_distribution<int> dist(0, 99);

void stopp(const std::string& message, const int i) {
	std::cout << message;
	system("pause >nul");
	if (i != 0)
		exit(0);
}

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int distance(const pt& a, const pt& b) { 
	return round(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2))); 
}

bool in_square(const pt& point, const pt& size_square) {
	return ((point.x < size_square.x) && (point.x > -1) && (point.y < size_square.y) && (point.y > -1)) ? true : false;
}

void fill(std::vector<std::vector<std::vector<bool>>>& space, const int& percent) {
	for (int i = 0; i < length; ++i)
		for (int j = 0; j < height; ++j) {
			if (dist(gen) < percent)
				space[i][j][1] = true;
			else
				space[i][j][1] = false;
		}
}

int index_center(const pt& point, const std::vector<pt>& centers) {
	int min_dist = length * height, index = 0;

	for (int i = 0; i < centers.size(); ++i) {
		int temp_dist = distance(point, centers[i]);

		if (temp_dist < min_dist) {
			min_dist = temp_dist;
			index = i;
		}
	}

	return index;
}

void alive_step(std::vector<std::vector<std::vector<bool>>>& space, const pt& point, const std::vector<pt>& centers) {
	int cent_index = index_center(point, centers), index_step = 0;
	const int size_sides = sizeof(sides) / sizeof(sides[0]), real_dist = distance(point, centers[cent_index]);

	bool go = false;

	int min_dist_step = length * height;
	for (int i = 0; i < size_sides; ++i) {
		pt step = point + sides[i];

		if ((in_square(step, pt(length, height))) && (!space[step.x][step.y][1])) {
			int temp_dist = distance(step, centers[cent_index]);

			if ((temp_dist < real_dist) && (temp_dist < min_dist_step)) {
				go = true;
				min_dist_step = temp_dist;
				index_step = i;
			}
		}
	}

	if (go) {
		pt step = point + sides[index_step];
		space[point.x][point.y][1] = false;
		space[step.x][step.y][1] = true;
		space[step.x][step.y][0] = true;
	}
}

void Full_step(std::vector<std::vector<std::vector<bool>>>& space, const std::vector<pt>& centers) {
	for (int i = 0; i < length; ++i)
		for (int j = 0; j < height; ++j) {
			if ((space[i][j][1]) && (!space[i][j][0]))
				alive_step(space, pt(i, j), centers);
		}
}

void _reboot(std::vector<std::vector<std::vector<bool>>>& space) {
	for (int i = 0; i < length; ++i)
		for (int j = 0; j < height; ++j)
			space[i][j][0] = false;
}

void show(const std::vector<std::vector<std::vector<bool>>>& space) {
	std::string res = "";

	for (int j = height - 1; j > -1; --j) {
		for (int i = 0; i < length; ++i) {
			if (space[i][j][1])
				res = res + char(219);
			else
				res = res + char(32);
		}
		res += "|\n";
	}
	
	for (int i = 0; i <= length; ++i)
		res += "=";

	std::cout << res;
}

int main() {
	const int pace = 10;
	int counter = 0;

	std::vector<pt> centers = { pt(rand(0, length - 1), rand(0, height - 1)), pt(rand(0, length - 1), rand(0, height - 1)), pt(rand(0, length - 1), rand(0, height - 1)) };
	std::vector<std::vector<std::vector<bool>>> space(length, std::vector<std::vector<bool>> (height, std::vector<bool> (2, false)));

	fill(space, 10);

	while (true) {
		setcur(0, 0);
		show(space);
		Sleep(30);
		if (++counter > pace) {
			counter = 0;
			centers = { pt(rand(0, length - 1), rand(0, height - 1)), pt(rand(0, length - 1), rand(0, height - 1)) };
		}

		_reboot(space);
		Full_step(space, centers);
	}
}