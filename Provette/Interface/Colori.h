#ifndef Colori_h
#define Colori_h

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~ Implementazione dell'utility Colori                     ~
  ~ Autori: Racca Eleonora - eleonora.racca288@edu.unito.it ~
  ~         Sauda Cristina - cristina.sauda@edu.unito.it    ~
  ~ Ultima modifica: 22/09/2018                             ~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <sstream> 

namespace colore{
  class Colori{
  public:
  Colori(const std::string& stringa)
    : stringa_(stringa)
      , coda_("\033[00m"){}
    
    template<typename T>
      std::string operator()(T i){
      
      std::ostringstream s1;
      s1 << stringa_ << i << coda_;
      return s1.str();
    }
  
  protected:
    std::string stringa_;
    std::string coda_;
    
  };
  
  static Colori Rosso("\033[0;31m");

  static Colori Verde("\033[0;32m");
  
  static Colori Arancione("\033[0;33m");
  
  static Colori Blu("\033[0;34m");

  static Colori Violetto("\033[0;35m");

  static Colori Azzurro("\033[0;36m");

  static Colori Grigio("\033[0;37m");
  
  static Colori Errore("\033[0;41m");
  
  static Colori Avvertimento("\033[0;43m");
}

#endif
