#include "level.h"
#include "pathfinder.h"
#include "gui.h"
#include "common.h"
#include "event.h"

#include "stdbool.h"

#define THEME "civ";

int main(int argc, char** argv);

/*zoek in level de oude headquarter (van zelfde speler) en verwijder die als die er is*/
/*return 0 als er een headquarter verwijderd is, return 1 anders*/
/*deze functie moet dus voor het plaatsen van de nieuwe headquarter opgeroepen worden*/
int remove_old_headquarter(Owner owner, Level* level);

/* Indien er 2 HeadQuarters (van de verschillende teams) geplaatst zijn en er dus
* een pad zou kunnen bestaan tussen de 2 hoofdkwartieren, wordt dit pad berekend.
* Indien een pad bestaat, wordt dit getekend op de editor. Zoniet wordt er het
* vorige pad weggehaald.
*
* Returns 0 : Everything is OK.
*		   1 : There aren't 2 headquarters.
*		   2 : There are 2 headquarters, but there is no possible path.
*
* @author : Domien
*/
int updatePath(Level* level);

/* Create a new empty level.
* If a command line argument is given of the form "width height", then
* a level of dimension width x height will be made when clear is pressed.
* If an invalid argument is given, or there is none, a level of the default dimensions
* is made.
*/
Level* make_new_level(bool dimarg_given, int arg_w, int arg_h);