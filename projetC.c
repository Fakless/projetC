#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAILLE 11 
#define BASE 10 

int error = 0;


typedef struct maillon{

    int valeur;
    struct maillon *suivant;

}maillon_m;


maillon_m* maillon_cree(int valeur, maillon_m* suivant){

    maillon_m*  element = (maillon_m*)malloc(sizeof(maillon_m));

    if(element == NULL){
        printf("Plus de mémoire");
        exit(1);
    }

    element->valeur = valeur;
    element->suivant = suivant;

    return element;
}


void maillon_libere(maillon_m* element){

    if (element != NULL){
        maillon_libere(element->suivant);
        free(element);
    }
}


void maillon_afficher(maillon_m* element, int i){

    if ( element != NULL ){
        maillon_afficher(element->suivant, i+1);
        printf("%d", element->valeur); 
        if ( i != 0 )
            printf(" ");
    }
}


maillon_m* addresse_suivant(maillon_m* maillon, maillon_m* element){

    if ( maillon == NULL ){
        element->suivant = NULL;
        return element;
    }

    element->suivant = maillon;

    return element;
}


int entier (char instruction[TAILLE]){

    int i;
    for ( i = 0; i < TAILLE && instruction[i] != '\0'; i++ )
        if ( instruction[0] != '-' && !isdigit(instruction[i]) )
            return 0;
    return 1;
}


int convertion_en_int (char instruction[TAILLE]){

    char *ptr;

    return strtol(instruction, &ptr, BASE);
}


maillon_m* maillon_pop ( maillon_m* element ){

    maillon_m* suivant = NULL;

    if ( element != NULL ){
        suivant = element->suivant;
        free(element);
    }

    return suivant;
}


maillon_m* maillon_add ( maillon_m* element ){

    if ( element == NULL )
        return NULL;

    if ( element->suivant == NULL ){
        error = 1;
        element = maillon_pop(element);
        return element;
    }

    int left = element->suivant->valeur, right = element->valeur;
    maillon_m* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);

    return addresse_suivant(suivant, maillon_cree(left+right, suivant));
}


maillon_m* maillon_sub ( maillon_m* element ){

    if ( element == NULL ) 
        return NULL;

    if ( element->suivant == NULL ){
        error = 1;
        element = maillon_pop(element);
        return element;
    }

    int left = element->suivant->valeur, right = element->valeur;
    maillon_m* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);

    return addresse_suivant(suivant, maillon_cree(left-right, suivant));
}


maillon_m* maillon_mul ( maillon_m* element ){

    if ( element == NULL )
        return NULL;

    if ( element->suivant == NULL ){
        error = 1;
        element = maillon_pop(element);
        return element;
    }

    int left = element->suivant->valeur, right = element->valeur;
    maillon_m* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);

    return addresse_suivant(suivant, maillon_cree(left*right, suivant));
}


maillon_m* maillon_div ( maillon_m* element ){

    if ( element == NULL )  
        return NULL;

    if ( element->suivant == NULL ){
        error = 1;
        element = maillon_pop(element);
        element = maillon_pop(element);
        return element;
    }

    int left = element->suivant->valeur, right = element->valeur;
    if ( right == 0 ){
        error = 1;
        element = maillon_pop(element);
        element = maillon_pop(element);
        return element;
    }

    maillon_m* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);

    return addresse_suivant(suivant, maillon_cree(left/right, suivant));
}


maillon_m* maillon_mod ( maillon_m* element ){

    if ( element == NULL ) 
        return NULL;

    if ( element->suivant == NULL ){
        error = 1;
        free(element);
        return element;
    }

    int left = element->suivant->valeur, right = element->valeur;
    maillon_m* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);

    return addresse_suivant(suivant, maillon_cree(left%right, suivant));
}


maillon_m* maillon_dup ( maillon_m* element ){

    if ( element == NULL ){
        return NULL;
    }

    return addresse_suivant(element, maillon_cree(element->valeur, element));
}


maillon_m* maillon_swp ( maillon_m* maillon ){

    if ( maillon == NULL ){
        return NULL;
    }

    if ( maillon->suivant != NULL ){
        int ech = maillon->valeur;
        maillon->valeur = maillon->suivant->valeur;
        maillon->suivant->valeur = ech;
    }

    return maillon;
}


int maillon_rol ( maillon_m * back, maillon_m* maillon, int i ){

    if ( back == NULL || maillon == NULL ){
        error = 1;
        return 0;
    }

    if ( i == 1 ){
        maillon_m* suivant = maillon->suivant;
        int valeur = maillon->valeur;
        free(maillon);
        back->suivant = suivant;
        return valeur;
    }

    else
         return maillon_rol ( maillon, maillon->suivant, i-1 );
}


maillon_m* faire_instruction ( maillon_m* maillon, char instruction[TAILLE] ){

    if ( !strcmp("ADD", instruction) )
        maillon = maillon_add(maillon);

    else if ( !strcmp("SUB", instruction) )
        maillon = maillon_sub(maillon);

    else if ( !strcmp("MUL", instruction) )
        maillon = maillon_mul(maillon);

    else if ( !strcmp("DIV", instruction) )
        maillon = maillon_div(maillon);

    else if ( !strcmp("MOD", instruction) )
        maillon = maillon_mod(maillon);

    else if ( !strcmp("POP", instruction) )
        maillon = maillon_pop(maillon);

    else if ( !strcmp("DUP", instruction) )
        maillon = maillon_dup(maillon);

    else if ( !strcmp("SWP", instruction) )
        maillon = maillon_swp(maillon);

    else if ( !strcmp("ROL", instruction) ){
        int n = maillon->valeur;
        maillon = maillon_pop(maillon);
        n = maillon_rol(maillon, maillon->suivant, n-1);
        maillon = addresse_suivant(maillon, maillon_cree(n, maillon));
    }
    else
        error = 1;

    return maillon;
}


int main(){

    int N, i;
    maillon_m* maillon = NULL;
    scanf("%d", &N);

    for ( i = 0; i < N && error != 1; i++ ){
        char instruction[TAILLE];
        scanf("%s", instruction);
        if ( entier(instruction) )
            maillon = addresse_suivant(maillon, maillon_cree(convertion_en_int(instruction), maillon));
        else
            maillon = faire_instruction(maillon, instruction);
    }

    maillon_afficher(maillon, 0);

    if ( error ){
        if ( maillon != NULL ){ 
               printf(" ");
           }       
        printf("ERROR");
    }

    printf("\n");
    maillon_libere(maillon);

    return 0;
}
