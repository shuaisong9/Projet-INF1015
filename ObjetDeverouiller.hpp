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

class ObjetDeverouiller : public Objet {
public:
    ObjetDeverouiller(string nom, string description, shared_ptr<Case> c1, shared_ptr<Case> c2, direction direction1_2, direction direction2_1) :
        Objet(nom, description), case1_(c1), case2_(c2), direction1_2_(direction1_2), direction2_1_(direction2_1) { }

    void effectuerAction() override {   
        case1_.lock()->addConnection(direction1_2_, case2_.lock());
        case2_.lock()->addConnection(direction2_1_, case1_.lock());
        cout << case2_.lock()->getNom() << " est maintenant connecté à " << case1_.lock()->getNom() << endl << endl;
    }

private:
    weak_ptr<Case> case1_;
    weak_ptr<Case> case2_;
    direction direction1_2_;
    direction direction2_1_;
};
