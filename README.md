# Projet-C++-
Le but de ce projet est d'écrire un programme dans C++ permettant de détecter des droites sur une image .ppm en utilisant:


  1-Transformé de Hough dans l'espace des paramètres (m,p) "La méthode naive" avec m - un coefficient directeur et p - une constante sur l'ordonnée (x=0) entre deux points:
y = m⋅ x + p avec m,p - des réels :
-Explication de code :

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Structure pour représenter un pixel 
struct Pixel {
    int r, g, b;  
};

--> Cette structure est utilisée pour représenter un pixel d'une image. Chaque pixel est composé de trois valeurs entières correspondant aux composantes rouge, verte et bleue (RGB).

Structure pour représenter un point dans l'espace de Hough
struct HoughPoint {
    double m, b;
};
--> Cette structure est utilisée pour représenter un point dans l'espace de Hough, utilisé généralement dans les algorithmes de détection de lignes.

// Fonction pour dessiner une droite dans une image PPM
void dessinerDroite(const std::string& filename, int largeur, int hauteur, double m, double b) {
    std::ofstream imageFile(filename); // Crée un fichier pour l'image PPM
    if (!imageFile) {
        std::cerr << "Impossible d'ouvrir le fichier pour l'écriture." << std::endl;
        return;
    }

    // Entête de l'image PPM
    imageFile << "P3\n" << largeur << " " << hauteur << "\n255\n";

    // Créer une image noire
    std::vector<std::vector<Pixel>> image(hauteur, std::vector<Pixel>(largeur, {0, 0, 0}));

    // Déterminer les coordonnées de début et de fin du segment visible de la droite
    int xA = 0;
    int yA = m * xA + b;

    int xB = largeur - 1;
    int yB = m * xB + b;

    // Dessiner la droite en parcourant les colonnes de xA à xB
    for (int j = xA; j <= xB; j++) {
        int i = m * j + b; // Calculer la valeur de la ligne correspondante
        if (i >= 0 && i < hauteur && j >= 0 && j < largeur) {
            // Assurez-vous que les coordonnées sont dans les limites de l'image
            image[i][j] = {255, 255, 255}; // Colorier le pixel en blanc
        }
    }

    // Écrire les pixels de l'image PPM dans le fichier
    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            imageFile << image[i][j].r << " " << image[i][j].g << " " << image[i][j].b << " ";
        }
        imageFile << "\n";
    }

    imageFile.close(); // Fermer le fichier
}
--> Cette fonction prend en paramètres un nom de fichier, la largeur et la hauteur de l'image, ainsi que les coefficients m et b d'une droite dans le plan. Elle crée un fichier image au format PPM représentant la droite tracée sur un fond noir

// Fonction pour appliquer la transformation de Hough de manière naïve
void houghTransformNaive(const std::vector<std::vector<Pixel>>& image) {
    // Taille de l'image
    int largeur = image[0].size();
    int hauteur = image.size();

    // Résolution de m et b
    double deltaM = 0.1; // Pas pour les valeurs de m
    double deltaB = 1.0; // Pas pour les valeurs de b

    // Calculer le nombre de pas pour m et b
    int numStepsM = static_cast<int>((2.0 / deltaM) + 1);
    int numStepsB = static_cast<int>((largeur + hauteur) / deltaB + 1);

    // Initialiser l'accumulateur pour l'espace de Hough avec des valeurs nulles
    std::vector<std::vector<int>> accumulator(numStepsM, std::vector<int>(numStepsB, 0));

    // Parcourir tous les pixels de l'image
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            // Si le pixel est coloré (par exemple, en utilisant un seuil)
            if (/* Condition pour détecter les contours */) {
                // Pour chaque valeur possible de m et b
                for (int mIndex = 0; mIndex < numStepsM; mIndex++) {
                    for (int bIndex = 0; bIndex < numStepsB; bIndex++) {
                        // Calculer les valeurs de m et b correspondantes
                        double m = -1.0 + mIndex * deltaM;
                        double b = -largeur / 2.0 + bIndex * deltaB;

                        // Calculer la valeur de b correspondante pour ce pixel
                        double bExpected = y - m * x;

                        // Si la valeur de b attendue est proche de la valeur de b calculée
                        if (std::abs(bExpected - b) < deltaB / 2.0) {
                            // Incrémenter le compteur correspondant dans l'accumulateur de Hough
                            accumulator[mIndex][bIndex]++;
                        }
                    }
                }
            }
        }
    }

    // Maintenant, l'accumulateur contient les votes pour les différentes droites dans l'espace de Hough
    // Vous pouvez procéder à la suite de l'algorithme de détection de droites ici
}
--> Cette fonction prend en paramètre une image représentée sous forme d'une matrice de pixels. Elle applique la transformation de Hough de manière naïve pour détecter des lignes dans l'image.

