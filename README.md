# ipp-gamma

### Content

The project involves implementing the game Gamma, played on a rectangular board composed of identical square fields. Adjacent fields are those that share sides, not just corners. Fields form an area if every field within it can be reached from any other by moving only through adjacent fields; a single field also constitutes an area.

Gameplay Overview:

- Players: One or more players can participate.
- Starting Condition: The board is empty at the beginning of the game.
- Turns: Players take turns placing their tokens on unoccupied fields.

Rules:

1. Field Occupation:
   
   - Players can occupy any unoccupied field.
   - The set of fields occupied by the same player cannot exceed a maximum number of areas, a game parameter.

2. Golden Move:
   
   - Each player can make one golden move per game, which involves:
     - Removing another player's token.
     - Placing their own token in the vacated spot.
     - This move must not violate the maximum area rule for any player.

3. Elimination and Re-entry:
   
   - A player unable to make a legal move is out of the game.
   - They can re-enter if another player's golden move changes the board state.

Game End and Winning:
The game ends when no player can make a valid move. The player who occupies the most fields at the end wins.

**Provided**
src/gamma.h, src/gamma_test.c, CMakeLists.txt, Doxyfile.in, MainPage.dox (some modified), rules of the game

---

**Set up**

```
mkdir release
cd release
cmake ..
make
make doc
```

or debug version

```
mkdir debug
cd debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
make doc
```

---

**Launch game**

The game can be launched in interactive mode - the board will be visible on the console, and the following actions are available:
- Arrow keys to move around the board,
- Spacebar to occupy a field,
- 'G' key to perform a special move on another player's field.

Console Game Launch Instructions:
- First, run `./gamma`.
- Then, enter `I <X> <Y> <N> <A>`, where:
   - \<X>, \<Y> - dimensions of the board,
   - \<N> - number of players,
   - \<A> - maximum number of areas for each player.

---

**Project preview**

| Doxygen documentation                                                                                      | Game screen                                                                                                |
|:----------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------:|
| ![](https://user-images.githubusercontent.com/73189722/132677316-b59a22ac-3460-4c5c-bb3f-c43f4ba0897f.png) |  |
