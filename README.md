# Pokémon Simulations

Modeled after the Pokémon franchise's computational methods, these programs simulate three key features of the original games: battling, catching Pokémon, and calculating stats. Written as a personal project in Summer 2020 for a simulation game of ~30 people.

### Background
Pokémon is a series of video games developed by Game Freak, published by Nintendo and The Pokémon Company International. It features monster-like creatures with experience levels (1-100) and designated elemental "types" (i.e. fire, water, grass, electric, ice), where certain types offer [advantages](https://pokemondb.net/type) over another (for example, water >> fire). A given Pokémon can have up to 3 evolutions, with each evolved form being more powerful than its predecessor, and are achieved through leveling up. Pokémon can be caught in the wild, and once caught, be used to battle against other Pokémon. The outcomes of battles depend on a variety of factors, including type advantages, the Pokémon's [stats](https://pokemondb.net/pokedex/all), and luck. A Pokédex refers to the complete glossary of all existing Pokémon, and can be found in *pokedex.txt*.

### Compile & Run
* Compile programs using "make"
* Run with executables:
  * battle: ./battle
  * catch:  ./catch \<route\> \<Pokédex\>
  * stats:  ./stats \<Pokédex\>

### Purpose
*battle*, *catch*, and *stats* simulate three key features of the original Pokémon franchise adventure games. Battling refers to a turn-based combat between Pokémon. Catching refers to the process of capturing a wild Pokémon, which involves a combination of turn-based combat and luck. Calculating stats refers to the computational methods of determining a Pokémon's current stats, given its current level and [base stats](https://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_base_stats_(Generation_VIII-present)).

### Files
* battle.cpp: Simulates a turn-based Pokémon battle. Prompts the user for the name, type, HP, attack, defense, and speed stats of two battling Pokémon, and automates the battle following the original franchise's computational methods. *battle* returns the outcome, the number of turns required, and final HP levels.
* catch.cpp: Simulates a Pokémon catching encounter. Uses file I/O to determine Pokémon available to be caught on the provided route and possible Pokémon stats. Prompts the user for name and level of Pokémon combating the encounter, and automates catch encounter following the original franchise's computational methods. *catch* returns the name and level of encountered Pokémon and whether it was caught.
* stats.cpp: Calculates a given Pokémon's HP, attack, defense, and speed stats by its level. Uses file I/O to determine Pokémon's base stats. If provided level aligns with specified Pokémon's next evolution, notifies user of evolution and returns stats of Pokémon's evolved form.
* pokedex.txt: File I/O-friendly glossary of all Pokémon and their base stats.
* routes/route1-6.txt: Directory containing Pokémon that can be caught on routes 1-6. Pokémon increase in level and rarity as the route number increases.
* Makefile: Contains code that builds *battle*, *stats*, and *catch*.
