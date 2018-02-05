/* 
 * Tests unitaires pour insertion-sort
 *
 * Copyright (C) 2016 Henri Devillez
 *
 * O. Bonaventure, ajout du support pour i18n

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#include <stdbool.h>
#include "itest.h"

#include <libintl.h> 
#include <locale.h> 
#define _(STRING) gettext(STRING)

#include "student_code.h"

int global_last_exit=0;
int global_exit_called=0;

void my_exit(int status) {
  global_last_exit=status;
  global_exit_called++;
}

void testTrue() {
  char *argv[] = { "true"  , "abc", 0 };
  global_exit_called=0;
  global_last_exit=-1000;
  student_code(2, argv);
  if(global_exit_called==0) {
    itest(__func__,false,_("Lorsque votre programme est lancé dans un exécutable nommé true, il n'appelle pas la fonction exit de la librarie standard\n"));
  } else {
    if(global_last_exit!=0){
      itest(__func__,false,_("Lorsque votre programme est lancé dans un exécutable nommé true, il appelle la fonction exit de la librarie standard mais celle-ci ne retourne pas 0.\n"));
    } else {
      itest(__func__,true,_("Lorsque votre programme est lancé dans un exécutable nommé true, il appelle la fonction exit de la librarie standard et retourne 0.\n"));
    }
  }
}

void testFalse() {
  char *argv[] = { "false", 0  };
  global_exit_called=0;
  global_last_exit=-1000;
  student_code(1, argv);
  if(global_exit_called==0) {
    itest(__func__,false,_("Lorsque votre programme est lancé dans un exécutable nommé false, il n'appelle pas la fonction exit de la librarie standard\n"));
  } else {
    if(global_last_exit==0){
      itest(__func__,false,_("Lorsque votre programme est lancé dans un exécutable nommé false, il appelle la fonction exit de la librarie standard mais celle-ci retourne 0.\n"));
    } else {
      itest(__func__,true,_("Lorsque votre programme est lancé dans un exécutable nommé true, il appelle la fonction exit de la librarie standard et retourne une valeur différente de 0.\n"));
    }
  }
}


int main(int argc, char * argv[]) 
{
  itest_init("truefalse",_("Les sources de cet exercice sont disponibles sur github. Si vous remarquez une erreur, merci de remplir un pull request à l'URL : https://github.com/obonaventure/cnp3/new/master?filename=truefalse&value=bug\n"));

  testTrue();
  testFalse(); 
  
  itest_close("tests.json");
}