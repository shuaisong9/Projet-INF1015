#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

//#include "Case.hpp"
using namespace std;

//class Case;

class Objet {
public:
    Objet(string nom, string description) : nom_(nom), description_(description) {}
    virtual ~Objet() = default;
    virtual void effectuerAction() {}; 
    ostream& afficher(ostream& os) {
        // Afficher description de l'objet ...?
        os << description_ << endl;
    }

    string getNom() { return nom_; }

protected:
    string nom_;
    string description_;
};

