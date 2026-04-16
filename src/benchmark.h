#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include <time.h>

// Structure pour stocker les résultats de benchmark
typedef struct {
    char nom_algo[50];
    int taille;
    double temps_croissant;
    double temps_decroissant;
    double temps_constant;
    double temps_moyen;
} ResultatBenchmark;

void generer_donnees(char *nom_fichier, int taille, int min, int max);

int* lire_tableau(FILE *fp, int taille);


double mesurer_temps(void (*fonction_tri)(int[], int), int tab[], int n);

double mesurer_temps_fusion(int tab[], int n);

double mesurer_temps_rapide(int tab[], int n);

double mesurer_temps_denombrement(int tab[], int n, int max_val);

// Nouveau : mesures pour tri par tas et tri Shell
double mesurer_temps_tas(int tab[], int n);
double mesurer_temps_shell(int tab[], int n);

// execution du benchmark
void executer_benchmark(char *nom_algo, void (*fonction_tri)(int[], int), 
                        char *fichier_donnees, int taille, ResultatBenchmark *resultat);

// SAUVEGARDe du resultat 
void sauvegarder_resultats_csv(char *nom_fichier, ResultatBenchmark resultats[], int nb_resultats);

#endif
