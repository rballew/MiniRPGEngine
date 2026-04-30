#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Player {
public:
    string name;
    int health;
    int maxHealth;
    int attack;
    int gold;
    vector<string> inventory;

    Player(string playerName) {
        name = playerName;
        maxHealth = 100;
        health = 100;
        attack = 15;
        gold = 0;
    }

    void showStats() {
        cout << "\n=== Player Stats ===\n";
        cout << "Name: " << name << "\n";
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

    void rest() {
        health += 25;

        if (health > maxHealth) {
            health = maxHealth;
        }

        cout << "\nYou rest by a small fire and recover health.\n";
        cout << "Health is now " << health << "/" << maxHealth << ".\n";
    }
};

class Enemy {
public:
    string name;
    int health;
    int attack;
    int goldReward;

    Enemy(string enemyName, int enemyHealth, int enemyAttack, int reward) {
        name = enemyName;
        health = enemyHealth;
        attack = enemyAttack;
        goldReward = reward;
    }
};

void showMenu() {
    cout << "\n=== ShadowVale RPG ===\n";
    cout << "1. Explore forest\n";
    cout << "2. Check stats\n";
    cout << "3. Open inventory\n";
    cout << "4. Use Healing Herb\n";
    cout << "5. Rest\n";
    cout << "6. Exit game\n";
    cout << "Choose an option: ";
}

void fightEnemy(Player& player, Enemy enemy) {
    cout << "\nA " << enemy.name << " appears!\n";

    while (player.health > 0 && enemy.health > 0) {
        cout << "\nYour Health: " << player.health << "\n";
        cout << enemy.name << " Health: " << enemy.health << "\n";

        cout << "\n1. Attack\n";
        cout << "2. Use Healing Herb\n";
        cout << "3. Run\n";
        cout << "Choose an action: ";

        int choice;
        cin >> choice;

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
            return;
        }
        else {
            cout << "\nInvalid choice. The enemy attacks while you hesitate!\n";
            player.health -= enemy.attack;
        }
    }

    if (player.health <= 0) {
        cout << "\nYou were defeated...\n";
        return;
    }

    cout << "\nYou defeated the " << enemy.name << "!\n";
    cout << "You found " << enemy.goldReward << " gold.\n";

    player.gold += enemy.goldReward;
}

void exploreForest(Player& player) {
    cout << "\nYou step into the dark forest...\n";

    int event = rand() % 4;

    if (event == 0) {
        cout << "You find a Healing Herb.\n";
        player.inventory.push_back("Healing Herb");
    }
    else if (event == 1) {
        cout << "You find a small pouch of gold.\n";
        player.gold += 10;
        cout << "You gained 10 gold.\n";
    }
    else if (event == 2) {
        Enemy goblin("Goblin", 30, 8, 15);
        fightEnemy(player, goblin);
    }
    else {
        Enemy wolf("Wolf", 40, 10, 20);
        fightEnemy(player, wolf);
    }
}

int main() {
    srand(time(0));

    string playerName;

    cout << "Enter your hero's name: ";
    getline(cin, playerName);

    Player player(playerName);

    int choice;

    cout << "\nWelcome to ShadowVale, " << player.name << ".\n";
    cout << "The forest ahead is old, hungry, and full of secrets.\n";

    do {
        if (player.health <= 0) {
            cout << "\nGame Over.\n";
            break;
        }

        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            exploreForest(player);
            break;

        case 2:
            player.showStats();
            break;

        case 3:
            player.showInventory();
            break;

        case 4:
            player.useHealingHerb();
            break;

        case 5:
            player.rest();
            break;

        case 6:
            cout << "\nThanks for playing ShadowVale RPG.\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
            break;
        }

    } while (choice != 6);

    return 0;
}
