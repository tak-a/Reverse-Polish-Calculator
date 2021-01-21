#include "expression.h"

// Construit une expression a partir du texte
Expr::Expr(){ expression = "" ;};

Expr::Expr ( std::string str ){
 expression = str ;
};
int rang ;
// --------destructor
Expr::~Expr(){};

//------------------------ordonne vector, verif type general ----------------
void Expr::ordonneTokenVector(std::vector<ExprToken> *tokenVector, std::stack<ExprToken> *pile,  std::string token){
  ExprToken tmp ;
  char valueTMP ;
  tmp.string_value = token;

  // si ce string est un nombre
  if(isdigit(token[0]) != 0){
    tmp.number = stod(token);
    tmp.typeToken = NOMBRE ;
    tokenVector->push_back(tmp);
  }

  // si le token est une variable
  else if(isalpha(token[0]) != 0){
    tmp.typeToken = VARIABLE ;
    (tmp.variableName) = new std::string(token) ;
    tokenVector->push_back(tmp);
  }

  //gestion nbre neg
  else if(token.size() > 1 && token[0] == '-' && isdigit(token[1]) != 0){
    tmp.number = stod(token);
    tmp.typeToken = NOMBRE ;
    tokenVector->push_back(tmp);
  }

  else{
    tmp.typeToken = OPERATEUR ;
    // en fonction de l'operateur, on instancie le bon
    switch(valueTMP = *(token.c_str())){
      case '+':
        tmp.operateur = new OpePlus('+');
        break;
      case '-':
        tmp.operateur = new OpeMoins('-');
        break;
      case '/':
        tmp.operateur = new OpeDiv('/');
        break;
      case '*':
        tmp.operateur = new OpeMul('*');
        break;
      //si on arrive dans le default, alors ni un int, ni une var, ni un operateur
      default:
        throw std::string("Erreur, cet élément n'est ni un nombre, ni une variable ou fonction, ni un operateur. ");
        break;
    }
    //si pile vide, push l'ope dans la pile
    if(pile->empty()){
      pile->push(tmp);
    }
    //sinon retire, top of pile, check prio et add dans vector si prio pile est supp
    else{
      while(!pile->empty()){
        if(pile->top().operateur->getPriority() < tmp.operateur->getPriority()){
          tokenVector->push_back(pile->top());
        }
        else{
          break;
        }
        pile->pop();
      }
      pile->push(tmp);
    }
  }
}

