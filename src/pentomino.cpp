#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <cassert>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include "unistd.h"
#include <stack>
#include <limits>
#include <sstream>
#include <ctime>
#include <chrono>

// no luck with switch on strings in C++
constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}


struct Pentomino {
	Pentomino(std::initializer_list<std::initializer_list<int>> lst) {
		for(const auto& l : lst) {
			cs.push_back(std::pair<int, int>(*l.begin(), *(l.begin()+1)));
		}
	}

	explicit Pentomino(const std::vector<std::pair<int, int>>& vector) : cs(vector) {}
	explicit Pentomino(std::vector<std::pair<int, int>>&& vector) : cs(vector) {}

	std::vector<std::pair<int, int>> cs; 
	std::vector<int> serialise(std::pair<int, int> rectangle) {
		std::vector<int> result;
		for(auto pair : cs) {
			result.push_back(pair.first  + rectangle.second * pair.second);
		}
		std::sort(result.begin(), result.end());
		return result;
	}

	Pentomino flip() const {
		std::vector<std::pair<int, int>> list;
		for_each(cs.begin(), cs.end(), [&list](const std::pair<int, int>& pair) {
					list.push_back(std::pair<int, int>(pair.second, pair.first));
				});

		return Pentomino(list);
	}

	Pentomino turn90() const {
		std::vector<std::pair<int, int>> list;
		for_each(cs.begin(), cs.end(), [&list](const std::pair<int, int>& pair) {
					list.push_back(std::pair<int, int>(pair.second, -pair.first));
				});
		return Pentomino(list);
	}

	Pentomino turn180() const {
		std::vector<std::pair<int, int>> list;
		for_each(cs.begin(), cs.end(), [&list](const std::pair<int, int>& pair) {
					list.push_back(std::pair<int, int>(-pair.first, -pair.second));
				});
		return Pentomino(list);
	}

	Pentomino turn270() const {
		std::vector<std::pair<int, int>> list;
		for_each(cs.begin(), cs.end(), [&list](const std::pair<int, int>& pair) {
					list.push_back(std::pair<int, int>(-pair.second, pair.first));
				});
		return Pentomino(list);
	}

	Pentomino shift(std::pair<int, int> pair) {
		std::vector<std::pair<int, int>> list;
		for_each(cs.begin(), cs.end(), [&pair, &list](std::pair<int, int>& in) {
					list.push_back(std::pair<int, int>(in.first + pair.first,
							in.second + pair.second));
				});
		return Pentomino(list);
	}
} 
	X{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
	Z{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}}, 
	I{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, 
	T{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}}, 
	U{{0, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}}, 
	V{{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}}, 
	W{{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}, 
	F{{1, 0}, {2, 0}, {0, 1}, {1, 1}, {1, 2}}, 
	L{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}}, 
	P{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {2, 1}}, 
	N{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}}, 
	Y{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {1, 1}};

namespace pent {
	static std::map<std::string, Pentomino> shapes = 
	{{"X", X}, {"Z", Z}, {"I", I}, {"T", T}, {"U", U}, {"V", V}, {"W", W},
		{"F", F}, {"L", L}, {"P", P}, {"N", N}, {"Y", Y}};

	void generateAllShapes(const std::pair<std::string, Pentomino>& shape, 
		std::stack<Pentomino>& stack) {
		switch(str2int(shape.first.c_str())) {
			case str2int("L"):
				;
			case str2int("N"):
				;
			case str2int("P"):
				;
			case str2int("F"):
				;
			case str2int("Y"):
				stack.push(shape.second.flip());
				stack.push(shape.second.flip().turn90());
				stack.push(shape.second.flip().turn180());
				stack.push(shape.second.flip().turn270());
			case str2int("V"):
				;
			case str2int("W"):
				;
			case str2int("T"):
				;
			case str2int("U"):
				stack.push(shape.second);
				stack.push(shape.second.turn90());
				stack.push(shape.second.turn180());
				stack.push(shape.second.turn270());
			break;

			case str2int("Z"):
				stack.push(shape.second.flip());
				stack.push(shape.second.flip().turn90());
			case str2int("I"):
				stack.push(shape.second.turn90());
			case str2int("X"):
				stack.push(shape.second);
			break;
		}
	}
}


template<typename T>
class DoublyLinkedList;
class IncidenceMatrix;
class NodeColumn;

class NodeBase {
	friend DoublyLinkedList<NodeBase>;
	friend DoublyLinkedList<NodeColumn>;
	friend IncidenceMatrix;
	friend inline std::ostream& operator<<(std::ostream& out,
			NodeBase& node) {
		return node.show(out);
	}

	public:
		NodeBase() {}

	public:
		NodeBase(const NodeBase&) = default;
		NodeBase& operator=(const NodeBase&) = default;

	virtual ~NodeBase() {};

