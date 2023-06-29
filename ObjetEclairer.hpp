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