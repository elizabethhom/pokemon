/*
 *      catch.cpp
 *      by Elizabeth Hom
 *
 *      Purpose: Implementation of a simplified pokemon catching encounter.
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

/*  a Pokemon describes a single pokemon, containing information about its name
 *  and relevant stats (speed, attack, defense, type, and HP). trainer describes
 *  the pokemon being used to capture the encountered pokemon.
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

/*  Range describes the range (low - high) of pokemon levels that can be
 *  caught on the current route.
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
        cout << "Hi" << endl;
        string file = argv[1];
        cout << "hello" << endl;
        string dexFile = argv[2];

        
        populateRoute(file, route);
        populateChart(typeChart);
        populateDex(dexFile, pokedex);

        populateStats(pokedex);

        int level = spawn(route);
        cout << "\nA LV. " << level << " " << encounter.name << " appeared!" << endl;

        battle(typeChart);
    }
}

/*
 *  populateDex()
 *
 *  Parameters: name of pokedex file, empty vector of pokedex pokemon
 *  Does:       populates the pokedex vector with information from file
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

    while(getline(input, info)) {
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
 *  Parameters: pokemon's name, type maximum HP, attack, defense, special
 *              attack, special defense, and speed stats.
 *  Does:       divides the max stat values by 100 and populates entry struct.
 *  Returns:    a Pokemon struct with respective stats populated.
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
 *  Does:       Prompts the user to enter relevant information of the pokemon
 *              being used to catch and saves it in a Pokemon struct.
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
 *  Does:       searches through the pokedex for the specific pokemon. If
 *              found, returns index within vector the pokemon is found at,
 *              otherwise returns -1.
 *  Returns:    index with which specific pokemon is found at, -1 if not found
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
        cout << "Pokemon not found." << endl;

    return index;
}

/*  populateChart()
 *
 *  Parameters: Pokemon type chart represented as a 2D vector of doubles, where
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
 *  Parameters: Filename of route, vector of all the Pokemon that can be
 *              caught in the route
 *  Does:       Populates and returns a vector of Pokemon that can be caught
 *              on the route. Parses the file contents and creates Pokemon
 *              and the route's level range accordingly.
 *  Returns:    Populated vector of Pokemon that can be caught on the route
 *              from the passed in file.
 */
void populateRoute(string file, vector<Pokemon> &route)
{
    ifstream input;
    string info;

    input.open(file);

    getline(input, info);
    istringstream ss(info);
    ss >> range.low >> range.high;      // obtains route's range of levels

    while(getline(input, info)) {
        istringstream ss(info);

        // creates pokemon
        Pokemon mon;
        ss >> mon.name >> mon.HP >> mon.attack >> mon.defense >> mon.speed
           >> mon.type;

        route.push_back(mon);
    }
}

/*  spawn()
 *
 *  Parameters: Vector of Pokemon on route
 *  Does:       Randomly generates a pokemon from the route to spawn and its
 *              level based on the range provided in the file. Populates the
 *              encountered pokemon's stats accordingly.
 *  Returns:    The level of the spawned pokemon.
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
 *  Parameters: Pokemon type chart
 *  Does:       Simulates a battle between the user's pokemon against the
 *              encountered pokemon. The battle ends when one pokemon cannot
 *              battle anymore (HP <= 0). The encountered pokemon can be
 *              caught if the user's pokemon wins in battle.
 *  Returns:    NA
 */
void battle(vector< vector<double> > &typeChart)
{
    int turns = 1;
    bool first = false;

    if (trainer.speed > encounter.speed)
        first = true;

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

/*  determineEffect()
 *
 *  Parameters: bool first, which is true when the trainer's pokemon is
 *              attacking first and Pokemon type chart
 *  Does:       Determines the effect of the attack based on the types of the
 *              two pokemon in battle using the type chart.
 *  Returns:    The effect of the attack (0.5, 1.0, or 2.0)
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

/*  calcDamage()
 *
 *  Parameters: bool first, which is true when the trainer's pokemon is
 *              attacking first and effect of the current attack
 *  Does:       calculates the damage of the attack by subtracting the
 *              attacking pokemon's attack stat from the receiving
 *              pokemon's defense stat.
 *  Returns:    NA
 */
void calcDamage(bool first, double effect)
{
    if (calcMiss())
        return;

    double damage = 0.0;

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

/*  calcMiss()
 *
 *  Parameters: NA
 *  Does:       Determines whether the attack misses by randomly generating
 *              a number 1-20. Attack misses if number is 1.
 *  Returns:    True if attack misses.
 */
bool calcMiss()
{
    int missIndex = (rand() % 20) + 1;

    if (missIndex == 1)
        return true;

    return false;
}

/*  calcCrit()
 *
 *  Parameters: NA
 *  Does:       Determines whether the attack is a critical hit by randomly
 *              generating a number 1-20. Attack is a critical hit if number
 *              is 20.
 *  Returns:    True if attack is a critical hit.
 */
bool calcCrit()
{
    int critIndex = (rand() % 20) + 1;

    if (critIndex == 20)
        return true;

    return false;
}
