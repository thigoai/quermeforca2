#include "hangman_gm.h"
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <codecvt>
#include <locale>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <map>
#include <sstream>


GameController::GameController()
    : m_gameState(game_state_e::WELCOME),
      m_menu(menu_e::INVALID),
      m_match(match_e::ON),
      misses(0),
      player(L"Default Player", 0),
      m_points(0) {}

GameController::GameController(const std::wstring& playerName) 
    : m_gameState(game_state_e::WELCOME),
      m_menu(menu_e::INVALID),
      m_match(match_e::ON),
      misses(0),
      player(playerName, 0),
      m_points(0) {}

// Função para exibir uma mensagem de boas-vindas
void GameController::welcome() const {
    std::wcout << L"Bem-vindo ao Jogo da Forca!" << std::endl;
}

// Função para exibir o menu principal
void GameController::menu() const {
    std::wcout << L"\n=======MENU=======" << std::endl;
    std::wcout << L"1. Jogar" << std::endl;
    std::wcout << L"2. Regras" << std::endl;
    std::wcout << L"3. Ver Pontuação" << std::endl;
    std::wcout << L"4. Sair" << std::endl;
}

// Função para exibir as regras do jogo
void GameController::showRules() const {
    std::wcout << L"Regras do Jogo da Forca:" << std::endl;
    std::wcout << L"Tente adivinhar a palavra secreta letra por letra." << std::endl;
    std::wcout << L"Você tem um número limitado de tentativas erradas." << std::endl;
}

// Função para exibir a pontuação
void GameController::showScore() const {
    
    std::wcout << L"Pontuação:" << std::endl;
    std::wstring filename = L"players.csv";
    //std::wstring filename = L"players.txt";
    std::vector<Player> players = Player::loadPlayers(filename);

    // Ordenar os jogadores pelo score em ordem decrescente
    std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.score() > b.score();
    });

    // Exibir os jogadores
    std::wcout << L"Jogadores carregados: " << std::endl;
    for (const Player& player : players) {
        std::wcout << L"Nome: " << player.name() << L", Pontuação: " << player.score() << std::endl;
    }
    std::wcout << L"Escolha uma opção:" << std::endl;
}

// Função para exibir o número de erros
void GameController::display_misses(int misses) const {
    std::wcout << L"Número de erros: " << misses << std::endl;
}

// Função para exibir o status do jogo
void GameController::display_status(const std::vector<char>& incorrect, const std::wstring& answer) const {
    std::wcout << L"Erros: ";
    for (char ch : incorrect) {
        std::wcout << ch << ' ';
    }
    std::wcout << std::endl;

    std::wcout << L"Palavra: " << answer << std::endl;
}
void GameController::display_hangman(int misses) const {
    switch (misses) {
        case 0:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |\n"
                          L" |\n"
                          L" |\n"
                          L" |\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 1:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (._.)\n"
                          L" |\n"
                          L" |\n"
                          L" |\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 2:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (._.)\n"
                          L" |       |\n"
                          L" |       |\n"
                          L" |\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 3:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (._.)\n"
                          L" |      \\|\n"
                          L" |       |\n"
                          L" |\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 4:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (._.)\n"
                          L" |      \\|/\n"
                          L" |       |\n"
                          L" |\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 5:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (._.)\n"
                          L" |      \\|/\n"
                          L" |       |\n"
                          L" |      /\n"
                          L" |\n"
                          L"_|___\n";
            break;
        case 6:
            std::wcout << L"  _______\n"
                          L" |/      |\n"
                          L" |     (x_x)\n"
                          L" |      \\|/\n"
                          L" |       |\n"
                          L" |      / \\\n"
                          L" |\n"
                          L"_|___\n";
            break;
        default:
            break;
    }
}
void GameController::add_points(size_t points){
    m_points += points;
}

// Função para lidar com o fim do jogo
void GameController::end_game(const std::wstring& answer, const std::wstring& secretWord) {
    if (answer == secretWord) {
        std::wcout << L"Parabéns! Você ganhou!" << std::endl;
        //player.add_score(20);  // Adiciona 10 pontos ao jogador
        player.add_score(m_points);
    } else {
        std::wcout << L"Você perdeu! A palavra era: " << secretWord << std::endl;
        display_hangman(misses);
    }
    //calcscore(current_score);
    player.add_word(secretWord);
    //player.save(L"players.txt");
    player.save(L"players.csv");
    misses = 0;
    m_gameState = game_state_e::WELCOME;
    //current_score = 0;
}