	NodeBase* getDown() {
		return down;
	}

	NodeBase* getRight() {
		return right;
	}

	NodeBase* getLeft() {
		return left;
	}

	NodeColumn* getColumn() {
		return column;
	}

	std::ostream& show(std::ostream&);

	protected:
		NodeColumn* column;
		NodeBase *up, *down;

	private:
		NodeBase *left, *right;
};

class NodeColumn : public NodeBase {
	friend DoublyLinkedList<NodeBase>;
	friend DoublyLinkedList<NodeColumn>;
	friend IncidenceMatrix;
	public:
		NodeColumn() : NodeBase(), size(0) {
			up = this;
			down = this;
			column = this;
		};

		NodeColumn(std::string name) : NodeBase(), name(name), size(0) {
			up = this;
			down = this;
			column = this;
		};

		NodeColumn(const NodeColumn& node) {
			name = node.name;
		}

		NodeColumn& operator=(const NodeColumn& rhs) {
			name = rhs.name;
			return *this;
		}

		bool isCircular() {
			return (right == this);
		}


		std::string getName() {
			return name;
		}

		int getSize() {
			return size;
		}

		int calculateSize() {
			int counter = 0;
			NodeBase* start(this);
			NodeBase* runner(this->getDown());
			while(runner != start) {
				runner = runner->getDown();
				counter++;
			}
			size = counter;
			return counter;
		}

		void increaseSize() {
			size++;
		}

		void decreaseSize() {
			assert(size > 0);
			size--;
		}

		NodeColumn* getRight() {
			return right;
		}

		NodeColumn* getLeft() {
			return left;
		}

	protected:
		std::string name;
		int size;

	private:
		NodeColumn* left, *right;
};

std::ostream& NodeBase::show(std::ostream& out) {
	for(NodeBase* p = this; p != this->left; p = p->right) {
		out << p->getColumn()->getName() << " ";
	}
	out << this->left->getColumn()->getName() << " ";
	return out;
}


template<typename T>
class DoublyLinkedList {
	friend IncidenceMatrix;
	friend inline std::ostream& operator<<(std::ostream& out, const
			DoublyLinkedList& list) {
		return list.show(out);
	}
	
	public:
		DoublyLinkedList() : firstNode(nullptr), lastNode(nullptr) {}
		T* addRowNode(T* node) {
			if(lastNode) {
				lastNode->right = node;
				node->left = lastNode;
				lastNode = lastNode->right;
			} else {
				firstNode = lastNode = node;
			}
			firstNode->left = lastNode;
			lastNode->right = firstNode;
			return lastNode;
		}
	private:
		T* firstNode; 
		T* lastNode; 

		std::ostream& show(std::ostream& out) const {
			auto node = firstNode;
			if(!firstNode) return out;

			while(node != lastNode) {
				out << node->column->getName() << " ";
				node = node->right;
			}
			return out << lastNode->column->getName() << std::endl;
		}
};

class IncidenceMatrix {
	public:
		IncidenceMatrix(std::pair<int, int> rectangle) : rectangle(rectangle) {
			createHeader();
			fillIn();
			clearUnusedColumns();
		}

		IncidenceMatrix() = default;

		void cacheVertically(const IncidenceMatrix& matrix) {
			rectangle = matrix.rectangle;
			std::unique_ptr<NodeColumn[]> ptrsArr(new NodeColumn[1]);
			NodeColumn* head = ptrsArr.get();
			stackOfColumnsArr.push(std::move(ptrsArr));
			head->name = "head";
			map["head"] = head;
			headerRow.addRowNode(head);

			for(NodeColumn* column = matrix.getHead()->right; column != matrix.getHead();
					column = column->right) {
				int columnSize = column->getSize();
				std::unique_ptr<NodeColumn[]> ptrsArr(new NodeColumn[columnSize + 1]);
				NodeColumn* columnArr = ptrsArr.get();
				stackOfColumnsArr.push(std::move(ptrsArr));
				*columnArr = *column;

				map[column->name] = columnArr;
				headerRow.addRowNode(map[column->name]);
			}

			int counter = 0;
			for(NodeColumn* column=matrix.getHead()->right; column != matrix.getHead();
					column=column->right) {
				if(pent::shapes.find(column->name) == pent::shapes.end()) continue;
					
				for(NodeBase* firstRowNode = column->down; firstRowNode != column;
						firstRowNode = firstRowNode->down) {
					DoublyLinkedList<NodeBase> nextRow;
					for(auto rowNode = firstRowNode; rowNode != firstRowNode->left;
							rowNode = rowNode->right) {
						assert(map[rowNode->column->name]->getSize() <= 
							matrix.map.at(rowNode->column->name)->getSize()); 

						NodeColumn* columnArr = map[rowNode->column->name];
						NodeBase* rowCopyNode = columnArr + columnArr->getSize() + 1;
						rowCopyNode->column = map[rowNode->column->name];
						nextRow.addRowNode(rowCopyNode);
					}
					NodeColumn* columnArr = map[firstRowNode->left->column->name];
					NodeBase* rowCopyNode = columnArr + columnArr->getSize() + 1;
					rowCopyNode->column = map[firstRowNode->left->column->name];
					nextRow.addRowNode(rowCopyNode);
					counter++;
					addRow(nextRow);
				}
			}
			numOfRows = counter;
		}

