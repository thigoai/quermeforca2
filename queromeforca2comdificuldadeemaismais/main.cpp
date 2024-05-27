#include "hangman_gm.h"
#include <iostream>
#include <filesystem>
#include <locale>
#include <locale>


int main() {

    // Criação do controlador do jogo
    GameController gameController;
    //gameController.welcome();

    // Loop principal do menu
    while (true) {
        // Renderiza a tela atual
        gameController.render();

        // Processa eventos (entrada do usuário)
        gameController.process_events();

        // Atualiza o estado do jogo
        gameController.update();

        // Verifica se o jogo deve encerrar
        if (gameController.game_over()) {
            break;
        }
    }

    return 0;
}


