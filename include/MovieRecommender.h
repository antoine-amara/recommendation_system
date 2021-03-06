#ifndef MOVIERECOMMENDER_H
#define MOVIERECOMMENDER_H

#include <string>
#include <vector>
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_rng.h>
#include <ctime>

#include "AiInterface.h"
#include "DataParser.h"
#include "mode.h"

class Saver;

/*!
 * \file MovieRecommender.h
 * \brief Algorithme d'apprentissage permettant de recommander des films à un utilisateur.
 * \author Antoine Amara, Jean-Fréderic Durand.
 * \version 1.0
 */

 /*! \class MovieRecommender
   * \brief Classe représentant l'algorithme de recommandation de film.
   *
   *  La classe gère la représentation des données, l'apprentissage, la sauvegarde des résultats et bien sûr la recommandation.
   *
   *  Notes concernant les matrices :
   *
   *   Matrice pour les données d'entrainements :
   *   - les lignes représentes les id des films.
   *   - les colonnes représentes les id des utilisateurs.
   *
   *   Matrice pour les recommandations (récupération des titres et des genres) :
   *
   *   - les Genres sont organisés de la façon suivante : vector[id_genre], et réciproquement pour récupérer
   *  les titres de films.
   */

class MovieRecommender : public AiInterface {

public:

  MovieRecommender();
  /*!
     *  \brief Constructeur de base.
     *
     *  Ce constructeur prend le nombre de films et d'utilisateur à traité. Le nom du dataset sera ici celui
     *  par défaut, c'est-à-dire "u", on aura donc les fichiers suivants:
     *    - u.base
     *    - u.genre
     *    - u.item
     *
     *  \param nbMovies: le nombre total de films traité par l'algorithme.
     *  \param nbUsers: le nombre total d'utilisateurs traité par l'algorithme.
     *  \param nbFeatures: le nombre total de genre qui vont servir à catégorisé les films.
     */
  MovieRecommender(int nbMovies, int nbUsers, int nbFeatures);

  /*!
     *  \brief Constructeur avec un nom de fichier.
     *
     *  Ce constructeur prend un nom de fichier et le nombre de films et d'utilisateurs à traité.
     *  Ici, on aura donc les fichiers suivants:
     *    - dataset.base
     *    - dataset.genre
     *    - dataset.item
     *
     *  \param dataset: le nom du dataset à utiliser.
     *  \param nbMovies: le nombre total de films traité par l'algorithme.
     *  \param nbUsers: le nombre total d'utilisateurs traité par l'algorithme.
     *  \param nbFeatures: le nombre total de genre qui vont servir à catégorisé les films.
     */
  MovieRecommender(std::string dataset, int nbMovies, int nbUsers, int nbFeatures);

  /*!
     *  \brief Constructeur avec initialisation des matrices thêta et X.
     *
     *  Ce constructeur prend un nom de fichier et le nombre de films et d'utilisateurs à traité.
     *  Ici, on aura donc les fichiers suivants:
     *    - dataset.base
     *    - dataset.genre
     *    - dataset.item\n
     *  Enfin, les deux matrices de paramètres thêta(représentant les préférences utilisateur par genre) et
     *  X(représentant la catégorisation par genre de chaque film) sont données. Les 2 matrices doivent
     *  avoir la taille donnée par les variables nbMovies et nbUsers.
     *
     *  \param dataset: le nom du dataset à utiliser.
     *  \param nbMovies: le nombre total de films traité par l'algorithme.
     *  \param nbUsers: le nombre total d'utilisateurs traité par l'algorithme.
     *  \param theta: la matrice de paramètres thêta, représentant ici les préférences utilisateur pour chaque genre de film.
     *  \param X: la matrice de paramètres X, représentant l'appartenance de chaque film à un genre.
     */
  MovieRecommender(std::string dataset, int nbMovies, int nbUsers, gsl_matrix* theta, gsl_matrix* X);

  /*!
     *  \brief Constructeur avec initialisation des matrices thêta et X via un Saver.
     *
     *  Constructeur permettant l'initialisation de l'algorithme via un objet Saver qui charge les informations
     *  via deux fichiers(filename.theta pour la matrice Theta et filename.X pour X), il charge également un dataset.
     *
     *  \param dataset: le nom du dataset à utiliser.
     *  \param saver: l'objet saver permettant de charger les matrices thêta et X.
     */
  MovieRecommender(std::string dataset, Saver saver);

