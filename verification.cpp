#include "verification.h"

//permet de savoir si ce string est un nombre negatif (return true s'il l'est)
bool isStringNegative(std::string strVal){
  if(strVal.length() > 1){
    return(strVal[0] == '-' && isdigit(strVal[1]));
  }
  else{
    return false ;
  }
}

//verifie que l'expression soit valide
void verifExprValide(std::vector<std::string> expr){
  int nbParentOpen =0 ;
  int nbParentClosed =0 ;
  //bool varOrFunction = false ; // init à c'est une variable

  //si l'expression est vide
  if(expr.empty()){
    throw std::string("Erreur, l'expression est vide.");
  }

  //si l'expression n'a qu'un argument et qu'il est ni un nombre, ni une variable
  if( expr.size() ==  1){
      if(!isdigit(expr[0][0]) && !isalpha(expr[0][0]) && !isStringNegative(expr[0])){
        throw std::string("Erreur, l'expression n'est pas correcte.");
      }
  }
  // si l'expression a pls args
  if (expr.size() >  1){
    // verifie le nombre de parenthese ouvrante
    for(size_t i = 0 ; i < expr.size() ; i++ ){
      if(expr[i] == "("){
        nbParentOpen++ ;
      }
      else if(expr[i] == ")"){
        nbParentClosed++;
      }
      //si une parenthese fermante est avant une ouvrante
      if(nbParentClosed > nbParentOpen){
        throw std::string("Erreur, parenthèse fermante avant une parenthèse ouvrante");
      }

      //si le string est vide, si ce n'est pas un nombre ou une variable ou une parenthese ou un operateur => exception
      if(expr[i] == "" || (!isdigit(expr[i][0]) && !isalpha(expr[i][0]) && !isStringNegative(expr[i]) && expr[i] != ","
      && expr[i] != "/" && expr[i] != "*" && expr[i] != "-" && expr[i] != "+" && expr[i] != "(" && expr[i] != ")" && expr[i] != ";" && expr[i][0] != '_')){
        throw std::string("Erreur, un caractère de l'expression est inconnu.");
      }
      if(expr[i][0] == '_'){
        for(size_t j = 1 ; j < expr[i].size(); j++){
          if(!isdigit(expr[i][j]))
            throw std::string("Erreur, char non valide pour le bind.");
        }
      }
      // si cet elem est un nombre (premier char est un digit) ou un nombre neg
      if(isdigit(expr[i][0]) || isStringNegative(expr[i])){
        for(size_t j = 1 ; j < expr[i].size(); j++){
          if(!isdigit(expr[i][j]) && expr[i][j] != '.')
            throw std::string("Erreur, char non valide dans un nombre.");
        }
      }
      //si le premier char de l'elem est une lettre
      else if(isalpha(expr[i][0])){
        for(size_t j = 1; j < expr[i].size(); j++){
          if( (!isdigit(expr[i][j])) && !isalpha(expr[i][0]) )
            throw std::string("Erreur, une variable ne peut posséder que des lettres et/ou des nombres dans son nom.");
        }
      }
      //si c'est un bind mais non suivie d'un nombre
      if(expr[i][0] == '_' && (expr[i].size() == 1 || !isdigit(expr[i][1])) ){
        throw std::string("Erreur, bind utilisé incorrectement.");
      }

      if(i < expr.size()-1){
        //si le string est une var ou nb et que le prochain n'est pas un operateur => exception
        if((isdigit(expr[i][0]) || isalpha(expr[i][0]) || isStringNegative(expr[i])) &&
        (isdigit(expr[i+1][0]) || isalpha(expr[i+1][0]) || isStringNegative(expr[i+1]))){
            throw std::string("Erreur, le nombre (ou variable), n'est pas suivi du bon élément.");
          }
        if((expr[i] == "/" || expr[i] == "*" || expr[i] == "-" || expr[i] == "+") &&
        (expr[i+1] == "/" || expr[i+1] == "*" || expr[i+1] == "-" || expr[i+1] == "+")){
          throw std::string("Erreur,  deux opérateurs à la suite.");
        }
      }
      // si c'est le dernier string, et que c'est pas une parenthese fermante, une variable, un nombre(pos ou neg)
      if((i == expr.size()-1) && (expr[i] != ")" && !isdigit(expr[i][0]) && !isalpha(expr[i][0]) && !isStringNegative(expr[i])) ){
        throw std::string("Erreur,  expression ne se termine pas correctement.");
      }
    }

  }

  //apres avoir verifier tout les arguments, verif le nbre de parenthese
 if(nbParentOpen != nbParentClosed){
    throw std::string("Erreur, nombre de parenthese ouvrant different du nombre de parenthèse fermante ");
  }

}

