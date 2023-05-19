#include "../include/main_test.h"


/*
* Permet de run tous les tests de chaque module de test + les tests finaux.
* @params : ... (inutilisé mais c'est pour que la fonction soit compatible avec le main du programme.test).
*
* @return : CUE_SUCCESS si tout s'est correctement déroulé. Erreur CUnit sinon.
*/
int main(int argc, char *argv[]) {

   // Initialise le registre de test CUnit
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();


   // Lance la fonction 'main' de tout les modules de test.
   if (main_verifyOutput_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_display() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_struct_initializer_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_struct_creater_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_file_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_bellman_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_struct_free_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_threads_test() != CUE_SUCCESS) {
      return CU_get_error();
   }
   if (main_final_test() != CUE_SUCCESS) {
      return CU_get_error();
   }

   // Lance tous les tests en utilisant l'interface CUnit Basic
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}