  /*!
     *  \brief Constructeur avec initialisation des matrices thêta et X via un Saver.
     *
     *  Constructeur permettant l'initialisation de l'algorithme via un objet Saver qui charge les informations
     *  via deux fichiers(filename.theta pour la matrice Theta et filename.X pour X).
     *
     *  \param dataset: le nom du dataset à utiliser.
     *  \param saver: l'objet saver permettant de charger les matrices thêta et X.
     */
  MovieRecommender(Saver saver);

  /*!
     *  \brief Permets d'effectuer l'apprentissage.
     *
     *  Méthode permettant d'effectuer l'apprentissage. C'est à dire d'affiner les paramètres thêta et X
     *  permettant de produire les meilleurs prédictions qui vont servir pour effectuer les recommandations.
     *
     *  On notera que l'entrainement se fait grâce à une descente de gradient. Le taux d'apprentissage alpha permet
     *  de déterminer la vitesse de la descente du gradient.
     * On notera que l'on sauvegarde régulièrement l'état des paramètres theta et X pour pouvoir les récupérer en cas de plantage.
     * Le nom du fichier de sauvegarde intermédiare est save_train (ce qui donne les fichiers save_train.X et save_train.theta), il est stocké dans le dossier data.
     *
     * A la fin de l'entrainement, on sauvegarde l'état de theta et X dans un fichier portant le même nom que le jeu d'entrainement.
     * Exemple, si le jeu d'entrainement est u1, on aura donc 2 fichiers u1.X et u1.theta.
     *
     *  \param alpha: le taux d'apprentissage utilisé pour la décente de gradient.
     *  \param lambda: le paramètre de régularisation.
     *  \param save: règle le pâs de la sauvegarde régulière de l'IA, les matrices theta et x seront sauvegardés tous les "save" itérations de l'entraînement.
     */
  void train(double alpha, double lambda, int save = 30);

  /*!
     *  \brief Construire la matrice complète notes des films/utilisateurs.
     *
     *  Méthode permettant de prédire les notes de chaque film pour chaque utilisateur. Ceci a pour résultat
     *  de construire la matrice complète des notes des films par rapport aux utilisateurs. On utilisera donc
     *  cette matrice pour effectuer les recommandations aux utilisateurs. On notera que l'on se base sur
     *  les matrices de paramètres thêta et X pour effectuer les prédictions, donc il faut utiliser ce modèle
     *  pour faire des recommandations seulement si l'entrainement a été effectué.
     *
     * \return une gsl_matrix avec les notes sous forme de réel entre 0 et 5.
     */
  gsl_matrix* predict();

  /*!
     *  \brief Construire la matrice complète et normalisé notes des films/utilisateurs.
     *
     *  Méthode permettant de construire la matrice des prédictions des notes des film par rapport aux utilisateurs
     *  mais avec des valeurs normalisé. En effet les notes sont des entiers entre 0 et 5 et les prédictions de l'IA
     *  nous renvoie des réels. Cette méthode est donc la pour résoudre ce problème.
     *
     *  \return une gsl_matrix avec les notes sous forme d'entier entre 0 et 5.
     */
  gsl_matrix* normalize();

  /*!
     *  \brief Recommander des films à un utilisateur.
     *
     *  Méthode permettant de trouver des films similaires à un film que l'utilisateur a apprécié.
     *
     *  \param user : l'id de l'utilisateur pour lequel ont fera la prédiction.
     *  \param nbMovies: le nombre de films a recommandé.
     *  \return un vector de string avec le titre des films recommandés.
     */
  std::vector<std::string> recommend(int user, int nbMovies);

  /*!
     *  \brief Calcul de la fonction de cout J(thêta).
     *
     *  Méthode permettant de calculer la fonction de coût, qui représente la performance de l'algorithme.
     *  C'est donc une erreur qui est calculée. Cette erreur doit être minimale pour assurer la performance des recommandations effectuée.
     *  L'entraînement est là pour minimiser cette erreur. On peut aussi calculé l'erreur sur un jeu de test.
     *
     *  \param mode: Le mode de fonctionnement du modèle, il est soit en entrainement(TRAINSET), soit en test(TESTSET).
     *  \param lambda : le paramètre de régularisation.
     *  \return un double représentant l'erreur globale que commet l'algorithme sur ces prédictions.
     */
  double computeCost(int mode, double lambda);

