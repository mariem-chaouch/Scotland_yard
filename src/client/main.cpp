#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <string>
#include "render.h"
// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

// LCOV_EXCL_START
void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>
#include <engine.h>

using namespace std;
using namespace state;
using namespace render;
// Nom lisible d'un ticket
static string ticketName(Ticket t) {
    switch (t) {
        case TaxiTicket:  return "Taxi";
        case BusTicket:   return "Bus";
        case TrainTicket: return "Metro";
        case BlackTicket: return "Black";
        default:          return "?";
    }
}

// Nom lisible d'un joueur : 0 = Mister X, puis les détectives
static string playerName(int i) {
    return (i == 0) ? "Mister X" : "Detective " + to_string(i);
}
// Mode "render" : la fenêtre graphique (travail d'Adam)
static void runRender() {
    GameState gameState;
    Scene scene(gameState);
    scene.runScene();
}
// Mode "board" : l'ancien test du plateau
static void runBoardDemo() {
    Board board;
    for (auto neighbor : board.getNeighbors(77))
        cout << neighbor << endl;
    cout << board.areConnected(77, 79) << endl;
    cout << board.getStation(78).getId() << endl;
    cout << "It works !" << endl;
}

// Mode "engine" : une partie complète, chaque joueur joue un coup légal au hasard
static void runEngineDemo(unsigned int seed) {
    cout << "=== Partie de démonstration (seed = " << seed << ") ===" << endl;

    mt19937 random(seed);
    engine::Engine engine;
    engine.init();
    GameState& game = engine.getState();
    int nbPlayers = (int) game.getPlayers().size();

    // ---- Tour 0 : tirage des cartes Départ, puis une StartCommand par joueur
    vector<int> starts = engine::Rules::drawStartStations(nbPlayers, seed);
    for (int i = 0; i < nbPlayers; i++) {
        engine.addCommand(make_unique<engine::StartCommand>(i, starts[i]));
        engine.update();
        cout << "[Tour 0] " << playerName(i) << " commence en " << starts[i] << endl;
    }

    // ---- Tours 1 à 24
    int safety = 0;                                     // évite une boucle infinie en cas de bug
    while (!engine::Rules::isGameOver(game) && safety < 1000) {
        safety++;
        int p = engine.getCurrentPlayerIndex();
        int turn = game.getTurn();
        unsigned int recordBefore = engine.getRecord().size();

        vector<pair<int, Ticket>> moves = engine::Rules::getLegalMoves(game, p);
        if (moves.empty()) {
            engine.addCommand(make_unique<engine::PassCommand>(p));
            cout << "[Tour " << turn << "] " << playerName(p) << " est bloqué : il passe" << endl;
        } else {
            uniform_int_distribution<int> pick(0, (int) moves.size() - 1);
            pair<int, Ticket> move = moves[pick(random)];
            engine.addCommand(make_unique<engine::MoveCommand>(p, move.first, move.second));
            cout << "[Tour " << turn << "] " << playerName(p) << " va en " << move.first
                 << " (" << ticketName(move.second) << ")" << endl;
        }
        engine.update();

        // Un coup tiré de getLegalMoves doit toujours être accepté : sinon c'est un bug
        if (engine.getRecord().size() == recordBefore) {
            cerr << "ERREUR : le moteur a refusé un coup légal !" << endl;
            break;
        }

        // Tour d'apparition : on affiche où Mister X a été vu
        if (p == 0 && engine::Rules::isRevealTurn(turn)) {
            MrX* mrx = dynamic_cast<MrX*>(game.getPlayers()[0]);
            cout << "   >>> Mister X se montre en " << mrx->getLastKnownStationId() << " !" << endl;
        }
    }

    // ---- Résultat
    cout << endl << "=== Fin de la partie au tour " << game.getTurn() << " ===" << endl;
    engine::Winner winner = engine::Rules::getWinner(game);
    if (winner == engine::DETECTIVES) {
        int d = engine::Rules::getCapturer(game);
        if (d != -1)
            cout << "Les détectives gagnent : " << playerName(d) << " a capturé Mister X !" << endl;
        else
            cout << "Les détectives gagnent : Mister X est bloqué !" << endl;
    } else if (winner == engine::MRX) {
        cout << "Mister X gagne !" << endl;
    } else {
        cout << "Partie interrompue (pas de gagnant)" << endl;
    }

    // ---- Sauvegarde de l'historique
    ofstream file("record.json");
    file << engine.getRecord();
    cout << engine.getRecord().size() << " coups enregistrés dans record.json" << endl;
}

int main(int argc, char* argv[])
{
    string mode = (argc >= 2) ? argv[1] : "render";

    if (mode == "engine") {
        unsigned int seed = (argc >= 3) ? stoul(argv[2]) : random_device{}();
        runEngineDemo(seed);
    } else if (mode == "board") {
        runBoardDemo();
    } else {
        runRender();                       // par défaut : la fenêtre graphique
    }
    return 0;
}

// LCOV_EXCL_END