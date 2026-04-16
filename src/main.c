#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "tri.h"
#include "benchmark.h"

#define NB_TAILLES 7
#define MIN_VAL 0
#define MAX_VAL 20000


void generer_tous_fichiers() {
    int tailles[NB_TAILLES] = {1000, 2000, 3000, 5000, 7000, 10000, 15000};

    printf("=== GÉNÉRATION DES FICHIERS DE DONNÉES ===\n\n");

    for (int i = 0; i < NB_TAILLES; i++) {
        char nom_fichier[100];
        sprintf(nom_fichier, "data/donnees_%d.txt", tailles[i]);
        generer_donnees(nom_fichier, tailles[i], MIN_VAL, MAX_VAL);
    }

    printf("\n✓ Tous les fichiers de données ont été générés!\n\n");
}

void executer_tous_benchmarks() {
    int tailles[NB_TAILLES] = {1000, 2000, 3000, 5000, 7000, 10000, 15000};
    ResultatBenchmark resultats[NB_TAILLES * 11]; 
    int idx = 0;

    printf("=== EXÉCUTION DES BENCHMARKS ===\n\n");

    for (int i = 0; i < NB_TAILLES; i++) {
        int taille = tailles[i];
        char fichier[100];
        sprintf(fichier, "data/donnees_%d.txt", taille);

        printf("\n--- Taille: %d ---\n", taille);

        // Tris quadratiques seulement pour les petites tailles
        if (taille <= 5000) {
            executer_benchmark("Bulle_Basique",   tri_bulle_basique,    fichier, taille, &resultats[idx++]);
            executer_benchmark("Bulle_OptBoucle", tri_bulle_opt_boucle, fichier, taille, &resultats[idx++]);
            executer_benchmark("Bulle_OptArret",  tri_bulle_opt_arret,  fichier, taille, &resultats[idx++]);
            executer_benchmark("Cocktail",        tri_cocktail,         fichier, taille, &resultats[idx++]);
            executer_benchmark("Selection",       tri_selection,        fichier, taille, &resultats[idx++]);
        }

        
        executer_benchmark("Insertion", tri_insertion, fichier, taille, &resultats[idx++]);

        
        FILE *fp = fopen(fichier, "r");
        if (!fp) {
            printf("ERREUR: Impossible d'ouvrir %s\n", fichier);
            continue;
        }

        int taille_lue;
        if (fscanf(fp, "%d", &taille_lue) != 1 || taille_lue != taille) {
            printf("ERREUR: Taille incorrecte dans %s\n", fichier);
            fclose(fp);
            continue;
        }

        
        int *base = lire_tableau(fp, taille);
        if (!base) {
            printf("ERREUR: Lecture échouée pour %s\n", fichier);
            fclose(fp);
            continue;
        }

        // tri fusin
        ResultatBenchmark fusion;
        strcpy(fusion.nom_algo, "Fusion");
        fusion.taille = taille;
        fusion.temps_croissant  = mesurer_temps_fusion(base, taille);
        fusion.temps_decroissant = mesurer_temps_fusion(base, taille);
        fusion.temps_constant   = mesurer_temps_fusion(base, taille);
        fusion.temps_moyen = 0.0;
        for (int j = 0; j < 100; j++) {
            fusion.temps_moyen += mesurer_temps_fusion(base, taille);
        }
        fusion.temps_moyen /= 100.0;
        resultats[idx++] = fusion;
        printf("Benchmark Fusion (taille %d) terminé\n", taille);

        // tri rapide 
        ResultatBenchmark rapide;
        strcpy(rapide.nom_algo, "Rapide");
        rapide.taille = taille;
        rapide.temps_croissant   = mesurer_temps_rapide(base, taille);
        rapide.temps_decroissant = mesurer_temps_rapide(base, taille);
        rapide.temps_constant    = mesurer_temps_rapide(base, taille);
        rapide.temps_moyen = 0.0;
        for (int j = 0; j < 100; j++) {
            rapide.temps_moyen += mesurer_temps_rapide(base, taille);
        }
        rapide.temps_moyen /= 100.0;
        resultats[idx++] = rapide;
        printf("Benchmark Rapide (taille %d) terminé\n", taille);

        // -tri denombrmenent
        ResultatBenchmark denomb;
        strcpy(denomb.nom_algo, "Denombrement");
        denomb.taille = taille;
        denomb.temps_croissant   = mesurer_temps_denombrement(base, taille, MAX_VAL);
        denomb.temps_decroissant = mesurer_temps_denombrement(base, taille, MAX_VAL);
        denomb.temps_constant    = mesurer_temps_denombrement(base, taille, MAX_VAL);
        denomb.temps_moyen = 0.0;
        for (int j = 0; j < 100; j++) {
            denomb.temps_moyen += mesurer_temps_denombrement(base, taille, MAX_VAL);
        }
        denomb.temps_moyen /= 100.0;
        resultats[idx++] = denomb;
        printf("Benchmark Denombrement (taille %d) terminé\n", taille);

        // tri ^par tas 
        ResultatBenchmark tas;
        strcpy(tas.nom_algo, "Tas");
        tas.taille = taille;
        tas.temps_croissant   = mesurer_temps_tas(base, taille);
        tas.temps_decroissant = mesurer_temps_tas(base, taille);
        tas.temps_constant    = mesurer_temps_tas(base, taille);
        tas.temps_moyen = 0.0;
        for (int j = 0; j < 100; j++) {
            tas.temps_moyen += mesurer_temps_tas(base, taille);
        }
        tas.temps_moyen /= 100.0;
        resultats[idx++] = tas;
        printf("Benchmark Tas (taille %d) terminé\n", taille);

        // tri shell
        ResultatBenchmark shell;
        strcpy(shell.nom_algo, "Shell");
        shell.taille = taille;
        shell.temps_croissant   = mesurer_temps_shell(base, taille);
        shell.temps_decroissant = mesurer_temps_shell(base, taille);
        shell.temps_constant    = mesurer_temps_shell(base, taille);
        shell.temps_moyen = 0.0;
        for (int j = 0; j < 100; j++) {
            shell.temps_moyen += mesurer_temps_shell(base, taille);
        }
        shell.temps_moyen /= 100.0;
        resultats[idx++] = shell;
        printf("Benchmark Shell (taille %d) terminé\n", taille);

        free(base);
        fclose(fp);
    }

    sauvegarder_resultats_csv("results/resultats_complets.csv", resultats, idx);
    printf("\n✓ Tous les benchmarks sont terminés et sauvegardés !\n");
}