		NodeColumn* findColumnWithLeastOnes() {
			int min = std::numeric_limits<int>::max();
			NodeColumn* minColumn = nullptr;

			NodeColumn* column = headerRow.firstNode->right;
			while(column != headerRow.firstNode) {
				column->calculateSize();
				if(column->getSize() < min) minColumn = column;
				column = column->right;
			}
			return minColumn;
		}

		NodeColumn*	getHead() const {
			return headerRow.firstNode;
		}

		template<typename T>
		void addRow(DoublyLinkedList<T>& row) {
			if(!row.firstNode) return;

			T* node = row.firstNode;
			while(node != row.lastNode) {
				addingNodeProcedure(node);
			}
			addingNodeProcedure(row.lastNode);
		}

		void cover(NodeColumn* column) {
			column->right->left = column->left;
			column->left->right = column->right;
			if(column == headerRow.lastNode) headerRow.lastNode = column->left;

			for(NodeBase* row = column->down; row != column; row = row->down) {
				for(NodeBase* rowTraverse = row->right; rowTraverse != row;
						rowTraverse = rowTraverse->right) {
					rowTraverse->up->down = rowTraverse->down;
					rowTraverse->down->up = rowTraverse->up;
				}
			}
		}

		void uncover(NodeColumn* column) {
			column->right->left = column;
			column->left->right = column;
			if(column->left == headerRow.lastNode) headerRow.lastNode = column;

			for(NodeBase* row = column->up; row != column; row = row->up) {
				for(NodeBase* rowTraverse = row->left; rowTraverse != row; 
						rowTraverse = rowTraverse->left) {
					rowTraverse->up->down = rowTraverse;
					rowTraverse->down->up = rowTraverse;
				}
			}
		}


	private:
		std::stack<std::unique_ptr<NodeBase[]>> stackOfNodesArr;
		std::stack<std::unique_ptr<NodeColumn[]>> stackOfColumnsArr;
		
		int findColumnSize(NodeColumn*& column) {
			int result = 0;
			NodeBase* runner(column);
			while(runner != column) {
				result++;
				runner = runner->down;
			}
			return result;
		}

		template<typename T>
		void addingNodeProcedure(T*& node) {
				node->column->increaseSize();
				auto nodeAbove = node->column->up;
				node->up = nodeAbove;
				node->down = node->column;
				nodeAbove->down = node;
				node->column->up = node;
				node = node->right;
		}

		void createHeader() {
			const int headerSize = rectangle.first * rectangle.second + pent::shapes.size() + 1;
			std::unique_ptr<NodeColumn[]> ptrsArr(new NodeColumn[headerSize]);
			int arrIndex = 0;
			NodeColumn* headNotes = ptrsArr.get(); 
			stackOfColumnsArr.push(std::move(ptrsArr));

			NodeColumn* headNode = headNotes + arrIndex++;
			headNode->name = "head";
			headerRow.addRowNode(headNode);
			map["head"] = headNode;

			for(int i = 0; i < rectangle.first * rectangle.second; i++) {
				std::string currentName  = std::to_string(i);
				NodeColumn* columnNode = headNotes + arrIndex++;
				columnNode->name = currentName;
				headerRow.addRowNode(columnNode);
				map[currentName] = columnNode;
			}

			for(auto shape : pent::shapes) {
				NodeColumn* columnNode = headNotes + arrIndex++;
				columnNode->name = shape.first;
				headerRow.addRowNode(columnNode);
				map[shape.first] = columnNode;
			}
		}

		std::map<std::string, NodeColumn*> map;
		DoublyLinkedList<NodeColumn> headerRow;
		std::pair<int,int> rectangle;
		int numOfRows;

