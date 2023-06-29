#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "Case.hpp"
#include "Objet.hpp"
#include "ObjetDeverouiller.hpp"
#include "ObjetEclairer.hpp"
using namespace std;



/*
Classe pour contenir la carte du jeu 
    * Contient un vector de toutes les cases du jeu.
    * Contient la position (case) par défaut.
    * Méthode initCarteJeu() pour définir toutes les cases du jeu 
      (incluant les objets qu'elles contiennent, et les connections entre elles), et la case par défaut.
        → Les données sont 'hard coded' dans notre cas, mais elles pourraient être importées d'un fichier.
    * Méthode getDefaultPosition() pour retourner la position par défaut
*/
class CarteDuJeu {
public:

    void initCarteJeu() {
        // Definition de toutes le cases du jeu
        shared_ptr<Case> entree = make_shared<Case>("Foyer", 
            "This is the entrance of the house. There is a sturdy carpet on the floor.");
		
        shared_ptr<Case> cuisine = make_shared<Case>("Kitchen", 
            "Vous êtes dans la cuisine. Elle est très jaune 'beurre' comme la mode d'une certaine époque.");
		
        shared_ptr<Case> chambre = make_shared<Case>("Small Bedroom", 
            "This is the small bedroom. it is not particularly clean or well organised. There is a small window.");
		
        shared_ptr<Case> salon = make_shared<Case>("Living Room", 
            "This is the living room. There is a computer desk with cameras and a bluescreen. Next to it is a TV and a couch.", false);
		
        shared_ptr<Case> couloir = make_shared<Case>("Main Hallway", 
            "This is the main hallway. There is a bunch of boxes against the wall.");    

        shared_ptr<Case> salleR = make_shared<Case>("Room R",
            "This is a strange room with red walls.");

        // Ajouter connections possibles pour chaque case
        entree->addConnection(N, couloir);
        entree->addConnection(E, salon);

		salon->addConnection(W, entree);

		couloir->addConnection(S, entree);
		couloir->addConnection(N, cuisine);
		couloir->addConnection(W, chambre);

		chambre->addConnection(E, couloir);

		cuisine->addConnection(S, couloir);

        // Definition des objets du jeu
        shared_ptr<Objet> piano = make_shared<Objet>("A cheap electric piano", "This is an old entry-level with 61 keys. It looks like any cheap stuff from the late 90s.", 
            vector<string>{"piano"});
        shared_ptr<ObjetEclairer> interrupteurSalon = make_shared<ObjetEclairer>("A light switch", "It appears to be able to control the lighting in a connected room.", 
            vector<string>{"light", "switch"}, salon);
        shared_ptr<ObjetDeverouiller> boutonRouge = make_shared<ObjetDeverouiller>("A red button", "It appears to be able to unlock a secret room.", 
            vector<string>{"button", "red"}, couloir, salleR, E, O);
        
        // Ajouter les objets présents dans chaque case
        salon->addObjet(piano);
        couloir->addObjet(interrupteurSalon);
        salon->addObjet(boutonRouge);

        // Enregistrer les cases dans le vector carte_
        carte_.push_back(entree);
        carte_.push_back(cuisine);
        carte_.push_back(chambre);
        carte_.push_back(salon);
        carte_.push_back(couloir);
        carte_.push_back(salleR);

        // Set position de depart (defaut)
        defaultPosition_ = entree;  
    }

    shared_ptr<Case> getDefaultPosition() {   
        return defaultPosition_;
    }

private:
    vector<shared_ptr<Case>> carte_;
    shared_ptr<Case> defaultPosition_; 
};



/*
Classe pour l'état du jeu (données actuelles et méthodes de mise à jour des données)
    * Contient la position (case) actuelle.
    * Contient la carte du jeu.
    * Dans son constructeur, initialise le carte du jeu et la position par défaut. 
    * Méthode getCurrentPosition() pour retourner la position actuelle 
    * Méthode move() pour mettre à jour la position actuelle, après déplacement vers une direction donnée 
    * Méthode look() pour afficher les informations sur une case ou un objet 
    * Méthode use() pour utiliser un objet 
*/
class EtatDuJeu {
public:
    EtatDuJeu() {
        carteJeu_.initCarteJeu();
        position_ = carteJeu_.getDefaultPosition();    // Position par defaut
    }
    
