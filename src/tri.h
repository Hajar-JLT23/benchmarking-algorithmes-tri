#ifndef TRI_H
#define TRI_H

// Tri à bulle - version basique
void tri_bulle_basique(int tab[], int n);

// Tri à bulle - optimisation de boucle
void tri_bulle_opt_boucle(int tab[], int n);

// Tri à bulle - optimisation d'arrêt
void tri_bulle_opt_arret(int tab[], int n);

// Tri cocktail
void tri_cocktail(int tab[], int n);

// Tri par sélection
void tri_selection(int tab[], int n);

// Tri par insertion
void tri_insertion(int tab[], int n);

// Tri par dénombrement
void tri_denombrement(int tab[], int n, int max_val);

// Tri fusion
void tri_fusion(int tab[], int debut, int fin);

// Tri rapide
void tri_rapide(int tab[], int debut, int fin);

// Fonctions utilitaires
void echanger(int *a, int *b);
void copier_tableau(int source[], int dest[], int n);
void afficher_tableau(int tab[], int n);
//Algorithmes bonus 
void tri_par_tas(int tableau[], int taille);
void tri_shell(int tableau[],int taille);

#endif