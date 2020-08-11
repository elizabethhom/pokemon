/*
 * battle.cpp
 * 
 * Elizabeth Hom
 * Last Modified: May 15, 2020
 * 
 * Purpose: Simulation of a Pokémon battle. Modeled on official franchise's
 *          compuational methods, invokes automated turn-based combat and
 *          accounts for type-advantages. Reports winning Pokémon and leftover
 *          HP.
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

void populateChart(vector< vector<double> > &typeChart);
void populateStats();
double determineEffect(bool first, vector< vector<double> > &typeChart);
void calcDamage(bool first, double effect);
void battle(vector< vector<double> > &typeChart);
bool calcMiss();
bool calcCrit();

/*
 * Pokémon
 *
 * Describes a single Pokémon: its name, type, stats (speed, attack, defense,
 * HP). Invokes two instances for the battling Pokemon.
 */
struct pokemon {
    string name;
    int speed;
    int attack;
    int defense;
    string type;
    double HP;
} mon1, mon2;

int main()
{
    srand((unsigned)time(0));
    vector<vector<double> >typeChart;
    
    // Populates Pokémon type chart
    populateChart(typeChart);
    // Populates battling Pokémons' stats
    populateStats();

    // Drives battle
    battle(typeChart);

    return 0;
}

/*
 *  battle()
 *
 *  Parameters: typeChart vector
 *  Does:       Drives automated battle. Reports which Pokémon is attacking
 *              per turn and winner.
 *  Returns:    NA
 */
void battle(vector< vector<double> > &typeChart)
{
    int turns = 1;
    bool first = false;

    if (mon1.speed > mon2.speed) {
        first = true;
    }
    cout << endl;

    do {
        cout << "------------ TURN " << turns << " ------------" << endl;

        if (first == true) {
            cout << "*** " << mon1.name << " is attacking. ***" << endl;
        } else {
            cout << "*** " << mon2.name << " is attacking. ***" << endl;
        }

        double effect = determineEffect(first, typeChart);
        calcDamage(first, effect);
        turns++;

        if (first == true)
            first = false;
        else
            first = true;

        cout << endl;
    } while (mon1.HP > 0 and mon2.HP > 0);

    if (mon1.HP <= 0)
        cout << mon2.name << " won!" << endl;
    else
        cout << mon1.name << " won!" << endl;
}

/*
 *  populateChart()
 *
 *  Parameters: typeChart vector
 *  Does:       Populates typeChart vector according to official Pokémon
 *              type advantages (https://pokemondb.net/type).
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

/*
 *  populateStats()
 *
 *  Parameters: NA
 *  Does:       Prompts user for stats of battling Pokémon and populates
 *              mon1 and mon2 struct.
 *  Returns:    NA
 */
void populateStats()
{
    cout << "Enter 1st pokemon's name, HP, attack, defense, speed, & type."
         << endl;

    cin >> mon1.name >> mon1.HP >> mon1.attack >> mon1.defense >> mon1.speed
        >> mon1.type;

    cout << "Enter 2nd pokemon's name, HP, attack, defense, speed, & type."
         << endl;

    cin >> mon2.name >> mon2.HP >> mon2.attack >> mon2.defense >> mon2.speed
        >> mon2.type;
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
        type1 = mon1.type;
        type2 = mon2.type;  
    } else {
        type1 = mon2.type;
        type2 = mon1.type;
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

    // Notifies user of attack effect
    if (effect == 2.0)
        cout << "\nIT'S SUPER EFFECTIVE!" << endl;
    else if (effect == 0.5)
        cout << "\nIT'S NOT VERY EFFECTIVE..." << endl;

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
    if (calcMiss()) {
        return;
    }

    double damage = 0.0;

    // Multiples attack power by effect factor
    if (first == true) {
        if (mon1.attack <= mon2.defense)
            damage = (1) * effect;
        else
            damage = (mon1.attack - mon2.defense) * effect; 
    } else {
        if (mon2.attack <= mon1.defense)
            damage = (1) * effect;
        else
            damage = (mon2.attack - mon1.defense) * effect;
    }

    cout << "DAMAGE: " << damage << endl;

    // Determines if attack is critical hit. If so, attack power is doubled
    if (calcCrit()) {
        if (first == true)
            mon2.HP -= (damage * 2);
        else
            mon1.HP -= (damage * 2);
    } else {
        if (first == true)
            mon2.HP -= damage;
        else
            mon1.HP -= damage;
    }

    cout << mon1.name << " HP: " << mon1.HP << endl;
    cout << mon2.name << " HP: " << mon2.HP << endl;
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

    if (missIndex == 1) {
        cout << "THE ATTACK MISSED!" << endl;
        return true;
    }

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

    if (critIndex == 20) {
        cout << "A CRITICAL HIT!" << endl;
        return true;
    }

    return false;
}
