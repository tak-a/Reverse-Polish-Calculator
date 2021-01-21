//fonctions.cpp
#include "fonctions.h"

//-------------------- Fonctions ------------------------
int Fonctions::getNumberOfParam(){ return nbParam;}
Fonctions::~Fonctions(){}

int Fonctions::getNbParam(){
  return nbParam ;
}

void Fonctions::setNumberOfParam(int numberParam){
  nbParam = numberParam ;
}

std::string Fonctions::getFctName(){
  return fctName;
}

//--------------------- FonctionSin ----------------------------
FonctionSin::FonctionSin(std::string name){
  nbParam = 1;
  fctName = name;
}

FonctionSin::~FonctionSin(){}

double FonctionSin::fctCalcul(std::vector<double> params){
  return sin(params[0]*3.14159265/180);
}


//-----------------------FonctionCos -------------------------
FonctionCos::FonctionCos(std::string name){
  nbParam = 1 ;
  fctName = name;
}
FonctionCos::~FonctionCos(){}

double FonctionCos::fctCalcul(std::vector<double> params){
  return cos(params[0]*3.14159265/180);
}


//----------------------- FonctionTan -----------------------
FonctionTan::FonctionTan(std::string name){
  nbParam = 1 ;
  fctName = name ;
}

FonctionTan::~FonctionTan(){}


double FonctionTan::fctCalcul(std::vector<double> params){
  return tan(params[0]*3.14159265/180);
}

//------------------------- FonctionSqrt --------------------

FonctionSqrt::FonctionSqrt(std::string name){
  nbParam = 1 ;
  fctName = name ;
}

FonctionSqrt::~FonctionSqrt(){}

double FonctionSqrt::fctCalcul(std::vector<double> params){
  return sqrt(params[0]);
}


//------------------------- FonctionLog --------------------

FonctionLog::FonctionLog(std::string name){
  nbParam = 1 ;
  fctName = name ;
}

FonctionLog::~FonctionLog(){}

double FonctionLog::fctCalcul(std::vector<double> params){
  return log(params[0]);
}

//------------------------- FonctionExp --------------------

FonctionExp::FonctionExp(std::string name){
  nbParam = 1 ;
  fctName = name ;
}

FonctionExp::~FonctionExp(){}

double FonctionExp::fctCalcul(std::vector<double> params){
  return exp(params[0]);
}

//------------------------- FonctionPow --------------------

FonctionPow::FonctionPow(std::string name){
  nbParam = 2 ;
  fctName = name ;
}

FonctionPow::~FonctionPow(){}

double FonctionPow::fctCalcul(std::vector<double> params){
  return pow((params[0]),(params[1]));
}


//------------------------- FonctionHypot --------------------

FonctionHypot::FonctionHypot(std::string name){
  nbParam = 2 ;
  fctName = name ;
}

FonctionHypot::~FonctionHypot(){}

double FonctionHypot::fctCalcul(std::vector<double> params){
  return hypot((params[0]),(params[1]));
}

//------------------------- FonctionLerp --------------------

FonctionLerp::FonctionLerp(std::string name){
  nbParam = 3 ;
  fctName = name ;
}

FonctionLerp::~FonctionLerp(){}

double FonctionLerp::fctCalcul(std::vector<double> params){
  return (params[0]+ (params[2])*(params[1]-params[0]) );
}

//------------------------- FonctionPoly --------------------

FonctionPoly::FonctionPoly(std::string name){
  // on initialise a 3 car c'est le nb min de param ;
  nbParam = 3 ;
  fctName = name ;
}

FonctionPoly::~FonctionPoly(){}

double FonctionPoly::fctCalcul(std::vector<double> params){
  int degre = params[0]; // degre en int, au cas ou l'user ai rentré un float
  int tailleParam = (int)params.size() -1 ;
  double result = 0 ;
  if(params[0] < 0) throw std::string("le degré du polynome n'est pas correct.");
  // on part de l'avant dernier param, donc du dernier coef
  for(int i = tailleParam -1 ; i > 0 ; i--){
    // si on a atteint degre = 0 alors on add simplement le coef
    if(degre == 0){
      result += params[i];
    }
    // sinon on fait le coef * (la variable ^ degre)
    else {
      result += params[i] * pow(params[tailleParam], degre);
      degre--;
    }

  }
  return result;
}
