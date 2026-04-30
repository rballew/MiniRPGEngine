#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

class Weapon {
public:
    string name;
    int attackBonus;

    Weapon(string weaponName, int bonus) {
        name = weaponName;
        attackBonus = bonus;
    }
};

class Player {
public:
    string name;
    int health;
    int maxHealth;
    int attack;
    int gold;
    int level;
    int xp;
    int xpToNextLevel;
    vector<string> inventory;

    Player(string playerName) {
        name = playerName;
        maxHealth = 100;
        health = 100;
        attack = 15;
        gold = 0;
        level = 1;
        xp = 0;
        xpToNextLevel = 50;
    }

    bool hasItem(string itemName) {
        for (const string& item : inventory) {
            if (item == itemName) {
                return true;
            }
        }

        return false;
    }

    void showStats() {
        cout << "\n=== Player Stats ===\n";
        cout << "Name: " << name << "\n";
        cout << "Level: " << level << "\n";
        cout << "XP: " << xp << "/" << xpToNextLevel << "\n";
        cout << "Health: " << health << "/" << maxHealth << "\n";
        cout << "Attack: " << attack << "\n";
        cout << "Gold: " << gold << "\n";
    }

    void showInventory() {
        cout << "\n=== Inventory ===\n";

        if (inventory.empty()) {
            cout << "Your inventory is empty.\n";
        }
        else {
            for (int i = 0; i < inventory.size(); i++) {
                cout << i + 1 << ". " << inventory[i] << "\n";
            }
        }
    }

    void useHealingHerb() {
        for (int i = 0; i < inventory.size(); i++) {
            if (inventory[i] == "Healing Herb") {
                health += 30;

                if (health > maxHealth) {
                    health = maxHealth;
                }

                inventory.erase(inventory.begin() + i);

                cout << "\nYou used a Healing Herb.\n";
                cout << "Health is now " << health << "/" << maxHealth << ".\n";
                return;
            }
        }

        cout << "\nYou do not have a Healing Herb.\n";
    }

    void equipWeapon(Weapon weapon) {
        if (hasItem(weapon.name)) {
            cout << "\nYou already have the " << weapon.name << ".\n";
            return;
        }

        inventory.push_back(weapon.name);
        attack += weapon.attackBonus;

        cout << "\nYou found a " << weapon.name << "!\n";
        cout << "Your attack increased by " << weapon.attackBonus << ".\n";
        cout << "Attack is now " << attack << ".\n";
    }

    void gainXP(int amount) {
        xp += amount;

        cout << "You gained " << amount << " XP.\n";

        while (xp >= xpToNextLevel) {
            xp -= xpToNextLevel;
            levelUp();
        }
    }

    void levelUp() {
        level++;
        maxHealth += 20;
        attack += 5;
        health = maxHealth;
        xpToNextLevel += 25;

        cout << "\n=== Level Up! ===\n";
        cout << "You reached level " << level << "!\n";
        cout << "Max health increased to " << maxHealth << ".\n";
        cout << "Attack increased to " << attack << ".\n";
        cout << "Your health has been fully restored.\n";
    }

    void rest() {
        health += 25;

        if (health > maxHealth) {
            health = maxHealth;
        }

        cout << "\nYou rest and recover health.\n";
        cout << "Health is now " << health << "/" << maxHealth << ".\n";
    }
};

class Enemy {
public:
    string name;
    int health;
    int attack;
    int goldReward;
    int xpReward;

    Enemy(string enemyName, int enemyHealth, int enemyAttack, int reward, int xpAmount) {
        name = enemyName;
        health = enemyHealth;
        attack = enemyAttack;
        goldReward = reward;
        xpReward = xpAmount;
    }
};

class Room {
public:
    string name;
    string description;
    int north;
    int south;
    int east;
    int west;
    bool hasRandomEvents;

