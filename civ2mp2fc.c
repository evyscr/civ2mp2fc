/**********************************************************************
 Civ2Mp2Fc - Copyright (C) 2004-2006 - Egor Vyscrebentsov
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char int2terrain(int value)
{
  char terra = ' ';
  if (value > 10) {
    value = value % 16;
  }
  switch (value) {
    case 0:
      terra = 'd';
      break;
    case 1:
      terra = 'p';
      break;
    case 2:
      terra = 'g';
      break;
    case 3:
      terra = 'f';
      break;
    case 4:
      terra = 'h';
      break;
    case 5:
      terra = 'm';
      break;
    case 6:
      terra = 't';
      break;
    case 7:
      terra = 'a';
      break;
    case 8:
      terra = 's';
      break;
    case 9:
      terra = 'j';
      break;
    case 10:
      terra = ' ';
      break;
  }
  return terra;
}

void usage(void)
{
  printf("Usage:\n\tciv2mp2fc [options] file.mp\n");
  printf("Options:\n");
  printf("\t-o file    Specify output file\n");
/*  printf("\t-r file    Specify rules file\n");*/
}

int main(int argc, char *argv[])
{
  FILE *fp, *fp1;
  int bu1, bu2;
  char **rivermap;
  int width, height, tiles_count;
  unsigned char topology_id, is_earth;
  int header1, header2, header3;
  int startpos[42];
  unsigned char tile1, tile2, tile3, tile4, tile5, tile6;
  int i, j;
  char *filename = NULL;
  char *fout = NULL;
  /* FIXME: Since there is no Celtic nation in freeciv, we use Irish */
  char *civ2nations[] = {
    "Roman", "Babylonian", "German", "Egyptian", "American", "Greek",
    "Indian", "Russian", "Zulu", "French", "Aztec", "Chinese", "English",
    "Mongol", "Irish", "Japanese", "Viking", "Spanish", "Persian",
    "Carthaginian", "Sioux"
  };

  if (argc < 2) {
    usage();
    exit(1);
  }

  i = 1;
  while (i < argc) {
    if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
      usage();
      exit(0);
    }
    if (argv[i][0] != '-') {
      filename = strdup(argv[i]);
    } else {
      if (strcmp(argv[i], "-o") == 0) {
	i++;
	if (i < argc) {
	  fout = strdup(argv[i]);
	} else {
	  printf("Error: '-o' without output filename");
	  usage();
	  exit(1);
	}
      }
    }
    i++;
  }

  if (!filename) {
    printf("Error: Not enough parameters\n");
    usage();
    exit(1);
  }

  if ((fp = fopen(filename, "rb")) == NULL) {
    printf("Could not open file %s\n", filename);
    exit(1);
  }

  if (fout) {
    if ((fp1 = fopen(fout, "w")) == NULL) {
      printf("Could not open output file %s\n", fout);
      exit(1);
    }
  } else {
    fp1 = stdout;
  }

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  width = bu1 / 2 + bu2 * 128;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  height = bu1 + bu2 * 256;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  tiles_count = bu1 + bu2 * 256;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  topology_id = 5 - bu1;
  is_earth = 1 - bu1;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  header1 = bu1 + bu2 * 256;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  header2 = bu1 + bu2 * 256;

  bu1 = fgetc(fp);
  bu2 = fgetc(fp);
  header3 = bu1 + bu2 * 256;

  for (i = 0; i < 42; i++) {
    bu1 = fgetc(fp);
    bu2 = fgetc(fp);
    startpos[i] = bu1 + bu2 * 256;
  }

  fprintf(fp1, "#  Converted from Civ2 map file \"%s\"\n", filename);
  fprintf(fp1, "# Keep this section first\n");
  fprintf(fp1, "[scenario]\n");
  fprintf(fp1, "name=\"Civ2 map '%s'\"\n", filename);
  fprintf(fp1, "description=\"Map converted from civ2 map file '%s'\"\n\n", filename);

  fprintf(fp1, "[game]\n");
  fprintf(fp1, "version=20099\n");
  fprintf(fp1, "server_state=0\n");
  fprintf(fp1, "metastring=\"Civ2 map '%s'\"\n", filename);
  fprintf(fp1, "metaserver=\"http://meta.freeciv.org\"\n");
  fprintf(fp1, "gold=50\n");
  fprintf(fp1, "tech=0\n");
  fprintf(fp1, "researchcost=20\n");
  fprintf(fp1, "skill_level=5\n");
  fprintf(fp1, "timeout=0\n");
  fprintf(fp1, "end_year=2000\n");
  fprintf(fp1, "year=-4000\n");
  fprintf(fp1, "techlevel=10\n");
  fprintf(fp1, "min_players=1\n");
  fprintf(fp1, "max_players=8\n");
  fprintf(fp1, "nplayers=0\n");
  fprintf(fp1, "globalwarming=0\n");
  fprintf(fp1, "warminglevel=8\n");
  fprintf(fp1, "nuclearwinter=0\n");
  fprintf(fp1, "coolinglevel=8\n");
  fprintf(fp1, "unhappysize=5\n");
  fprintf(fp1, "cityfactor=15\n");
  fprintf(fp1, "civilwarsize=10\n");
  fprintf(fp1, "borders=0\n");
  fprintf(fp1, "happyborders=0\n");
  fprintf(fp1, "diplcost=0\n");
  fprintf(fp1, "freecost=0\n");
  fprintf(fp1, "conquercost=0\n");
  fprintf(fp1, "foodbox=10\n");
  fprintf(fp1, "techpenalty=100\n");
  fprintf(fp1, "razechance=20\n");
  fprintf(fp1, "civstile=2\n");
  fprintf(fp1, "save_nturns=5\n");
  fprintf(fp1, "save_name=\"civgame\"\n");

  j = 0;
  for (i = 0; i < 21; i++) {
    if (startpos[i] < width * 2 && startpos[i + 21] < height) {
      j++;
    }
  }
  fprintf(fp1, "aifill=%d\n", j < 8 ? j : 8);

  fprintf(fp1, "scorelog=0\n");
  fprintf(fp1, "fogofwar=1\n");
  fprintf(fp1, "spacerace=0\n");

  fprintf(fp1, "auto_ai_toggle=0\n");
  fprintf(fp1, "diplchance=80\n");
  fprintf(fp1, "aqueductloss=0\n");
  fprintf(fp1, "killcitizen=1\n");
  fprintf(fp1, "turnblock=0\n");
  fprintf(fp1, "fixedlength=0\n");
  fprintf(fp1, "barbarians=2\n");
  fprintf(fp1, "onsetbarb=-2000\n");
  fprintf(fp1, "occupychance=0\n");

  fprintf(fp1, "ruleset.techs=\"civ2\"\n");
  fprintf(fp1, "ruleset.units=\"civ2\"\n");
  fprintf(fp1, "ruleset.buildings=\"civ2\"\n");
  fprintf(fp1, "ruleset.terrain=\"civ2\"\n");
  fprintf(fp1, "ruleset.governments=\"civ2\"\n");
  fprintf(fp1, "ruleset.nations=\"civ2\"\n");
  fprintf(fp1, "ruleset.cities=\"civ2\"\n");
  fprintf(fp1, "ruleset.game=\"civ2\"\n");
  fprintf(fp1, "demography=\"NASRLPEMOqrb\"\n");
  fprintf(fp1, "settlers=2\n");
  fprintf(fp1, "explorer=1\n");
  fprintf(fp1, "dispersion=0\n"); /* Do we need this? */
  fprintf(fp1, "randseed=0\n"); /* Do we need this? */

  fprintf(fp1, "\n[savefile]\n");
  fprintf(fp1, "options=\"1.7 startoptions spacerace2 rulesets diplchance_percent startpos riversoverlay\"\n");

  fprintf(fp1, "\n[map]\n");
  fprintf(fp1, "width=%d\n", width);
  fprintf(fp1, "height=%d\n", height);
  fprintf(fp1, "topology_id=%d\n", topology_id);
  fprintf(fp1, "seed=0\n");
  fprintf(fp1, "landpercent=40\n"); /* FIXME!!! */
  fprintf(fp1, "riches=250\n"); /* FIXME!!! */
  fprintf(fp1, "swampsize=0\n"); /* Do we need this? */
  fprintf(fp1, "deserts=20\n"); /* Do we need this? */
  fprintf(fp1, "riverlength=100\n"); /* Do we need this? */
  fprintf(fp1, "mountains=100\n"); /* Do we need this? */
  fprintf(fp1, "forestsize=20\n"); /* Do we need this? */
  fprintf(fp1, "huts=50\n"); /* FIXME!!! */
  fprintf(fp1, "generator=0\n");