// gestionFonctionPartielle
void Expr::gestionFonctionPartielle(std::vector<std::tuple<std::string, int>> *functions, std::map<std::string, Expr> *variables, std::string fctName,
                                    std::vector<ExprToken> *vectorToReturn, size_t *i, int rangFunction, std::vector<std::string> vectorOfString ){

  std::vector<std::string> declarationFuncPar =splitAny((variables->find(fctName)->second).getExpression(), ' ');
  int nbParenthese = 0 ;
  std::vector<std::vector <std::string> > Vecofvecofstring ; // contiendra les param de la fonction partielle
  std::vector<std::string> tmpParam ;
  std::string nomFuncAppelee = declarationFuncPar[0];
  std::vector<std::tuple<std::string, int>>::iterator it ;

  //on recupere les parametres
  (*i)++; // on est au rang de la parenthese ouvrante
  (*i)++ ;
  while(nbParenthese >= 0){ // tant qu'on n'a pas fini les param de la fonction
    if(vectorOfString[*i] == "(") {
      nbParenthese++ ;
      tmpParam.push_back(vectorOfString[*i]) ;
    }
    else if(vectorOfString[*i] == ")"){
      nbParenthese--;
      if(nbParenthese >=0) tmpParam.push_back(vectorOfString[*i]);
    }
    else if(vectorOfString[*i] == "," && nbParenthese == 0){
      Vecofvecofstring.push_back(tmpParam);
      tmpParam.clear();
    }
    else{
      tmpParam.push_back(vectorOfString[*i]);
    }
    (*i)++;
  }
  (*i)--;
  Vecofvecofstring.push_back(tmpParam);

  ///verif que le nb param est le bon
  if(declarationFuncPar[0] != "polynome"){
    if((std::get<1>((*functions)[rangFunction])) != (int)Vecofvecofstring.size() ) throw std::string("Erreur, nombre de parametre.") ;
  }
  else{ // pour polynome, on verifie juste qu'il y a assez de parametres
    if((std::get<1>((*functions)[rangFunction])) > (int)Vecofvecofstring.size() ) throw std::string("Erreur, nombre de parametre (polynome).") ;
  }

  int nbParamAdded = 0; //nombre de parametre de ajoute
  std::vector<int> paramAdded ; // rang des param ajotues



  // la taille de j et de declarationFuncPar peut bouger car on y ajoute des valeurs, donc crée des variables
  int sizeofDFP = (int)declarationFuncPar.size() ;
  int j = 0 ;
  while(j < sizeofDFP){

    // s'il s'agit d'un bind
    if(declarationFuncPar[j][0] == '_'){
      int rangParam = std::stoi(declarationFuncPar[j].substr(1)); // on veut savoir quel param de la fonc partielle a inserer a la place
      if(rangParam > (int)Vecofvecofstring.size()) throw std::string("Erreur avec bind valeur.") ;// si le bind demande un param non existant
      declarationFuncPar[j] = Vecofvecofstring[rangParam-1][0];

      // pour rajouter le nouveau param a la declaration
      std::vector<std::string> vectorforDFP;
      for(int c = 0 ; c < j+1 ; c++){
        vectorforDFP.push_back(declarationFuncPar[c]);
      }
      for(size_t c = 1 ; c < Vecofvecofstring[rangParam-1].size() ; c ++){
        vectorforDFP.push_back( Vecofvecofstring[rangParam-1][c]);
      }

      for(size_t c = j+1 ; c < declarationFuncPar.size() ; c ++){
        vectorforDFP.push_back( declarationFuncPar[c]);
      }

      declarationFuncPar.clear();
      declarationFuncPar = vectorforDFP;
      vectorforDFP.clear();
      j += ((int)Vecofvecofstring[rangParam-1].size())-1 ;
      sizeofDFP += ((int)Vecofvecofstring[rangParam-1].size())-1 ;
      paramAdded.push_back(rangParam-1) ;
      nbParamAdded++ ;
    }
    j++ ;
  }
  // s'il reste des params à Add, on se position a la fin de la declaration
  std::vector<std::string>::iterator rangAjout ;
  if(declarationFuncPar[declarationFuncPar.size()] == ";"){
    rangAjout = declarationFuncPar.end() - 2 ;
  }
  else{
    rangAjout = declarationFuncPar.end() - 1 ;
  }
  //s'il reste des params à ajouté
  for(size_t j = 0 ; j < Vecofvecofstring.size() ; j++){
    if(std::find(paramAdded.begin(), paramAdded.end(), (int)j) == paramAdded.end()){
      rangAjout = declarationFuncPar.insert(rangAjout,Vecofvecofstring[j].begin(), Vecofvecofstring[j].end()) ;
      rangAjout = declarationFuncPar.insert(rangAjout,",") ;
    }
  }


  //on cherche le rang de la fonction appele dans fonction partielle
  (it = std::find_if(functions->begin(), functions->end(),[&nomFuncAppelee](const std::tuple<std::string, int>& tmp)
  {return std::get<0>(tmp) == nomFuncAppelee;})) ;
  size_t depart = 0 ;
  gestionFonction(functions, &depart , declarationFuncPar, it-functions->begin(), variables,  vectorToReturn );

}




