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

        // Positions de départ, choisies parmi les cartes « Départ » du vrai jeu.
        // Elles sont fixes pour que le moteur soit déterministe (rejouer une partie
        // à partir du record donne exactement le même résultat).
        const int mrxStart = 51;
        const int detectiveStarts[] = { 13, 26, 29, 34, 50 };

        players[0]->setCurrentStationId(mrxStart);                 // Mister X
        for (int i = 1; i < (int) players.size() && i <= 5; i++)
            players[i]->setCurrentStationId(detectiveStarts[i - 1]); // détectives

        currentState.setTurn(1);                       // la partie commence au tour 1
        currentState.setCurrentPlayer(players[0]);     // Mister X joue en premier
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
        int turn = currentState.getTurn();

        // Mister X vient de jouer pendant un tour d'apparition : sa position devient visible
        if (index == 0 && rules.isRevealTurn(turn)) {
            state::MrX* mrx = dynamic_cast<state::MrX*>(players[0]);
            if (mrx)
                mrx->setLastKnownStationId(mrx->getCurrentStationId());
        }

        if (index + 1 < (int) players.size()) {
            currentState.setCurrentPlayer(players[index + 1]);   // joueur suivant
        } else {
            currentState.setTurn(turn + 1);                      // tout le monde a joué : nouveau tour
            currentState.setCurrentPlayer(players[0]);           // Mister X recommence
        }
    }

    // Exécute les commandes en attente, dans l'ordre d'arrivée
    void Engine::update() {
        for (auto& cmd : commands) {
            if (rules.isGameOver(currentState))
                break;                                     // partie terminée : on ne joue plus
            if (cmd->getPlayerId() != getCurrentPlayerIndex())
                continue;                                  // pas son tour : commande ignorée
            if (cmd->execute(this)) {                      // la commande vérifie les règles et modifie l'état
                Json::Value entry = cmd->serialize();
                entry["turn"] = currentState.getTurn();
                record.append(entry);                      // on note le coup joué, avec son tour
                nextPlayer();                              // au joueur suivant
            }
            int turn = currentState.getTurn();
            if (cmd->execute(this)) {
                Json::Value entry = cmd->serialize();
                entry["turn"] = turn;
            }
            commands.clear();                                  // la file est vide
        }
    }
}