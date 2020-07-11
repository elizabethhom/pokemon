/*
 *      stats.cpp
 *      by Elizabeth Hom
 *
 *      Purpose: Calculates a pokemon's stats (HP, attack, defense, speed) at
 *               a given level.
 *
 *      Last modified: May 30, 2020
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

/*  a Pokemon describes a single pokemon, containing information about its name
 *  and relevant stats (speed, attack, defense, type, and HP).
 */
struct Pokemon {
    string name;
    double HP;
    double attack;
    double defense;
    double speed;
    string type;
    int nextEvol;
};

vector<Pokemon> populateDex(string file, vector<Pokemon> pokedex);
Pokemon calculateStats(string name, double maxHP, double maxAtk, double maxDef,
                       double spAtk, double spDef, double maxSpd, string type,
                       int nextEvol);
int searchDex(string pokemon, vector<Pokemon> pokedex);
void generateStats(int level, int index, vector<Pokemon> pokedex, bool first);
void baseStats(int index, vector<Pokemon> pokedex);

int main(int argc, char* argv[])
{
    if (argc != 2)
        cout << "Usage: ./stats [pokedex]" << endl;
    else {
        vector<Pokemon> pokedex;
        string file = argv[1];
        string pokemon;
        int level;

        pokedex = populateDex(file, pokedex);          // populates pokedex
        
        cout << "Enter pokemon's name and level: ";
        cin >> pokemon >> level;

        int index = searchDex(pokemon, pokedex);

        if (index == -1)
            exit(1);
        else if (level == 1)
            baseStats(index, pokedex);
        else
            generateStats(level, index, pokedex, true);      // generates & prints
    }                                                  // stats
}

/*
 *  populateDex()
 *
 *  Parameters: name of pokedex file, empty vector of pokedex pokemon
 *  Does:       populates the pokedex vector with information from file
 *              regarding each pokemon's name, HP, attack, defense, and speed.
 *  Returns:    filled pokedex
 */
vector<Pokemon> populateDex(string file, vector<Pokemon> pokedex)
{
    ifstream input;
    string info;

    input.open(file);

    string name, type;
    int nextEvol;
    double maxHP, maxAtk, maxDef, spAtk, spDef, maxSpd;

    while(getline(input, info)) {
        istringstream ss(info);

        ss >> name >> maxHP >> maxAtk >> maxDef >> spAtk >> spDef >> maxSpd
           >> type >> nextEvol;

        Pokemon entry;
        entry = calculateStats(name, maxHP, maxAtk, maxDef, spAtk, spDef, maxSpd, type, nextEvol);

        pokedex.push_back(entry);
    }

    return pokedex;
}

/*
 *  calculateStats()
 *
 *  Parameters: pokemon's name, type maximum HP, attack, defense, special
 *              attack, special defense, and speed stats.
 *  Does:       divides the max stat values by 100 and populates entry struct.
 *  Returns:    a Pokemon struct with respective stats populated.
 */
Pokemon calculateStats(string name, double maxHP, double maxAtk, double maxDef, double spAtk,
                       double spDef, double maxSpd, string type, int nextEvol)
{
    Pokemon entry;

    entry.name = name;
    entry.HP = maxHP / 100;
    entry.attack = ((maxAtk + spAtk) / 2) / 100;
    entry.defense = ((maxDef + spDef) / 2) / 100;
    entry.speed = maxSpd / 100;
    entry.type = type;
    entry.nextEvol = nextEvol;

    return entry;
}

/*
 *  searchDex()
 *
 *  Parameters: name of pokemon, pokedex vector
 *  Does:       searches through the pokedex for the specific pokemon. If
 *              found, returns index within vector the pokemon is found at,
 *              otherwise returns -1.
 *  Returns:    index with which specific pokemon is found at, -1 if not found
 */
int searchDex(string pokemon, vector<Pokemon> pokedex)
{
    int index = -1;
    bool found = false;

    for (unsigned long i = 0; i < pokedex.size(); i++) {
        if (pokedex[i].name == pokemon) {
            index = i;
            found = true;
        }
    }

    if (not found)
        cout << "Pokemon not found." << endl;

    return index;
}

/*
 *  generateStats()
 *
 *  Parameters: user-specified level of pokemon, index of pokedex pokemon is
 *              found at, pokedex vector
 *  Does:       calculates stats based on user's specified level and rounds.
 *              Prints HP, attack, defense, and speed stats.
 *  Returns:    NA
 */
void generateStats(int level, int index, vector<Pokemon> pokedex, bool first)
{
    int HP, attack, defense, speed;

    HP = round(pokedex[index].HP * level);
    attack = round(pokedex[index].attack * level);
    defense = round(pokedex[index].defense * level);
    speed = round(pokedex[index].speed * level);

    if (level >= pokedex[index].nextEvol and pokedex[index].nextEvol != 0
        and first == true) {
        
        if (pokedex[index].name == "eevee") {
            cout << "*** EEVEE IS EVOLVING. PICK EVOLUTION. ***" << endl;
        } else {
            string evolution = pokedex[index + 1].name;
            cout << "\n*** " << pokedex[index].name << " IS EVOLVING INTO "
                 << evolution << "! ***\n";

            generateStats(level, index + 1, pokedex, false);
        }
    } else {
        cout << "\n------ " << pokedex[index].name << "'s STATS ------\n" << endl;

        cout << "HP: " << HP << endl;
        cout << "Attack: " << attack << endl;
        cout << "Defense: " << defense << endl;
        cout << "Speed: " << speed << endl;
        cout << "Type: " << pokedex[index].type << endl;

        if (pokedex[index].nextEvol == 0)
            cout << "CANNOT EVOLVE." << endl;
        else
            cout << "EVOLVES AT: " << pokedex[index].nextEvol << endl; 
    }
}

/*
 *  baseStats()
 *
 *  Parameters: index of pokedex pokemon is found at, pokedex vector
 *  Does:       prints the pokemon's base stats (at level 1)
 *  Returns:    NA
 */
void baseStats(int index, vector<Pokemon> pokedex)
{
    cout << "HP: " << pokedex[index].HP << endl;
    cout << "Attack: " << pokedex[index].attack << endl;
    cout << "Defense: " << pokedex[index].defense << endl;
    cout << "Speed: " << pokedex[index].speed << endl;
    cout << "Type: " << pokedex[index].type << endl;
}
