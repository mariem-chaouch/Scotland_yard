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

    // Prépare une nouvelle partie
    void Engine::init() {
        commands.clear();                          // aucune commande en attente
        record = Json::Value(Json::arrayValue);    // historique vide
        // plus tard : mise en place des joueurs, positions de départ, tour 1…
    }

    // Range une commande dans la file d'attente (elle n'est pas encore exécutée)
    void Engine::addCommand(std::unique_ptr<Command> ptr_cmd) {
        commands.push_back(std::move(ptr_cmd));
    }

    // Renvoie l'historique des coups joués, au format JSON
    Json::Value Engine::getRecord() {
        return record;
    }

    // Exécute toutes les commandes en attente, dans l'ordre d'arrivée
    void Engine::update() {
        for (auto& cmd : commands) {
            if (cmd->execute(this)) {              // la commande vérifie les règles et modifie l'état
                record.append(cmd->serialize());   // on note le coup joué dans l'historique
            }
        }
        commands.clear();                          // la file est vide
    }
    Rules& Engine::getRules() {
        return rules;
    }

}