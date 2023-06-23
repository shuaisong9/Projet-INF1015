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

using namespace std;


/*
Classe pour contenir une case du jeu
    * Contient un nom, une description
    * Contient un map pour les connections possibles entre cette case et les autres cases
    * Méthode pour afficher les données d'une case (afficher)
    * Méthode pour ajouter une nouvelle connection à la case (addConnection)
    * Méthode pour vérifier si une direction donnée fait partie des connections possibles (isValidMove)
    * Méthode pour retourner la position (case) correspondant à une direction donnée (getNewPosition)
    * Méthode pour retourner le nom (getNow)
*/
class Case {
public:
    Case(string nom, string description) : nom_(nom), description_(description) {}

    ostream& afficher(ostream& os) {
        os << "-- " << nom_ << " --" << endl;
        os << description_ << endl;
        for(auto& c: connections_){
            os << c.second.lock()->getNom() << " est vers  " << c.first << endl; // le nord; l'est
        }
        return os;
    }

    void addConnection(string direction, std::shared_ptr<Case> connection) {
        connections_[direction] = connection;
    }

    bool isValidMove(string direction) const {
        return (connections_.find(direction) != connections_.end());
    }

    shared_ptr<Case> getNewPosition(string direction) {         
        return connections_[direction].lock();
    }

    string getNom(){
        return nom_;
    }

private:
    string nom_;
    string description_; 

    // À chaque Direction X: doit correspondre un pointeur vers une case de la carte
    map<string, weak_ptr<Case>> connections_; 
};


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

    void initJeu() {
        // Definition de toutes le cases du jeu
        shared_ptr<Case> entree = make_shared<Case>("Entree", 
            "Vous êtes dans l'entrée. Il y a un tapis sur le sol.");
		
        shared_ptr<Case> cuisine = make_shared<Case>("Cuisine", 
            "Vous êtes dans la cuisine. Elle est très jaune 'beurre' comme la mode d'une certaine époque.");
		
        shared_ptr<Case> chambre = make_shared<Case>("Chambre", 
            "Vous êtes dans une petite chambre. Elle n'a rien de spécial autre que d'être petite.");
		
        shared_ptr<Case> salon = make_shared<Case>("Salon", 
            "Vous êtes dans le salon.");
		
        shared_ptr<Case> couloir = make_shared<Case>("Couloir", 
            "Vous êtes dans le couloir.");    

        // Ajouter connections possibles pour chaque case
        entree->addConnection("N", couloir);
        entree->addConnection("E", salon);

		salon->addConnection("W", entree);

		couloir->addConnection("S", entree);
		couloir->addConnection("N", cuisine);
		couloir->addConnection("W", chambre);

		chambre->addConnection("E", couloir);

		cuisine->addConnection("S", couloir);

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
        carteJeu_.initJeu();
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
    void run() {
        bool stop = false;
        cout << "> > > > JEU INF1015 < < < <" << endl;

        etatJeu_.getCurrentPosition()->afficher(cout); //the first pos, defaultpos

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
};


