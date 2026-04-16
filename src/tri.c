#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tri.h"

// Fonction utilitaire pour échanger deux éléments
void echanger(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction utilitaire pour copier un tableau
void copier_tableau(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

// Fonction pour afficher un tableau
void afficher_tableau(int tab[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

//  TRI À BULLE - VERSION BASIQUE
void tri_bulle_basique(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (tab[j] > tab[j + 1]) {
                echanger(&tab[j], &tab[j + 1]);
            }
        }
    }
}

// TRI À BULLE - OPTIMISATION DE BOUCLE 
void tri_bulle_opt_boucle(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (tab[j] > tab[j + 1]) {
                echanger(&tab[j], &tab[j + 1]);
            }
        }
    }
}

// TRI À BULLE - OPTIMISATION D'ARRÊT 
void tri_bulle_opt_arret(int tab[], int n) {
    int echange;
    for (int i = 0; i < n - 1; i++) {
        echange = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (tab[j] > tab[j + 1]) {
                echanger(&tab[j], &tab[j + 1]);
                echange = 1;
            }
        }
        if (echange == 0) break; // Arrêt si aucun échange
    }
}

// tri cocktail
void tri_cocktail(int tab[], int n) {
    int echange = 1;
    int debut = 0;
    int fin = n - 1;
    
    while (echange) {
        echange = 0;
        
        // Parcours de gauche à droite
        for (int i = debut; i < fin; i++) {
            if (tab[i] > tab[i + 1]) {
                echanger(&tab[i], &tab[i + 1]);
                echange = 1;
            }
        }
        
        if (!echange) break;
        
        echange = 0;
        fin--;
        
        // Parcours de droite à gauche
        for (int i = fin - 1; i >= debut; i--) {
            if (tab[i] > tab[i + 1]) {
                echanger(&tab[i], &tab[i + 1]);
                echange = 1;
            }
        }
        
        debut++;
    }
}

// tri par sélection 
void tri_selection(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            echanger(&tab[i], &tab[min_idx]);
        }
    }
}

//  TRI PAR INSERTION 
void tri_insertion(int tab[], int n) {
    for (int i = 1; i < n; i++) {
        int cle = tab[i];
        int j = i - 1;
        
        while (j >= 0 && tab[j] > cle) {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = cle;
    }
}

// RI PAR DÉNOMBREMENT
void tri_denombrement(int tab[], int n, int max_val) {
    // Créer un tableau de comptage
    int *compteur = (int*)calloc(max_val + 1, sizeof(int));
    
    // Compter les occurrences
    for (int i = 0; i < n; i++) {
        compteur[tab[i]]++;
    }
    
    // Reconstruire le tableau trié
    int idx = 0;
    for (int i = 0; i <= max_val; i++) {
        while (compteur[i] > 0) {
            tab[idx++] = i;
            compteur[i]--;
        }
    }
    
    free(compteur);
}

//TRI FUSION - FONCTIONS AUXILIAIRES
void fusionner(int tab[], int debut, int milieu, int fin) {
    int n1 = milieu - debut + 1;
    int n2 = fin - milieu;
    
    int *gauche = (int*)malloc(n1 * sizeof(int));
    int *droite = (int*)malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        gauche[i] = tab[debut + i];
    for (int j = 0; j < n2; j++)
        droite[j] = tab[milieu + 1 + j];
    
    int i = 0, j = 0, k = debut;
    
    while (i < n1 && j < n2) {
        if (gauche[i] <= droite[j]) {
            tab[k++] = gauche[i++];
        } else {
            tab[k++] = droite[j++];
        }
    }
    
    while (i < n1) tab[k++] = gauche[i++];
    while (j < n2) tab[k++] = droite[j++];
    
    free(gauche);
    free(droite);
}

void tri_fusion(int tab[], int debut, int fin) {
    if (debut < fin) {
        int milieu = debut + (fin - debut) / 2;
        tri_fusion(tab, debut, milieu);
        tri_fusion(tab, milieu + 1, fin);
        fusionner(tab, debut, milieu, fin);
    }
}

//TRI RAPIDE
int partitionner(int tab[], int debut, int fin) {
    int pivot = tab[fin];
    int i = debut - 1;
    
    for (int j = debut; j < fin; j++) {
        if (tab[j] < pivot) {
            i++;
            echanger(&tab[i], &tab[j]);
        }
    }
    echanger(&tab[i + 1], &tab[fin]);
    return i + 1;
}

void tri_rapide(int tab[], int debut, int fin) {
    if (debut < fin) {
        int pi = partitionner(tab, debut, fin);
        tri_rapide(tab, debut, pi - 1);
        tri_rapide(tab, pi + 1, fin);
    }
}
// TRI PAR TAS (HEAP SORT) 

void tamiser(int tableau[], int n, int i) {
    int max = i;
    int gauche = 2 * i + 1;
    int droite = 2 * i + 2;
    int temp;
    
    if (gauche < n && tableau[gauche] > tableau[max])
        max = gauche;
    
    if (droite < n && tableau[droite] > tableau[max])
        max = droite;
    
    if (max != i) {
        temp = tableau[i];
        tableau[i] = tableau[max];
        tableau[max] = temp;
        tamiser(tableau, n, max);
    }
}

void tri_par_tas(int tableau[], int taille) {
    int i, temp;
    
    // Construire le ta
    for (i = taille / 2 - 1; i >= 0; i--)
        tamiser(tableau, taille, i);
    
    // Extraire les éléments du tas un par un
    for (i = taille - 1; i > 0; i--) {
        
        temp = tableau[0];
        tableau[0] = tableau[i];
        tableau[i] = temp;
        
        // Tamiser la racine réduite
        tamiser(tableau, i, 0);
    }
}

// TRI SHELL 

void tri_shell(int tableau[], int taille) {
    int gap, i, j, temp;
    
    // Utiliser la séquence de Knuth: 1, 4, 13, 40, 121...
    gap = 1;
    while (gap < taille / 3)
        gap = 3 * gap + 1;
    
    // Tri par insertion avec différents gaps
    while (gap >= 1) {
        for (i = gap; i < taille; i++) {
            temp = tableau[i];
            j = i;
            
            while (j >= gap && tableau[j - gap] > temp) {
                tableau[j] = tableau[j - gap];
                j -= gap;
            }
            tableau[j] = temp;
        }
        gap = gap / 3;
    }
}