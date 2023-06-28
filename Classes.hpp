/*
Livrable 1. 
	Ikram Arroud 2210444
	Shu Ai Song 2025639
	Abdelnour Sikouky 21582444
*/

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
    * Méthode initJeu() pour définir toutes les cases du jeu (ainsi que les connections entre elles), et la case par défaut.
        → Les données sont 'hard coded' dans notre cas, mais elles pourraient être importées d'un fichier.
    * Méthode pour retourner la position par défaut (getDefaultPosition)
*/
class CarteDuJeu {
public:

    void initCarteJeu() {
        // Definition de toutes le cases du jeu
        shared_ptr<Case> entree = make_shared<Case>("Entree", 
            "Vous êtes dans l'entrée. Il y a un tapis sur le sol.");
		
        shared_ptr<Case> cuisine = make_shared<Case>("Cuisine", 
            "Vous êtes dans la cuisine. Elle est très jaune 'beurre' comme la mode d'une certaine époque.");
		
        shared_ptr<Case> chambre = make_shared<Case>("Chambre", 
            "Vous êtes dans une petite chambre. Elle n'a rien de spécial autre que d'être petite.");
		
        shared_ptr<Case> salon = make_shared<Case>("Salon", 
            "Vous êtes dans le salon.", false);
		
        shared_ptr<Case> couloir = make_shared<Case>("Couloir", 
            "Vous êtes dans le couloir.");    

        shared_ptr<Case> salleR = make_shared<Case>("Salle R",
            "Vous êtes dans la salle secrète R.");

        // Ajouter connections possibles pour chaque case
        entree->addConnection(N, couloir);
        entree->addConnection(E, salon);

		salon->addConnection(W, entree);

		couloir->addConnection(S, entree);
		couloir->addConnection(N, cuisine);
		couloir->addConnection(W, chambre);

		chambre->addConnection(E, couloir);

		cuisine->addConnection(S, couloir);

        //////
        // Definition des objets du jeu
        shared_ptr<Objet> piano = make_shared<Objet>("Piano", "C'est un vieux piano Yamaha des années 90s.");     // CANNOT INSTANTIATE ABSTRACT CLASS !!!
        shared_ptr<ObjetEclairer> interrupteurSalon = make_shared<ObjetEclairer>("Interrupteur", "Il semble pouvoir allumer la lumière dans une salle connexe.", salon);
        shared_ptr<ObjetDeverouiller> boutonRouge = make_shared<ObjetDeverouiller>("Bouton rouge", "Il semble pouvoir déverouiller une salle.", couloir, salleR, E, W);
        
        // Ajouter les objets présents dans chaque case
        salon->addObjet(piano);
        couloir->addObjet(interrupteurSalon);
        salon->addObjet(boutonRouge);

        //////

        // Enregistrer les cases dans le vector carte_
        carte_.push_back(entree);
        carte_.push_back(cuisine);
        carte_.push_back(chambre);
        carte_.push_back(salon);
        carte_.push_back(couloir);

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
Classe pour l'état du jeu (données actuelles et méthodes de mise à jour)
    * Contient la position (case) actuelle.
    * Contient la carte du jeu.
    * Dans son constructeur, initialise le jeu et la position par défaut. 
    * Méthode pour mettre à jour la position actuelle, après déplacement vers une direction donnée (move)
    * Méthode pour retourner la position actuelle (getCurrentPosition)
*/
class EtatDuJeu {
public:
    /*EtatDuJeu(shared_ptr<Case> position, vector<string> movements) : position_(position),
        movements_(movements) {}*/

    // Constructeur sans parametres d'entree
    EtatDuJeu() {
        carteJeu_.initCarteJeu();
        position_ = carteJeu_.getDefaultPosition();    // Position par defaut
    }

    
    bool move(string direction) { //shared_ptr<Case> newPosition
        if (position_->isValidMove(direction)) {            
            position_ = position_->getNewPosition(direction);     
            cout << "Déplacement vers " + direction << endl;
            return true;
        }
        else {
            cout << "Ne peut pas aller là!" << endl;  
            return false;
        }
        //movements_.push_back(newPosition); //do we need to store all positions in a vector? 
    }

    shared_ptr<Case> getCurrentPosition() {
        return position_;
    }
    
    /////
    void look(string objetName) {
        if (objetName.empty()) {
            position_->afficher(cout);
        }
        else { // Dans la meme fonction look, ou on peut creer une fonction separee pour lookObject???
            shared_ptr<Objet> obj = position_->getObjet(objetName); 
            if (obj != nullptr) {
                obj->afficher(cout);
            }
            else {
                cout << "Objet n'existe pas!" << endl;  // ??
            }
        }    
    }


    void use(string objetName) {
        if (objetName.empty()) {
            cout << "use ne peut pas être utilisé sans argument. Veuillez préciser un nom d'objet ou un mot-clé." << endl;
        }
        
        shared_ptr<Objet> obj = position_->getObjet(objetName);
        if (obj != nullptr) {
            obj->effectuerAction();
        }
        else {
            cout << "Objet n'existe pas!" << endl;  // ??
        }
    }


private:
    shared_ptr<Case> position_;
    CarteDuJeu carteJeu_;  
    //vector<string> movements_;
};


/*
Classe pour la logique et l'exécution du jeu
   * Contient l'état du jeu 
   * Méthode pour exécuter le jeu (run)
        → Affichage sur la console de l'état du jeu
        → Interaction avec l'utilisateur
        → Mise à jour de l'état du jeu en fonction de l'action de l'utilisateur
*/
class GamePlay {
public:

    void initJeu() {
        // Definition de l'aiguillage des commandes
        commandActionMap_["look"] = [this](string& arg) {etatJeu_.look(arg); };
        commandActionMap_["use"] = [this](string& arg) {etatJeu_.use(arg); };

    }



    //// Function to split a string into words
    //vector<string> splitString(const string& input) {
    //    istringstream iss(input);
    //    vector<string> words;
    //    std::string word;
    //    while (iss >> word) {
    //        words.push_back(word);
    //    }
    //    return words;
    //}

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
        bool stop = false;
        cout << "> > > > JEU INF1015 < < < <" << endl;

        etatJeu_.getCurrentPosition()->afficher(cout); //the first pos, defaultpos


        std::string userInput;
        std::cout << "Enter your command: ";
        std::getline(std::cin, userInput);

        // Split the user input into words
        //std::vector<std::string> words = splitString(userInput);

        // Check the command and perform the corresponding action
        // if (!words.empty()) {
            //std::string command = words[0];
            //if (command == "look") {
            //    if (words.size() > 1) {
            //        std::string object = words[1];
            //        // Perform action for "look" command with the specified object
            //        std::cout << "Looking at: " << object << std::endl;
            //    }
            //    else {
            //        // Handle "look" command without object
            //        std::cout << "Look at what?" << std::endl;
            //    }
            //}
            // if (otherCommand)
            // else { cout << "Command n'existe pas" }




       


        // Extract the command and argument from the user input
        std::string command;
        std::string argument;
        size_t spacePos = userInput.find(' ');
        if (spacePos != std::string::npos) {
            command = userInput.substr(0, spacePos);
            argument = userInput.substr(spacePos + 1);
        }
        else {
            command = userInput;
        }

        //// Perform the action based on the user command
        //auto it = commandActionMap_.find(command);
        //if (it != commandActionMap_.end()) {
        //    it->second(argument);  // Call the member function with the argument
        //}
        //else {
        //    // Handle unrecognized command
        //    std::cout << "Unrecognized command: " << command << std::endl;
        //}


        while (!stop) {
            cout << endl << "> ";
            string userInput;
            cin >> userInput;
            
            if (userInput == "exit") {
                stop = true;
            }
            else if (userInput == "look"){
				etatJeu_.getCurrentPosition()->afficher(cout); 
            }
            else if (userInput == "N" || "S" || "W" || "E") {
                if(etatJeu_.move(userInput))    // Si déplacement réussi
                    etatJeu_.getCurrentPosition()->afficher(cout);
            }
            else {
                cout << "Je ne sais pas ça." << endl;
            }
        }   
    }    

private:
    EtatDuJeu etatJeu_;  
    unordered_map<string, function<void(string&)>> commandActionMap_;    // function<void(EtatDuJeu&)>
    
};


