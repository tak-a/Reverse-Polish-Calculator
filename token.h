//token.h
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <vector>
#include <string>

#include "fonctions.h"
enum Kind {OPERATEUR, NOMBRE, VARIABLE, PARENTHESE, FONCTION}; // type possible d'un token


//----------------------------------- class Operatoreux -------------------------
class Operatoreux {

public:
  virtual double calcul(double val1, double val2) = 0 ;
  virtual ~Operatoreux();
  int getPriority();

protected:
  int priority; // priorite de l'operation
  char opeVal;
};

//----------------------------------- FIN class Operatoreux -------------------------


//----------------------------- polymorphisme de Operatoreux ------------------------
//operateur Addtion +
class OpePlus: public Operatoreux{

public:
  OpePlus(char ope);
  ~OpePlus();
  //OpePlus* clone();
  double calcul(double val1, double val2);
};


//operateur Soustraction -
class OpeMoins: public Operatoreux{

public:
  OpeMoins(char ope);
  ~OpeMoins();
  double calcul(double val1, double val2);
};


//operateur Division /
class OpeDiv: public Operatoreux{

public:
  OpeDiv(char ope);
  ~OpeDiv();
  double calcul(double val1, double val2);
};


//operateur Multiplication *
class OpeMul: public Operatoreux{

public:
  OpeMul(char ope);
  ~OpeMul();
  double calcul(double val1, double val2);
};

//----------------------------- FIN polymorphisme de Operatoreux ------------------------


//-------------------------- struct ExprToken ----------------------------

// structure Token : sera les element du vector , soit un entier, une variable
// ou un operateur
struct ExprToken{
  ExprToken(){}
  std::string string_value ; // variable juste pour afficher dans la sortie, pour verif fonction est correcte
  Kind typeToken ; // quel type de token est ce
  // soit un entier , soit un operateur, soit une variable, soit une parenthese, soit une fonction
  union{
    char parentheses;
    double number;
    Operatoreux* operateur;
    std::string* variableName;
    Fonctions* fonction ;
  };

  //destructor
  ~ExprToken(){};

};
