/********************************************
* POKEMON BATTLE GAME
* ELIZABETH HOM
* 15 MAY 2020
* 
* battle.cpp
*
* implementation of a simplified pokemon battle
*********************************************/

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

    populateChart(typeChart);
    populateStats();

    battle(typeChart);

    return 0;
}

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

    if (effect == 2.0)
        cout << "\nIT'S SUPER EFFECTIVE!" << endl;
    else if (effect == 0.5)
        cout << "\nIT'S NOT VERY EFFECTIVE..." << endl;

    type.clear();

    return effect;
}

void calcDamage(bool first, double effect)
{
    if (calcMiss()) {
        return;
    }

    double damage = 0.0;

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

bool calcMiss()
{
    int missIndex = (rand() % 20) + 1;

    if (missIndex == 1) {
        cout << "THE ATTACK MISSED!" << endl;
        return true;
    }

    return false;
}

bool calcCrit()
{
    int critIndex = (rand() % 20) + 1;


    if (critIndex == 20) {
        cout << "A CRITICAL HIT!" << endl;
        return true;
    }

    return false;
}
