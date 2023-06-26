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
    ObjetEclairer(string nom, string description, shared_ptr<Case> c) : Objet(nom, description), caseCible_(c) { }

    void effectuerAction() override {
        caseCible_->setEclairage();
    }

private:
    shared_ptr<Case> caseCible_;
};