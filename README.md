# Projet-C++-
Le but de ce projet est d'écrire un programme dans C++ permettant de détecter des droites sur une image .ppm en utilisant:


1-Transformé de Hough dans l'espace des paramètres (m,p) avec m - un coefficient directeur et p - une constante sur l'ordonnée (x=0) entre deux points:
y = m⋅ x + p avec m,p - des réels

2-Transformé de Hough dans l'espace des paramètres (r,θ) qui est un transformé de (m,p) en coordonnées polaires:
r = x⋅ cosθ + y⋅ sinθ avec θ ∈ [0, 2π[ et r-un réel

3-Choix des meilleures droites par suppression des doublons.
De plus, il fallait créer une manière à dessiner des droites avec des coefficients (m,p) pour tester le fonctionnement de la détection programmée.

*le Plan ou la méthode de réalisation :
1-un module de la lecture d'un fichier .ppm + des fichiers .cpp et .hpp référencé dans un fichier d'exécution main.cpp
2-un module de la détection des droites sur une image (.ppm) + des fichiers .cpp et .hpp référencé dans un fichier d'exécution main.cpp
3-un module du dessin d'une droite sur une image (.ppm) + des fichiers .cpp et .hpp référencé dans un fichier d'exécution main.cpp