//-----------------------------gestionFonction : permet de creer un ExprToken de type fonction
//les params de cette dite fonction seront stock dans l'objet fonction et non present dans le vector de ExprToken
void Expr::gestionFonction(std::vector<std::tuple<std::string, int>> *functions, size_t *i, std::vector<std::string> vectorOfString, int rangFunction,
std::map<std::string, Expr> *variables, std::vector<ExprToken> *vectorToReturn){
  int nbParam  = 0;
  ExprToken token ;
  std::stack<ExprToken> newPile ;
  token.typeToken = FONCTION ;
  std::string fonctionName = vectorOfString[*i];

  //en fonction du rang auquel on a trouvé le nom de la fonction dans la liste
  // on crée la bonne instance
  (*i)++;
  switch(rangFunction){
    case 0:
      token.fonction = new FonctionCos(vectorOfString[(*i)-1]);
      break;
    case 1:
      token.fonction = new FonctionSin(vectorOfString[(*i)-1]);
      break;
    case 2:
      token.fonction = new FonctionTan(vectorOfString[(*i)-1]);
      break;
    case 3:
      token.fonction = new FonctionSqrt(vectorOfString[(*i)-1]);
      break;
    case 4:
      token.fonction = new FonctionLog(vectorOfString[(*i)-1]);
      break;
    case 5:
      token.fonction = new FonctionExp(vectorOfString[(*i)-1]);
      break;
    case 6:
      token.fonction = new FonctionPow(vectorOfString[(*i)-1]);
      break;
    case 7:
      token.fonction = new FonctionHypot(vectorOfString[(*i)-1]);
      break;
    case 8:
      token.fonction = new FonctionLerp(vectorOfString[(*i)-1]);
      break;
    case 9:
      token.fonction = new FonctionPoly(vectorOfString[(*i)-1]);
      break;
    default:
      throw std::string("erreur definition Fonction");
      break;
  }

  (*i)++ ;
  if(vectorOfString[*i] == ")"){
    throw std::string("pas de paramètres pour la fonction appelée !");
  }
  //tant qu'on n'a pas la fin des parametres
  while(vectorOfString[*i] != ")"){
    //si c'est une ',' alors c'est le premier param de la fonction : on a ajoute les var et nb, mtn on add les ope
    if(vectorOfString[*i] == ","){
      nbParam++ ;
      while(!newPile.empty()){
        vectorToReturn->push_back(newPile.top());
        newPile.pop();
      }
    }
    else{
      orderingTokenVector(functions, variables, vectorOfString, i, vectorToReturn, &newPile);
    }
    (*i)++ ;
  }
  //le string est ")" donc c'est le dernier param
  nbParam ++;
  while(!newPile.empty()){
    vectorToReturn->push_back(newPile.top());
    newPile.pop();
  }


  //si ce n'est pas une declaration de fonction partielle, donc s'il n'y a

  // si c'est la fct polynome, on verif seulement que le nb de param est suffisant
  if(token.fonction->getFctName() == "polynome"){
    if( nbParam < token.fonction->getNbParam() /*|| paramList[0] < 0*/ ){
      throw std::string("erreur Nb Param poly");
    }
    else{
      token.fonction->setNumberOfParam(nbParam);
    }
  }
  else if(nbParam != token.fonction->getNbParam()){
    throw std::string("erreur Nb Param");
  }

  token.string_value = fonctionName ;
  vectorToReturn->push_back(token);
}

// ------------------------- orderingTokenVector ------------------
// ordonne en RPN chaque element,  gere les parentheses, appelle les fonctions correspondante si string est une fonction, ou une variable, ou autre
void Expr::orderingTokenVector(std::vector<std::tuple<std::string, int>> *functions, std::map<std::string, Expr> *variables,
std::vector<std::string> vectorOfString, size_t *i, std::vector<ExprToken> *vectorToReturn, std::stack<ExprToken> *pile){

  std::vector<std::tuple<std::string, int>>::iterator it ;
  std::string vectmp = vectorOfString[*i];
  if(vectorOfString[*i] == "("){
    std::stack<ExprToken> newPile ;
    *i = *i + 1 ;

    // tant qu'on trouve pas la parenthese fermante
    while (vectorOfString[*i] != ")"){
      orderingTokenVector(functions, variables, vectorOfString, i, vectorToReturn, &newPile);
      *i = *i + 1 ;
    }
    while(!newPile.empty()){
      vectorToReturn->push_back(newPile.top());
      newPile.pop();
    }
  }
  // check si cette variable est une fonction enregistrée
  else if((it = std::find_if(functions->begin(), functions->end(),[&vectmp](const std::tuple<std::string, int>& tmp) {return std::get<0>(tmp) == vectmp;})) != functions->end()){
    if(vectorOfString[(*i)+1] != "("){
      throw std::string("Erreur, une fonction est appelée sans paramètres");
    }
    else{
      try{
        //donc fonction partielle caar n'est pas dans les 1à fonctions de bases
        if(it-functions->begin() > 9){
          gestionFonctionPartielle(functions, variables, vectorOfString[(*i)], vectorToReturn, i, it-functions->begin(), vectorOfString );
        }
        else {
          gestionFonction(functions, i, vectorOfString, it-functions->begin(), variables, vectorToReturn);
        }

      }
      catch(std::string const& erreur){
        std::cerr << erreur << std::endl;
        exit(1);
      }
    }
  }
  //si ce n'est pas une fonction , ordonne le vecteur normalement
  else{
    ordonneTokenVector(vectorToReturn, pile, vectorOfString[*i]);
  }

}

