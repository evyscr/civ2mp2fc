/**********************************************************************
 civ2mp2fc - Copyright (C) 2004-2018 - Egor Vyscrebentsov
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
  char **basemap;
  char **roadmap;
  int width, height, tiles_count, maxplayers, aifill;
  unsigned char is_earth;
  int header1, header2, header3;
  int startpos[42];
  unsigned char tile1, tile2, tile3, tile4, tile5, tile6;
  int i, j;
  char *filename = NULL;
  char *fout = NULL;
  char *civ2nations[] = {
    "Roman", "Babylonian", "German", "Egyptian", "American", "Greek",
    "Indian", "Russian", "Zulu", "French", "Aztec", "Chinese", "English",
    "Mongol", "Celtic", "Japanese", "Viking", "Spanish", "Persian",
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
  is_earth = bu1;

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
  fprintf(fp1, "[scenario]\n");
  fprintf(fp1, "is_scenario=TRUE\n");
  fprintf(fp1, "name=\"Civ2 map '%s'\"\n", filename);
  fprintf(fp1, "description=\"Map converted from civ2 map file '%s'\"\n", filename);
  fprintf(fp1, "players=FALSE\n");

  /* [savefile] section */
  fprintf(fp1, "\n[savefile]\n");
  fprintf(fp1, "options=\" +version2\"\n");
  fprintf(fp1, "version=20\n");
  fprintf(fp1, "reason=\"Scenario\"\n");
  fprintf(fp1, "rulesetdir=\"civ2\"\n");
  /* TODO: classic ruleset */
  /*fprintf(fp1, "rulesetdir=\"classic\"\n");*/

  fprintf(fp1, "improvement_size=68\n");
  fprintf(fp1, "improvement_vector=\"Airport\",\"Aqueduct\",\"Bank\",");
  fprintf(fp1, "\"Barracks\",\"Barracks II\",\"Barracks III\",");
  fprintf(fp1, "\"Cathedral\",\"City Walls\",\"Coastal Defense\",");
  fprintf(fp1, "\"Colosseum\",\"Courthouse\",\"Factory\",\"Granary\",");
  fprintf(fp1, "\"Harbour\",\"Hydro Plant\",\"Library\",\"Marketplace\",");
  fprintf(fp1, "\"Mass Transit\",\"Mfg. Plant\",\"Nuclear Plant\",");
  fprintf(fp1, "\"Offshore Platform\",\"Palace\",\"Police Station\",");
  fprintf(fp1, "\"Port Facility\",\"Power Plant\",\"Recycling Center\",");
  fprintf(fp1, "\"Research Lab\",\"SAM Battery\",\"SDI Defense\",");
  fprintf(fp1, "\"Sewer System\",\"Solar Plant\",\"Space Component\",");
  fprintf(fp1, "\"Space Module\",\"Space Structural\",\"Stock Exchange\",");
  fprintf(fp1, "\"Super Highways\",\"Supermarket\",\"Temple\",");
  fprintf(fp1, "\"University\",\"Apollo Program\",");
  fprintf(fp1, "\"A.Smith's Trading Co.\",\"Colossus\",");
  fprintf(fp1, "\"Copernicus' Observatory\",\"Cure For Cancer\",");
  fprintf(fp1, "\"Darwin's Voyage\",\"Eiffel Tower\",\"Great Library\",");
  fprintf(fp1, "\"Great Wall\",\"Hanging Gardens\",\"Hoover Dam\",");
  fprintf(fp1, "\"Isaac Newton's College\",\"J.S. Bach's Cathedral\",");
  fprintf(fp1, "\"King Richard's Crusade\",\"Leonardo's Workshop\",");
  fprintf(fp1, "\"Lighthouse\",\"Magellan's Expedition\",");
  fprintf(fp1, "\"Manhattan Project\",\"Marco Polo's Embassy\",");
  fprintf(fp1, "\"Michelangelo's Chapel\",\"Oracle\",\"Pyramids\",");
  fprintf(fp1, "\"SETI Program\",\"Shakespeare's Theatre\",");
  fprintf(fp1, "\"Statue of Liberty\",\"Sun Tzu's War Academy\",");
  fprintf(fp1, "\"United Nations\",\"Women's Suffrage\",");
  fprintf(fp1, "\"Capitalization\"\n");

  fprintf(fp1, "technology_size=89\n");
  fprintf(fp1, "technology_vector=\"A_NONE\",\"Advanced Flight\",");
  fprintf(fp1, "\"Alphabet\",\"Amphibious Warfare\",\"Astronomy\",");
  fprintf(fp1, "\"Atomic Theory\",\"Automobile\",\"Banking\",");
  fprintf(fp1, "\"Bridge Building\",\"Bronze Working\",");
  fprintf(fp1, "\"Ceremonial Burial\",\"Chemistry\",\"Chivalry\",");
  fprintf(fp1, "\"Code of Laws\",\"Combined Arms\",\"Combustion\",");
  fprintf(fp1, "\"Communism\",\"Computers\",\"Conscription\",");
  fprintf(fp1, "\"Construction\",\"Currency\",\"Democracy\",");
  fprintf(fp1, "\"Economics\",\"Electricity\",\"Electronics\",");
  fprintf(fp1, "\"Engineering\",\"Environmentalism\",\"Espionage\",");
  fprintf(fp1, "\"Explosives\",\"Feudalism\",\"Flight\",");
  fprintf(fp1, "\"Fundamentalism\",\"Fusion Power\",");
  fprintf(fp1, "\"Genetic Engineering\",\"Guerilla Warfare\",");
  fprintf(fp1, "\"Gunpowder\",\"Horseback Riding\",\"Industrialization\",");
  fprintf(fp1, "\"Invention\",\"Iron Working\",\"Labor Union\",\"Laser\",");
  fprintf(fp1, "\"Leadership\",\"Literacy\",\"Machine Tools\",");
  fprintf(fp1, "\"Magnetism\",\"Map Making\",\"Masonry\",");
  fprintf(fp1, "\"Mass Production\",\"Mathematics\",\"Medicine\",");
  fprintf(fp1, "\"Metallurgy\",\"Miniaturization\",\"Mobile Warfare\",");
  fprintf(fp1, "\"Monarchy\",\"Monotheism\",\"Mysticism\",\"Navigation\",");
  fprintf(fp1, "\"Nuclear Fission\",\"Nuclear Power\",\"Philosophy\",");
  fprintf(fp1, "\"Physics\",\"Plastics\",\"Polytheism\",\"Pottery\",");
  fprintf(fp1, "\"Radio\",\"Railroad\",\"Recycling\",\"Refining\",");
  fprintf(fp1, "\"Refrigeration\",\"Robotics\",\"Rocketry\",");
  fprintf(fp1, "\"Sanitation\",\"Seafaring\",\"Space Flight\",");
  fprintf(fp1, "\"Stealth\",\"Steam Engine\",\"Steel\",");
  fprintf(fp1, "\"Superconductors\",\"Tactics\",\"The Corporation\",");
  fprintf(fp1, "\"The Republic\",\"The Wheel\",\"Theology\",");
  fprintf(fp1, "\"Theory of Gravity\",\"Trade\",\"University\",");
  fprintf(fp1, "\"Warrior Code\",\"Writing\"\n");

  fprintf(fp1, "activities_size=21\n");
  fprintf(fp1, "activities_vector=\"Idle\",\"Pollution\",\"Unused Road\",");
  fprintf(fp1, "\"Mine\",\"Irrigate\",\"Fortified\",\"Fortress\",");
  fprintf(fp1, "\"Sentry\",\"Unused Railroad\",\"Pillage\",\"Goto\",");
  fprintf(fp1, "\"Explore\",\"Transform\",\"Unused\",\"Unused Airbase\",");
  fprintf(fp1, "\"Fortifying\",\"Fallout\",\"Unused Patrol\",\"Base\",");
  fprintf(fp1, "\"Road\",\"Convert\"\n");

  fprintf(fp1, "trait_size=3\n");
  fprintf(fp1, "trait_vector=\"Expansionist\",\"Trader\",\"Aggressive\"\n");
  fprintf(fp1, "specials_size=6\n");
  fprintf(fp1, "specials_vector=\"Irrigation\",\"Mine\",\"Pollution\",");
  fprintf(fp1, "\"Hut\",\"Farmland\",\"Fallout\"\n");
  fprintf(fp1, "bases_size=2\n");
  fprintf(fp1, "bases_vector=\"Fortress\",\"Airbase\"\n");
  fprintf(fp1, "roads_size=3\n");
  fprintf(fp1, "roads_vector=\"Road\",\"Railroad\",\"River\"\n");

  /* [game] section */
  fprintf(fp1, "\n[game]\n");
  fprintf(fp1, "version=20500\n");
  fprintf(fp1, "server_state=\"S_S_INITIAL\"\n");
  fprintf(fp1, "meta_patches=\"none\"\n");
  fprintf(fp1, "meta_usermessage=TRUE\n");
  fprintf(fp1, "meta_message=\"Civ2 map '%s'\"\n", filename);
  fprintf(fp1, "meta_server=\"http://meta.freeciv.org/metaserver.php\"\n");
  fprintf(fp1, "id=\"\"\n");
  fprintf(fp1, "serverid=\"\"\n");
  fprintf(fp1, "skill_level=5\n");
  fprintf(fp1, "phase_mode=0\n");
  fprintf(fp1, "phase_mode_stored=0\n");
  fprintf(fp1, "phase=0\n");
  fprintf(fp1, "scoreturn=20\n");
  fprintf(fp1, "timeoutint=0\n");
  fprintf(fp1, "timeoutintinc=0\n");
  fprintf(fp1, "timeoutinc=0\n");
  fprintf(fp1, "timeoutincmult=1\n");
  fprintf(fp1, "timeoutcounter=1\n");
  fprintf(fp1, "turn=0\n");
  fprintf(fp1, "year=-4000\n");
  fprintf(fp1, "year_0_hack=FALSE\n");

  fprintf(fp1, "globalwarming=0\n");
  fprintf(fp1, "heating=0\n");
  fprintf(fp1, "warminglevel=8\n");
  fprintf(fp1, "nuclearwinter=0\n");
  fprintf(fp1, "cooling=0\n");
  fprintf(fp1, "coolinglevel=8\n");
  fprintf(fp1, "save_players=FALSE\n");
  fprintf(fp1, "save_known=FALSE\n");


  maxplayers = 0;
  for (i = 0; i < 21; i++) {
    if (startpos[i] < width * 2 && startpos[i + 21] < height) {
      maxplayers++;
    }
  }

  aifill = maxplayers < 8 ? maxplayers : 8;

  fprintf(fp1, "\n[random]\n");
  fprintf(fp1, "save=FALSE\n");

  fprintf(fp1, "\n[script]\n");
  fprintf(fp1, "code=$$\n");
  fprintf(fp1, "vars=$$\n");

  /* [settings] section */
  fprintf(fp1, "\n[settings]\n");
  fprintf(fp1, "set={\"name\",\"value\",\"gamestart\"\n");
  fprintf(fp1, "\"aifill\",%d,%d\n", aifill, aifill);
  fprintf(fp1, "\"generator\",\"SCENARIO\",\"SCENARIO\"\n");
  fprintf(fp1, "\"maxplayers\",%d,%d\n", maxplayers, maxplayers);
  if (is_earth == 0) {
    fprintf(fp1, "\"topology\",\"WRAPX|ISO\",\"WRAPX|ISO\"\n");
  } else {
    fprintf(fp1, "\"topology\",\"ISO\",\"ISO\"\n");
  }
  fprintf(fp1, "\"xsize\",%d,%d\n", width, width);
  fprintf(fp1, "\"ysize\",%d,%d\n", height, height);
  fprintf(fp1, "}\n");
  fprintf(fp1, "set_count=6\n");
  fprintf(fp1, "gamestart_valid=FALSE\n");

  /* [map] section */
  fprintf(fp1, "\n[map]\n");
  fprintf(fp1, "have_huts=TRUE\n");

  basemap = (char **)malloc(height * sizeof(*basemap));
  roadmap = (char **)malloc(height * sizeof(*roadmap));

  for (i = 0; i < height; i++) {
    basemap[i] = (char *)malloc((width + 1) * sizeof(**basemap));
    roadmap[i] = (char *)malloc((width + 1) * sizeof(**roadmap));
    fprintf(fp1, "t%04d=\"", i);
    for (j = 0; j < width; j++) {
      tile1 = fgetc(fp);
      tile2 = fgetc(fp);
      tile3 = fgetc(fp);
      tile4 = fgetc(fp);
      tile5 = fgetc(fp);
      tile6 = fgetc(fp);
      fprintf(fp1, "%c", int2terrain(tile1));
      basemap[i][j] = '0';
      if (tile1 & 0x80) {
	roadmap[i][j] = '4';
      } else {
	roadmap[i][j] = '0';
      }
    }
    fprintf(fp1, "\"\n");
    basemap[i][width] = '\0';
    roadmap[i][width] = '\0';
  }

  fprintf(fp1, "startpos_count=%d\n", maxplayers);
  fprintf(fp1, "startpos={\"x\",\"y\",\"exclude\",\"nations\"\n");

  for (i = 0; i < 21; i++) {
    if (startpos[i] < width * 2 && startpos[i + 21] < height) {
      /*fprintf(fp1, "%d,", (startpos[i] / 2) + (startpos[i + 21] % 2));*/
      fprintf(fp1, "%d,", startpos[i] / 2);
      fprintf(fp1, "%d,", startpos[i + 21]);
      fprintf(fp1, "FALSE,\"%s\"\n", civ2nations[i]);
    }
  }

  fprintf(fp1, "}\n");

  for (i = 0; i < height; i++) {
    fprintf(fp1, "b00_%04d=\"%s\"\n", i, basemap[i]);
    free(basemap[i]);
  }

  for (i = 0; i < height; i++) {
    fprintf(fp1, "r00_%04d=\"%s\"\n", i, roadmap[i]);
    free(roadmap[i]);
  }

  free(basemap);
  free(roadmap);

  fprintf(fp1, "\n[mapimg]\ncount=0\n");

  fclose(fp1);
  fclose(fp);
  return 0;
}

