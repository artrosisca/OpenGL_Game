#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

// Enumeração para representar os estados do jogo
enum class GameState {
    Playing,
    GameOver
};

// Dimensões da janela
const int W = 600;
const int H = 480;

// Velocidade global dos jogadores
int velocidade = 2;

// Matriz que representa o campo de jogo
bool campo[W][H] = { 0 };

// Estrutura para representar o primeiro jogador
struct Player1 {
    int x, y, dir, score;
    Texture texture;

    // Construtor inicializando o jogador na posição e direção inicial
    Player1(Color c) {
        x = 580;
        y = 460;
        dir = 1;
        score = 0;
        texture.loadFromFile("player1.45.png");
    }

    // Função para movimentar o jogador
    void move() {
        if (dir == 0) y += 1;
        if (dir == 1) x -= 1;
        if (dir == 2) x += 1;
        if (dir == 3) y -= 1;

        // Lógica para manter o jogador dentro dos limites da janela
        if (x >= W) x = 0;
        if (x < 0) x = W - 1;
        if (y >= H) y = 0;
        if (y < 0) y = H - 1;
    }
};

// Estrutura para representar o segundo jogador
struct Player2 {
    int x, y, dir, score;
    Texture texture;

    // Construtor inicializando o jogador na posição e direção inicial
    Player2(Color c) {
        x = 20;
        y = 20;
        dir = 2;
        score = 0;
        texture.loadFromFile("player2.45.png");
    }

    // Função para movimentar o jogador
    void move() {
        if (dir == 0) y += 1;
        if (dir == 1) x -= 1;
        if (dir == 2) x += 1;
        if (dir == 3) y -= 1;

        // Lógica para manter o jogador dentro dos limites da janela
        if (x >= W) x = 0;
        if (x < 0) x = W - 1;
        if (y >= H) y = 0;
        if (y < 0) y = H - 1;
    }
};

// Constantes para os pontos no jogo
constexpr int pointRadius = 10;  // Raio dos pontos
constexpr int collisionMargin = 10;  // Valor de margem para aumentar a hitbox

// Estrutura para representar um ponto no jogo
struct Point {
    int x, y;
    Color color;
    bool isGreen;
    Texture texture;

    // Construtor para inicializar um ponto
    Point(int xPos, int yPos, Color c, bool green) : x(xPos), y(yPos), color(c), isGreen(green) {
        // Carrega a textura dependendo se é um ponto verde ou vermelho
        if (isGreen) {
            texture.loadFromFile("cow.png");
        }
        else {
            texture.loadFromFile("bomb.png");
        }
    }

    // Função para verificar a colisão com o primeiro jogador
    // "const" garante que nada seja modificado
    bool checkCollision(const Player1& player) const {
        return (x - pointRadius - collisionMargin <= player.x && player.x <= x + pointRadius + collisionMargin &&
            y - pointRadius - collisionMargin <= player.y && player.y <= y + pointRadius + collisionMargin);
    }

    // Função para verificar a colisão com o segundo jogador
    bool checkCollision(const Player2& player) const {
        return (x - pointRadius - collisionMargin <= player.x && player.x <= x + pointRadius + collisionMargin &&
            y - pointRadius - collisionMargin <= player.y && player.y <= y + pointRadius + collisionMargin);
    }
};

// Função para gerar pontos verdes e vermelhos
// std::vector é usado para representar um vetor dinâmico de pontos
void generatePoints(std::vector<Point>& points, Player1& p1, Player2& p2) {
    // Gera entre 3 e 4 pontos verdes
    int greenPoints = rand() % 4 + 3;
    for (int i = 0; i < greenPoints; ++i) {
        int soma, x, y;
        soma = rand() % W;
        if (soma <= (p1.x + 5)) {
            soma += 60;
        }
        x = soma;
        soma = rand() % H;
        if (soma <= (p1.y + 5)) {
            soma += 60;
        }
        y = soma;
        points.push_back(Point(x, y, Color::Green, true));
    }

    // Gera entre 2 e 6 pontos vermelhos
    int redPoints = rand() % 6 + 2;
    for (int i = 0; i < redPoints; ++i) {
        int soma, x, y;
        soma = rand() % W;
        if (soma <= (p1.x + 5)) {
            soma += 60;
        }
        x = soma;
        soma = rand() % H;
        if (soma <= (p1.y + 5)) {
            soma += 60;
        }
        y = soma;
        points.push_back(Point(x, y, Color::Red, false));
    }
}

