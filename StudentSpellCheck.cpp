#include "StudentSpellCheck.h"
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
	// TODO
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
	m_dictionary = new Trie();
	std::ifstream infile(dictionaryFile);
	if (!infile)
	{
		return false;
	}
	std::string s;
	while (getline(infile, s))
	{
		for (int i = 0;i < s.size();i++)
		{
			//prune the letters.
			if (isupper(s[i]))
				s[i] = tolower(s[i]);
			if (s[i] != '\'' && !isalpha(s[i]))
			{
				s.erase(s.begin() + i);
			}
		}
		m_dictionary->insert(s);
	}
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
	for (int i = 0;i < word.size();i++)
		word[i] = tolower(word[i]);
	if (!m_dictionary->search(word))
	{
		std::string s = word;
		int count_sug = 0;
		suggestions.clear();
		for (int i = 0; i < word.size();i++)
		{
			std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
			for (char k : alphabet)
			{
				s.replace(i, 1, std::string(1, k));
				if (m_dictionary->search(s))
				{
					suggestions.push_back(s);
					count_sug++;
					if (count_sug > max_suggestions)
						break;
				}
			}
		}
	}
	else
		return true;
	// TODO
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
	// 
	std::string word = "";
	problems.clear();
	for (int i = 0;i < line.size();i++)
	{
		if (isalpha(line[i]) || line[i] == '\'')
		{
			word += line[i];
		}
		else
		{
			if (!m_dictionary->search(word))
			{
				Position p;
				p.start = i - word.size();
				p.end = i;
				problems.push_back(p);
			}
			word = "";
		}
		if (i == line.size() - 1)
		{
			if (!m_dictionary->search(word))
			{
				Position p;
				p.start = i - word.size() + 1;
				p.end = i;
				problems.push_back(p);
			}
		}
		/*if (!m_dictionary->search(word))
		{
			Position p;
			p.start = i - word.size();
			p.end = i;
			problems.push_back(p);
		}*/
	}
}


/*
if (line[i] == '\'' && word[word.size() - 1] == '\'')
	{
		if (!m_dictionary->search(word))
		{
			Position p;
			p.start = i - word.size() - 1;
			p.end = i - 1;
			problems.push_back(p);
		}
		//spellCheck(word);
		word = "";
	}
else*/