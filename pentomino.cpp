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

	NodeColumn*& getColumnRef() {
		return column;
	}

	std::ostream& show(std::ostream&);

    protected:
		NodeBase *up, *down;

	private:
		NodeColumn* column;
        NodeBase *left, *right;
};

class NodeColumn : public NodeBase {
    friend DoublyLinkedList<NodeBase>;
    friend DoublyLinkedList<NodeColumn>;
    friend IncidenceMatrix;
    public:
        NodeColumn(std::string name_) : NodeBase(), name(name_), size(0) {};

		bool isCircular() {
			return (right == this);
		}


		std::string getName() {
			return name;
        }

        int getSize() {
        	return size;
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

		NodeColumn* getColumn() {
			return column;
		}

		NodeColumn*& getColumnRef() {
			return column;
		}

	protected:
		std::string name;
		int size;

	private:
		NodeColumn* column;
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

		NodeColumn* findColumnWithLeastOnes() {
			int min = std::numeric_limits<int>::max();
			NodeColumn* minColumn = nullptr;

			NodeColumn* column = headerRow.firstNode->right;
			while(column != headerRow.firstNode) {
				if(column->column->getSize() < min) minColumn = column;
				column = column->right;
			}
			return minColumn;
		}

		NodeColumn* getHead() {
			return headerRow.firstNode;
		}

		template<typename T>
		void addRow(DoublyLinkedList<T> row) {
			if(!row.firstNode) return;

			T* node = row.firstNode;
			while(node != row.lastNode) {
				node->column->increaseSize();
				auto nodeAbove = node->column->up;
				node->up = nodeAbove;
				node->down = node->column;
				nodeAbove->down = node;
				node->column->up = node;
				node = node->right;
			}
			row.lastNode->column->increaseSize();
			auto nodeAbove = row.lastNode->column->up;
			row.lastNode->up = nodeAbove;
			row.lastNode->down = row.lastNode->column;
			nodeAbove->down = row.lastNode;
			row.lastNode->column->up = row.lastNode;
		}

		void cover(NodeColumn*& column) {
			column->right->left = column->left;
			column->left->right = column->right;
			if(column == headerRow.lastNode) headerRow.lastNode = column->left;

			for(NodeBase* row = column->down; row != column; row = row->down) {
				for(NodeBase* rowTraverse = row->right; rowTraverse != row;
						rowTraverse = rowTraverse->right) {
					rowTraverse->up->down = rowTraverse->down;
					rowTraverse->down->up = rowTraverse->up;
					rowTraverse->column->decreaseSize();
				}
			}
		}

		void uncover(NodeColumn*& column) {
			column->right->left = column;
			column->left->right = column;
			if(column->left == headerRow.lastNode) headerRow.lastNode = column;

			for(NodeBase* row = column->up; row != column; row = row->up) {
				for(NodeBase* rowTraverse = row->left; rowTraverse != row; 
						rowTraverse = rowTraverse->left) {
					rowTraverse->up->down = rowTraverse;
					rowTraverse->down->up = rowTraverse;
					rowTraverse->column->increaseSize();
				}
			}
		}


	private:
		std::map<std::string, Pentomino> shapes;
		std::stack<std::unique_ptr<NodeBase>> stackOfNodes;
		
		void createHeader() {
			shapes = {{"X", X}, {"Z", Z}, {"I", I}, {"T", T}, {"U", U},
					{"V", V}, {"W", W}, {"F", F}, {"L", L}, {"P", P}, {"N", N},
					{"Y", Y}};

			auto uHeadNode = std::unique_ptr<NodeColumn>(new NodeColumn("head"));
			auto headNode = uHeadNode.get();
			stackOfNodes.push(std::move(uHeadNode));

			headNode->column = headNode;
			headNode->up = headNode;
			headNode->down = headNode;
			headerRow.addRowNode(headNode);
			map["head"] = headNode;

			for(int i = 0; i < rectangle.first * rectangle.second; i++) {
				std::string currentName  = std::to_string(i);
				auto uColumnNode = std::unique_ptr<NodeColumn>(new NodeColumn(currentName));
				auto columnNode = uColumnNode.get();
				stackOfNodes.push(std::move(uColumnNode));

				columnNode->column = columnNode;
				columnNode->up = columnNode;
				columnNode->down = columnNode;
				headerRow.addRowNode(columnNode);
				map[currentName] = columnNode;
			}

			for(auto shape : shapes) {
				auto uColumnNode = std::unique_ptr<NodeColumn>(new NodeColumn(shape.first));
				auto columnNode = uColumnNode.get();
				stackOfNodes.push(std::move(uColumnNode));

				columnNode->column = columnNode;
				columnNode->up = columnNode;
				columnNode->down = columnNode;
				headerRow.addRowNode(columnNode);
				map[shape.first] = columnNode;
			}
		}
		std::map<std::string, NodeColumn*> map;
		DoublyLinkedList<NodeColumn> headerRow;
		std::pair<int,int> rectangle;

		void fillIn() {
			for(auto shape : shapes) {
				std::stack<Pentomino> stack;
				generateAllShapes(shape, stack);

				while(!stack.empty()) {
					auto pentomino = stack.top();
					stack.pop();

					int x = 0;
					while(x < rectangle.first) {
						int y = 0;
						while(y < rectangle.second) {
							Pentomino movedPentomino = pentomino.shift({x, y});
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
							DoublyLinkedList<NodeBase> list;
							for_each(shapeSerialised.begin(), shapeSerialised.end(), 
									[&list, this](int& integer) {
								std::string str = std::to_string(integer);
								auto uNode = std::unique_ptr<NodeBase>(new NodeBase);
								auto node = uNode.get();
								stackOfNodes.push(std::move(uNode));

								node->column = map[str];
								list.addRowNode(node);
							});

							auto uNode = std::unique_ptr<NodeBase>(new NodeBase);
							auto node = uNode.get();
							stackOfNodes.push(std::move(uNode));
							node->column = map[shape.first];
							list.addRowNode(node);
							addRow(list);
							// std::cout << list;
						}
						x++;
					}
				}
			}
		}

		void clearUnusedColumns() {
			for(NodeColumn* column = headerRow.firstNode->right; column !=
					headerRow.firstNode; column = column->right) {
				if(column->down == column) cover(column);
			}
		}

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
};

void applyKnuthAlgo(IncidenceMatrix& matrix, std::vector<std::string>& solution, int& counter, int k=0) {
	if(matrix.getHead()->isCircular()) {
		for(auto str : solution) {
			std::cout << str << std::endl;
		}
		std::cout << std::endl;
		counter++;
		return;
	};

	NodeColumn* column(std::move(matrix.findColumnWithLeastOnes()));
	if(column->getSize() == 0) return;
	matrix.cover(column);

	for(NodeBase* row(std::move(column->getDown())); row != column; row = std::move(row->getDown())) {
		std::stringstream buff;
		buff << *row;
		solution[k] = buff.str();
		for(NodeBase* rowTraverse = row->getRight(); rowTraverse != row; rowTraverse = rowTraverse->getRight()) {
			matrix.cover(rowTraverse->getColumnRef());
		}

		applyKnuthAlgo(matrix, solution, counter, k+1);

		for(NodeBase* rowTraverse(std::move(row->getLeft())); rowTraverse != row; rowTraverse = std::move(rowTraverse->getLeft())) {
			matrix.uncover(rowTraverse->getColumnRef());
		}
	}
	matrix.uncover(column);
}

void solve(std::pair<int,int> rectangle) {
    IncidenceMatrix matrix(rectangle);
    int counter = 0;
	std::vector<std::string> solution = std::vector<std::string>(rectangle.first*rectangle.second/5, "");
    applyKnuthAlgo(matrix, solution, counter);
	std::cout << "Found " << counter << " solutions" << std::endl;
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
	solve({6,10});
	return 0;
}
