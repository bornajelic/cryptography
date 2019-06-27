#include <iostream>
#include <list>
#include <unordered_set>

using std::list;
using std::unordered_set;

bool R_solve_maze(list<int> maze[], int start, int finish, unordered_set<int> &visited, list<int>& path) {
	if (start == finish) {
		path.push_front(start);
		return true;
	}
	visited.insert(start);
	list<int>::iterator iter = maze[start].begin();
	while (iter != maze[start].end()) {
		if (visited.count(*iter) == 0) {
			if (R_solve_maze(maze, *iter, finish, visited, path)) {
				path.push_front(start);
				return true;
			}
		}
		iter++;
	}
	return false;
}

list<int> R_solve_maze(list<int> maze[], int start, int finish) {
	unordered_set<int> visited;
	list<int> solution_path;
	R_solve_maze(maze, start, finish, visited, solution_path);
	return solution_path;
}

int main() {

	std::list<int> sample_maze[9];
	sample_maze[0].push_back(1);
	sample_maze[0].push_back(3);

	sample_maze[1].push_back(0);
	sample_maze[1].push_back(2);

	sample_maze[2].push_back(1);

	sample_maze[3].push_back(0);
	sample_maze[3].push_back(4);
	sample_maze[3].push_back(6);

	sample_maze[4].push_back(3);
	sample_maze[4].push_back(5);
	sample_maze[4].push_back(7);

	sample_maze[5].push_back(4);
	sample_maze[6].push_back(3);


	sample_maze[7].push_back(4);
	sample_maze[8].push_back(7);


}