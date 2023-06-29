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


class Objet {

public:
    Objet() = default;
    Objet(string nom, string description,vector<string> keywords, bool changeEclairage, 
        bool unlocks, string unlockedCommand="") : nom_(nom), description_(description), 
        keywords_(keywords), changeEclairage_(changeEclairage), 
        unlocks_(unlocks) {}

    ~Objet() = default;
    //virtual void command() = 0; //denclenche par use 
    //Le code de l'action devrait être associé au type d'objet(ou à l'objet lui-même
    virtual void command(shared_ptr<Case>& currentCase, shared_ptr<CarteDuJeu>& carte) = 0;
    void addObjet( shared_ptr<Objet> object) {
        //connections_[objectName] = object;
        objects_.push_back( object);
    }
    string getObjetNom() {
        return nom_;
    }
    string getDescription() {
        return description_;
    }
    void getMotsImportants(string objectName) {
        string importantWord; 
    }

    vector<string> getKeywords() {
        return keywords_;

    }


    // ça devrait seulement créer une nouvelle connexion, pas une nouvelle zone en soi
    //create the zone before. 
    // 
    // mais elle est seulement inaccessible avant d'utiliser l'objet approprié.

    // Il pourrait être préférable d'utiliser une structure de données pour améliorer
    //la lisibilité du programme

    //la description est donnée quand on observe l'objet (commande look). 
    
protected:
    //map<string, weak_ptr<Objet>> objects_;
    vector<shared_ptr<Objet>> objects_;
    //map<string, string> objectDesc;
    vector<string> keywords_; //keywords raised an error. 
private:
    string nom_;
    string description_;
    bool changeEclairage_;
    bool unlocks_;
    string unlockedCommand_; 

    //function<void(shared_ptr<Case>&, shared_ptr<CarteDuJeu>&)> command_;
};

class Case: public Objet {
public:

    Case(string nom, string description) : nom_(nom), description_(description) {}

