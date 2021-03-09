#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>
#include <map>


const int CHARS = 30;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct Node {
        char cur;
        std::map<char, Node*> links;
        bool isEnd;
        Node(char c, bool e)
        {
            cur = c;
            isEnd = e;
        }
    };

    class Trie
    {
    public:
        Trie() {
            m_root = new Node('\0', false);
        }

        void insert(std::string word) {
            Node* cur = m_root;
            for (int i = 0;i < word.size();i++)
            {
                char c = word[i];
                c = tolower(c);
                auto it = cur->links.find(c);
                if (it == cur->links.end())
                {
                    cur->links[c] = new Node(c, false);
                    cur = cur->links[c];
                }
                else
                    cur = cur->links[c];
            }
            cur->isEnd = true;

        }

        bool search(std::string word) {
            if (word.size() == 0)
                return true;
            Node* cur = m_root;
            for (int i = 0;i < word.size();i++)
            {
                char c = word[i];
                c = tolower(c);
                auto it = cur->links.find(c);
                if (it == cur->links.end())
                {
                    return false;
                }
                else
                {
                    cur = cur->links[c];
                }
            }
            if (cur->isEnd == true)
                return true;
            else
                return false;
        }

        bool startsWith(std::string prefix) {
            Node* cur = m_root;
            for (int i = 0;i < prefix.size();i++)
            {
                char c = prefix[i];
                auto it = cur->links.find(c);
                if (it == cur->links.end())
                {
                    return false;
                }
                else
                {
                    cur = cur->links[c];
                }
            }
            return true;
        }

    private:
        Node* m_root;
    };

    Trie* m_dictionary;

    
};

#endif  // STUDENTSPELLCHECK_H_
