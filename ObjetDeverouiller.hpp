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

    void effectuerAction() override {   // ??
        case1_->addConnection(direction1_2_, case2_);
        case2_->addConnection(direction2_1_, case1_);
    }

private:
    shared_ptr<Case> case1_;
    shared_ptr<Case> case2_;
    direction direction1_2_;
    direction direction2_1_;
};
