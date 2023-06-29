#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <algorithm>

using namespace std;

/*
Classe pour un objet de base 
    * Contient un nom, une description, et un vector de mots-clé
    * Méthode virtuel effectuerAction() 
    * Méthode afficher() pour afficher la description de l'objet
*/
class Objet {
public:
    Objet(string nom, string description, vector<string> keywords) : 
        nom_(nom), description_(description), motsCles_(keywords) {
        //genererMotsCles();
    }

    virtual ~Objet() = default;

    virtual void effectuerAction() { 
        cout << "Rien de particulier ne se produit." << endl << endl;
    }; 

    void afficher() {
        cout << description_ << endl << endl;
    }

    string getNom() { return nom_; }

    // On n'est pas certains s'il faut générer les mots clés avec une fonction ou s'il faut les définir à la main
    // Nous avons implémenté les deux choix
    void genererMotsCles() { 
        motsCles_.clear();
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

