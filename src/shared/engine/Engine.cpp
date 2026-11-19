#include "Engine.h"
#include "state/MrX.h"

namespace engine {
    // Constructeur : l'historique commence comme un tableau JSON vide
    Engine::Engine() : record(Json::arrayValue) {}

    // Destructeur : les commandes restantes sont supprimées automatiquement
    // grâce aux std::unique_ptr
    Engine::~Engine() {}

    // Donne accès à l'état actuel de la partie (référence : pas de copie)
    state::GameState& Engine::getState() {
        return currentState;
    }

    // Donne accès aux règles du jeu
    Rules& Engine::getRules() {
        return rules;
    }

    // Prépare une nouvelle partie : positions de départ, tour 1, Mister X commence
    void Engine::init() {
        commands.clear();                              // aucune commande en attente
        record = Json::Value(Json::arrayValue);        // historique vide

        std::vector<state::Player*> players = currentState.getPlayers();
        if (players.empty())
            return;

        currentState.setTurn(0);                       // tour 0 : placement
        currentState.setCurrentPlayer(players[0]);     // Mister X se place en premier
    }
    // Range une commande dans la file d'attente (elle n'est pas encore exécutée)
    void Engine::addCommand(std::unique_ptr<Command> ptr_cmd) {
        commands.push_back(std::move(ptr_cmd));
    }

    // Renvoie l'historique des coups joués, au format JSON
    Json::Value Engine::getRecord() {
        return record;
    }

    // Position (0, 1, 2…) du joueur dont c'est le tour dans la liste des joueurs
    int Engine::getCurrentPlayerIndex() {
        std::vector<state::Player*> players = currentState.getPlayers();
        for (int i = 0; i < (int) players.size(); i++) {
            if (players[i] == currentState.getCurrentPlayer())
                return i;
        }
        return 0;    // par défaut : Mister X
    }

    // Passe la main au joueur suivant, et au tour suivant si tout le monde a joué
    void Engine::nextPlayer() {
        std::vector<state::Player*> players = currentState.getPlayers();
        if (players.empty())
            return;

        int index = getCurrentPlayerIndex();
        if (index + 1 < (int) players.size()) {
            currentState.setCurrentPlayer(players[index + 1]);          // joueur suivant
        } else {
            currentState.setTurn(currentState.getTurn() + 1);           // nouveau tour
            currentState.setCurrentPlayer(players[0]);                  // Mister X recommence
        }
    }

    // Exécute les commandes en attente, dans l'ordre d'arrivée
    // Exécute les commandes en attente, dans l'ordre d'arrivée
    void Engine::update() {
        for (auto& cmd : commands) {
            if (rules.isGameOver(currentState))
                break;                                  // partie terminée : on ne joue plus
            if (cmd->getPlayerId() != getCurrentPlayerIndex())
                continue;                               // pas son tour : commande ignorée
            int turn = currentState.getTurn();          // tour AVANT execute (un Coup double l'avance)
            if (cmd->execute(this)) {
                Json::Value entry = cmd->serialize();
                entry["turn"] = turn;
                record.append(entry);                   // on note le coup joué
                nextPlayer();                           // au joueur suivant
            }
        }
        commands.clear();                               // la file est vidée APRÈS la boucle
    }
    // Un joueur vient d'aller de "from" à "to" : on met à jour l'occupation des stations
    void Engine::updateTaken(int from, int to) {
        currentState.setStationTaken(from, false);   // l'ancienne station est libérée
        currentState.setStationTaken(to, true);      // la nouvelle est occupée
    }
}