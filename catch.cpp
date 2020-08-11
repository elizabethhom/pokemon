/*
 *      catch.cpp
 *      Elizabeth Hom
 *
 *      Purpose: Simulation of Pokémon catching encounter. Reads and populates
 *      route with Pokémon from route file, randomly spawns a Pokémon to be
 *      encountered. Automates battle between trainer's Pokémon and encounter.
 *      If trainer's Pokémon wins, encountered Pokémon is able to be captured.
 *
 *      Last modified: June 7, 2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <time.h>

using namespace std;

/*
 * Pokémon
 *
 * Describes a single Pokémon: its name, type, stats (speed, attack, defense,
 * HP), and level of next evolution. Creates instances for encountered
 * Pokémon and Trainer's offensive Pokémon.
 */
struct Pokemon {
    string name;
    double HP;
    double attack;
    double defense;
    double speed;
    string type;
    int nextEvol;
} encounter, trainer;

/*
 * Range
 *
 * Describes the range (low - high) of pokemon levels that can be
 * caught on the current route.
 */
struct Range {
    int low;
    int high;
} range;

void populateRoute(string file, vector<Pokemon> &route);
int spawn(vector<Pokemon> &route);
void populateChart(vector< vector<double> > &typeChart);
void populateDex(string dexFile, vector<Pokemon> &pokedex);
int searchDex(string pokemon, vector<Pokemon> &pokedex);
Pokemon calculateStats(string name, double maxHP, double maxAtk, double maxDef,
                       double spAtk, double spDef, double maxSpd, string type,
                       int nextEvol);
void populateStats(vector<Pokemon> &pokedex);
double determineEffect(bool first, vector< vector<double> > &typeChart);
void calcDamage(bool first, double effect);
void battle(vector< vector<double> > &typeChart);
bool calcMiss();
bool calcCrit();

int main(int argc, char* argv[])
{
    srand((unsigned)time(0));
    vector<Pokemon> route;
    vector<Pokemon> pokedex;
    vector<vector<double> >typeChart;

    if (argc != 3)
        cout << "Usage: ./catch [route] [pokedex.txt]" << endl;
    else {
        string file = argv[1];
        string dexFile = argv[2];

        // Populates route Pokémon, type chart, and Pokédex
        populateRoute(file, route);
        populateChart(typeChart);
        populateDex(dexFile, pokedex);

        populateStats(pokedex);

        int level = spawn(route);
        cout << "\nA LV. " << level << " " << encounter.name << " appeared!" << endl;

        // Drives battle to determine if Pokémon is catchable
        battle(typeChart);
    }
}

/*
 *  populateDex()
 *
 *  Parameters: name of pokedex file, empty vector of pokedex pokemon
 *  Does:       Populates the Pokédex vector with information from file
 *              regarding each pokemon's name, HP, attack, defense, and speed.
 *  Returns:    NA
 */
void populateDex(string dexFile, vector<Pokemon> &pokedex)
{
    ifstream input;
    string info;

    input.open(dexFile);

    string name, type;
    int nextEvol;
    double maxHP, maxAtk, maxDef, spAtk, spDef, maxSpd;

    while (getline(input, info)) {
        istringstream ss(info);

        ss >> name >> maxHP >> maxAtk >> maxDef >> spAtk >> spDef >> maxSpd
           >> type >> nextEvol;

        Pokemon entry;
        entry = calculateStats(name, maxHP, maxAtk, maxDef, spAtk, spDef,
                               maxSpd, type, nextEvol);

        pokedex.push_back(entry);
    }
}

/*
 *  calculateStats()
 *
 *  Parameters: Pokémon's name, type, maximum HP, attack, defense, special
 *              attack, special defense, and speed stats.
 *  Does:       Divides the max stat values by 100 and populates entry struct.
 *  Returns:    A Pokémon struct with respective stats populated.
 */
Pokemon calculateStats(string name, double maxHP, double maxAtk, double maxDef,
                       double spAtk, double spDef, double maxSpd, string type,
                       int nextEvol)
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

/*  populateStats()
 *
 *  Parameters: NA
 *  Does:       Prompts user to enter name and level of offensive Pokémon
 *              and populates trainer Pokémon struct
 *  Returns:    NA
 */
void populateStats(vector<Pokemon> &pokedex)
{
    string name;
    int level = 0;
    int index;

    cout << "Enter trainer Pokemon's name and level: ";
    cin >> name >> level;

    index = searchDex(name, pokedex);

    trainer.name = pokedex[index].name;
    trainer.HP = pokedex[index].HP * level;
    trainer.attack = pokedex[index].attack * level;
    trainer.defense = pokedex[index].defense * level;
    trainer.speed = pokedex[index].speed * level;
    trainer.type = pokedex[index].type;
    trainer.nextEvol = pokedex[index].nextEvol;
}

