#include <iostream>
#include <vector>
#include "lecture.hpp"

struct PPMImage {
    std::string format;
    int largeur;
    int hauteur;
    int maxval;
    std::vector<int> data;
};

PPMImage LoadPPM(const std::string& nomFichier) {
    PPMImage image;

    image.format = standard(nomFichier);
    image.largeur = largeur(nomFichier);
    image.hauteur = hauteur(nomFichier);
    image.maxval = max_value(nomFichier);
    image.data = image(nomFichier);

    return image;
}

int main() {
    std::string nomFichier = "m1projetcpp2.ppm";

    PPMImage image = LoadPPM(nomFichier);

    std::cout << "Format : " << image.format << std::endl;
    std::cout << "Largeur : " << image.largeur << std::endl;
    std::cout << "Hauteur : " << image.hauteur << std::endl;
    std::cout << "Valeur maximale de couleur : " << image.maxval << std::endl;

    /*
    for(unsigned int i = 0; i < image.data.size(); i++){
        //std::cout << image.data[i] <<std::endl;
    } 
    */

    return 0;
}
