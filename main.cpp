#include <iostream>
#include <sstream>


#include "program.h"



//------------------------------- MAIN -----------------------------

int main(){

  Program prog ;

  prog.getEntry();

  //std::cout << "apres getentry" << std::endl;
  prog.stringVecToExprVec();
  //std::cout << "apres strinvecto.." << std::endl;
  prog.calculEachExpression();

  std::cout << "\n" << "Memoire : " << std::endl;
  std::map<std::string, Expr> tmpMap = prog.getMapVariables() ;
  for(std::map<std::string,Expr>::iterator it = tmpMap.begin(); it != tmpMap.end(); ++it) {
    std::cout << it->first << " = " << it->second.getExpression() << std::endl;
  }
  std::cout << std::endl;

  return 0 ;
}
