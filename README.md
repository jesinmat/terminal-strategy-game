# Terminal strategy game
A simple strategy game in a Linux terminal. Semestral work for the BI-PA2 course.

The goal of this game is to survive on a hostile planet after a sudden crash. Build structures, collect resources, take care of your workers and pray a disaster doesn't hit! Can you escape in time?

## Features

- every map element loaded from a file, including disasters (see `resources/data`)
- simulated day cycle (turn-based)
- save & load your games (`resources/savegame`)
- rating system
- multiple endings

## Screenshots

![Main menu](img/menu.png "Main menu")

![First day](img/start.png "First day")

![Gameplay](img/lategame.png "Gameplay")

## Compiling

This game uses ncurses. Install the `libncurses-dev` package and run `make compile`.

If you also want to generate documentation, install `doxygen` and `graphviz` and run `make doc`.
Running `make` is equivalent to running `make compile` and `make doc`.

## Running

Run the game with `make run` or `./jesinmat`. The game will ask you to resize your terminal window to 80x24 before starting. 