// Função auxiliar para ler palavras de um arquivo para um vetor
std::vector<std::wstring> GameController::readWordsFromFile(const std::wstring& filename) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string utf8Filename = converter.to_bytes(filename);

    std::wifstream file(utf8Filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    std::vector<std::wstring> words;
    std::wstring word;
    while (std::getline(file, word)) {
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    return words;
}

void GameController::load_words(const std::wstring& filename, std::multimap<std::wstring, std::pair<std::wstring, std::wstring>>& wordsbank) {
    std::string filenameUtf8 = wstring_to_utf8(filename);
    std::wifstream file(filenameUtf8);
    if (!file.is_open()) {
        std::wcerr << L"Não foi possível abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
    file.imbue(loc);

    std::wstring line;
    while (std::getline(file, line)) {
        std::wstringstream stream(line);
        std::wstring category, word, difficulty;

        if (std::getline(stream, category, L';') && std::getline(stream, word, L';') && std::getline(stream, difficulty)) {
            wordsbank.insert({category, {word, difficulty}});
        }
    }

    file.close();
}

std::wstring GameController::getRandomWord(std::multimap<std::wstring, std::pair<std::wstring, std::wstring>>& wordsbank, const std::wstring& category, const std::wstring& difficulty) {
    auto range = wordsbank.equal_range(category);
    std::vector<std::wstring> words;

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second.second == difficulty) { // Check if the difficulty matches
            words.push_back(it->second.first);
        }
    }

    if (words.empty()) {
        return L"";
    }

    srand(static_cast<unsigned>(time(0)));
    int randoindex = rand() % words.size();
    return words[randoindex];
}


// Função para esconder a resposta
std::wstring GameController::hideAnswer(const std::wstring& secretWord) {
    std::wstring hidden(secretWord.length(), L'_');
    return hidden;
}

// Função para ler o nome do usuário
std::wstring GameController::read_user_name() {

    std::wstring filename = L"players.csv";
    //std::wstring filename = L"players.txt";
    std::vector<Player> players = Player::loadPlayers(filename);

    std::wstring playerName;
    bool nameExists = false;
    bool loadExistingSave = false;

    do {
        nameExists = false;
        std::wcout << L"Digite seu nome: ";
        std::wcin >> playerName;

        // Verificar se o nome já existe
        for (const Player& player : players) {
            if (player.name() == playerName) {
                std::wcout << L"O nome já existe. Você deseja carregar o save existente? (Y/N): ";
                wchar_t choice;
                std::wcin >> choice;
                choice = std::towupper(choice);
                if (choice == L'Y') {
                    loadExistingSave = true;
                    return playerName;
                } else {
                    nameExists = true;
                    std::wcout << L"Por favor, escolha outro nome." << std::endl;
                }
                break;
            }
        }
    } while (nameExists);

    player.setName(playerName);
    //player.save(L"players.txt");
    player.save(L"players.csv");

    return playerName;
}

Player GameController::read_player_name() {
    //std::wstring filename = L"players.txt";
    std::wstring filename = L"players.csv";
    std::vector<Player> players = Player::loadPlayers(filename);

    std::wstring playerName;
    bool nameExists = false;
    bool loadExistingSave = false;

    do {
        nameExists = false;
        std::wcout << L"Digite seu nome: ";
        std::wcin >> playerName;

        // Verificar se o nome já existe
        for (const Player& player : players) {
            if (player.name() == playerName) {
                std::wcout << L"O nome já existe. Você deseja carregar o save existente? (Y/N): ";
                wchar_t choice;
                std::wcin >> choice;
                choice = std::towupper(choice);
                if (choice == L'Y') {
                    loadExistingSave = true;
                    return player;
                } else {
                    nameExists = true;
                    std::wcout << L"Por favor, escolha outro nome." << std::endl;
                }
                break;
            }
        }
    } while (nameExists);

    Player player(playerName, 0);
    //player.save(L"players.txt");
    player.save(L"players.csv");

    return player;
}

