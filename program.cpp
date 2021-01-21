//program.cpp
#include <map>
#include "program.h"
#include <tuple>

//---------------------- class Program -----------------------
Program::Program(){
  //init la list qui contient le nom des fonctions prisent en charge
  ListOfFunctions.push_back(std::tuple<std::string, int> ("cos",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("sin",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("tan",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("sqrt",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("log",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("exp",1));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("pow",2));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("hypot",2));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("lerp",3));
  ListOfFunctions.push_back(std::tuple<std::string, int> ("polynome",3));

}

Program::~Program(){}


//--------- getEntry : recup ce qui est tapé au clavier ------------
void Program::getEntry(){
  std::string tmpstring;
  std::string expressions ;
  size_t found ;
  std::vector<std::tuple<std::string, int >>::iterator it ;
  Expr newExpr;
  int paramPartialFunc = 0 ; // nbparam si functionpartielle ;

  std::cout << "Entrez la(les) expressions (terminez une expression par ';' ou '\\n') :" << std::endl;
  std::cout << "(Pour terminer la saisie, entrez le mot 'quit' et appuyez sur la touche 'entrée')" << std::endl;
  // on recupere chaque ligne qui sont entrées
  while (std::getline(std::cin, expressions) && expressions != "quit") {
    // ajoute pas une expr vide et si c'est une assignation de variable (presente d'un = )
    //alors ajout dans la map de variables et dans vector des expressions
    if(expressions != "" && (found = expressions.find("=")) != std::string::npos){
      // au cas ou il y aurait des espaces apres le = , ex: rayon = 3*8;
      int tmp = found ;
      while(expressions[tmp+1] == ' '){
        tmp++;
      }
      tmpstring = expressions.substr(0,found-1) ;
      //si le nom de la variable crspd à une fonction
      if((it = std::find_if(ListOfFunctions.begin(), ListOfFunctions.end(),[&tmpstring](const std::tuple<std::string, int>& tmp) {return std::get<0>(tmp) == tmpstring;})) != ListOfFunctions.end()){
        //throw std::string("Erreur, impossible d'utiliser le nom d'une fonction comme nom de variable");
        std::cout << "Erreur, impossible d'utiliser le nom d'une fonction comme nom de variable : " << std::get<0>(ListOfFunctions[it - ListOfFunctions.begin()]) << std::endl;
      }
      else {
        while(expressions[found-1] == ' '){
          found--;
        }

        //on verifie que le nom de la variable est valable.
        if(splitAny(expressions.substr(0,found), ' ').size() > 1 || !isalpha(expressions.substr(0,found)[0])){
          //throw std::string("Erreur, nom de la variable nom correcte.");
          std::cout << "Erreur, nom de la variable nom correcte : " << expressions.substr(0,found) << std::endl;
        }
        else {
          try{
            verifExprValide(splitAny(expressions.substr(tmp+1), ' '));

            paramPartialFunc = isItFunction(splitAny(expressions.substr(tmp+1), ' '), &ListOfFunctions);
            //si c'est une fonction(c-a-d qu'on renvoie la difference du nombre de param que la fonction devrait avoir - ce qu'on compter comme param )
            if( paramPartialFunc > 0){
              ListOfFunctions.push_back(std::tuple<std::string, int> (expressions.substr(0,found), paramPartialFunc));
              variables[expressions.substr(0,found)] = (Expr(expressions.substr(tmp+1)));
            }
            //sinon variable
            else if(paramPartialFunc == 0){
              variables[expressions.substr(0,found)] = (Expr(expressions.substr(tmp+1)));
              expressionsString.push_back(expressions.substr(tmp+1));
            }
            else{
              throw std::string("Erreur declaration fonction partielle.") ;
            }

          }
          catch(std::string const& erreur){
            std::cerr << erreur << std::endl;
          }

        }
      }
    }
    else if(expressions != "") {
      try{
        verifExprValide(splitAny(expressions, ' '));
        expressionsString.push_back(expressions);
      }
      catch(std::string const& erreur){
        std::cerr << erreur << std::endl;
      }
    }
  }
}


// convertir le vector de string en vector d'expressions
void Program::stringVecToExprVec(){
  for(size_t i = 0 ; i < expressionsString.size() ; i++){
    Expr expression = Expr(expressionsString[i]);
    expressionsVector.push_back(expression);
  }
}


//---- calculEachExpression : calcul et renvoie le resultat des expressions
void Program::calculEachExpression(){
  std::vector<double> results ;
  double resultat ;
  std::size_t found ;

  std::cout << std::endl << "Resultat : " << std::endl;
  for(size_t i = 0 ; i < expressionsVector.size() ; i++){

    //decoupe l'expression avec la bonne fonction
    expressionsVector[i].setStringVector(splitAny(expressionsVector[i].getExpression(), ' ')) ;
    //ordonne l'expression en RPN
    try{
      expressionsVector[i].gestionTokenVector(&ListOfFunctions, &variables) ;
    }
    catch(std::string const& erreur){
      std::cerr << "Dans gestionTokenVector , calculEachExpression : " << erreur << std::endl;
      exit(1);
    }

    //recupere le resultat de l'operation
    try{
      resultat = expressionsVector[i].eval(&variables, &ListOfFunctions) ;
      results.push_back(resultat);
      std::string currentExpr = expressionsVector[i].getExpression();

      //si l'expression a un ';' a la fin, on ne l'affiche pas
      found = currentExpr.find(";");
      if(found == std::string::npos){
        std::cout << resultat << std::endl;
      }
    }
    catch(std::string const& erreur){
      std::cerr << erreur << std::endl;
    }

  }
}

//getter map
const std::map<std::string, Expr>& Program::getMapVariables(){
  return variables;
}