/*
 *  searchDex()
 *
 *  Parameters: name of pokemon, pokedex vector
 *  Does:       Searches through Pokédex for the specific Pokémon. If
 *              found, returns index within vector Pokémon is found at,
 *              otherwise returns -1.
 *  Returns:    Index with which specific Pokémon is found at, -1 if not found
 */
int searchDex(string pokemon, vector<Pokemon> &pokedex)
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
        cout << "Pokémon not found." << endl;

    return index;
}

/*  populateChart()
 *
 *  Parameters: Pokémon type chart represented as a 2D vector of doubles, where
 *              super effective, normal, not very effective, and no effect type
 *              matchups are represented as 2.0, 1.0, 0.5, and 0.
 *              Chart: https://rankedboost.com/pokemon-sun-moon/type-chart/
 *  Does:       Fills the typeChart vector that is passed in.
 *  Returns:    NA
 */
void populateChart(vector< vector<double> > &typeChart)
{
    typeChart = { {1, 1, 1, 1, 1,.5, 1, 0,.5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {2, 1, .5,.5,1, 2,.5, 0, 2, 1, 1, 1, 1,.5, 2, 1, 2,.5},
                  {1, 2, 1, 1, 1,.5, 2, 1,.5, 1, 1, 2,.5, 1, 1, 1, 1, 1},
                  {1, 1, 1,.5,.5,.5, 1,.5, 0, 1, 1, 2, 1, 1, 1, 1, 1, 2},
                  {1, 1, 0, 2, 1, 2,.5, 1, 2, 2, 1,.5, 2, 1, 1, 1, 1, 1},
                  {1,.5, 2, 1,.5, 1, 2, 1,.5, 2, 1, 1, 1, 1, 2, 1, 1, 1},
                  {1,.5,.5,.5, 1, 1, 1,.5,.5,.5, 1, 2, 1, 2, 1, 1, 2,.5},
                  {0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1,.5, 1},
                  {1, 1, 1,1 ,1 , 2, 1, 1,.5,.5,.5, 1,.5, 1, 2, 1, 1, 2},
                  {1, 1, 1, 1, 1,.5, 2, 1, 2,.5,.5, 2, 1, 1, 2,.5, 1, 1},
                  {1, 1, 1, 1, 2, 2, 1, 1, 1, 2,.5,.5, 1, 1, 1,.5, 1, 1},
                  {1, 1,.5,.5, 2, 2,.5, 1,.5,.5, 2,.5, 1, 1, 1,.5, 1, 1},
                  {1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2,.5,.5, 1, 1,.5, 1, 1},
                  {1, 2, 1, 2, 1, 1, 1, 1,.5, 1, 1, 1, 1,.5, 1, 1, 0, 1},
                  {1, 1, 2, 1, 2, 1, 1, 1,.5,.5,.5, 2, 1, 1,.5, 2, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1,.5, 1, 1, 1, 1, 1, 1, 2, 1, 0},
                  {1,.5, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1,.5,.5},
                  {1, 2, 1,.5 ,1, 1, 1, 1,.5,.5, 1, 1, 1, 1, 1, 2, 2, 1} };
}

/*  populateRoute()
 *
 *  Parameters: file name of route, vector of all Pokémon that can be
 *              caught on the route
 *  Does:       Populates and returns a vector of Pokémon that can be caught
 *              on the route. Parses file contents and populates Pokémon 
 *              and route's level range.
 *  Returns:    Populated vector of Pokémon that can be caught on the route
 *              from the passed in file.
 */
void populateRoute(string file, vector<Pokemon> &route)
{
    ifstream input;
    string info;

    input.open(file);

    getline(input, info);
    istringstream ss(info);
    // Obtain's route's range of levels
    ss >> range.low >> range.high;

    while(getline(input, info)) {
        istringstream ss(info);

        // Creates Pokémon
        Pokemon mon;
        ss >> mon.name >> mon.HP >> mon.attack >> mon.defense >> mon.speed
           >> mon.type;

        route.push_back(mon);
    }
}

/*  spawn()
 *
 *  Parameters: Vector of Pokémon on route
 *  Does:       Randomly generates a Pokémon from the route to spawn and its
 *              level based on the range provided in the route file. Populates
 *              encountered Pokémon's stats.
 *  Returns:    The level of the spawned Pokémon.
 */
int spawn(vector<Pokemon> &route)
{
    int index = (rand() % 20);
    int level = (rand() % ((range.high - range.low) + 1)) + range.low;

    encounter.name    = route[index].name;
    encounter.HP      = route[index].HP      * level;
    encounter.attack  = route[index].attack  * level;
    encounter.defense = route[index].defense * level;
    encounter.speed   = route[index].speed   * level;
    encounter.type    = route[index].type;

    return level;
}

/*  battle()
 *
 *  Parameters: Pokémon type chart
 *  Does:       Simulates a battle between the user's Pokémon against the
 *              encountered Pokémon. The battle ends when either Pokémon cannot
 *              battle anymore (HP <= 0). The encountered Pokémon is able to be
 *              caught if the user's Pokémon wins in battle.
 *  Returns:    NA
 */
void battle(vector< vector<double> > &typeChart)
{
    int turns = 1;
    bool first = false;

    if (trainer.speed > encounter.speed)
        first = true;

    // Loop runs while both Pokémon's HP still > 0
    do {
        double effect = determineEffect(first, typeChart);
        calcDamage(first, effect);
        turns++;

        if (first == true)
            first = false;
        else
            first = true;
        turns++;
    } while (trainer.HP > 0 and encounter.HP > 0);

    if (trainer.HP <= 0)
        cout << encounter.name << " won! Cannot catch." << endl;
    else
        cout << trainer.name << " won! Can catch." << endl;
}

/*
 *  determineEffect()
 *
 *  Parameters: true if Pokémon was first to attack, typeChart vector
 *  Does:       Determines the numerical "effect" of the attacking Pokémon's
 *              attack based on the type advantage, using the typeChart. If
 *              attack is SUPER EFFECTIVE, NEUTRAL, NOT VERY EFFECTIVE, or
 *              NO EFFECT, attack power is multiplied by 2.0, 1.0, 0.5, and 0,
 *              respectively.
 *  Returns:    Attack effect (2.0, 1.0, 0.5, or 0)
 */
double determineEffect(bool first, vector< vector<double> > &typeChart)
{
    string type1, type2;
    if (first == true) {
        type1 = trainer.type;
        type2 = encounter.type;  
    } else {
        type1 = encounter.type;
        type2 = trainer.type;
    }

    vector<string> type = {"normal", "fighting", "flying", "poison", "ground",
                           "rock", "bug", "ghost", "steel", "fire", "water",
                           "grass", "electric", "psychic", "ice", "dragon",
                           "dark", "fairy"};

    int index1 = 0;
    int index2 = 0;

    for (int i = 0; i < 18; i++) {
        if (type1 == type[i])
            index1 = i;
        if (type2 == type[i])
            index2 = i;
    }

    double effect = typeChart[index1][index2];

    type.clear();

    return effect;
}

/*
 *  calcDamage()
 *
 *  Parameters: true if Pokémon was first to attack, attack effect (double)
 *  Does:       Calculates and deducts damage from defensive Pokémon's HP
 *              using mathematical methods of original Pokémon franchise.
 *  Returns:    NA
 */
void calcDamage(bool first, double effect)
{
    if (calcMiss())
        return;

    double damage = 0.0;

    // Multiples attack power by effect factor
    if (first == true) {
        if (trainer.attack <= encounter.defense)
            damage = (1) * effect;
        else
            damage = (trainer.attack - encounter.defense) * effect;
    } else {
        if (encounter.attack <= trainer.defense)
            damage = (1) * effect;
        else
            damage = (encounter.attack - trainer.defense) * effect;
    }

    // Determines if attack is critical hit. If so, attack power is doubled
    if (calcCrit()) {
        if (first == true)
            encounter.HP -= (damage * 2);
        else
            trainer.HP -= (damage * 2);
    } else {
        if (first == true)
            encounter.HP -= damage;
        else
            trainer.HP -= damage;
    }
}

/*
 *  calcMiss()
 *
 *  Parameters: NA
 *  Does:       Determines whether attack missed using random number
 *              generator.
 *  Returns:    True if attack missed, otherwise false
 */
bool calcMiss()
{
    int missIndex = (rand() % 20) + 1;

    if (missIndex == 1)
        return true;

    return false;
}

/*
 *  calcCrit()
 *
 *  Parameters: NA
 *  Does:       Determines whether attack is critical hit using random number
 *              generator.
 *  Returns:    True if attack is critical, otherwise false
 */
bool calcCrit()
{
    int critIndex = (rand() % 20) + 1;

    if (critIndex == 20)
        return true;

    return false;
}
