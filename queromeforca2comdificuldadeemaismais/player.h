#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <set>
#include <locale>
#include <codecvt>

class Player {
public:
    Player(const std::wstring &name, size_t score) : m_name(name), m_score(score) {}
    std::wstring name() const;
    void setName(const std::wstring &name);
    void add_score(size_t s);
    void set_score(size_t s);
    size_t score() const;
    void add_word(const std::wstring &w);
    bool has_played(const std::wstring &w) const;
    void clear_word_list();
    static void savePlayers(const std::vector<Player>& players, const std::wstring& filename);
    void save(const std::wstring& filename) const;
    static std::vector<Player> loadPlayers(const std::wstring& filename);


private:
  std::wstring m_name;
  size_t m_score;
  std::set<std::wstring> m_played_words;

};

// Função auxiliar para conversão
std::string wstring_to_utf8(const std::wstring& wstr);

#endif // PLAYER_H