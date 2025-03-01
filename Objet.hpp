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
    * Méthode getNom() pour retourner le nom de l'objet
    * Méthode genererMotsCles() pour générer automatiquement des mots-clé à partir du nom de l'objet
    * Méthode getMotsCles() pour retourner les mots-clé
Classe pour un objet regulier
   *Herite de la classe Objet
   *Un objet regulier est un objet qui effectue une commande sans rien changer dans le reste du jeu (par exemple: piano)
*/
class Objet {
public:
    Objet(string nom, string description, vector<string> keywords) :
        nom_(nom), description_(description), motsCles_(keywords) {
        //genererMotsCles();
    }

    virtual ~Objet() = default;

    virtual void effectuerAction() { 
        cout << "Nothing special happens." << endl << endl;
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
    string command_;
    string nom_;
    string description_;
    vector<string> motsCles_;
};

class ObjetRegulier : public Objet {
public:

    ObjetRegulier(string nom, string description, vector<string> keywords, string command) :
        Objet(nom, description, keywords), command_(command) { }


    void effectuerAction() override {
        cout << command_ << endl;
    }

private:
    string command_;
};
