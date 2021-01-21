#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include "token.h"
#include "verification.h"

//-------------------- classe Expr --------------------------------

//class Expr pour reconnaitre une expression mathematiques simples
class Expr {
public:

  Expr();

  Expr( std::string str );

  ~Expr();

  void gestionFonctionPartielle(std::vector<std::tuple<std::string, int>> *functions, std::map<std::string, Expr> *variables, std::string fctName,
  std::vector<ExprToken> *vectorToReturn, size_t *i, int rangFunction , std::vector<std::string> vectorOfString);

  void gestionFonction(std::vector<std::tuple<std::string,int>> *functions, size_t *i, std::vector<std::string> vectorOfString, int rangFunction,
  std::map<std::string, Expr> *variables, std::vector<ExprToken> *vectorToReturn);

  void orderingTokenVector(std::vector<std::tuple<std::string,int>> *functions, std::map<std::string, Expr> *variables,
  std::vector<std::string> vectorOfString, size_t *i, std::vector<ExprToken> *vectorToReturn, std::stack<ExprToken> *pile);

  void gestionTokenVector(std::vector<std::tuple<std::string,int>> *functions, std::map<std::string, Expr> *variables);

  const std::vector<ExprToken>& getTokenVector();

  void setTokenVector(std::vector<ExprToken> vecParam);

  const std::string& getExpression();

  const std::vector<std::string>& getStringVector();

  void setStringVector(std::vector<std::string> stringVecParam);

  void ordonneTokenVector(std::vector<ExprToken> *tokenVector,std::stack<ExprToken> *pile,  std::string token);

  double eval( std::map<std::string, Expr> *variables, std::vector<std::tuple<std::string,int>> *functions);

private:
  std::string expression ; // l'expression simple en string
  std::vector<std::string> stringVector ; //expression "decoupee" dans un vector
  std::vector<ExprToken> tokenVector ; // expression avec tokens
};
//----------------------- fin class Expr ------------------------------