    shared_ptr<Case> getCurrentPosition() {
        return position_;
    }

    void move(direction direction) { 
        if (position_->isValidMove(direction)) {            
            position_ = position_->getNewPosition(direction);     
            cout << "Going to " << char(direction) << endl << endl;            
            position_->afficher();
        }
        else {
            cout << "Can't go there!" << endl << endl;
        }
    }    
    
    void look(string objetNom) {
        if (objetNom.empty()) {
            position_->afficher();
        }
        else { 
            shared_ptr<Objet> obj = position_->getObjet(objetNom); 
            if (obj != nullptr) {
                obj->afficher();
            }
            else {
                cout <<  "Object does not exist!" << endl << endl; 
            }
        }    
    }

    void use(string objetNom) {
        if (objetNom.empty()) {
            cout << "use cannot be used without an argument. Please specify an object name or keyword." << endl << endl;
        }
        else {
            shared_ptr<Objet> obj = position_->getObjet(objetNom);
            if (obj != nullptr) {
                obj->effectuerAction();
            }
            else {
                cout << "Object does not exist!" << endl << endl; 
            }        
        }        
    }

private:
    shared_ptr<Case> position_;
    CarteDuJeu carteJeu_;  
};


/*
Classe pour la logique et l'exécution du jeu
   * Contient l'état du jeu 
   * Contient un unordered_map associant les commandes aux actions (fonctions à exécuter)
   * Méthode initJeu() pour initier le jeu (définir la map commande-action) 
   * Méthode splitUserInputStr() pour traiter l'entrée de l'utilisateur (séparation en commande-argument)
   * Méthode run() pour exécuter le jeu 
        → Affichage sur la console de l'état du jeu
        → Interaction avec l'utilisateur
        → Mise à jour de l'état du jeu en fonction de l'action de l'utilisateur
*/
class GamePlay {
public:

    GamePlay() {
        initJeu();
    }

    void initJeu() {
        // Definition de l'aiguillage des commandes
        commandActionMap_["look"] = [this](string arg) {etatJeu_.look(arg); };
        commandActionMap_["use"] = [this](string arg) {etatJeu_.use(arg); };

        commandActionMap_["N"] = [this](string arg) {etatJeu_.move(N); };
        commandActionMap_["S"] = [this](string arg) {etatJeu_.move(S); };
        commandActionMap_["E"] = [this](string arg) {etatJeu_.move(E); };
        commandActionMap_["W"] = [this](string arg) {etatJeu_.move(W); };

        commandActionMap_["exit"] = [this](string arg) { stop_ = true;
        cout << "Game Over!" << endl; };
    }

    pair<string, string> splitUserInputStr(string userInput) {
        string command;
        string argument;
        size_t spacePos = userInput.find(' ');
        if (spacePos != string::npos) {
            command = userInput.substr(0, spacePos);
            argument = userInput.substr(spacePos + 1);
        }
        else {
            command = userInput;
        }
        return make_pair(command, argument);
    }

    void run() {
        cout << "> > > > JEU INF1015 < < < <" << endl << endl;
        etatJeu_.look(""); // Description de la position par defaut

        while (!stop_) {
            string userInput;
            cout << "> ";
            getline(std::cin, userInput);

            // Extraire la commande et l'argument de l'entree de l'utilisateur
            pair<string, string> splitInput = splitUserInputStr(userInput);
            
            // Effectuer un action correspondant à la commande
            auto it = commandActionMap_.find(splitInput.first);
            if (it != commandActionMap_.end()) {
                it->second(splitInput.second);  // Appel de la fonction d'action, avec l'argument
            }
            else {
                // Gestion de commande non reconnue
                cout << "Unknown Command: " << splitInput.first << endl << endl;
            }
        }   
    }    

private:
    EtatDuJeu etatJeu_;  
    unordered_map<string, function<void(string)>> commandActionMap_;
    bool stop_ = false; // Par defaut, jeu n'arrete pas
};