// ------------------------------------------------------------
// --------- function qui va gérer la creation de la RPN pour cette expression ----------------------------
void Expr::gestionTokenVector(std::vector<std::tuple<std::string, int>> *functions, std::map<std::string, Expr> *variables){
  std::stack<ExprToken> pile ;
  std::vector<ExprToken> vectorToReturn ;

  //pour chaque string du vecteur, on l'ordonne en RPN
  for(size_t i = 0 ; i < stringVector.size() ; i++ ){
    orderingTokenVector(functions, variables, stringVector, &i, &vectorToReturn, &pile);
  }

  while(!pile.empty()){
    vectorToReturn.push_back(pile.top());
    pile.pop();
  }

  this->tokenVector = vectorToReturn ;
}


//------------------------------
//--------- GETTER ANS SETTER TOKENVECTOR
const std::vector<ExprToken>& Expr::getTokenVector(){
  return tokenVector;
}
void Expr::setTokenVector(std::vector<ExprToken> vecParam){
   tokenVector = vecParam;
}
const std::string& Expr::getExpression(){
  return expression;
}
const std::vector<std::string>& Expr::getStringVector(){
  return stringVector;
}
void Expr::setStringVector(std::vector<std::string> stringVecParam){
  stringVector = stringVecParam;
}



// Retourne la valeur de l'expression
double Expr::eval( std::map<std::string, Expr> *variables, std::vector<std::tuple<std::string, int>> *functions){
  std::stack<double> pileResult ;
  double val1, val2, tmp;
  std::vector<double> paramForFunctions ;
  if(!tokenVector.empty()){
    for(size_t i = 0 ; i < tokenVector.size() ; i++){

      if(tokenVector[i].typeToken == NOMBRE ){
        pileResult.push(tokenVector[i].number);
      }
      else if(tokenVector[i].typeToken == VARIABLE){
        std::map<std::string,Expr>::iterator it = variables->find(*(tokenVector[i].variableName)) ;

        // si la cle (la variable) est enregistré en mémoire
        if( it != variables->end()){
          Expr expressionOfVariable =(variables->find(*(tokenVector[i].variableName))->second);
          expressionOfVariable.setStringVector(splitAny(expressionOfVariable.getExpression(), ' ')) ;
          try{
            expressionOfVariable.gestionTokenVector(functions, variables) ;
          }
          catch(std::string const& erreur){
            std::cerr << "Dans eval , orderedVector : " << erreur << std::endl;
          }
          double val = expressionOfVariable.eval(variables, functions);
          pileResult.push(val);
        }
        else{
          //throw error here
          throw std::string("Attention, cette variable n'est pas en mémoire");
        }
      }
      //si le token est une fonction, on calcul !
      else if(tokenVector[i].typeToken == FONCTION){
        for(int j =0 ; j < tokenVector[i].fonction->getNbParam() ; j++){
          paramForFunctions.push_back(pileResult.top());
          pileResult.pop();
        }
        std::reverse(paramForFunctions.begin(), paramForFunctions.end());
        try{
          pileResult.push(tokenVector[i].fonction->fctCalcul(paramForFunctions));
          paramForFunctions.clear();
        }
        catch(std::string const& erreur){
          std::cerr << erreur << std::endl;
          exit(1);
        }

      }
      else{
        val1 = pileResult.top();
        pileResult.pop();
        val2 = pileResult.top();
        pileResult.pop();
        tmp = tokenVector[i].operateur->calcul(val2, val1);
        pileResult.push(tmp);
      }

    }
  }
  return pileResult.top() ;
}
