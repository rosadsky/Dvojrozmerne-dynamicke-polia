#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define SUBOR "osemsmerovka.txt"
#define N 3
#define NSLOVO 100

//---------------------------------------------------//
//              Projekt_1 | ROMAN OSADSKÝ            //
//---------------------------------------------------//

char **nacitaj(FILE *f,char **smerovka) {
    int m,n,riadky=0,stplce=0,necitat = 0;
    char c;

    if ((f = fopen(SUBOR, "r")) == NULL) {
        printf("Subor sa nepodarilo otvorit\n");
        return 0;
    }

    // ---------------- ZISTENIE VEĽKOSTI POĽA -----------------//

    fscanf(f, "%d %d",&m, &n );

    // ----------------------- MALLOC POLA  --------------------//

    smerovka = (char**)malloc(m * sizeof(char*));
    for (int k = 0; k < n ; k++) {
        smerovka[k] = (char*)malloc(n * sizeof(char));
    }

    // ----------------- NAČÍTAVANIE TAJNIČKY -------------------//

    while( (c=fgetc(f)) != EOF ) {
        if(c>='A' && c <= 'Z') {
            smerovka[riadky][stplce] = c;
            stplce++;
        }
        if(c =='\n'){
            necitat++;
            if(necitat>1) {
                riadky++;
                stplce = 0;
            }
        }

        if(riadky==m)
            break;
    }
    fclose(f);
    return smerovka;
}

int **vyskyt(char **smerovka,int **index){
    FILE *f;
    int m,n,x=0;
    char abc = 'A';

    index = (int**)malloc(26 * sizeof(int*));
    for (int rea = 0; rea < 26 ; rea++) {
        index[rea] = (int*)malloc(N * sizeof(int));
    }

    // ----------------- HODNOTY M,N -------------------//

    if ((f = fopen(SUBOR, "r")) == NULL);
    fscanf(f, "%d %d",&m, &n );

    // ----------------- HODIM TAM -1 -------------------//

    for (int l = 0; l < 26 ; l++) {
        for (int i = 0; i < N ; i++) {
            index[l][i] = -1;
        }
    }

    // ----------------- NAČÍTAVANIE HODNOT M,N -------------------//

    int col=0,realoc = 1,colmax=0;

    for (int k = 0; k < 26; ++k) {             //CELA ABECEDA
        for ( int i = 0; i < m; i++) {          //RIADKY
            for ( int j = 0; j < n; j++) {       //STLPCE
                if (smerovka[i][j] == abc){

                    // ----------------- AK TREBA REALOKOVAŤ -------------------//

                    if (col == N*realoc-1){
                        for (int l = 0; l < 26; l++) {
                            index[l] = (int *) realloc(index[l], (N * col+1) * sizeof(int));
                        }

                        realoc++;

                        if(realoc>2)
                            colmax=col;

                        int mk;

                        if(k == 0){
                            mk =1;
                        } else{
                            mk = k;
                        }

                        // ----------------- DÁVAME -1 NA ZVYŠOK -------------------//

                        for (int j1 = mk; j1 < 26 ; j1++) {
                            for (int l = 0; l < N * realoc+colmax; l++) {
                                index[j1][l] = -1;
                            }
                        }
                    }

                    // ----------------- ZÁPIS SÚRADNÍC -------------------//

                    index[abc - 'A' ][x] = i;   //X suradnica
                    x++;
                    index[abc - 'A'][x] = j;    // Y suradnica
                    x++;
                    col++;
                }
            }
        }
        col = 0; abc++; x=0; //0
    }

    // ----------------- KTORÉ PÍSMENO NEMÁ ŽIADNU SÚRADNICU TAK NULL -------------------//

    for (int k1 = 0; k1 < 26 ; k1++) {
        if(index[k1][0]==-1){
            for (int i = 0; i < N* realoc+colmax; i++)
                index[k1][i]=NULL;
        }
    }

    for (int i1 = 0; i1 < 26; i1++) {
        for (int i = 0; i < N*realoc+colmax ; i++) {
        }
    }
    return index;
}

