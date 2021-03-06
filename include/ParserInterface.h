#ifndef PARSERINTERFACE_H
#define PARSERINTERFACE_H

/*!
 * \file ParserInterface.h
 * \brief Base de tous parser de fichier.
 * \author Antoine Amara, Jean-Fréderic Durand.
 * \version 1.0
 */

/*! \class ParserInterface
   * \brief Interface permettant de construire la base sur laquelle on implémente un parser.
   *
   *  Cette classe contient les méthodes de base de tout parser.
   */

class ParserInterface {

public:
  /*!
     *  \brief Méthode permettant de parser un fichier.
     *
     *  On récupère ici tous les éléments utiles du fichier à parser.
     */
  virtual void parse() = 0;
};

#endif