void GameController::read_enter_to_proceed(){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool GameController::read_user_confirmation() {
    wchar_t choice;
    std::wcout << L"Please enter 'Y' for Yes or 'N' for No: ";
    std::wcin >> choice;
    choice = std::towupper(choice);
    return (choice == L'Y');
}

// Função para atualizar o estado do jogo com base na entrada do usuário e na lógica do jogo
void GameController::update() {
    if(m_gameState == game_state_e::WELCOME){
        //no processamento vai esperar o enter
        m_gameState = game_state_e::MAIN_MENU;
    }else if(m_gameState == game_state_e::MAIN_MENU){
        if(m_menu == menu_e::PLAY){
            m_gameState = game_state_e::PLAYING;
        }else if (m_menu == menu_e::RULES){
            m_gameState = game_state_e::SHOW_RULES;
        }else if (m_menu == menu_e::SCORE){
            m_gameState = game_state_e::SHOW_SCORE;
        }else if (m_menu == menu_e::EXIT){
            m_gameState = game_state_e::ENDING;
        }else if (m_menu == menu_e::INVALID||m_menu == menu_e::UNDEFINED){
            m_gameState = game_state_e::MAIN_MENU;
        }
    }else if (m_gameState == game_state_e::PLAYING){
        if(m_match == match_e::ON){
            //continua jogando
            m_gameState = game_state_e::PLAYING;
        }else if (m_match == match_e::PLAYER_WON||m_match == match_e::PLAYER_LOST){
            m_gameState = game_state_e::MAIN_MENU;
        }
    }else if (m_gameState == game_state_e::SHOW_RULES ||m_gameState == game_state_e::SHOW_SCORE){
        //no processamento vai esperar o enter
        m_gameState = game_state_e::MAIN_MENU;
    }else if (m_gameState == game_state_e::ENDING){
        m_match = match_e::UNDEFINED;
    }
}

// Função para processar a entrada do usuário e atualizar o estado do menu
void GameController::process_events() {

    std::setlocale(LC_ALL, "");
    std::wcout.imbue(std::locale(""));  // Para imprimir caracteres acentuados corretamente
    std::wcin.imbue(std::locale(""));   // Para ler caracteres acentuados corretamente
    
    if(m_gameState == game_state_e::WELCOME){
        //ler nome do jogador
        //read_user_name();
        player = read_player_name();
    }else if (m_gameState == game_state_e::MAIN_MENU){
        //ler a opção escolhida pelo jogador
        int choice;
        std::wcin >> choice;
        switch (choice) {
            case 1:
                m_menu = menu_e::PLAY;
                break;
            case 2:
                m_menu = menu_e::RULES;
                break;
            case 3:
                m_menu = menu_e::SCORE;
                break;
            case 4:
                m_menu = menu_e::EXIT;
                break;
            default:
                m_menu = menu_e::INVALID;
                std::wcout << L"Escolha inválida!" << std::endl;
                break;
        }
    }else if(m_gameState == game_state_e::SHOW_RULES ||m_gameState == game_state_e::SHOW_SCORE){
        //esperar o jogador dar enter para voltar para menu
        read_enter_to_proceed();
    }else if(m_gameState == game_state_e::ENDING ||m_gameState == game_state_e::QUITTING){
        //ler confirmação do jogador
        read_user_confirmation();
    }else if (m_gameState == game_state_e::PLAYING){
        if(m_match == match_e::ON){
            //ler a tentativa do jogador
            //verificar se a tentativa é certa ou errada
            playGame();

        }else if (m_match == match_e::PLAYER_WON||m_match == match_e::PLAYER_LOST){
            //esperar o jogador dar enter para voltar para menu 
            read_enter_to_proceed();
            
        }
    }
}

// Função para renderizar o estado do jogo para o usuário
void GameController::render() const {
    std::setlocale(LC_ALL, "");
    std::wcout.imbue(std::locale(""));  // Para imprimir caracteres acentuados corretamente
    std::wcin.imbue(std::locale(""));   // Para ler caracteres acentuados corretamente
    
    if (m_gameState == game_state_e::WELCOME){
        welcome();
    }else if (m_gameState == game_state_e::MAIN_MENU){
        menu();
    }else if (m_gameState == game_state_e::SHOW_RULES){
        showRules();
    }else if (m_gameState == game_state_e::SHOW_SCORE){
        showScore();
    }
    //a função playgame já renderiza o jogo, talvez seja melhor mudar depois
}

bool GameController::playGame() {
    std::setlocale(LC_ALL, "");
    std::wcout.imbue(std::locale(""));  // Para imprimir caracteres acentuados corretamente
    std::wcin.imbue(std::locale(""));   // Para ler caracteres acentuados corretamente

    std::multimap<std::wstring, std::pair<std::wstring, std::wstring>> wordsBank;
    //load_words(L"words.txt", wordsBank);
    load_words(L"words.csv", wordsBank);
    
    
    std::wstring secretWord;
    
    do {
        std::wstring category_op;
        std::wcout << L"Escolha uma categoria:" << std::endl;
        std::wcout << L"1. Profissões" << std::endl;
        std::wcout << L"2. Comidas" << std::endl;
        std::wcout << L"3. Países" << std::endl;
        std::wcout << L"4. Nomes" << std::endl;
        std::wcout << L"5. Capitais" << std::endl;
        std::wcout << L"6. Voltar para menu" << std::endl;
        int op;
        std::wcin >> op;

        if (op == 6) {
            m_gameState = game_state_e::MAIN_MENU;
            m_menu = menu_e::INVALID;
        }else{

        if(op == 1){
            category_op = L"Profissões";
        }
        if(op == 2){
            category_op = L"Comidas";
        }
        if(op == 3){
            category_op = L"Países";
        }
        if(op == 4){
            category_op = L"Nomes";
        }
        if(op == 5){
            category_op = L"Capitais";
        }

        std::wstring difficulty_op;
        std::wcout << L"Escolha a dificuldade:" << std::endl;
        std::wcout << L"1. Fácil" << std::endl;
        std::wcout << L"2. Médio" << std::endl;
        std::wcout << L"3. Difícil" << std::endl;
        std::wcin >> op;
        if(op == 1){
            difficulty_op = L"Fácil";
        }
        if(op == 2){
            difficulty_op = L"Médio";
        }
        if(op == 3){
            difficulty_op = L"Difícil";
        }
        
        
        secretWord = getRandomWord(wordsBank, category_op, difficulty_op);
        
        if (secretWord.empty()) {
            std::wcout << L"Categoria inválida ou sem palavras disponíveis!" << std::endl;
            return 1;
        }
        if (player.has_played(secretWord)) {
            std::wcout << L"Você já jogou todas as palavras dessa categoria. Escolha outra categoria." << std::endl;
        }
        }
    } while (player.has_played(secretWord));

    
    if( m_gameState != game_state_e::MAIN_MENU && m_menu != menu_e::INVALID){
    std::wstring answer = hideAnswer(secretWord);
    std::vector<wchar_t> incorrect;
    misses = 0;
    m_points = 0;

    while (answer != secretWord && misses < 6) {
        display_hangman(misses);
        std::wcout << L"Palavra: " << answer << std::endl;
        std::wcout << L"Tentativas erradas: " << misses << std::endl;
        std::wcout << L"Letras incorretas: ";
        for (wchar_t ch : incorrect) {
            std::wcout << ch << ' ';
        }
        std::wcout << std::endl;
        std::wcout << L"Digite uma letra ou uma palavra: ";
        std::wstring guess;
        std::wcin >> guess;

        if (guess.length() == 1) {
            wchar_t letter = guess[0];

            if (letter == L'#') {
                std::wcout << L"Você escolheu sair do jogo." << std::endl;
                m_menu = menu_e::INVALID;  // Retorna ao menu principal
                return false;
            }
            
            if (std::find(incorrect.begin(), incorrect.end(), letter) != incorrect.end() || answer.find(letter) != std::wstring::npos) {
                std::wcout << L"Você já tentou essa letra. Tente outra." << std::endl;
                continue;
            }

            bool correctGuess = false;
            for (size_t i = 0; i < secretWord.length(); ++i) {
                if (towlower(secretWord[i]) == towlower(letter)) {
                    answer[i] = secretWord[i];
                    correctGuess = true;
                    add_points(5); // Add points for each correct letter
                }
            }

            if (!correctGuess) {
                incorrect.push_back(letter);
                misses++;
            }

            display_hangman(misses);
        } else {
            if (guess == secretWord) {
                answer = secretWord;
                add_points(10);
                break;
            } else {
                bool correctGuess = true;
                for (size_t i = 0; i < secretWord.length(); ++i) {
                    if (towlower(secretWord[i]) != towlower(guess[i])) {
                        correctGuess = false;
                        break;
                    }
                }

                if (correctGuess) {
                    answer = secretWord;
                    break;
                } else {
                    std::wcout << L"Palavra incorreta." << std::endl;
                    misses++;
                    display_hangman(misses);
                }
            }
        }
    }
    end_game(answer, secretWord);
    return answer == secretWord;
        
    }else return false;
    
}



// Função para verificar se o jogo terminou
bool GameController::game_over() const {
    return m_match != match_e::ON;
}

// Loop principal do jogo
void GameController::run() {
    while (!game_over()) {
        render();
        process_events();
        update();
    }
}

std::string wstring_to_Utf8(const std::wstring& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(str);
}