    Room(string roomName, string roomDescription, int n, int s, int e, int w, bool randomEvents) {
        name = roomName;
        description = roomDescription;
        north = n;
        south = s;
        east = e;
        west = w;
        hasRandomEvents = randomEvents;
    }
};
int getNumberInput(int minChoice, int maxChoice) {
    int choice;

    while (true) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number between " << minChoice << " and " << maxChoice << ": ";
        }
        else if (choice < minChoice || choice > maxChoice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number between " << minChoice << " and " << maxChoice << ": ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

bool fightEnemy(Player& player, Enemy enemy) {
    cout << "\nA " << enemy.name << " appears!\n";

    while (player.health > 0 && enemy.health > 0) {
        cout << "\nYour Health: " << player.health << "\n";
        cout << enemy.name << " Health: " << enemy.health << "\n";

        cout << "\n1. Attack\n";
        cout << "2. Use Healing Herb\n";
        cout << "3. Run\n";
        cout << "Choose an action: ";

        int choice = getNumberInput(1, 3);

        if (choice == 1) {
            enemy.health -= player.attack;
            cout << "\nYou strike the " << enemy.name << " for " << player.attack << " damage.\n";

            if (enemy.health > 0) {
                player.health -= enemy.attack;
                cout << "The " << enemy.name << " hits you for " << enemy.attack << " damage.\n";
            }
        }
        else if (choice == 2) {
            player.useHealingHerb();

            if (enemy.health > 0) {
                player.health -= enemy.attack;
                cout << "The " << enemy.name << " attacks while you recover and deals " << enemy.attack << " damage.\n";
            }
        }
        else if (choice == 3) {
            cout << "\nYou escape back to the path.\n";
            return false;
        }
        else {
            cout << "\nInvalid choice. The enemy attacks while you hesitate!\n";
            player.health -= enemy.attack;
        }
    }

    if (player.health <= 0) {
        cout << "\nYou were defeated...\n";
        return false;
    }

    cout << "\nYou defeated the " << enemy.name << "!\n";
    cout << "You found " << enemy.goldReward << " gold.\n";

    player.gold += enemy.goldReward;
    player.gainXP(enemy.xpReward);

    return true;
}

bool fightBoss(Player& player) {
    cout << "\nThe black iron gate groans open.\n";
    cout << "A Shadow Beast rises from the darkness beyond it.\n";
    cout << "This is the final fight.\n";

    Enemy boss("Shadow Beast", 90, 15, 100, 100);

    bool defeatedBoss = fightEnemy(player, boss);

    if (player.health <= 0) {
        return false;
    }

    if (defeatedBoss) {
        cout << "\n=== Victory ===\n";
        cout << "The Shadow Beast falls, and the forest grows quiet.\n";
        cout << "ShadowVale is safe, at least for now.\n";
        cout << "You completed the adventure, " << player.name << "!\n";
        return true;
    }

    cout << "\nYou retreat from the Boss Gate.\n";
    return false;
}

void triggerRoomEvent(Player& player, Room room, bool& gameWon) {
    if (room.name == "Boss Gate") {
        gameWon = fightBoss(player);
        return;
    }

    if (!room.hasRandomEvents) {
        if (room.name == "Old Shrine") {
            cout << "\nThe shrine feels quiet and safe.\n";
            cout << "A soft light restores 10 health.\n";

            player.health += 10;

            if (player.health > player.maxHealth) {
                player.health = player.maxHealth;
            }

            cout << "Health is now " << player.health << "/" << player.maxHealth << ".\n";
        }

        return;
    }

    int event = rand() % 5;

    if (event == 0) {
        cout << "\nYou find a Healing Herb.\n";
        player.inventory.push_back("Healing Herb");
    }
    else if (event == 1) {
        cout << "\nYou find a small pouch of gold.\n";
        player.gold += 10;
        cout << "You gained 10 gold.\n";
    }
    else if (event == 2) {
        Weapon rustySword("Rusty Sword", 10);
        player.equipWeapon(rustySword);
    }
    else if (event == 3) {
        Enemy goblin("Goblin", 30, 8, 15, 20);
        fightEnemy(player, goblin);
    }
    else {
        Enemy wolf("Wolf", 40, 10, 20, 30);
        fightEnemy(player, wolf);
    }
}

void showRoom(const Room& room) {
    cout << "\n=== " << room.name << " ===\n";
    cout << room.description << "\n";
}

void showMenu() {
    cout << "\n=== Actions ===\n";
    cout << "1. Move north\n";
    cout << "2. Move south\n";
    cout << "3. Move east\n";
    cout << "4. Move west\n";
    cout << "5. Check stats\n";
    cout << "6. Open inventory\n";
    cout << "7. Use Healing Herb\n";
    cout << "8. Rest\n";
    cout << "9. Exit game\n";
    cout << "Choose an option: ";
}

void movePlayer(int& currentRoomIndex, vector<Room>& rooms, int destination, Player& player, bool& gameWon) {
    if (destination == -1) {
        cout << "\nYou cannot go that way.\n";
        return;
    }

    currentRoomIndex = destination;

    showRoom(rooms[currentRoomIndex]);
    triggerRoomEvent(player, rooms[currentRoomIndex], gameWon);
}

vector<Room> createWorld() {
    vector<Room> rooms;

    rooms.push_back(Room(
        "Forest Path",
        "A narrow path twists through dark trees. The air smells like rain and old leaves.",
        1, -1, 2, -1,
        true
    ));

    rooms.push_back(Room(
        "Old Shrine",
        "A cracked stone shrine stands beneath hanging vines. It feels ancient, but peaceful.",
        -1, 0, 3, -1,
        false
    ));

    rooms.push_back(Room(
        "Moonlit Lake",
        "A silver lake reflects the sky. Something moves beneath the water.",
        3, -1, -1, 0,
        true
    ));

    rooms.push_back(Room(
        "Bandit Camp",
        "Old campfires and broken crates litter the ground. This place has seen trouble.",
        -1, 2, 4, 1,
        true
    ));

    rooms.push_back(Room(
        "Boss Gate",
        "A black iron gate blocks the path. Beyond it, something powerful waits.",
        -1, -1, -1, 3,
        false
    ));

    return rooms;
}

int main() {
    srand(time(0));

    string playerName;

    cout << "Enter your hero's name: ";
    getline(cin, playerName);

    Player player(playerName);
    vector<Room> rooms = createWorld();

    int currentRoomIndex = 0;
    int choice;
    bool gameWon = false;

    cout << "\nWelcome to ShadowVale, " << player.name << ".\n";
    cout << "The world ahead is old, hungry, and full of secrets.\n";

    showRoom(rooms[currentRoomIndex]);

    do {
        if (player.health <= 0) {
            cout << "\nGame Over.\n";
            break;
        }

        if (gameWon) {
            cout << "\nThanks for playing ShadowVale RPG.\n";
            break;
        }

        showMenu();
        choice = getNumberInput(1, 9);

        switch (choice) {
        case 1:
            movePlayer(currentRoomIndex, rooms, rooms[currentRoomIndex].north, player, gameWon);
            break;

        case 2:
            movePlayer(currentRoomIndex, rooms, rooms[currentRoomIndex].south, player, gameWon);
            break;

        case 3:
            movePlayer(currentRoomIndex, rooms, rooms[currentRoomIndex].east, player, gameWon);
            break;

        case 4:
            movePlayer(currentRoomIndex, rooms, rooms[currentRoomIndex].west, player, gameWon);
            break;

        case 5:
            player.showStats();
            break;

        case 6:
            player.showInventory();
            break;

        case 7:
            player.useHealingHerb();
            break;

        case 8:
            player.rest();
            break;

        case 9:
            cout << "\nThanks for playing ShadowVale RPG.\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
            break;
        }

    } while (choice != 9);

    return 0;
}