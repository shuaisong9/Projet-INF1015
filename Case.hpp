#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "Objet.hpp"

using namespace std;


enum direction { N = 'N', S = 'S', E = 'E', W = 'W' };


/*
Classe pour contenir une case du jeu
    * Contient un nom, une description
    * Contient un map pour les connections possibles entre cette case et les autres cases
    * Contient un vector d'objets de la case
    * Contient un bool pour l'état d'éclairage de la case
    * Méthode afficher() pour afficher les données d'une case 
    * Méthode getNom() pour retourner le nom 
    * Méthode addConnection() pour ajouter une nouvelle connection à la case 
    * Méthode isValidMove() pour vérifier si une direction donnée fait partie des connections possibles 
    * Méthode getNewPosition() pour retourner la position (case) correspondant à une direction donnée 
    * Méthode addObjet() pour ajouter un objet à la case
    * Méthode containsKeyword() pour vérifier si un string objet reçu en paramètre contient un mot-clé des objets de la case
    * Méthode getObjet() pour retourner un objet selon un string objet reçu en paramètre
    * Méthode setEclairage() pour changer l'état d'éclairage de la case
*/
class Case {
public:
    Case(string nom, string description, bool isBright = true) : nom_(nom), description_(description), isBright_(isBright) {}

    void afficher() {
        cout << "-- " << nom_ << " --" << endl;        
        if (isBright_) {
            cout << description_ << endl;
            if (!objets_.empty()) {
                cout << "You notice: " << endl;
                for (auto& o : objets_) {
                    cout << "\t" << o->getNom() << endl;
                }
            }            
        }
        else {
            cout << "It is near pitch black and you cannot discern any details. You can only see the access to adjacent areas." << endl;
        }
        for (auto& c : connections_) {
            cout << c.second.lock()->getNom() << " is towards " << char(c.first) << endl; 
        }
    }

    string getNom() {
        return nom_;
    }

    void addConnection(direction d, shared_ptr<Case> connection) {
        connections_[d] = connection;
    }

    bool isValidMove(direction d) {   
        return (connections_.find(d) != connections_.end());
    }

    shared_ptr<Case> getNewPosition(direction d ) {    
        return connections_[d].lock();
    }

    void addObjet(shared_ptr<Objet> objet) {
        objets_.push_back(objet);
    }

    bool containsKeyword(const string& input, shared_ptr<Objet> obj) {
        // Rendre le string lowercase pour rendre la recherche case-insensitive  
        string lowerInput = input;
        transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), 
            [](unsigned char c) { return static_cast<char>(tolower(c)); });

        // Verifier si le string d'entree contient les mots-cles d'un objet
        for (auto& str : obj->getMotsCles()) {
            if (lowerInput.find(str) != string::npos) {
                return true;
            }
        }
        return false;
    }

    shared_ptr<Objet> getObjet(string objetNom) {
        auto it = find_if(objets_.begin(), objets_.end(),
            [&](shared_ptr<Objet> obj) { return containsKeyword(objetNom, obj); });

        return (it != objets_.end()) ? *it : nullptr;
    }
 
    void setEclairage() {
        if (isBright_) {
            isBright_ = false;
        }
        else {
            isBright_ = true;
        }
    }

private:
    string nom_;
    string description_;

    // À chaque Direction X: doit correspondre un pointeur vers une case de la carte
    map<direction, weak_ptr<Case>> connections_;

    // Objets présents dans la case 
    vector<shared_ptr<Objet>> objets_; 

    // Etat de l'eclairage 
    bool isBright_; // Éclairé par défaut
};
