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

PPMImage LoadPPM(const std::string& filename) {
    PPMImage pict;

    std::ifstream fichier(filename, std::ios::binary);
    if (!fichier) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier" << std::endl;
        return pict;
    }

    std::string format;
    fichier >> format >> pict.largeur >> pict.hauteur;
    int maxval;
    fichier >> maxval;

    pict.data.resize(pict.largeur * pict.hauteur);
    for (int i = 0; i < pict.largeur * pict.hauteur; ++i) {
        fichier >> pict.data[i].r >> pict.data[i].g >> pict.data[i].b;
    }

    fichier.close();
    return pict;
}

void houghTransformNaive(const PPMImage& image) {
    int largeur = image.largeur;
    int hauteur = image.hauteur;

    double deltaM = 0.1;
    double deltaB = 1.0;

    int numStepsM = static_cast<int>((2.0 / deltaM) + 1);
    int numStepsB = static_cast<int>((largeur + hauteur) / deltaB + 1);

    std::vector<std::vector<int>> accumulator(numStepsM, std::vector<int>(numStepsB, 0));

    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (image.data[y * largeur + x].r != 255) {
                for (int mIndex = 0; mIndex < numStepsM; mIndex++) {
                    for (int bIndex = 0; bIndex < numStepsB; bIndex++) {
                        double m = -1.0 + mIndex * deltaM;
                        double b = -largeur / 2.0 + bIndex * deltaB;

                        double bExpected = y - m * x;

                        if (std::abs(bExpected - b) < deltaB / 2.0) {
                            accumulator[mIndex][bIndex]++;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    PPMImage image = LoadPPM("m1projetcpp2.ppm");

    if (image.data.empty()) {
        std::cerr << "Erreur : impossible de charger l'image" << std::endl;
        return -1;
    }

    houghTransformNaive(image);

    return 0;
}