//  fprintf(fp1, "is_earth=%d\n", is_earth); /* Do we need this? */

  j = 0;
  for (i = 0; i < 21; i++) {
    if (startpos[i] < width * 2 && startpos[i + 21] < height) {
      fprintf(fp1, "r%dsx=%d\n", j, (startpos[i] / 2) + (startpos[i + 21] % 2));
      fprintf(fp1, "r%dsy=%d\n", j, startpos[i + 21]);
      fprintf(fp1, "r%dsnation=\"%s\"\n", j, civ2nations[i]);
      j++;
    }
  }

  rivermap = (char **)malloc(height * sizeof(*rivermap));

  for (i = 0; i < height; i++) {
    rivermap[i] = (char *)malloc((width + 1) * sizeof(**rivermap));
    fprintf(fp1, "t%03d=\"", i);
    for (j = 0; j < width; j++) {
      tile1 = fgetc(fp);
      tile2 = fgetc(fp);
      tile3 = fgetc(fp);
      tile4 = fgetc(fp);
      tile5 = fgetc(fp);
      tile6 = fgetc(fp);
      fprintf(fp1, "%c", int2terrain(tile1));
      if (tile1 & 0x80) {
	rivermap[i][j] = '2';
      } else {
	rivermap[i][j] = '0';
      }
    }
    fprintf(fp1, "\"\n");
    rivermap[i][width] = '\0';
  }

  for (i = 0; i < height; i++) {
    fprintf(fp1, "n%03d=\"%s\"\n", i, rivermap[i]);
    free(rivermap[i]);
  }

  free(rivermap);
  fclose(fp1);
  fclose(fp);
  return 0;
}

