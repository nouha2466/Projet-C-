#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

struct Pixel {
    int r, g, b;
};

struct PPMImage {
    int largeur;
    int hauteur;
    std::vector<Pixel> data;
};

PPMImage chargerPPM(const std::string& filename) {
    PPMImage image;

    std::ifstream fichier(filename, std::ios::binary);
    if (!fichier) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier" << std::endl;
        return image;
    }

    std::string format;
    fichier >> format >> image.largeur >> image.hauteur;
    int maxval;
    fichier >> maxval;

    image.data.resize(image.largeur * image.hauteur);
    for (int i = 0; i < image.largeur * image.hauteur; ++i) {
        fichier >> image.data[i].r >> image.data[i].g >> image.data[i].b;
    }

    fichier.close();
    return image;
}

void houghTransformNaive(const PPMImage& image) {
    int largeur = image.largeur;
    int hauteur = image.hauteur;

    double deltaM = 0.1;
    double deltaB = 1.0;

    int numPasM = static_cast<int>((2.0 / deltaM) + 1);
    int numPasB = static_cast<int>((largeur + hauteur) / deltaB + 1);

    std::vector<std::vector<int>> accumulator(numPasM, std::vector<int>(numPasB, 0));

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (image.data[y * largeur + x].r != 255) {
                for (int mIndex = 0; mIndex < numPasM; mIndex++) {
                    for (int bIndex = 0; bIndex < numPasB; bIndex++) {
                        double m = -1.0 + mIndex * deltaM;
                        double b = -largeur / 2.0 + bIndex * deltaB;

                        double Newb = y - m * x;

                        if (std::abs(Newb - b) < deltaB / 2.0) {
                            accumulator[mIndex][bIndex]++;
                        }
                    }
                }
            }
        }
    }
}
void SavePPM(const PPMImage& image, const std::string& filename) {
    std::ofstream fichier(filename, std::ios::binary);
    if (!fichier) {
        std::cerr << "Erreur : impossible de crÃ©er le fichier" << std::endl;
        return;
    }

    fichier << "P3\n" << image.largeur << " " << image.hauteur << "\n255\n";
    for (const Pixel& pixel : image.data) {
        fichier << pixel.r << " " << pixel.g << " " << pixel.b << "\n";
    }

    fichier.close();
}
void superposerLignes(const PPMImage& image, const std::vector<std::vector<int>>& accumulator) {
    PPMImage resultat = image;

    // Parcourir l'accumulateur pour trouver les lignes dÃ©tectÃ©es
    for (int mIndex = 0; mIndex < accumulator.size(); mIndex++) {
        for (int bIndex = 0; bIndex < accumulator[mIndex].size(); bIndex++) {
            
            if (accumulator[mIndex][bIndex] > 100) { 
                double m = -1.0 + mIndex * 0.1; 
                double b = -image.largeur / 2.0 + bIndex * 1.0; 
                for (int x = 0; x < image.largeur; x++) {
                    int y = static_cast<int>(m * x + b); 
                    if (y >= 0 && y < image.hauteur) { 
                        resultat.data[y * image.largeur + x].r = 255; 
                        resultat.data[y * image.largeur + x].g = 0;   
                        resultat.data[y * image.largeur + x].b = 0;
                    }
                }
            }
        }
    }
}

int main() {
    PPMImage image = chargerPPM("m1.ppm");

    if (image.data.empty()) {
        std::cerr << "Erreur : impossible de charger l'image" << std::endl;
        return -1;
    }
    std::vector<std::vector<int>> accumulator;

    houghTransformNaive(image);
    
    superposerLignes(image, accumulator);
    SavePPM(image, "m1.ppm");


    return 0;
}