// notrre menu 
void afficher_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║   COMPARAISON D'ALGORITHMES DE TRI - TD4      ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. Générer les fichiers de données\n");
    printf("2. Exécuter tous les benchmarks\n");
    printf("3. Générer les graphiques (script Python)\n");
    printf("4. Tout faire automatiquement\n");
    printf("5. Quitter\n");
    printf("\nVotre choix: ");
}

// Fonction pour générer les graphiques avec le script Python
void generer_graphiques() {
    printf("\n=== GÉNÉRATION DES GRAPHIQUES ===\n\n");

    int ret = system("python generate_graphs.py");
    

    if (ret != 0) {
        printf("❌ Erreur: impossible d'exécuter generate_graphs.py\n");
        printf("Vérifiez que Python est installé et que matplotlib/pandas sont présents.\n");
    } else {
        printf("✓ Graphiques générés dans le dossier graphs/\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    int choix = 0;

    do {
        afficher_menu();
        if (scanf("%d", &choix) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            choix = 0;
        }

        switch (choix) {
            case 1:
                generer_tous_fichiers();
                break;
            case 2:
                executer_tous_benchmarks();
                break;
            case 3:
                generer_graphiques();
                break;
            case 4:
                generer_tous_fichiers();
                executer_tous_benchmarks();
                generer_graphiques();
                break;
            case 5:
                printf("Quitter...\n");
                break;
            default:
                if (choix != 0) printf("Choix invalide. Reessayez.\n");
        }

    } while (choix != 5);

    return 0;
}
