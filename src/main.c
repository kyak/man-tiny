
#include <stdio.h> // for fprintf
#include <stdlib.h> // for getenv
#include <unistd.h> // for access


typedef  int  bool;
#define  FALSE  0
#define  TRUE   1

#define  LENGTH_OF(a)  ( sizeof(a) / sizeof(a[0]) )


#define  ANY_SECTION  0


// This is for different forms that man page file names can take such as:
//  + time.2
//  + time.2.gz
//
typedef struct
{
  char  *name_template;
  char  *emitter;
}
Form;


int main( int argc, char **argv)
{
  // Parse the command-line
  if ( argc < 2  ||  3 < argc)
  {
    fprintf( stderr, "Use: %s [section] page\n", argv[0]);
    return 1;
  }

  int   section;
  char  *page;

  if ( 2 == argc)
  {
    page = argv[ 1];
    section = ANY_SECTION;
  }
  else {
    section = atoi( argv[1]);
    page = argv[ 2];
  }

  // Parse the MANPATH
  char  *man_path_s = getenv("MANPATH");
  if ( ! man_path_s)
    man_path_s = "/usr/share/man";
  #define  MAX_MAN_PATH_ITEMS  32
  char  *man_path_item[ MAX_MAN_PATH_ITEMS];
  int   man_path_items = 0;
  // Split the colon separated MANPATH in to discrete strings
  char  *cursor = man_path_s;
  bool  fresh_item = TRUE;
  while( *cursor != '\0')
  {
    if ( fresh_item)
    {
      if ( man_path_items < MAX_MAN_PATH_ITEMS)
        man_path_item[ man_path_items++] = cursor;
      fresh_item = FALSE;
    }
    if ( ':' == *cursor)
    {
      *cursor = '\0';
      fresh_item = TRUE;
    }
    cursor += 1;
  }

  // Go through every item on MANPATH
  int  mpi;
  for ( mpi = 0;  mpi < man_path_items;  ++ mpi)
  {
    char  *man_dir = man_path_item[ mpi];

    // Go through sections 1..9 or just the specified section
    int  min_s = ANY_SECTION == section ? 1 : section;
    int  max_s = ANY_SECTION == section ? 9 : section;
    int  s;
    for ( s = min_s;  s <= max_s;  ++ s)
    {
      // If the named man page exists in this (MANPATH item, section) then
      // display it
      char  path_to_file[ 256];
      Form  form[] = {
        {"%s/man%i/%s.%i",    emitter:"cat"},
        {"%s/man%i/%s.%i.gz", emitter:"zcat"},
      };
      char  cmd[ 512] = {'\0'};
      int  i;
      for ( i = 0;  i < LENGTH_OF(form);  ++ i)
      {
        Form  f = form[ i];

        snprintf( path_to_file, sizeof(path_to_file), f.name_template, man_dir, s, page, s);
        if ( ! access( path_to_file, R_OK))
        {
          snprintf( cmd, sizeof(cmd), "%s %s | mandoc | less", f.emitter, path_to_file);
          break;
        }
      }

      if ( cmd[0] != '\0')
      {
        system( cmd);
        return 0;
      }
    }
  }

  fprintf( stderr, "No manual entry for %s\n", page);
  return 1;
}

