//program.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "expression.h"



class Program {

public:
  Program();
  ~Program();
  void getEntry();
  void stringVecToExprVec();
  void calculEachExpression();
  const std::map<std::string, Expr>& getMapVariables();

protected:
    std::vector<std::string> expressionsString ;
    std::vector<Expr> expressionsVector ;
    std::map<std::string, Expr> variables ;
    std::vector< std::tuple<std::string, int >> ListOfFunctions ;

};