  /*!
     *  \brief Sauvegarde d'un état de l'objet.
     *
     *  Méthode permettant de sauvegarder l'état de l'objet, c'est-à-dire l'état des matrices thêta et X.
     *  Cette méthode utilise simplement un objet Saver pour faire ça.
     *
     *  \param file : le nom du fichier de sauvegarde.
     */
  void saveState(std::string file);

  /*!
     *  \brief Chargement d'un état de l'objet.
     *
     *  Méthode permettant de charger l'état de l'objet précédemment sauvegardé par un Saver.
     *  On va charger directement thêta et X dans les attributs respectifs de l'objet. Le chargement se
     *  fera via un objet Saver.
     *
     *  \param file : le nom du fichier de sauvegarde.
     */
  void loadState(std::string file);

  /*!
     *  \brief Affichage de l'état de l'objet.
     *
     *  Méthode permettant d'afficher les informations utiles sur l'état de l'objet, c'est-à-dire:
     *    - la matrice de paramètres thêta.
     *    - la matrice de paramètres X.
     *    - la fonction de cout J(thêta).
     *
     *  \param lambda : Le paramètre de régularisation.
     */
  void printState(double lamba);

  /*!
     *  \brief Changer le dataset d'entraînement.
     *
     *  Setter permettant de charger un nouveau dataset pour l'entraînement.
     *
     *  \param set : le nom du dataset.
     *  \param nbMovies : le nombre de films.
     *  \param nbUsers : le nombre d'utilisateurs.
     */
  void setDatas(std::string set, int nbMovies, int nbUsers);

  /*!
     *  \brief Changer le dataset de test.
     *
     *  Setter permettant de charger un nouveau dataset pour l'entraînement.
     *
     *  \param set : le nom du dataset.
     *  \param nbMovies : le nombre de films.
     *  \param nbUsers : le nombre d'utilisateurs.
     *  \param N: La taille du set de test.
     */
  void setTestDatas(std::string set, int nbMovies, int nbUsers, int N);

  /*!
     *  \brief Récupérer la matrice de paramètres thêta.
     *
     *  Getter permettant de récupérer la matrice de paramètre thêta qui correspond aux préférences des utilisateurs.
     *
     *  \return une gsl_matrix avec l'ensemble des données de préférence, par utilisateur.
     */
  gsl_matrix* getTheta();

  /*!
     *  \brief Récupérer la matrice de paramètres X.
     *
     *  Getter permettant de récupérer la matrice de paramètre X qui correspond aux catégorisations des films.
     *
     *  \return une gsl_matrix avec des valeurs(double) représentant l'appartenance de chaque film aux différentes catégories.
     */
  gsl_matrix* getX();

  /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe MovieRecommender.
     *
     *  Permets de désallouer les éléments dynamiques, ici ce sera les gsl_matrix m_theta, m_X et m_ratings.
     */
  virtual ~MovieRecommender();

protected:
  int m_nbMovies;/*!< Le nombre total de films que l'on traite. */
  int m_nbUsers;/*!< Le nombre total d'utilisateurs' que l'on traite. */
  gsl_matrix* m_theta; /*!< Matrice de paramètres, elle représente les préférences des utilisateurs en fonction des genres de films. */
  gsl_matrix* m_X; /*!< Matrice de paramètres, elle représente la catégorie de chaque film. */
  DataParser *m_parser; /*!< Objet DataParser contenant les informations du dataset, c'est-à-dire la matrice des notes données par les utilisateurs(incomplète), l'ensemble des genres de films présents dans le dataset ainsi que les titres de tous les films. */

private:
  gsl_matrix* computeTrainError();

  gsl_matrix* computeTestError();

  /*
   * Méthode permettant d'arrondir un nombre.
  */
  int round(double a);

  /*
   *  Methode permettant d'initialiser les matrices de paramètres theta et X, les 2 matrices sont initialisées
   *  via un generateur de nombre aléatoire, ces nombres sont compris entre 0 et 1.
  */
  void initParams();

  /*
   *  Methode permettant d'afficher l'état d'une gsl_matrix sur la sortie standard.
  */
  void printMatrix(std::string message, gsl_matrix *matrix);
};

#endif
