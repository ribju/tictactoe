// Julia Pereira Maia Ribeiro e Clara Temponi Marigo


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>

// Classe TicTacToe
class TicTacToe {
private:
    std::array<std::array<char, 3>, 3> board; // Tabuleiro do jogo
    std::mutex board_mutex; // Mutex para controle de acesso ao tabuleiro
    std::condition_variable turn_cv; // Variável de condição para alternância de turnos
    char current_player; // Jogador atual ('X' ou 'O')
    bool game_over; // Estado do jogo
    char winner; // Vencedor do jogo

public:
    TicTacToe()
         : current_player('X'), game_over(false), winner(' ') {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                board[i][j] = ' ';
    }// Inicializar o tabuleiro e as variáveis do jogo
}

    void display_board() {
        std::unique_lock<std::mutex> lock(board_mutex);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << board[i][j];
                if (j < 2) std::cout << "|";
            }
            std::cout << "\n";
            if (i < 2) std::cout << "-----\n";
        }
        std::cout << std::endl;// Exibir o tabuleiro no console
    }

    bool make_move(char player, int row, int col) {
        std::unique_lock<std::mutex> lock(board_mutex);
        if (player != current_player || board[row][col] != ' ' || game_over)
            return false;

        board[row][col] = player;
        if (check_win(player)) {
            winner = player;
            game_over = true;
        } else if (check_draw()) {
            player = 'D';
            game_over = true;
        } else {
            current_player = (playerr == 'X') ? 'O' : 'X';
        }
        turn_cv.notify_all();
        return true;// Implementar a lógica para realizar uma jogada no tabuleiro
        // Utilizar mutex para controle de acesso
        // Utilizar variável de condição para alternância de turnos
    }

    bool check_win(char player) {
        for (int i = 0; i < 3; ++i)
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] ==player && board[2][i] == player))
                return true;

        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player))
            return true;

        return false;// Verificar se o jogador atual venceu o jogo
    }

    bool check_draw() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == ' ')
                    return false;
        return true;// Verificar se houve um empate
    }

    bool is_game_over() {
        std::unique_lock<std::mutex> lock(board_mutex);
        return game_over;// Retornar se o jogo terminou
    }

    char get_winner() {
        std::unique_lock<std::mutex> lock(board_mutex);
        return winner;// Retornar o vencedor do jogo ('X', 'O', ou 'D' para empate)
    }
};

// Classe Player
class Player {
private:
    TicTacToe& game; // Referência para o jogo
    char symbol; // Símbolo do jogador ('X' ou 'O')
    std::string strategy; // Estratégia do jogador

public:
    Player(TicTacToe& g, char s, std::string strat) 
        : game(g), symbol(s), strategy(strat) {}

    void play() {
        while (!game.is_game_over()) {
            std::unique_lock<std::mutex> lock(game.get_turn_cv().mutex);
            jogo.get_turn_cv().wait(lock, [&]() { return symbol == game.get_current_player() || game.is_game_over(); });
            if (game.is_game_over())
                return;

            if (strategy == "sequential")
                play_sequential();
            else if (strategy == "random")
                play_random();
        }// Executar jogadas de acordo com a estratégia escolhida
    }

private:
    void play_sequential() {
       for (int i = 0; i < 3 && !game.is_game_over(); ++i) {
            for (int j = 0; j < 3 && !game.is_game_over(); ++j) {
                if (game.make_move(symbol, i, j))
                    return;
            }
        } // Implementar a estratégia sequencial de jogadas
    }

    void play_random() {
         std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);

        while (!game.is_game_over()) {
            int i = dis(gen);
            int j = dis(gen);
            if (game.make_move(symbol, i, j))
                return;
        }// Implementar a estratégia aleatória de jogadas
    }
};

// Função principal
int main() {
   TicTacToe game;
    Player player1(game, 'X', "sequential");
    Player player2(game, 'O', "random");

    std::thread t1(&Player::play, &player1);
    std::thread t2(&Player::play, &player2);

    t1.join();
    t2.join();

    game.display_board();
    char winner = game.get_winner();
    if (winner == 'D') {
        std::cout << "O jogo terminou em empate!" << std::endl;
    } else {
        std::cout << "O vencedor é o jogador: " << winner << "!" << std::endl;
    } // Inicializar o jogo e os 

    // Criar as threads para os jogadores

    // Aguardar o término das threads

    // Exibir o resultado final do jogo

    return 0;
}
