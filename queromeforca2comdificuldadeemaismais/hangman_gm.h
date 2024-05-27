#ifndef HANGMAN_GM_H
#define HANGMAN_GM_H

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <filesystem>
#include <map>
#include <sstream>
#include "player.h"

enum class game_state_e { WELCOME, MAIN_MENU, PLAYING, SHOW_RULES, SHOW_SCORE, ENDING, QUITTING };
enum class menu_e { UNDEFINED, PLAY, RULES, SCORE, EXIT, INVALID };
enum class match_e { UNDEFINED, ON, PLAYER_WON, PLAYER_LOST, OFF };

class GameController {
public:
    GameController();
    GameController(const std::wstring& playerName);

    bool initialize(int argc, char *argv[]);
    bool read_user_confirmation();
    void read_enter_to_proceed();
    void welcome() const;
    void menu() const;
    void showRules() const;
    void showScore() const;
    void display_misses(int misses) const;
    void display_status(const std::vector<char>& incorrect, const std::wstring& answer) const;
    void add_points(size_t points);
    void end_game(const std::wstring& answer, const std::wstring& secretWord);
    void load_words(const std::wstring& filename, std::multimap<std::wstring, std::pair<std::wstring, std::wstring>>& wordsbank);
    std::wstring getRandomWord(std::multimap<std::wstring, std::pair<std::wstring, std::wstring>>& wordsbank, const std::wstring& category, const std::wstring& difficulty);
    std::wstring getRandomWord(const std::wstring& category);
    std::wstring hideAnswer(const std::wstring& secretWord);
    std::wstring read_user_name();
    Player read_player_name();
    std::vector<std::wstring> readWordsFromFile(const std::wstring& filename);

    void update();
    void process_events();
    void render() const;
    bool playGame();
    void run();
    bool game_over() const;  // Declaração da função game_over
    void display_hangman(int misses) const;
    std::wstring getRandomWord(int category);

private:
    game_state_e m_gameState;
    menu_e m_menu;
    match_e m_match;
    int misses;
    Player player;
    size_t m_points;

};

#endif // HANGMAN_GM_H



