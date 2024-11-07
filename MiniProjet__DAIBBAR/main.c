#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration de la structure pour les moteurs
struct moteurs {
    char nom[100];
    int prix;
    int disponible;
};

// Déclaration de la structure pour les adhérents
struct adherents {
    char nom[100];
    char nomMoteur[100];
};

// Fonction pour afficher le menu principal
void afficherMenu() {
    printf("\n---------------------------------------------  Menu des Moteurs  ----------------------------------------------------\n\n");
    printf("\t\t\t\t\t[1]. Afficher les adherents :\n\t\t\t\t\t\tListe alphabetique des adherents\n\n");
    printf("\t\t\t\t\t[2]. Gestion des Moteurs :\n");
    printf("\t\t\t\t\t\t[1] Ajouter\n");
    printf("\t\t\t\t\t\t[2] Modifier\n");
    printf("\t\t\t\t\t\t[3] Supprimer\n");
    printf("\t\t\t\t\t\t[4] Afficher (liste alphabetique)\n\n");
    printf("\t\t\t\t\t[3]. Gestion des Emprunts :\n");
    printf("\t\t\t\t\t\t[1] Emprunter un moteur\n");
    printf("\t\t\t\t\t\t[2] Rendre un moteur\n\n");
    printf("\t\t\t\t\t[4]. Quitter\n");
    printf("\n---------------------------------------------------------------------------------------------------------------------\n\n");
}

// Fonction pour ajouter un nouveau moteur
void ajouterMoteur(struct moteurs* moteur, int* pborne, int taille) {
    if (*pborne >= taille) {
        printf("Le stockage est plein!\n");
        return;
    }

    // Saisie des informations du nouveau moteur
    printf("Veuillez entrer le nom du nouveau moteur: ");
    fgets(moteur[*pborne].nom, 100, stdin);
    moteur[*pborne].nom[strcspn(moteur[*pborne].nom, "\n")] = '\0';

    printf("Veuillez entrer le prix du moteur: ");
    scanf("%d", &moteur[*pborne].prix);
    getchar();

    printf("Si le moteur est disponible, entrez 1, sinon entrez 0: ");
    scanf("%d", &moteur[*pborne].disponible);
    getchar();

    // Enregistrement du moteur dans le fichier store.txt
    FILE* file = fopen("store.txt", "a");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }
    fprintf(file, "%s %d %d\n", moteur[*pborne].nom, moteur[*pborne].prix, moteur[*pborne].disponible);
    fclose(file);

    (*pborne)++;

    // Mise à jour du fichier pborne.txt avec le nouveau nombre de moteurs
    FILE* fb = fopen("pborne.txt", "w");
    if (fb == NULL) {
        perror("Erreur d'ouverture de pborne.txt");
        exit(1);
    }
    fprintf(fb, "%d", *pborne);
    fclose(fb);
}

// Fonction pour remplir le stock des moteurs à partir du fichier store.txt
void remplirStock(struct moteurs* moteur, int* pborne) {
    FILE* file = fopen("store.txt", "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    for (int i = 0; i < *pborne; i++) {
        fscanf(file, "%s %d %d", moteur[i].nom, &moteur[i].prix, &moteur[i].disponible);
    }
    fclose(file);
}

// Fonction pour enregistrer les moteurs dans le fichier store.txt
void remplirFichier(struct moteurs* moteur, int* pborne) {
    FILE* file = fopen("store.txt", "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    for (int i = 0; i < *pborne; i++) {
        fprintf(file, "%s %d %d\n", moteur[i].nom, moteur[i].prix, moteur[i].disponible);
    }
    fclose(file);
}

// Fonction pour rechercher un moteur par son nom
void rechercherMoteur(struct moteurs* moteur, int* pborne) {
    char nomMoteur[100];

    if (*pborne == 0) {
        printf("Le stock est vide!\n");
        return;
    }

    printf("Entrez le nom du moteur que vous voulez: ");
    fgets(nomMoteur, 100, stdin);
    nomMoteur[strcspn(nomMoteur, "\n")] = '\0';

    for (int i = 0; i < *pborne; i++) {
        if (strcmp(nomMoteur, moteur[i].nom) == 0) {
            printf("Nom du moteur: %s\n", moteur[i].nom);
            printf("Prix du moteur: %d\n", moteur[i].prix);
            printf("Numero de commande: %d\n", i);
            return;
        }
    }

    printf("Ce moteur n'est pas disponible. Veuillez verifier si le nom est correct.\n");
}

