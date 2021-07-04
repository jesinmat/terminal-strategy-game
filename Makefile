# define variables
CC=g++
CFLAGS=-Wall -pedantic -Wextra -std=c++11 -Wno-long-long -O0 -ggdb
LIBS=-lncurses
OBJ_FOLDER=obj
SRC_FOLDER=src
CPP_FILES=$(wildcard $(SRC_FOLDER)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_FOLDER)/%.cpp,$(OBJ_FOLDER)/%.o,$(CPP_FILES))
HEADER_FILES = $(wildcard $(SRC_FOLDER)/*.hpp)
USERNAME=jesinmat

all: compile doc
	@echo "Done."

.PHONY: compile

compile: $(USERNAME)
	
	
$(USERNAME): $(OBJ_FILES)
	@echo "Linking files..."
	@$(CC) $(CFLAGS) $^ -o $(USERNAME) $(LIBS)
	@echo "Compilation successful."

$(OBJ_FOLDER):
	@mkdir $(OBJ_FOLDER)
	
$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp | $(OBJ_FOLDER)
	@echo -n "Compiling $(notdir $@)... "
	@$(CC) -c $(CFLAGS) -o $@ $<
	@echo "Done"

.PHONY: clean

clean:
	@rm -f $(USERNAME)
	@rm -rf $(OBJ_FOLDER)
	@rm -rf doc
	
.PHONY: run	

run:
	@./$(USERNAME)
	
.PHONY: doc	

doc: doc/index.html
	
		
doc/index.html: $(HEADER_FILES) Doxyfile
	@echo "Generating documentation."
	@doxygen Doxyfile > /dev/null
	@echo "Documentation generated."
	
.PHONY: debug

debug: compile
	@echo "Debugging started"
	@valgrind --log-file="valgrind.txt" --leak-check=full ./$(USERNAME)
    
# generated with g++ -MM *.cpp

building.o: building.cpp building.hpp worldMap.hpp colors.hpp \
 interfaceElement.hpp gameEngine.hpp dailyAction.hpp gameBase.hpp \
 fileio.hpp dataStore.hpp
colors.o: colors.cpp colors.hpp
controls.o: controls.cpp controls.hpp
dailyAction.o: dailyAction.cpp dailyAction.hpp
dataStore.o: dataStore.cpp dataStore.hpp dailyAction.hpp gameBase.hpp \
 interfaceElement.hpp fileio.hpp gameEngine.hpp worldMap.hpp colors.hpp \
 building.hpp
fileio.o: fileio.cpp fileio.hpp gameBase.hpp interfaceElement.hpp \
 dataStore.hpp dailyAction.hpp gameEngine.hpp worldMap.hpp colors.hpp \
 building.hpp
gameBase.o: gameBase.cpp gameBase.hpp interfaceElement.hpp fileio.hpp \
 dataStore.hpp dailyAction.hpp gameEngine.hpp worldMap.hpp colors.hpp \
 building.hpp menu.hpp controls.hpp infoPanel.hpp
gameEngine.o: gameEngine.cpp gameEngine.hpp interfaceElement.hpp \
 worldMap.hpp colors.hpp building.hpp dailyAction.hpp controls.hpp \
 gameBase.hpp fileio.hpp dataStore.hpp menu.hpp infoPanel.hpp
infoPanel.o: infoPanel.cpp infoPanel.hpp interfaceElement.hpp \
 gameBase.hpp fileio.hpp dataStore.hpp dailyAction.hpp gameEngine.hpp \
 worldMap.hpp colors.hpp building.hpp menu.hpp controls.hpp
interfaceElement.o: interfaceElement.cpp interfaceElement.hpp
main.o: main.cpp gameBase.hpp interfaceElement.hpp fileio.hpp \
 dataStore.hpp dailyAction.hpp gameEngine.hpp worldMap.hpp colors.hpp \
 building.hpp
menu.o: menu.cpp dataStore.hpp dailyAction.hpp menu.hpp \
 interfaceElement.hpp gameEngine.hpp worldMap.hpp colors.hpp building.hpp \
 infoPanel.hpp controls.hpp gameBase.hpp fileio.hpp
worldMap.o: worldMap.cpp worldMap.hpp colors.hpp interfaceElement.hpp \
 building.hpp gameEngine.hpp dailyAction.hpp controls.hpp gameBase.hpp \
 fileio.hpp dataStore.hpp