    ostream& afficher(ostream& os) {
        os << "-- " << nom_ << " --" << endl;
        os << description_ << endl;
        for (auto& c : connections_) {
            os << c.second.lock()->getNom() << " est vers  " << c.first << endl; // le nord; l'est
        }
        
        for (auto& o : objects_) {
            //os <<"You notice: "<<endl<< o.second.lock()->getNom()<<o.first<<endl;
            os << "You notice: " << endl << "\t" <<o->getObjetNom() << endl;
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

    string getNom() {
        return nom_;
    }

    shared_ptr<Objet> getObject() {
        return objet_;
    }

    vector<shared_ptr<Objet>> getObjects() {
        return objects_;
    }

private:
    string nom_;
    string description_;
    shared_ptr<Objet> objet_;
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

        shared_ptr<Case> salleR = make_shared<Case>("Salle R",
            "Vous êtes dans la salle R.");

        //description des objets. 

        shared_ptr<Objet> piano = make_shared<Objet>("A cheap electric piano", "This is an old entry-level Yamaha with 61 keys.",  vector<string>{"piano"}, false, false);
        shared_ptr<Objet> button = make_shared<Objet>("A red button", " hg", vector<string>{ "button" }, false, true);

        //shared_ptr<Objet> switch = make_shared<Objet>("A red button", " hg");
        shared_ptr<Objet> lightswitch = make_shared<Objet>("A light switch", "hfuheue ", vector<string>{ "light"  },true, false);

        entree->addObjet( piano);
        couloir->addObjet(lightswitch);
        salon->addObjet(button);

        // Ajouter connections possibles pour chaque case
        entree->addConnection("N", couloir);
        entree->addConnection("E", salon);

        salon->addConnection("W", entree);

        couloir->addConnection("S", entree);
        couloir->addConnection("N", cuisine);
        couloir->addConnection("W", chambre);

        //couloir->addConnection("E", salleR); // hidden room

        chambre->addConnection("E", couloir);

        cuisine->addConnection("S", couloir);

        //button->command = [this]() {salleR->addConnection("E", couloir); };
        //piano->command = 

        // Enregistrer les cases dans le vector carte_
        carte_.push_back(entree);
        carte_.push_back(cuisine);
        carte_.push_back(chambre);
        carte_.push_back(salon);
        carte_.push_back(couloir);

        // Set position de depart (defaut)
        defaultPosition_ = entree;

        //look describes the zone if nothing follows. 


    }
    //create a ptr with shared_ptr<Case>

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

    void printObjInfo(string input) {
        shared_ptr<Case> currentPos = position_;
        vector<shared_ptr<Objet>> objectsAtCurrentPos = currentPos->getObjects();
        vector<string> keywords =  currentPos->getKeywords();

        for (const auto& obj : objectsAtCurrentPos) {
            //cout<< obj->getKeywords()[0];
            for (string k : obj->getKeywords()) {
                //cout << k;
                if (k == input) {
                    string description = obj->getDescription();
                    cout << description; //there's probably a better way to do this.

                }
            }
        }
    }

    bool move(string direction) { //shared_ptr<Case> newPosition
        if (position_->isValidMove(direction)) {
            position_ = position_->getNewPosition(direction);
            cout << "Déplacement vers " + direction << endl;
            return true;
        }
        else {
            //cout << "Ne peut pas aller là!" << endl;
            return false;
        }
        //movements_.push_back(newPosition); //do we need to store all positions in a vector? 
    }

    shared_ptr<Case> getCurrentPosition() {
        return position_;
    }


    string getDescription() {
        return objet_->getDescription();
    }
    void useCommand(EtatDuJeu gameState, string input) {
        string use = "use"; 
        if (input.length() > use.length()) {
            size_t index = input.find(" ");
            gameState.printObjInfo(input.substr(index + 1));
        }
    }

private:
    shared_ptr<Case> position_;
    shared_ptr<Objet> objet_;
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
//enum for string
class GamePlay {
public:
    void run() {
        //string object;
        bool stop = false;
        cout << "> > > > JEU INF1015 < < < <" << endl;
        
        etatJeu_.getCurrentPosition()->afficher(cout); //the first pos, defaultpos

        string input ="piano";
        string look = "look";
        string use = "use";
        
 
        while (!stop) {
            cout << endl << "> ";
            string userInput;
            cin >> userInput;

            if (userInput == "exit") {
                stop = true;
            }
            //else if (userInput == "look") {
              //  etatJeu_.getCurrentPosition()->afficher(cout);
            //}
            else if (userInput.length() > look.length()) { //this is borderline hardcoding but idc anymore, condense it into a function.
                size_t index = userInput.find(" ");
                etatJeu_.printObjInfo(userInput.substr(index+1));
            }
           // else if (userInput.length() == look.length()){
             //   etatJeu_.getCurrentPosition()->afficher(cout);
           // }

            else if (userInput == "N" || "S" || "W" || "E") { 
                if (etatJeu_.move(userInput))    // Si déplacement réussi
                    etatJeu_.getCurrentPosition()->afficher(cout);
            }

            else if (userInput.length() > use.length()) { //this is borderline hardcoding but idc anymore, condense it into a function.
                size_t index = userInput.find(" ");
                etatJeu_.printObjInfo(userInput.substr(index + 1));

            }
            else if (userInput == "use") { //must +command
                //if command ="" then must enter something else: blah blah. 

            }
            else {
                cout << "Je ne sais pas ça." << endl;
            }
        }
    }

private:
    EtatDuJeu etatJeu_;
};


class ObjectDeverrouillant : public Objet {


};


/*
*
*
*     //get current object.


void getObjInfo() {
    shared_ptr<Case> currentPos = position_;
    vector<shared_ptr<Objet>> objectsAtCurrentPos = currentPos->getObjects();
    for (const auto& obj : objectsAtCurrentPos) {
        string name = obj->getObjetNom();
        string description = obj->getDescription();
        cout << description; //there's probably a better way to do this.
    }

}
*
*
         string name = obj->getObjetNom();
        string description = obj->getDescription();
        cout << description; //there's probably a better way to do this.

static void lookCommand(shared_ptr<Objet> object,string input) {

    vector<string> keywords = object->getKeywords();
    for (const string& keyword : keywords) {
        if (input == keyword) {
            object->getDescription();
            //etObjInfo();
            std::cout << "Vous regardez : " << object->getObjetNom() << std::endl;
            return;

        }
    }
    std::cout << "Cet objet n'est pas trouvé." << std::endl;
}
*/
