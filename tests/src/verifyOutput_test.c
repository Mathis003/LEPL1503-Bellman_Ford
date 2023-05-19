#include "../include/verifyOutput_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_verifyOutput(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_verifyOutput(void) {
    return EXIT_SUCCESS;
}

/*
* Teste la fonction check_same_size.
* La fontion vérifie si deux fichiers ont la même taille.
* Ici, les deux fichiers ont la même taille.
*/
void check_same_size_True_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        closeFile(f2, true);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f2, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if ((f1 = fopen("fichier1", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    if ((f2 = fopen("fichier2", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_TRUE(check_same_size(f1, f2, false));

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }

    remove("fichier1");
    remove("fichier2");
}


/*
* Teste la fonction check_same_size.
* La fontion vérifie si deux fichiers ont la même taille.
* Ici, les deux fichiers n'ont pas la même taille.
*/
void check_same_size_False_Test(void) {
    
    FILE *f1;  
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        closeFile(f2, true);
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if ((f1 = fopen("fichier1", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    if ((f2 = fopen("fichier2", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_FALSE(check_same_size(f1, f2, false));
    
    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }

    remove("fichier1");
    remove("fichier2");
}


/*
* Teste la fonction check_same_contains.
* La fontion vérifie si deux fichiers ont les mêmes données.
* Ici, les deux fichiers ont les mêmes données.
* Note: Cette fonction est seulement utilisée pour deux fichiers de même taille.
*/
void check_same_contains_True_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        closeFile(f2, true);
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if ((f1 = fopen("fichier1", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    if ((f2 = fopen("fichier1", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_TRUE(check_same_contains(f1, f2));

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    
    remove("fichier1");
    remove("fichier2");
}


/*
* Teste la fonction check_same_contains.
* La fontion vérifie si deux fichiers ont les mêmes données.
* Ici, les deux fichiers n'ont pas les mêmes données.
* Note: Cette fonction est seulement utilisée pour deux fichiers de même taille.
*/
void check_same_contains_False_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;
    uint32_t data4 = 2;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data4, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        closeFile(f2, true);
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    if ((f1 = fopen("fichier1", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    
    if ((f2 = fopen("fichier2", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_FALSE(check_same_contains(f1, f2));

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    
    remove("fichier1");
    remove("fichier2");
}


/*
* Test final de la fonction compare_two_outputFile.
* La fontion vérifie si deux fichiers sont identiques (même taille + même contenu).
* Ici, les deux fichiers sont identiques.
*/
void compare_two_outputFile_True_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_TRUE(compare_two_outputFile("fichier1", "fichier2", false));

    remove("fichier1");
    remove("fichier2");
}


/*
* Test final de la fonction compare_two_outputFile.
* La fontion vérifie si deux fichiers sont identiques (même taille + même contenu).
* Ici, les deux fichiers ne sont pas identiques (en contenu).
*/
void compare_two_outputFile_False_1_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    
    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;
    uint32_t data4 = 2;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data4, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_FALSE(compare_two_outputFile("fichier1", "fichier2", false));

    remove("fichier1");
    remove("fichier2");
}


/*
* Test final de la fonction compare_two_outputFile.
* La fontion vérifie si deux fichiers sont identiques (même taille + même contenu).
* Ici, les deux fichiers ne sont pas identiques (en taille).
*/
void compare_two_outputFile_False_2_Test(void) {

    FILE *f1;
    if ((f1 = fopen("fichier1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    FILE *f2;
    if ((f2 = fopen("fichier2", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t data1 = 356;
    int64_t data2 = -99902;
    uint32_t data3 = 1;

    if (fwrite(&data1, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data1, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data2, sizeof(int64_t), 1, f1) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&data3, sizeof(uint32_t), 1, f2) != 1) {
        printMethodFailed("fwrite", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(f1, true) == EXIT_FAILURE) {
        remove("fichier1");
        closeFile(f2, true);
        remove("fichier2");
        exit(EXIT_FAILURE);
    }
    if (closeFile(f2, true) == EXIT_FAILURE) {
        remove("fichier1");
        remove("fichier2");
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_FALSE(compare_two_outputFile("fichier1", "fichier2", false));

    remove("fichier1");
    remove("fichier2");
}


/*
* Fonction principale lançant les tests du module verifyOutput.c.
* Cette fonction est appelée par le main du programme.test
*/
int main_verifyOutput_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test verifyOutput.c", init_suite_verifyOutput, clean_suite_verifyOutput);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "check_same_size_True_Test", check_same_size_True_Test)) ||
        (NULL == CU_add_test(pSuite, "check_same_size_False_Test", check_same_size_False_Test)) ||
        (NULL == CU_add_test(pSuite, "check_same_contains_True_Test", check_same_contains_True_Test)) ||
        (NULL == CU_add_test(pSuite, "check_same_contains_False_Test", check_same_contains_False_Test)) ||
        (NULL == CU_add_test(pSuite, "compare_two_outputFile_True_Test", compare_two_outputFile_True_Test)) ||
        (NULL == CU_add_test(pSuite, "compare_two_outputFile_False_1_Test", compare_two_outputFile_False_1_Test)) ||
        (NULL == CU_add_test(pSuite, "compare_two_outputFile_False_2_Test", compare_two_outputFile_False_2_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}