int stvorsmerovka (char **smerovka, int **index ){
    FILE *f;
    int m,n,i,j,x = 0,zhoda = 0,stlpcei = 0,zalomenie = 0;
    char c;
    char slovo[NSLOVO];
    int vyskyt=0,tajnickaslov = 0;

    if ((f = fopen(SUBOR, "r")) == NULL);
    fscanf(f, "%d %d",&m, &n );

    // ----------------ZISTIM POČET STPCOV PRE IDEX -----------------//

    for (int i = 0; i < 25 ; i++) {
        stlpcei++;
        if(i%2!=0)
            if (index[0][i+1]==0)
                if(index[0][i+2]==0)
                    if(index[0][i+3]==0)
                        break;
    }

    // ---------------- ČÍTAM TAJNIČKU -----------------//

    while( (c=fgetc(f)) != EOF ) {
        if(c== '\n' ){
            zalomenie++;
        }
        if(zalomenie>m){
            fscanf(f,"%s",slovo);

            // ---------------- AK JE STRING PRÁZDNY, ALEBO VAČŠÍ AKO 100 -----------------//

            if(strlen(slovo) < 1 || strlen(slovo)>NSLOVO)
                break;

            // ---------------- OŠETRENIE ABY SA NÁM NENAČÍTALI -1 -----------------//

            for (int g = 0; g < stlpcei; g++) {
                if (index[slovo[0] - 'A'][g*2] == -1){
                    break;
                }

                // ---------------- VÝCHOD -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < strlen(slovo); k++) { //po dlžku slova idem
                    if (j>=n) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++; j++; zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < strlen(slovo); l++) {
                                j--;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ---------------- JUH -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i>=m) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++;i++;zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i--;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ---------------- JUHOVÝCHOD -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i>=m || j>=n) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++;j++;i++;zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i--;j--;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ---------------- ZÁPAD -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k <strlen(slovo); k++) {
                    if(j<0) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++; j--; zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                j++;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ---------------- SEVER -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i<0) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++;i--;zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i++;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ---------------- SEVERO VÝCHOD -----------------//


                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i>=m ||j<0 ) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++; i++; j--; zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i--; j++;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ----------------  JUHO ZÁPAD -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i<0 || j<0) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++;i--;j--;zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i++;j++;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

                // ----------------  JUHOVÝCHOD -----------------//

                i = index[slovo[0] - 'A'][g*2]; //x
                j = index[slovo[0] - 'A'][g*2 + 1]; //y
                x = 0;

                for (int k = 0; k < (strlen(slovo)); k++) {
                    if(i<0 ||j>=n ) // --- OŠETRENIE PROGRAMU, ABY NEŠIEL MIMO POĽA SMEROVKA
                        break;
                    if (smerovka[i][j] == slovo[x] || ((smerovka[i][j]) - 32) == slovo[x]) {
                        x++; i--; j++; zhoda++;
                        if (zhoda == strlen(slovo)) { //dlžka slova
                            vyskyt++;
                            for (int l = 0; l < (strlen(slovo)); l++) {
                                i++; j--;
                                smerovka[i][j] = tolower(smerovka[i][j]);
                            }
                        }
                    }
                } zhoda = 0;

            }

            // --------------  KEĎ SA SLOVO V TAJNIČKE NENACHÁDZA --------------- //

            if(vyskyt==0){
                printf("Slovo ");
                printf("%s",slovo);
                printf(" sa nenachádza v tajničke\n");
            } vyskyt=0;

            // ----------------  VÝPIS UPRAVENEJ OSMESMEROVKY ----------------- //

            for (int i1 = 0; i1 < m ; i1++) {
                for (int k = 0; k < n ; k++) {
                    printf("%c",smerovka[i1][k]);

                }
                printf("\n");
            }
            printf("\n");

            // ----------------  RESET SLOVA  -----------------//

            for (int l = 0; l <NSLOVO ; l++)
                slovo[l] = NULL;
        }
    }

    // ----------------  VYPIS VŠETKÝCH MALÝCH PÍSMEN  -----------------//


    for (int j1 = 0; j1 < m ; j1++) {
        for (int k = 0; k < n ; k++) {
            if(smerovka[j1][k] > 64 && smerovka[j1][k] < 91 ){
                printf("%c",smerovka[j1][k]);
                tajnickaslov++;
            }
        }
    }

    // ----------------  AK SÚ VŠETKY PÍSMENA VYŠKRTANÉ -----------------//

    if(tajnickaslov==0){
        printf("Tajnicka je prazdna.\n");
    }

    // ----------------  UVOĽNENIE PAMATI-----------------//

    for(int i=0;i<26;i++){
        free(index[i]);
    }

    for(int i=0;i<m;i++){
        free(smerovka[i]);
    }

}

int main() {
    char **smerovka;
    FILE *f;
    int **index;

    smerovka = nacitaj(f,smerovka);

    index = vyskyt(smerovka,index);

    stvorsmerovka(smerovka,index);

    fclose(f);
    return 0;
}