#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "Objet.hpp"
// Besoin d'include les autres ObjetDeverouiller et ObjetEclairer ??

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
    Case(string nom, string description, bool isBright = true) : nom_(nom), description_(description), isBright_(isBright) {}

    ostream& afficher(ostream& os) {

        if (isBright_) {
            os << "-- " << nom_ << " --" << endl;
            os << description_ << endl;
            for (auto& c : connections_) {
                os << c.second.lock()->getNom() << " est vers  " << c.first << endl; // le nord; l'est
            }
            os << "Vous observez:" << endl;
            for (auto& o : objets_) {
                os << "\t" << o->getNom() << endl;
            }
        }
        else {
            os << "Il fait très sombre et vous ne discernez aucun détail. Vous voyez seulement l'accès menant aux pièces connexes." << endl;
        }
        return os;
    }

    void addConnection(string direction, shared_ptr<Case> connection) {
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


    ///////
    void addObjet(shared_ptr<Objet> objet) {
        // On pourrait creer les objets a l'exterieur de la case (dans Carte)
        // On pourrait aussi les creer a l'appelation de addObjet()...comment????   // string nom, string description, shared_ptr<Objet> objet
        objets_.push_back(objet);
    }


    shared_ptr<Objet> getObjet(string objetName) {
        auto it = find_if(objets_.begin(), objets_.end(), [&](shared_ptr<Objet> obj) { return obj->getNom() == objetName; });
            // Cela ne respecte pas toutes les possibilites de inputs de l'utilisateur
            // Creer une correspondance entre tous les inputs possibles pour un objet (en se basant sur sa description) et son nom ...
        return *it;
    }

    // Set eclairage 
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
    map<string, weak_ptr<Case>> connections_;

    // Objets présents dans la case ?
    vector<shared_ptr<Objet>> objets_; // Vector, map??

    // Etat de l'eclairage ?
    bool isBright_; // Éclairé par défaut
};