// Função para gerar pontos vermelhos
void generatePointsRed(std::vector<Point>& points, Player1& p1, Player2& p2) {
    int redPoints = rand() % 1 + 1; // Gera 1 ponto vermelho
    for (int i = 0; i < redPoints; ++i) {
        int soma, x, y;
        soma = rand() % W;
        if (soma <= (p1.x + 5)) {
            soma += 60;
        }
        x = soma;
        soma = rand() % H;
        if (soma <= (p1.y + 5)) {
            soma += 60;
        }
        y = soma;
        // Adiciona um elemento ao final do vetor
        points.push_back(Point(x, y, Color::Red, false));
    }
}

// Função para reiniciar o jogo
void restartGame(Player1& p1, Player2& p2, std::vector<Point>& points, Clock& gameClock, GameState& gameState) {
    p1.score = p2.score = 0;
    points.clear();
    generatePoints(points, p1, p2);
    gameClock.restart();

    // Reposiciona os jogadores para as posições originais
    p1.x = 580;
    p1.y = 460;
    p1.dir = 1;

    p2.x = 20;
    p2.y = 20;
    p2.dir = 2;

    gameState = GameState::Playing;
}

// Função para lidar com a entrada do teclado
void handleInput(Player1& p1, Player2& p2, GameState& gameState, Clock& gameClock, std::vector<Point>& points) {
    // Controles do jogador 1
    if (Keyboard::isKeyPressed(Keyboard::Left)) if (p1.dir != 2) p1.dir = 1;
    if (Keyboard::isKeyPressed(Keyboard::Right)) if (p1.dir != 1)  p1.dir = 2;
    if (Keyboard::isKeyPressed(Keyboard::Up)) if (p1.dir != 0) p1.dir = 3;
    if (Keyboard::isKeyPressed(Keyboard::Down)) if (p1.dir != 3) p1.dir = 0;

    // Controles do jogador 2
    if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir != 2) p2.dir = 1;
    if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir != 1)  p2.dir = 2;
    if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir != 0) p2.dir = 3;
    if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir != 3) p2.dir = 0;

    // Verificação para reiniciar o jogo
    if (Keyboard::isKeyPressed(Keyboard::R) && gameState == GameState::GameOver) {
        restartGame(p1, p2, points, gameClock, gameState);
    }

    // Verificação para sair do jogo
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        if (gameState == GameState::GameOver) {
            exit(0);  // Encerrar o aplicativo se estiver na tela de fim de jogo
        }
        else {
            gameState = GameState::GameOver;  // Mudar para o estado de jogo terminado se estiver jogando
        }
    }
}

// Função para atualizar o estado do jogo
void updateGame(Player1& p1, Player2& p2, RenderTexture& t, GameState& gameState, std::vector<Point>& points, Sprite& background, Clock& gameClock, bool& colisao, bool& colisaoPlayer) {
    if (gameState == GameState::Playing) {
        // Lógica do jogo enquanto estiver em andamento

        // Limpar o campo
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                campo[i][j] = 0;
            }
        }

        // Atualizar a posição dos jogadores e renderizar na textura
        for (int i = 0; i < velocidade; i++) {
            p1.move();
            p2.move();

            t.clear();
            t.draw(background);

            // Renderizar jogador 1
            Sprite player1Sprite(p1.texture);
            player1Sprite.setPosition((p1.x - 17), (p1.y - 20));
            t.draw(player1Sprite);

            // Renderizar jogador 2
            Sprite player2Sprite(p2.texture);
            player2Sprite.setPosition((p2.x - 17), (p2.y - 20));
            t.draw(player2Sprite);

            // Renderizar pontos
            for (auto& point : points) {
                Sprite pointSprite(point.texture);
                pointSprite.setPosition((point.x - 17), (point.y - 20));
                t.draw(pointSprite);

                // Verificar colisões com jogadores
                if (point.checkCollision(p1)) {
                    if (point.isGreen) {
                        p1.score++;
                        generatePointsRed(points, p1, p2);
                    }
                    else {
                        colisaoPlayer = true;
                        colisao = true;
                        gameState = GameState::GameOver;
                    }
                    point.x = rand() % W;
                    point.y = rand() % H;
                }

                if (point.checkCollision(p2)) {
                    if (point.isGreen) {
                        p2.score++;
                        generatePointsRed(points, p1, p2);
                    }
                    else {
                        colisaoPlayer = false;
                        colisao = true;
                        gameState = GameState::GameOver;
                    }
                    point.x = rand() % W;
                    point.y = rand() % H;
                }
            }
        }

        // Lógica de tempo
        int remainingTime = 30 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
        if (remainingTime < 0) {
            gameState = GameState::GameOver;
            remainingTime = 0;
        }

        // Renderizar o tempo restante na textura
        Font font;
        font.loadFromFile("04B_30__.ttf");
        Text timer;
        timer.setFont(font);
        timer.setCharacterSize(20);
        timer.setFillColor(Color::White);
        timer.setString("Tempo: " + std::to_string(remainingTime) + "s");
        timer.setPosition(W - 390, 5);
        t.draw(timer);

        t.display();
    }
    else if (gameState == GameState::GameOver) {
        // Lógica da tela de fim de jogo
        Font font;
        font.loadFromFile("04B_30__.ttf");
        Text gameOverText;
        Text winner;
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(28);
        gameOverText.setFillColor(Color::Green);

        // Determinar o vencedor com base nas pontuações
        if (colisao) {
            if (!colisaoPlayer) {
                winner.setString("JOGADOR VERMELHO VENCEU");
            }
            else {
                winner.setString("  JOGADOR AZUL VENCEU");
            }
        }
        else {
            if (p1.score < p2.score) winner.setString("  JOGADOR AZUL VENCEU");
            if (p1.score > p2.score) winner.setString("JOGADOR VERMELHO VENCEU");
            if (p1.score == p2.score) winner.setString("          EMPATE");
        }

        gameOverText.setString("       Fim de Jogo\n" + winner.getString() + "\n\n Pressione R para Reiniciar\n  Pressione Esc para Sair");
        gameOverText.setPosition(W / 2 - 300, H / 2 - 50);
        t.draw(gameOverText);
    }

    t.display();
}

