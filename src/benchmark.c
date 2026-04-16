#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "benchmark.h"
#include "tri.h"

// ========== GÉNÉRATION DES DONNÉES ==========
void generer_donnees(char *nom_fichier, int taille, int min, int max) {
    FILE *fp = fopen(nom_fichier, "w");
    if (fp == NULL) {
        printf("Erreur lors de la création du fichier %s\n", nom_fichier);
        return;
    }
    
    fprintf(fp, "%d\n", taille);
    
    // Tableau croissant
    for (int i = 0; i < taille; i++) {
        fprintf(fp, "%d ", min + i);
    }
    fprintf(fp, "\n");
    
    // Tableau décroissant
    for (int i = 0; i < taille; i++) {
        fprintf(fp, "%d ", min + taille - i - 1);
    }
    fprintf(fp, "\n");
    
    // Tableau constant
    int val = rand() % (max - min) + min;
    for (int i = 0; i < taille; i++) {
        fprintf(fp, "%d ", val);
    }
    fprintf(fp, "\n");
    
    // 100 tableaux aléatoires pour calculer la moyenne
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < taille; j++) {
            fprintf(fp, "%d ", rand() % (max - min) + min);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
    printf("Fichier %s créé avec succès (taille: %d)\n", nom_fichier, taille);
}

// lecture d'un tableau depuis un fichier
int* lire_tableau(FILE *fp, int taille) {
    int *tab = (int*)malloc(taille * sizeof(int));
    if (tab == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }
    
    for (int i = 0; i < taille; i++) {
        if (fscanf(fp, "%d", &tab[i]) != 1) {
            printf("Erreur de lecture\n");
            free(tab);
            return NULL;
        }
    }
    
    return tab;
}

// mesure du temps d'exécution d'une fonction de tri
double mesurer_temps(void (*fonction_tri)(int[], int), int tab[], int n) {
    clock_t debut, fin;
    
    debut = clock();
    fonction_tri(tab, n);
    fin = clock();
    
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}

double mesurer_temps_fusion(int tab[], int n) {
    clock_t debut, fin;
    int *tab_copie = (int*)malloc(n * sizeof(int));
    
    if (!tab_copie) return 0.0;
    
    // Copier le tableau pour ne pas modifier l'original
    for (int i = 0; i < n; i++) {
        tab_copie[i] = tab[i];
    }
    
    debut = clock();
    tri_fusion(tab_copie, 0, n - 1);
    fin = clock();
    
    free(tab_copie);
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}

double mesurer_temps_rapide(int tab[], int n) {
    clock_t debut, fin;
    int *tab_copie = (int*)malloc(n * sizeof(int));
    
    if (!tab_copie) return 0.0;
    
    // Copier le tableau pour ne pas modifier l'original
    for (int i = 0; i < n; i++) {
        tab_copie[i] = tab[i];
    }
    
    debut = clock();
    tri_rapide(tab_copie, 0, n - 1);
    fin = clock();
    
    free(tab_copie);
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}

double mesurer_temps_denombrement(int tab[], int n, int max_val) {
    clock_t debut, fin;
    int *tab_copie = (int*)malloc(n * sizeof(int));
    
    if (!tab_copie) return 0.0;
    
    // Copier le tableau pour ne pas modifier l'original
    for (int i = 0; i < n; i++) {
        tab_copie[i] = tab[i];
    }
    
    debut = clock();
    tri_denombrement(tab_copie, n, max_val);
    fin = clock();
    
    free(tab_copie);
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}
// ========== MESURE POUR TRI PAR TAS ==========
double mesurer_temps_tas(int tab[], int n) {
    clock_t debut, fin;
    int *tab_copie = (int*)malloc(n * sizeof(int));
    
    if (!tab_copie) return 0.0;
    
    // Copier le tableau pour ne pas modifier l'original
    for (int i = 0; i < n; i++) {
        tab_copie[i] = tab[i];
    }
    
    debut = clock();
    tri_par_tas(tab_copie, n);
    fin = clock();
    
    free(tab_copie);
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}

// pour tri shell
double mesurer_temps_shell(int tab[], int n) {
    clock_t debut, fin;
    int *tab_copie = (int*)malloc(n * sizeof(int));
    
    if (!tab_copie) return 0.0;
    
    // Copier le tableau pour ne pas modifier l'original
    for (int i = 0; i < n; i++) {
        tab_copie[i] = tab[i];
    }
    
    debut = clock();
    tri_shell(tab_copie, n);
    fin = clock();
    
    free(tab_copie);
    return (double)(fin - debut) / CLOCKS_PER_SEC;
}

// exceution du benchmark
void executer_benchmark(char *nom_algo, void (*fonction_tri)(int[], int), 
                        char *fichier_donnees, int taille, ResultatBenchmark *resultat) {
    FILE *fp = fopen(fichier_donnees, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fichier_donnees);
        return;
    }
    
    int taille_lue;
    fscanf(fp, "%d", &taille_lue);
    
    if (taille_lue != taille) {
        printf("Erreur: taille du fichier différente de celle attendue\n");
        fclose(fp);
        return;
    }
    
    strcpy(resultat->nom_algo, nom_algo);
    resultat->taille = taille;
    
    // Test sur tableau croissant
    int *tab = lire_tableau(fp, taille);
    if (!tab) { fclose(fp); return; }
    resultat->temps_croissant = mesurer_temps(fonction_tri, tab, taille);
    free(tab);
    
    // Test sur tableau décroissant
    tab = lire_tableau(fp, taille);
    if (!tab) { fclose(fp); return; }
    resultat->temps_decroissant = mesurer_temps(fonction_tri, tab, taille);
    free(tab);
    
    // Test sur tableau constant
    tab = lire_tableau(fp, taille);
    if (!tab) { fclose(fp); return; }
    resultat->temps_constant = mesurer_temps(fonction_tri, tab, taille);
    free(tab);
    
    // Test sur tableaux aléatoires (moyenne)
    double somme_temps = 0.0;
    for (int i = 0; i < 100; i++) {
        tab = lire_tableau(fp, taille);
        if (!tab) break;
        somme_temps += mesurer_temps(fonction_tri, tab, taille);
        free(tab);
    }
    resultat->temps_moyen = somme_temps / 100.0;
    
    fclose(fp);
    
    printf("Benchmark %s (taille %d) terminé\n", nom_algo, taille);
}

// on  SAUVEGARDE DES RÉSULTATS CSV
void sauvegarder_resultats_csv(char *nom_fichier, ResultatBenchmark resultats[], int nb_resultats) {
    FILE *fp = fopen(nom_fichier, "w");
    if (fp == NULL) {
        printf("Erreur lors de la création du fichier CSV\n");
        return;
    }
    
    
    fprintf(fp, "Algorithme,Taille,Croissant,Decroissant,Constant,Moyen\n");
    
    //  notre Données
    for (int i = 0; i < nb_resultats; i++) {
        fprintf(fp, "%s,%d,%.6f,%.6f,%.6f,%.6f\n",
                resultats[i].nom_algo,
                resultats[i].taille,
                resultats[i].temps_croissant,
                resultats[i].temps_decroissant,
                resultats[i].temps_constant,
                resultats[i].temps_moyen);
    }
    
    fclose(fp);
    printf("✓ Résultats sauvegardés dans %s (%d résultats)\n", nom_fichier, nb_resultats);
}

