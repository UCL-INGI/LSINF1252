#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include "itest.h"


// global variables

json_t *itest_json ;
json_t *itest_results;

void itest_init(const char *name, char *comment) {
  int err;
  itest_json=json_object();
  if(itest_json==NULL) {
    fprintf(stderr,"Cannot create JSON root object\n");
    exit(EXIT_FAILURE);
  }
  itest_results = json_array();
  if(itest_results==NULL) {
    fprintf(stderr,"Cannot create JSON array\n");
    exit(EXIT_FAILURE);
  }  
  err=json_object_set_new( itest_json, "itest_name", json_string(name ) );
  if(err<0) {
    fprintf(stderr,"Cannot set itest_name object\n");
    exit(EXIT_FAILURE);
  }
  err=json_object_set_new( itest_json, "itest_comment", json_string(comment));
  if(err<0) {
    fprintf(stderr,"Cannot set itest_comment object\n");
    exit(EXIT_FAILURE);
  }

}

void itest(const char *name, int outcome, char *message) {
  int err;
  err=json_array_append_new( itest_results, json_pack("[sbs]",name, outcome,message));
  if(err<0) {
    fprintf(stderr,"Cannot append JSON array\n");
    exit(EXIT_FAILURE);
  }
}

void itest_close(char *file) {
  int err;
  err=json_object_set_new( itest_json, "itest_results", itest_results );
  if(err<0) {
    fprintf(stderr,"Cannot set itest_results object\n");
    exit(EXIT_FAILURE);
  }

  if(json_dump_file(itest_json, file, 0)<0) {
    fprintf(stderr,"Cannot write JSON file\n");
    exit(EXIT_FAILURE);
  }

}