int main() {
    // Utilisation des fonctions
    // Exemple d'utilisation de la fonction drawLinePPM
    drawLinePPM("output.ppm", 800, 600, 1.0, 100.0);

    // Exemple d'utilisation de la fonction houghTransformNaive
    std::vector<std::vector<Pixel>> image; // Initialisez votre image ici
    houghTransformNaive(image);

    return 0;
} 

--> Cette fonction est le point d'entrée du programme.
Elle appelle la fonction drawLinePPM pour dessiner une droite sur une image PPM.
Elle appelle la fonction houghTransformNaive avec une image (non initialisée dans cet exemple) pour détecter les lignes.


   2-Transformé de Hough dans l'espace des paramètres (r,θ) qui est un transformé de (m,p) en coordonnées polaires "La méthode moins naive":
r = x⋅ cosθ + y⋅ sinθ avec θ ∈ [0, 2π[ et r-un réel 

--> On essaie de faire la transformation de Hough à l'image "m1projetcpp2.ppm". L'objectif est de trouver les segments de l'image grâce un algorithme qui change chaque pixel du contour en une caractérisation par une droite (p,theta).

----> Création de la structure Pixel qui donne la teinte en rouge vert et bleu du pixel

using namespace std;

struct Pixel {
    int r, g, b;                          
----> Création d'une classe coordonnées polaires définie par un rho et un theta

class CoordPolaires {
public:
    double theta;                         

    CoordPolaires(double t, double r) : theta(t), p(r) {}
};
----> fonction qui crée une matrice nulle de taille choisie

vector<vector<double>> matrice(int a,int b) {
    const int lignes = a;                                                               
    const int colonnes = b;

    vector<vector<double>> matrice(lignes, vector<double>(colonnes));                 
    for (int i=0;i<lignes;++i){
      for (int j=0;j<colonnes;++j){
        matrice[i][j] = 0;
      }
    }

    return matrice;
}
---- > Fonction qui lit un fichier .ppm

vector<vector<Pixel>> lirePPM(string chemin) {                       
    ifstream fichier(chemin);                                       //ouverture du fichier choisi avec ifstream
    vector<vector<Pixel>> matrice;                                 //création d'une matrice vide pour la remplir des codes des pixels                                                     

    if (!fichier.is_open()) {
        cerr << "Impossible d'ouvrir le fichier : " << chemin << endl;          //message d'erreur si le fichier ne s'ouvre pas
        return matrice;
    }

    string format;                                                //création des variables qui définissent l'image comme le format (ici P3) sa taille et la valeur max des couleurs des pixels
    int largeur, hauteur, maxVal;
    fichier >> format >> largeur >> hauteur >> maxVal;

    if (format != "P3") {
        cerr << "Mauvais format" << endl;
        return matrice;                                            //message d'erreur qui dit que si le format n'est pas P3 on n'a pas le bon format
    }

    matrice.resize(hauteur, vector<Pixel>(largeur));              //redimensionner la matrice à la même taille que l'image

    for (int i = 0; i < hauteur; ++i) {                               
        for (int j = 0; j < largeur; ++j) {
            Pixel pixel;
            fichier >> pixel.r >> pixel.g >> pixel.b;                 //remplissage de la matrice des triplets étant les pixels
            matrice[i][j] = pixel;
        }
    }

    fichier.close();                                   //fermeture du fichier
    return matrice;
}
----> Création de la classe transformée de Hough

class transformHough {                                       //création de la classe transformée de Hough
private:
  map<pair<int,int>,int> compteur;                          //map contenant les paires de rho theta et leurs itérations
public:
  transformHough() {}

  void ajoutePoint(int x, int y) {
    for (int theta =0;theta<180;theta++) {
      float thetaRad = M_PI/180*theta;                                 //fonction ajoutant les doublets rho theta à la map et les compte 
      double p = x*cos(thetaRad) + y*sin(thetaRad);
      pair<int, int> pt = make_pair(theta, (int)p);
      if (compteur.find(pt) == compteur.end()) {
         compteur[pt] = 1;
      } else {
         compteur[pt]++;
        }
    }
  }

  void afficheCompteur() {
    for (const auto &pair : compteur) {
      if (pair.second>=250){
        cout << "Theta: " << pair.first.first << ", p: " << pair.first.second             //fonction affichant chaque paire de rho theta et leur nombre d'itérations
        << " -> Compte: " << pair.second << endl;                                    
      }
    }
  }

  vector<pair<int,int>> stock(int seuil){
    vector<pair<int,int>> vecteur;
    for (const auto &pair : compteur) {
      if (pair.second>=seuil){                                       //donne les rho theta ayant un compte supérieur à un seuil défini. On le choisira selon les valeurs qu'on obtient. Ici 
        vecteur.push_back(pair.first);                               // on prend 250 car 254 est le compteur le plus élevé et le deuxième est seulement 67
      }
    }
    return vecteur;
  }

};
----> Fonction main

int main() {
  transformHough TH;

  string chemin = "m1projetcpp2.ppm";
  vector<vector<Pixel>> matriceImage = lirePPM(chemin);                                //lecture du fichier proposé

  int rows = matriceImage.size();
  int cols = matriceImage[0].size();

  for (int i=0;i<rows;++i){
    for (int j=0;j<cols;++j){
      if (matriceImage[i][j].r > 0 || matriceImage[i][j].g > 0 || matriceImage[i][j].b > 0){            //pour chaque pixel de l'image , s'il est coloré on le rajoute au contour et on ------> calcule son rho theta
        TH.ajoutePoint(i,j);
      }
    }
  }
  TH.afficheCompteur();
  vector<pair<int,int>> resultat = TH.stock(250);
  vector<vector<double>> MatResult = matrice(rows,cols);

  /*for (int i=0;i<resultat;i++){
    if (resultat[i][0] != 90){
      int theta = M_PI/180*resultat[i][0];
      int p = resultat[i][1];
  
    }*/

  return 0;
}
--> On obtient une droite de theta = 135° et un p = 0 qui est alors la droite diagonale descendante de l'image .

-----> fonctions supplémentaires implémentées mais non utiles au code ci-dessus

CoordPolaires convertirEnCP(int a,int b){
  CoordPolaires polaires;

  polaires.p = sqrt(a*a+b*b);
  polaires.theta = atan2(b,a);

  return polaires;
}

void detectionContours(const vector<vector<double>>& Mat) {
  vector<CoordPolaires> points;

  for (int i = 0; i < Mat.size(); ++i) {
    for (int j = 0; j < Mat[i].size(); ++j) {
      if (Mat[i][j]==1){
        points.push_back(convertirEnCP(i,j));
      }
    }
  }
  cout << std::setw(10) << "Rayon" << std::setw(10) << "Theta" << endl;
    for (int i = 0; i < points.size(); ++i) {
      cout << std::setw(10) << points[i].p << std::setw(10) << points[i].theta << endl;
    }
  cout << endl;
}

  3-Choix des meilleures droites par suppression des doublons.
De plus, il fallait créer une manière à dessiner des droites avec des coefficients (m,p) pour tester le fonctionnement de la détection programmée.