// Fonction pour modifier les informations d'un moteur
void modifierMoteur(struct moteurs* moteur, int* pborne) {
    int choix, i;

    printf("Pour changer le nom, entrez 1\nPour changer le prix, entrez 2\nPour changer la disponibilite, entrez 3\nToute autre entree quittera le programme\n");
    scanf("%d", &choix);
    getchar();

    printf("Entrez le numero de commande: ");
    scanf("%d", &i);
    getchar();

    if (i >= *pborne || i < 0) {
        printf("Numero de commande invalide.\n");
        return;
    }

    if (choix == 1) {
        printf("Entrez le nouveau nom: ");
        fgets(moteur[i].nom, 100, stdin);
        moteur[i].nom[strcspn(moteur[i].nom, "\n")] = '\0';
        printf("Le changement a ete effectue avec succes!\n");
    } else if (choix == 2) {
        printf("Entrez le nouveau prix: ");
        scanf("%d", &moteur[i].prix);
        printf("Le changement a ete effectue avec succes!\n");
    } else if (choix == 3) {
        printf("Entrez 1 pour disponible, 0 pour indisponible: ");
        scanf("%d", &moteur[i].disponible);
        printf("Le changement a ete effectue avec succes!\n");
    } else {
        printf("Choix invalide. Quitter.\n");
    }
}

// Fonction pour supprimer un moteur
void supprimerMoteur(struct moteurs* moteur, int* pborne) {
    int commande;
    printf("Entrez le numero de commande du moteur a supprimer: ");
    scanf("%d", &commande);
    getchar();

    if (commande >= *pborne || commande < 0) {
        printf("Numero de commande invalide.\n");
        return;
    }

    // Décalage des moteurs pour supprimer celui sélectionné
    for (int i = commande; i < *pborne - 1; i++) {
        moteur[i] = moteur[i + 1];
    }
    (*pborne)--;

    // Mise à jour des fichiers après la suppression
    remplirFichier(moteur, pborne);
    FILE* fb = fopen("pborne.txt", "w");
    if (fb == NULL) {
        perror("Erreur d'ouverture de pborne.txt");
        exit(1);
    }
    fprintf(fb, "%d", *pborne);
    fclose(fb);

    printf("Moteur supprime avec succes.\n");
}

// Fonction pour afficher tous les moteurs en stock
void afficherStockMoteurs(struct moteurs* moteur, int* pborne) {
    FILE* file = fopen("store.txt", "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    for (int i = 0; i < *pborne; i++) {
        fscanf(file, "%s %d %d", moteur[i].nom, &moteur[i].prix, &moteur[i].disponible);
        printf("Moteur %d: %s,    Prix: %d\n", i + 1, moteur[i].nom, moteur[i].prix);
        printf("\n--------------------------------------------------------------\n");
    }
    fclose(file);
}

// Fonction pour gérer le paiement et l'emprunt d'un moteur
void methodePaiement(struct moteurs* moteur, struct adherents* adherent, int* pborne, int* borno) {
    int commandeMoteur, prix;

    printf("Si vous voulez le prendre, veuillez entrer le numero de commande pour verifier: ");
    scanf("%d", &commandeMoteur);
    getchar();

    if (commandeMoteur >= *pborne || commandeMoteur < 0) {
        printf("Numero de commande invalide.\n");
        return;
    }

    if (moteur[commandeMoteur].disponible == 1) {
        printf("Le prix est: %d\n", moteur[commandeMoteur].prix);
        printf("Veuillez payer le prix: ");
        scanf("%d", &prix);
        getchar();

        printf("Veuillez entrer votre nom: ");
        fgets(adherent->nom, 100, stdin);
        adherent->nom[strcspn(adherent->nom, "\n")] = '\0';

        FILE* fm = fopen("borno.txt", "w+");
        (*borno)++;
        fprintf(fm, "%d", *borno);
        fclose(fm);

        // Enregistrement de l'emprunt dans le fichier baseAdherents.txt
        FILE* f = fopen("baseAdherents.txt", "a");
        if (f == NULL) {
            perror("Erreur d'ouverture de baseAdherents.txt");
            exit(1);
        }
        fprintf(f, "%s %s\n", adherent->nom, moteur[commandeMoteur].nom);
        fclose(f);

        moteur[commandeMoteur].disponible = 0;
        remplirFichier(moteur, pborne);

        printf("Merci pour votre paiement!\n");
    } else {
        printf("Ce moteur n'est pas disponible.\n");
    }
}

// Fonction pour comparer les noms des adhérents (pour tri alphabétique)
int comparerNoms(const void *a, const void *b) {
    struct adherents *adherent1 = (struct adherents *)a;
    struct adherents *adherent2 = (struct adherents *)b;
    return strcmp(adherent1->nom, adherent2->nom);
}

