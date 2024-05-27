/*!
 * Player class implementation.
 *
 * \author Selan
 * \date April 21st, 2022
 * \file player.cpp
 */

#include <cstddef>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "player.h"

// Função auxiliar para conversão
std::string wstring_to_utf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    return convert.to_bytes(wstr);
}

std::wstring Player::name() const { return m_name; }

void Player::setName(const std::wstring &name){m_name = name;}

void Player::add_score(size_t s) { m_score += s; }

void Player::set_score(size_t s){ }

size_t Player::score() const { return m_score; }

void Player::add_word(const std::wstring &w) { m_played_words.insert(w); }

bool Player::has_played(const std::wstring &w) const {
    return m_played_words.count(w) != 0;
}

void Player::clear_word_list() { m_played_words.clear(); }

/*void Player::savePlayers(const std::vector<Player>& players, const std::wstring& filename) {
    std::string utf8_filename = wstring_to_utf8(filename);
    std::wofstream outFile(utf8_filename);
    outFile.imbue(std::locale(outFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (outFile.is_open()) {
        for (const Player& player : players) {
            outFile << player.m_name << L" " << player.m_score << std::endl;
        }
        outFile.close();
        std::wcout << L"Player data saved successfully." << std::endl;
    } else {
        std::wcerr << L"Error opening file for writing." << std::endl;
    }
}

void Player::save(const std::wstring& filename) const {
    std::vector<Player> players = loadPlayers(filename);

    bool playerExists = false;
    for (Player& player : players) {
        if (player.name() == m_name) {
            player = *this; // Atualiza o jogador existente
            playerExists = true;
            break;
        }
    }

    if (!playerExists) {
        players.push_back(*this); // Adiciona um novo jogador
    }
    savePlayers(players, filename);  
}

std::vector<Player> Player::loadPlayers(const std::wstring& filename) {
    std::vector<Player> players;
    std::string utf8_filename = wstring_to_utf8(filename);
    std::wifstream inFile(utf8_filename);
    inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (inFile.is_open()) {
        std::wstring name;
        size_t score;
        while (inFile >> name >> score) {
            players.emplace_back(name, score);
        }
        inFile.close();
        std::wcout << L"Player data loaded successfully." << std::endl;
    } else {
        std::wcerr << L"Error opening file for reading. Creating new file..." << std::endl;
        std::wofstream newFile(utf8_filename);
        newFile.imbue(std::locale(newFile.getloc(), new std::codecvt_utf8<wchar_t>));
        newFile.close();
    }
    return players;
}*/


void Player::savePlayers(const std::vector<Player>& players, const std::wstring& filename) {
    std::string utf8_filename = wstring_to_utf8(filename);
    std::wofstream outFile(utf8_filename);
    outFile.imbue(std::locale(outFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (outFile.is_open()) {
        for (const Player& player : players) {
            // Escapar nomes de jogador que contenham vírgulas ou aspas
            std::wstring escapedName = player.m_name;
            if (escapedName.find(L',') != std::wstring::npos || escapedName.find(L'"') != std::wstring::npos) {
                escapedName = L"\"" + escapedName + L"\"";
            }
            outFile << escapedName << L"," << player.m_score << std::endl;
        }
        outFile.close();
        std::wcout << L"Player data saved successfully." << std::endl;
    } else {
        std::wcerr << L"Error opening file for writing." << std::endl;
    }
}

void Player::save(const std::wstring& filename) const {
    std::vector<Player> players = loadPlayers(filename);

    bool playerExists = false;
    for (Player& player : players) {
        if (player.name() == m_name) {
            player = *this; // Atualiza o jogador existente
            playerExists = true;
            break;
        }
    }

    if (!playerExists) {
        players.push_back(*this); // Adiciona um novo jogador
    }
    savePlayers(players, filename);  
}

std::vector<Player> Player::loadPlayers(const std::wstring& filename) {
    std::vector<Player> players;
    std::string utf8_filename = wstring_to_utf8(filename);
    std::wifstream inFile(utf8_filename);
    inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8<wchar_t>));

    if (inFile.is_open()) {
        std::wstring line;
        while (std::getline(inFile, line)) {
            std::wistringstream ss(line);
            std::wstring name;
            size_t score;
            if (std::getline(ss, name, L',') && ss >> score) {
                // Remover aspas extras dos nomes escapados
                if (!name.empty() && name.front() == L'"' && name.back() == L'"') {
                    name = name.substr(1, name.length() - 2);
                }
                players.emplace_back(name, score);
            }
        }
        inFile.close();
        std::wcout << L"Player data loaded successfully." << std::endl;
    } else {
        std::wcerr << L"Error opening file for reading. Creating new file..." << std::endl;
        std::wofstream newFile(utf8_filename);
        newFile.imbue(std::locale(newFile.getloc(), new std::codecvt_utf8<wchar_t>));
        newFile.close();
    }
    return players;
}