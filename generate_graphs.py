#!/usr/bin/env python3
"""
Script de génération de graphiques pour le TD de comparaison d'algorithmes de tri
Nécessite: pip install matplotlib pandas
"""

import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

# Configuration du style
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (14, 8)
plt.rcParams['font.size'] = 10

def charger_donnees():
    """Charge les données depuis le CSV"""
    try:
        df = pd.read_csv('results/resultats_complets.csv')
        return df
    except FileNotFoundError:
        print("❌ Erreur: Fichier results/resultats_complets.csv introuvable")
        print("Exécutez d'abord le programme C pour générer les résultats!")
        sys.exit(1)

def graphique_cas_moyen(df):
    """Graphique comparatif - Cas moyen (algos principaux Insertion/Fusion/Rapide/Denombrement/Tas/Shell)"""
    plt.figure(figsize=(14, 8))
    
    # Ordre et sélection des algorithmes comme dans la commande gnuplot donnée
    algos_ordre = ["Insertion", "Fusion", "Rapide", "Denombrement", "Tas", "Shell"]
    marqueurs = ['o', 's', '^', 'D', 'v', 'P']  # Un marqueur différent par algo
    
    for algo, marker in zip(algos_ordre, marqueurs):
        if algo in df['Algorithme'].unique():
            data = df[df['Algorithme'] == algo].sort_values('Taille')
            plt.plot(
                data['Taille'],
                data['Moyen'],
                marker=marker,
                linewidth=2,
                label=algo
            )
    
    plt.xlabel('Taille des tableaux', fontsize=12)
    plt.ylabel('Temps d\'exécution (secondes)', fontsize=12)
    plt.title(
        'Comparaison des algorithmes de tri - Cas moyen (100 tableaux aléatoires)',
        fontsize=14,
        fontweight='bold'
    )
    plt.legend(loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('graphs/comparaison_cas_moyen.png', dpi=150)
    print("✓ Graphique 'comparaison_cas_moyen.png' généré")
    plt.close()

def graphique_meilleur_pire_cas(df):
    """Graphique comparatif - Meilleur et pire cas"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
    
    # Meilleur cas (croissant)
    for algo in df['Algorithme'].unique():
        data = df[df['Algorithme'] == algo]
        ax1.plot(data['Taille'], data['Croissant'], marker='o', label=algo, linewidth=2)
    
    ax1.set_xlabel('Taille des tableaux', fontsize=11)
    ax1.set_ylabel('Temps (secondes)', fontsize=11)
    ax1.set_title('Meilleur cas (Tableau croissant)', fontsize=12, fontweight='bold')
    ax1.legend()
    ax1.grid(True)
    
    # Pire cas (décroissant)
    for algo in df['Algorithme'].unique():
        data = df[df['Algorithme'] == algo]
        ax2.plot(data['Taille'], data['Decroissant'], marker='s', label=algo, linewidth=2)
    
    ax2.set_xlabel('Taille des tableaux', fontsize=11)
    ax2.set_ylabel('Temps (secondes)', fontsize=11)
    ax2.set_title('Pire cas (Tableau décroissant)', fontsize=12, fontweight='bold')
    ax2.legend()
    ax2.grid(True)
    
    plt.tight_layout()
    plt.savefig('graphs/comparaison_tous_cas.png', dpi=150)
    print("✓ Graphique 'comparaison_tous_cas.png' généré")
    plt.close()

def graphique_complexite(df):
    """Graphique de vérification de la complexité"""
    plt.figure(figsize=(14, 8))
    
    tailles = sorted(df['Taille'].unique())
    
    # Courbes théoriques
    if len(tailles) > 1:
        n_base = tailles[0]
        t_base = 0.001  # Temps de référence
        
        # O(n^2)
        n_squared = [t_base * (n/n_base)**2 for n in tailles]
        plt.plot(tailles, n_squared, 'k--', label='O(n²)', linewidth=2, alpha=0.7)
        
        # O(n log n)
        import math
        n_log_n = [t_base * (n/n_base) * math.log(n/n_base + 1) for n in tailles]
        plt.plot(tailles, n_log_n, 'r--', label='O(n log n)', linewidth=2, alpha=0.7)
        
        # O(n)
        n_linear = [t_base * (n/n_base) for n in tailles]
        plt.plot(tailles, n_linear, 'g--', label='O(n)', linewidth=2, alpha=0.7)
    
    # Algorithmes réels
    for algo in df['Algorithme'].unique():
        data = df[df['Algorithme'] == algo]
        plt.plot(data['Taille'], data['Moyen'], marker='o', label=f'{algo} (réel)', linewidth=2)
    
    plt.xlabel('Taille des tableaux', fontsize=12)
    plt.ylabel('Temps d\'exécution (secondes)', fontsize=12)
    plt.title('Vérification de la complexité théorique vs réelle', fontsize=14, fontweight='bold')
    plt.legend(loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('graphs/verification_complexite.png', dpi=150)
    print("✓ Graphique 'verification_complexite.png' généré")
    plt.close()

def graphique_efficacite_relative(df):
    """Graphique d'efficacité relative"""
    plt.figure(figsize=(14, 8))
    
    algos = df['Algorithme'].unique()
    
    for algo in algos:
        data = df[df['Algorithme'] == algo]
        efficacite = data['Moyen'].values
        plt.plot(data['Taille'], efficacite, marker='o', label=algo, linewidth=2)
    
    plt.xlabel('Taille des tableaux', fontsize=12)
    plt.ylabel('Temps (secondes)', fontsize=12)
    plt.title('Efficacité relative des algorithmes (Cas moyen)', fontsize=14, fontweight='bold')
    plt.legend()
    plt.grid(True)
    plt.yscale('log')  # Échelle logarithmique pour mieux voir
    plt.tight_layout()
    plt.savefig('graphs/efficacite_relative.png', dpi=150)
    print("✓ Graphique 'efficacite_relative.png' généré")
    plt.close()

def graphique_optimisations_bulle(df):
    """Graphique des optimisations du tri à bulle"""
    plt.figure(figsize=(14, 8))
    
    # Filtrer les variantes de bulle
    algos_bulle = [algo for algo in df['Algorithme'].unique() 
                   if 'Bulle' in algo]
    
    for algo in algos_bulle:
        data = df[df['Algorithme'] == algo].sort_values('Taille')
        plt.plot(data['Taille'], data['Moyen'], marker='o', label=algo, linewidth=2)
    
    plt.xlabel('Taille des tableaux', fontsize=12)
    plt.ylabel('Temps d\'exécution (secondes)', fontsize=12)
    plt.title('Impact des optimisations - Tri à bulle', fontsize=14, fontweight='bold')
    plt.legend(loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('graphs/optimisations_bulle.png', dpi=150)
    print("✓ Graphique 'optimisations_bulle.png' généré")
    plt.close()

def tableau_recapitulatif(df):
    """Affiche un tableau récapitulatif"""
    print("\n" + "="*80)
    print("TABLEAU RÉCAPITULATIF DES RÉSULTATS")
    print("="*80)
    print(df.to_string(index=False))
    print("="*80)

def main():
    print("\n================================================================================")
    print(" GÉNÉRATION DES GRAPHIQUES - TD ALGORITHMES DE TRI")
    print("================================================================================\n")
    
    # Créer le dossier graphs s'il n'existe pas
    if not os.path.exists('graphs'):
        os.makedirs('graphs')
    
    # Charger les données
    df = charger_donnees()
    
    print(f"✓ Données chargées : {len(df)} résultats\n")
    
    # Générer les graphiques
    print("Génération des graphiques...\n")
    graphique_cas_moyen(df)
    graphique_meilleur_pire_cas(df)
    graphique_complexite(df)
    graphique_efficacite_relative(df)
    graphique_optimisations_bulle(df)
    tableau_recapitulatif(df)
    
    print("\n✓ Tous les graphiques ont été générés dans le dossier 'graphs/'")
    print("================================================================================\n")

if __name__ == "__main__":
    main()