// Fonction pour afficher les adhérents triés par ordre alphabétique
void afficherAdherents(int borno) {
    struct adherents adherento[borno];
    FILE* ff = fopen("baseAdherents.txt", "r");
    if (ff == NULL) {
        perror("Erreur d'ouverture de baseAdherents.txt");
        return;
    }
    for(int i=0; i<borno; i++){
        fscanf(ff,"%s %s",adherento[i].nom,adherento[i].nomMoteur);
    }
    qsort(adherento,borno,sizeof(struct adherents),comparerNoms);

    for(int i =0 ; i<borno;i++) {
        printf("\t\t\t\t\t  Nom de l'adherent: %s\n\t\t\t\t\t  Nom du moteur emprunte: %s\n", adherento[i].nom, adherento[i].nomMoteur);
        printf("------------------------------------------------------------------------------------------------------------------------\n");
    }
    fclose(ff);
}

// Fonction pour mettre à jour le fichier des adhérents après une modification
void miseAJourFichierAdherents(struct adherents* listeAdherents, int count) {
    FILE* file = fopen("baseAdherents.txt", "w");
    if (file == NULL) {
        perror("Erreur d'ouverture de baseAdherents.txt");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", listeAdherents[i].nom, listeAdherents[i].nomMoteur);
    }
    fclose(file);
}

// Fonction pour mettre à jour les informations d'un adhérent
void miseAJourAdherent(char* nomAdherent) {
    struct adherents listeAdherents[100];
    int count = 0;

    FILE* file = fopen("baseAdherents.txt", "r");
    if (file == NULL) {
        perror("Erreur d'ouverture de baseAdherents.txt");
        return;
    }

    while (fscanf(file, "%s %s", listeAdherents[count].nom, listeAdherents[count].nomMoteur) != EOF) {
        if (strcmp(listeAdherents[count].nom, nomAdherent) == 0) {
            strcpy(listeAdherents[count].nomMoteur, "None");
        }
        count++;
    }
    fclose(file);

    miseAJourFichierAdherents(listeAdherents, count);
}

// Fonction pour rendre un moteur emprunté
void rendreMoteur(struct moteurs* moteur, int* pborne) {
    char nomAdherent[100];

    printf("Entrez votre nom: ");
    fgets(nomAdherent, 100, stdin);
    nomAdherent[strcspn(nomAdherent, "\n")] = '\0';

    miseAJourAdherent(nomAdherent);

    for (int i = 0; i < *pborne; i++) {
        if (moteur[i].disponible == 0) {
            moteur[i].disponible = 1;
            printf("Le moteur a ete retourne avec succes.\n");
            remplirFichier(moteur, pborne);
            return;
        }
    }
    printf("Moteur non trouve.\n");
}

int main() {
    struct moteurs moteur[100];
    struct adherents adherent;
    int pborne, borno, choix;
    system("color f1");

    // Lecture du nombre de moteurs stockés à partir du fichier pborne.txt
    FILE* fb = fopen("pborne.txt", "r");
    if (fb == NULL) {
        pborne = 0;
    } else {
        fscanf(fb, "%d", &pborne);
        fclose(fb);
    }

    // Lecture du nombre d'adhérents à partir du fichier borno.txt
    FILE* fm = fopen("borno.txt", "r");
    if (fm == NULL) {
        borno = 0;
    } else {
        fscanf(fm, "%d", &borno);
        fclose(fm);
    }

    remplirStock(moteur, &pborne);

    while (1) {
        afficherMenu();
        printf("\t\t\t\t\tVeuillez entrer votre choix: ");

        scanf("%d", &choix);
        getchar();
        system("cls");
        switch (choix) {
            case 1:
                afficherAdherents(borno);
                break;
            case 2:
                printf("[1] Ajouter Moteur\n[2] Modifier Moteur\n[3] Supprimer Moteur\n[4] Afficher Tous les Moteurs\n");
                int sousChoix;
                scanf("%d", &sousChoix);
                getchar();
                if (sousChoix == 1) {
                    ajouterMoteur(moteur, &pborne, 100);
                } else if (sousChoix == 2) {
                    modifierMoteur(moteur, &pborne);
                } else if (sousChoix == 3) {
                    rechercherMoteur(moteur, &pborne);
                    supprimerMoteur(moteur, &pborne);
                } else if (sousChoix == 4) {
                    afficherStockMoteurs(moteur, &pborne);
                }
                break;
            case 3:
                printf("                     [1] Emprunter Moteur\n        [2] Rendre Moteur\n");
                scanf("%d", &choix);
                getchar();
                if (choix == 1) {
                    rechercherMoteur(moteur, &pborne);
                    methodePaiement(moteur, &adherent, &pborne, &borno);
                } else if (choix == 2) {
                    rendreMoteur(moteur, &pborne);
                }
                break;
            case 4:
                printf("Au revoir!\n");
                return 0;
            default:
                printf("Choix invalide.\n");
                break;
        }
    }

    return 0;
}