// Função principal
int main() {
    srand(time(0));

    // Configuração da janela
    RenderWindow window(VideoMode(W, H), "Aliens");
    window.setFramerateLimit(60);

    // Carregamento da textura de fundo
    Texture texture;
    texture.loadFromFile("background.png");
    Sprite spriteBackground(texture);

    // Carregamento da fonte para os textos de pontuação
    Font font;
    font.loadFromFile("04B_30__.ttf");

    // Inicialização dos jogadores
    Player1 p1(Color::Blue);
    Player2 p2(Color::Magenta);

    // Inicialização dos textos de pontuação
    Text player1Score, player2Score;
    player1Score.setFont(font);
    player2Score.setFont(font);
    player1Score.setCharacterSize(20);
    player2Score.setCharacterSize(20);
    player1Score.setFillColor(Color::White);
    player2Score.setFillColor(Color::White);
    p1.score = p2.score = 0;

    // Inicialização dos pontos
    std::vector<Point> points;
    generatePoints(points, p1, p2);

    // Configuração da textura e sprite para renderização
    Sprite sprite;
    RenderTexture t;
    t.create(W, H);
    t.setSmooth(true);
    sprite.setTexture(t.getTexture());
    t.clear();
    t.draw(spriteBackground);

    // Configurações adicionais
    bool Game = true; // Flag que indica se o jogo está em execução
    GameState gameState = GameState::Playing; // Estado atual do jogo

    Clock gameClock; // Relógio para controlar o tempo do jogo
    gameClock.restart(); // Reinicia o relógio para começar a contar o tempo do jogo

    bool colisao = NULL; // Flag que indica se houve uma colisão genérica
    bool colisaoPlayer = false; // Flag que indica se houve uma colisão com o jogador

    // Loop principal do jogo que continua enquanto a janela estiver aberta
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close(); // Fecha a janela se o evento de fechamento for detectado
        }

        handleInput(p1, p2, gameState, gameClock, points); // Lida com a entrada do usuário
        updateGame(p1, p2, t, gameState, points, spriteBackground, gameClock, colisao, colisaoPlayer); // Atualiza o estado do jogo

        // Atualiza os textos de pontuação
        player1Score.setString(std::to_string(p1.score));
        player2Score.setString(std::to_string(p2.score));

        // Posiciona os textos de pontuação na janela, levando em consideração a área não alcançável pelos jogadores
        player1Score.setPosition(std::max(10.0f, static_cast<float>(p1.x - 3)), std::max(10.0f, static_cast<float>(p1.y - 50)));
        player2Score.setPosition(std::max(10.0f, static_cast<float>(p2.x - 3)), std::max(10.0f, static_cast<float>(p2.y - 50)));

        window.clear(); // Limpa o conteúdo da janela
        // Desenha os elementos na janela
        window.draw(sprite); // Desenha o sprite de fundo
        window.draw(player1Score); // Desenha a pontuação do jogador 1
        window.draw(player2Score); // Desenha a pontuação do jogador 2

        window.display(); // Exibe o conteúdo desenhado na janela
    }
}