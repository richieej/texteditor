#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct Node {
		/*Node(Action action, int m_row, int m_col, char ch)
		{
			m_action = action;
			m_row = m_row;
			m_col = m_col;
			ch = ch;
		}*/
		Action m_action;
		int m_row;
		int m_col;
		char ch;
	};
	std::stack<Node> m_stack;

};

#endif // STUDENTUNDO_H_
