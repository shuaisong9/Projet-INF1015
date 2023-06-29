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
    Objet(string nom, string description) : nom_(nom), description_(description) {
        genererMotsCles();
    }
    virtual ~Objet() = default;
    virtual void effectuerAction() { 
        cout << "Rien de particulier ne se produit." << endl << endl;
    }; 
    void afficher() {
        cout << description_ << endl << endl;
    }

    string getNom() { return nom_; }

    void genererMotsCles() {
        string lowerNom = nom_;
        transform(lowerNom.begin(), lowerNom.end(), lowerNom.begin(),
            [](unsigned char c) { return static_cast<char>(tolower(c)); });

        istringstream iss(lowerNom);
        string mot;
        while (iss >> mot) {
            motsCles_.push_back(mot);
        }
    }

    vector<string> getMotsCles() {
        return motsCles_;
    }

protected:
    string nom_;
    string description_;
    vector<string> motsCles_;
};

