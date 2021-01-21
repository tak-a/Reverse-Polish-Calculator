//fonctions.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

class Fonctions{

public:
  virtual ~Fonctions();
  virtual double fctCalcul(std::vector<double> params) = 0 ;
  int getNumberOfParam();
  void setNumberOfParam(int numberParam);
  void setfctParam(std::vector<double> paramList);
  int getNbParam();
  std::string getFctName();
protected:
  int nbParam;
  std::string fctName;

};

class FonctionSin: public Fonctions{

public:
  FonctionSin(std::string name);
  ~FonctionSin();
  double fctCalcul(std::vector<double> params);

};


class FonctionCos: public Fonctions{

public:
  FonctionCos(std::string name);
  ~FonctionCos();
  double fctCalcul(std::vector<double> params);
};


class FonctionTan: public Fonctions{

public:
  FonctionTan(std::string name);
  ~FonctionTan();
  double fctCalcul(std::vector<double> params);
};

class FonctionSqrt: public Fonctions{

public:
  FonctionSqrt(std::string name);
  ~FonctionSqrt();
  double fctCalcul(std::vector<double> params);
};

class FonctionLog:public Fonctions{

public:
  FonctionLog(std::string name);
  ~FonctionLog();
  double fctCalcul(std::vector<double> params);
};

class FonctionExp:public Fonctions{

public:
  FonctionExp(std::string name);
  ~FonctionExp();
  double fctCalcul(std::vector<double> params);
};

class FonctionPow:public Fonctions{

public:
  FonctionPow(std::string name);
  ~FonctionPow();
  double fctCalcul(std::vector<double> params);
};

class FonctionHypot:public Fonctions{

public:
  FonctionHypot(std::string name);
  ~FonctionHypot();
  double fctCalcul(std::vector<double> params);
};

class FonctionLerp:public Fonctions{

public:
  FonctionLerp(std::string name);
  ~FonctionLerp();
  double fctCalcul(std::vector<double> params);
};

class FonctionPoly:public Fonctions{

public:
  FonctionPoly(std::string name);
  ~FonctionPoly();
  double fctCalcul(std::vector<double> params);
};
