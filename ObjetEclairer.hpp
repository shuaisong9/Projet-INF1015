#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

#include "Objet.hpp"
#include "Case.hpp"

using namespace std;

/*
Classe pour un objet qui permet de changer l'éclairage d'une case
Hérite de la classe Objet
    * Contient la case cible
    * Méthode effectuerAction() (override) qui change l'état d'éclairage dans la case cible
*/
class ObjetEclairer : public Objet {
public:
    ObjetEclairer(string nom, string description, vector<string> keywords, shared_ptr<Case> c) 
        : Objet(nom, description, keywords), caseCible_(c) { }

    void effectuerAction() override {
        caseCible_.lock()->setEclairage();
        cout << "L'éclairage de " << caseCible_.lock()->getNom() << " a été changé" << endl << endl;
    }

private:
    weak_ptr<Case> caseCible_;
};