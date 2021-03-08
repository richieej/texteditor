#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	// TODO
	m_curCol = 0;
	m_curRow = 0;
	m_rows = 0;
	m_Editor.push_back("");
	m_line = m_Editor.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	// TODO
	for (auto it = m_Editor.begin();it != m_Editor.end();)
	{
		it = m_Editor.erase(it);
	}
}

bool StudentTextEditor::load(std::string file) {
	ifstream infile(file);
	if (!infile)
	{
		return false;
	}

	reset();
	string s;
	while (getline(infile, s))
	{
		if (s.size() > 1 && s[s.size() - 2] == '\r')
		{
			s.erase(s.size() - 2);
		}
		m_Editor.push_back(s);
		m_rows++;
	}
	m_line = m_Editor.begin();
	return true;
	
}

bool StudentTextEditor::save(std::string file) {
	//return false;  // TODO
	ofstream outfile(file);
	if (!outfile)
		return false;
	for (auto it = m_Editor.begin();it != m_Editor.end();it++)
	{
		outfile << *it << endl;
	}
	return true;
}

void StudentTextEditor::reset() {
	// TODO
	//m_line = m_Editor.end();
	//for (auto it = m_Editor.begin();it != m_Editor.end();it++)
	//{
		//it = m_Editor.erase(it);
	//}
	m_Editor.clear();
	//m_Editor.push_back("");
	m_line = m_Editor.begin();
	m_curCol = 0;
	m_curRow = 0;
	m_rows = 0;
}

void StudentTextEditor::move(Dir dir) {
	// TODO
	if (dir == Dir::UP)
	{
		if (m_curRow > 0)
		{
			m_curRow--;
			m_line--;
		}
		return;
	}
	if (dir == Dir::DOWN)
	{
		if (m_curRow < m_rows)
		{
			m_curRow++;
			m_line++;
		}
		return;
	}
	if (dir == Dir::LEFT)
	{
		if (m_curCol == 0 && m_curRow == 0)
			return;
		if (m_curCol == 0 && m_curRow > 0)
		{
			m_curRow--;
			m_line--;
			m_curCol = m_line->size() - 1;
		}
		if (m_curCol > 0)
			m_curCol--;
		return;
	}
	if (dir == Dir::HOME)
	{
		m_curCol = 0;
		return;
	}
	if (dir == Dir::END)
	{
		m_curCol = m_line->size();
		return;
	}
	if (dir == Dir::RIGHT)
	{
		if (m_curRow == m_rows && m_curCol == m_Editor.rbegin()->size() - 1)
			return;
		if (m_line->size() - 1 == m_curCol)
		{
			m_curCol = 0;
			m_curRow++;
			m_line++;
			return;
		}
		if (m_curCol < m_line->size() && m_curRow <= m_rows)
		{
			m_curCol++;
		}
	}

}

void StudentTextEditor::del() {
	// TODO
	//list<string>::iterator it = m_Editor.begin();
	//advance(it, m_curRow);
	if (m_curRow == m_rows && m_curCol == m_line->size() - 1)
		return;
	if (m_curCol == m_line->size() - 1)
	{
		getUndo()->submit(Undo::Action::JOIN, m_curRow, m_curCol, '0');
		m_line++;
		string line = *m_line;
		m_line = m_Editor.erase(m_line);
		m_line--;
		(*m_line) += line;
	}
	char ch = (*m_line)[m_curCol + 1];
	(*m_line).erase(m_line->begin() + m_curCol + 1);
	getUndo()->submit(Undo::Action::DELETE, m_curRow, m_curCol, ch);

}

void StudentTextEditor::backspace() {
	// TODO
	if (m_curCol == 0 && m_curRow == 0)
	{
		return;
	}
	if (m_curCol == 0 && m_curRow > 0)
	{
		string line = *m_line;
		m_line = m_Editor.erase(m_line);
		m_line--;
		m_curRow--;
		m_curCol = m_line->size();
		(*m_line) += line;
		m_rows--;
		getUndo()->submit(Undo::Action::JOIN, m_curRow, m_curCol, '0');
		return;
	}
	//list<string>::iterator it = m_Editor.begin();
	//advance(it, m_curRow);
	char ch = (*m_line)[m_curCol - 1];
	(*m_line).erase(m_line->begin() + m_curCol - 1);
	m_curCol--;
	getUndo()->submit(Undo::Action::DELETE, m_curRow, m_curCol, ch);
}

void StudentTextEditor::insert(char ch) {
	// TODO
	if (ch == '\t')
	{
		for (int i = 0;i < 4;i++)
			insert(' ');
		return;
	}

	//it->insert(m_curCol, string(1,ch));
	//getUndo()->submit(Undo::Action::INSERT, m_curRow, m_curCol, ch);
	m_line->insert(m_line->begin()+m_curCol, ch);
	m_curCol++;
	getUndo()->submit(Undo::Action::INSERT, m_curRow, m_curCol, ch);
}

void StudentTextEditor::enter() {
	// TODO
	getUndo()->submit(Undo::Action::SPLIT, m_curRow, m_curCol, '0');
	string newline = (*m_line).substr(m_curCol, (*m_line).size() - m_curCol);
	(*m_line) = (*m_line).substr(0, m_curCol);
	m_Editor.insert(++m_line, newline);
	m_curCol = 0;
	m_line--;
	m_curRow++;
	m_rows++;
}

void StudentTextEditor::getPos(int& row, int& col) const {
	// TODO
	row = m_curRow;
	col = m_curCol;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	// TODO
	if (startRow < 0 || numRows < 0)
		return -1;
	lines.clear();
	list<string>::const_iterator it = m_Editor.cbegin();
	advance(it, startRow);
	//list<string>::const_iterator it= m_line;
	//int steps = m_curRow - startRow;
	//advance(it, steps);
	for (int i = 0;i < numRows && it != m_Editor.cend();i++)
	{
		lines.push_back(*it);
		it++;
	}
	return lines.size();
}

void StudentTextEditor::undo() {
	// TODO
	int count = 0;
	string text = "";
	Undo::Action act = getUndo()->get(m_curRow, m_curCol, count, text);
	m_line = m_Editor.begin();
	advance(m_line, m_curRow);
	if (act == Undo::Action::INSERT)
	{
		(*m_line).insert(m_curCol, text);
	}
	else if (act == Undo::Action::DELETE)
	{
		m_curCol--;
		(*m_line).erase(m_curCol, count);
	}
	else if (act == Undo::Action::SPLIT)
	{
		string newline = (*m_line).substr(m_curCol, (*m_line).size() - m_curCol);
		(*m_line) = (*m_line).substr(0, m_curCol);
		m_Editor.insert(++m_line, newline);
		m_curCol = 0;
		m_line--;
		m_curRow++;
		m_rows++;
	}
	else if (act == Undo::Action::JOIN)
	{
		string line = *m_line;
		m_line = m_Editor.erase(m_line);
		m_line--;
		m_curRow--;
		m_curCol = m_line->size();
		(*m_line) += line;
		m_rows--;
	}
}