		void fillIn() {
			int counter = 0;
			for(auto shape : pent::shapes) {
				std::stack<Pentomino> stack;
				pent::generateAllShapes(shape, stack);

				while(!stack.empty()) {
					auto pentomino = stack.top();
					stack.pop();

					int x = 0;
					while(x < rectangle.first) {
						int y = 0;
						while(y < rectangle.second) {
							// confining X to lower quarter-plane
							if(shape.first == "X") {
								if(1.5 + x > rectangle.first / 2.0) break;
								if(1.5 + y > rectangle.second / 2.0) break;
							}

							Pentomino movedPentomino = pentomino.shift({x, y});
							// serialiased means grid coordinates are represented as numbers
							std::vector<int> shapeSerialised;
							bool isFitted =
								std::accumulate(movedPentomino.cs.begin(),
									movedPentomino.cs.end(), true,
										[this, &shapeSerialised](bool result, std::pair<int, int>& pair) {
										shapeSerialised.push_back(pair.first + pair.second * rectangle.first);

										return result && (pair.first > -1) && (pair.second > -1)
											&& (pair.first < rectangle.first)
											&& (pair.second < rectangle.second);
									});
							y++;
							if(!isFitted) continue;

							std::sort(shapeSerialised.begin(), shapeSerialised.end());
							addSerialisedNode(shapeSerialised, shape.first);
							counter++;
							//std::cout << list;
						}
						x++;
					}
				}
			}
			this->numOfRows = counter;
		}

		void clearUnusedColumns() {
			for(NodeColumn* column = headerRow.firstNode->right; column !=
					headerRow.firstNode; column = column->right) {
				if(column->down == column) cover(column);
			}
		}

		void addSerialisedNode(std::vector<int>& shapeSerialised, const
				std::string& shapeName) {
			DoublyLinkedList<NodeBase> list;
			std::unique_ptr<NodeBase[]> uArr(new NodeBase[6]); 
			NodeBase* arr = uArr.get();
			stackOfNodesArr.push(std::move(uArr));
			int arrIndex = 0;
			for_each(shapeSerialised.begin(), shapeSerialised.end(), 
					[&list, this, &arrIndex, &arr](int& integer) {
				std::string str = std::to_string(integer);
				auto node = (arr + arrIndex++);

				node->column = map[str];
				list.addRowNode(node);
			});
			auto node = (arr + arrIndex);
			node->column = map[shapeName];
			list.addRowNode(node);
			addRow(list);
		}

		bool wasInsertedFirst(NodeColumn* first, NodeColumn* second) {
			std::string firstName = first->column->name;

			auto secondName = second->column->name;
			bool isFirstShape = pent::shapes.find(firstName) == pent::shapes.end();
			bool isSecondShape = pent::shapes.find(secondName) == pent::shapes.end();
			if(isFirstShape && isSecondShape) {
				return firstName < secondName;
			} else if (!isFirstShape && !isSecondShape) {
				return std::atoi(firstName.c_str()) <
					std::atoi(secondName.c_str());
			} else if (!isFirstShape && isSecondShape) {
				return true;
			} else {
				return false;
			}
		}
};

void applyKnuthAlgo(IncidenceMatrix& matrix, std::vector<NodeBase*>& solution,
		int& counter, bool show = true, int k=0) {
	if(matrix.getHead()->isCircular()) {
		if(show) {
			for(auto row : solution) {
				std::cout << *row << std::endl;
			}
			std::cout << std::endl;
		}
		counter++;
		return;
	};

	NodeColumn* column(matrix.getHead()->getRight());

	matrix.cover(column);

	for(NodeBase* row(column->getDown()); row != column; row = row->getDown()) {
		solution[k] = row;
		for(NodeBase* rowTraverse = row->getRight(); rowTraverse != row; rowTraverse = rowTraverse->getRight()) {
			matrix.cover(rowTraverse->getColumn());
		}

		applyKnuthAlgo(matrix, solution, counter, show, k+1);

		for(NodeBase* rowTraverse(row->getLeft()); rowTraverse != row; rowTraverse = rowTraverse->getLeft()) {
			matrix.uncover(rowTraverse->getColumn());
		}
	}
	matrix.uncover(column);
}

void solve(std::pair<int,int> rectangle) {
	std::chrono::time_point<std::chrono::system_clock> start, matrixGenFinished, end;
	start = std::chrono::system_clock::now();
	IncidenceMatrix matrix(rectangle);
	matrixGenFinished = std::chrono::system_clock::now();

	int counter = 0;
	auto solution = std::vector<NodeBase*>(rectangle.first*rectangle.second/5, nullptr);
	applyKnuthAlgo(matrix, solution, counter, true);

	end = std::chrono::system_clock::now();

	std::chrono::duration<double> matrixElapsedSeconds = matrixGenFinished-start;
	std::chrono::duration<double> elapsedSeconds = end-matrixGenFinished;

	std::cout << "Found " << counter << " solutions" << std::endl;
	std::cout << "Matrix generation time: " << matrixElapsedSeconds.count() << " seconds" << std::endl;
	std::cout << "Elapsed time: " << elapsedSeconds.count() << " seconds" << std::endl;
}

void handler(int sig) {
	void *array[10];
	size_t size;
	
	// get void*'s for all entries on the stack
	size = backtrace(array, 10);
	
	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int main(int, char**) {
	signal(SIGSEGV, handler);
	solve({6, 10});
	return 0;
}
