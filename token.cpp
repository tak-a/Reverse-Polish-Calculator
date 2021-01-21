//token.cpp

#include "token.h"

//-----------Operatoreux -------------

int Operatoreux::getPriority() { return priority ;}
Operatoreux::~Operatoreux(){}

//----------- OpePlus ----------------
OpePlus::OpePlus(char ope){ priority = 2 ; opeVal = ope ;}

double OpePlus::calcul(double val1, double val2) { return(val1 + val2) ;}
OpePlus::~OpePlus(){}


//----------- OpeMoins ----------------
OpeMoins::OpeMoins(char ope)
        { priority = 2 ; opeVal = ope ;}

double OpeMoins::calcul(double val1, double val2) { return(val1 - val2) ;}
OpeMoins::~OpeMoins(){}


//------------ OpeDiv ---------------
OpeDiv::OpeDiv(char ope)
        { priority = 1 ; opeVal = ope ;}

double OpeDiv::calcul(double val1, double val2) {
  if(val2 == 0){
    throw std::string("Erreur, Division par zéro.");
  }
  return (val1 / val2) ;}
OpeDiv::~OpeDiv(){}


//----------- OpeMul ---------------
OpeMul::OpeMul(char ope)
        { priority = 1 ; opeVal = ope ;}

double OpeMul::calcul(double val1, double val2) { return (val1 * val2) ;}
OpeMul::~OpeMul(){}


//----------------------- fin Operatoreux --------------------------------