// regarde si cette declaration est une fonction ou une variable
// si une fonction est appelée sans le bon nb de param => declaration de fonction
// sinon => declaration variable
int isItFunction(std::vector<std::string> expression, std::vector<std::tuple<std::string, int>> *functions){
  int nbParam = 0  ;
  int functionParam = 0 ;
  std::string tmpstring ;
  int nbParenthese = 0 ;
  int nbBind = 0 ;
  std::vector<std::tuple<std::string, int>>::iterator it ;
  //si ce string est une fonction
  tmpstring = expression[0];
  if((it = std::find_if(functions->begin(), functions->end(),[tmpstring](const std::tuple<std::string, int>& tmp)
          {return std::get<0>(tmp) == tmpstring;})) != functions->end() ){ // si le premier elem est une fonction

    functionParam = std::get<1>(*it) ; // nb param voulu pour cette fonction

    int i = 2 ;//pas besoin de verifier si le suivant est bien une parenthese, cette fct est appelée apres verifExprValide()
    while(nbParenthese >= 0){ // tant qu'on n'a pas fini les param de la fonction
      if(expression[i] == "(") nbParenthese++ ;
      else if(expression[i] == ")") nbParenthese--;
      else if(expression[i] == "," and nbParenthese == 0){ // si c'est fin d'un param ET que c'est la fonction globale
        nbParam++ ;
      }
      else if(expression[i][0] == '_'){
        nbBind++ ;
      }
      i++;
    }
    nbParam++ ;
    if(i == (int)expression.size() || expression[i+1] == ";") { // si c'est la fin de la declaration donc possiblement une fonction partielle
      if(expression[0] == "polynome"){ //si c'est polynome, donc fonction partielle direct
        if(functionParam - nbParam <= 0) return(nbBind); // s'il y a au moins 3 params, on veut juste au moins 1 de plus et les possible bind
        else{return functionParam - nbParam +nbBind ; }
      }
      else{
        return(functionParam - nbParam +nbBind);
      }
    }
    else{
      return 0 ;
    }

    //break ;
  }
  else{ // sinon alors renvoie direct 0 car declaration variable
    return 0 ;
  }
}





std::vector<std::string> splitAny(std::string expression, char delim){
  std::string tmpstring ;
  std::string previousString =" ";
  std::size_t found ;
  std::stringstream ss(expression) ;
  std::string tmp = "";
  std::vector<std::string> stringVector ;

  while(std::getline(ss, tmpstring, delim)){
    if(tmpstring != "" && tmpstring != ";"){

      //check si un ';' est "attaché " a un nombre/variable
      if((found = tmpstring.find(";")) != std::string::npos){
        tmpstring = tmpstring.substr(0, found);
      }

      for(size_t i =0 ; i < tmpstring.length() ; i++){
        tmp ="";
        if(isdigit(tmpstring[i]) != 0){
          //tant que c'est un chiffre, ou un . pour les decimaux
          while(isdigit(tmpstring[i]) != 0 || tmpstring[i] == '.'){
            tmp +=tmpstring[i] ;
            i++ ;
          }
          i--;
        }
        else if(isalpha(tmpstring[i]) != 0){
          while(isalpha(tmpstring[i]) != 0 || isdigit(tmpstring[i]) != 0){
            tmp +=tmpstring[i];
            i++ ;
          }
          i--;

        }
        //s'il s'agit d'un bind
        else if(tmpstring[i] == '_' && isdigit(tmpstring[i+1])){
          tmp += tmpstring[i];
          i++ ;
          while(isdigit(tmpstring[i])){
            tmp += tmpstring[i];
            i++ ;
          }

          i-- ;
        }
        //si c'est un nombre negatif
        else if(tmpstring.length() > 1 && tmpstring[i] == '-' && isdigit(tmpstring[i+1])
        && !isdigit(previousString[0]) && !isalpha(previousString[0]) ){
          tmp +=tmpstring[i] ;
          i++;
          //tant que c'est un chiffre, ou un . pour les decimaux
          while(isdigit(tmpstring[i]) != 0 || tmpstring[i] == '.'){
            tmp +=tmpstring[i] ;
            i++ ;
          }
          i--;
        }
        // si c'est une parenthese ou un operateur
        else if(tmpstring[i] == '(' || tmpstring[i] == ')' || tmpstring[i] == '/' || tmpstring[i] == '*'
        || tmpstring[i] == '-' || tmpstring[i] == '+' || tmpstring[i] == ','){
          tmp+= tmpstring[i];
        }
        stringVector.push_back(tmp);
        previousString = tmp ;
      }
    }

  }
  return stringVector;
}
