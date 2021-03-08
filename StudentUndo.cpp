#include "StudentUndo.h"
#include <stack>
#include <string.h>

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	Node s;
	s.m_action = action;
	s.m_row = row;
	s.m_col = col;
	s.ch = ch;
	m_stack.push(s);
	
	// TODO
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {
	if (m_stack.empty())
		return Undo::Action::ERROR;
	Action ac = m_stack.top().m_action;
	Action doing = m_stack.top().m_action;
	if (ac == Undo::Action::INSERT)
		doing = Undo::Action::DELETE;
	else if (ac == Undo::Action::DELETE)
		doing = Undo::Action::INSERT;
	else if (ac == Undo::Action::JOIN)
		doing = Undo::Action::SPLIT;
	else if (ac == Undo::Action::SPLIT)
		doing = Undo::Action::JOIN;
	m_stack.top().m_row = row;
	m_stack.top().m_col = col;
	count = 1;
	text = "";

	if (doing == Undo::Action::DELETE)
	{
		count = 0;
		if (m_stack.top().ch == ' ')
		{
			count = 1;
			row = m_stack.top().m_row;
			col = m_stack.top().m_col;
			m_stack.pop();
			return doing;
		}
		while (!m_stack.empty() && m_stack.top().m_action == Undo::Action::INSERT && m_stack.top().ch != ' ')
		{
			count++;
			row = m_stack.top().m_row;
			col = m_stack.top().m_col;
			m_stack.pop();
		}
	}
	else if (doing == Undo::Action::INSERT)
	{
		while (!m_stack.empty() && m_stack.top().m_action == Undo::Action::DELETE)
		{
			text += m_stack.top().ch;
			m_stack.pop();
		}
	}
	else
	{
		m_stack.pop();
	}

    return doing;  // TODO

}

void StudentUndo::clear() {
	// TODO
}
