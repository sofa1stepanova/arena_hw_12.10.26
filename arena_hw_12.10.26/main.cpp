#include <iostream>
using namespace std;

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;

public:
    Fighter(const char* name_, int health_, int damage_)
        : name(name_), health(health_), maxHealth(health_), damage(damage_) {
    }

    void TakeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
    }

    void Heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }

    bool IsAlive() const {
        return health > 0;
    }

    void PrintStatus() const {
        cout << name << ": " << health << "/" << maxHealth << "\n";
    }
};

void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;
    int medkits = 1;

public:
    Game()
        : player("player", 20, 8),
          enemies{
              {"enemy 1", 12, 3},
              {"enemy 2", 18, 4},
              {"enemy 3", 24, 5}
          } {
    }

    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;
        if (number < 1 || number > 3) {
            return false;
        }
        if (!enemies[number - 1].IsAlive()) {
            return false;
        }
        outTarget = &enemies[number - 1];
        return true;
    }

    bool AllEnemiesDefeated() const {
        bool allDead = true;
        for (int i = 0; i < 3; ++i) {
            if (enemies[i].IsAlive()) {
                allDead = false;
            }
        }
        return allDead;
    }

    void PrintState() const {
        cout << "state:\n";
        PrintFighter(player);
        for (int i = 0; i < 3; ++i) {
            PrintFighter(enemies[i]);
        }
        if (currentTarget != nullptr) {
            cout << "current target: ";
            currentTarget->PrintStatus();
        } else {
            cout << "current target: none\n";
        }
        cout << "medkits: " << medkits << "\n";
    }

    void HandleSelectTarget() {
        int number = 0;
        cout << "type target's number (1/2/3): ";
        cin >> number;
        if (TrySelectTarget(number, currentTarget)) {
            cout << "target selected: ";
            currentTarget->PrintStatus();
        } else {
            cout << "wrong number\n";
            currentTarget = nullptr;
        }
    }

    void HandleAttack() {
        if (currentTarget == nullptr) {
            cout << "no current target; the attack is impossible\n";
            return;
        }

        player.Attack(*currentTarget);

        if (currentTarget->IsAlive()) {
            currentTarget->Attack(player);
        } else {
            cout << "target eliminated\n";
            currentTarget = nullptr;
        }
    }

    void HandleHeal() {
        if (currentTarget == nullptr) {
            cout << "no current target; healing is impossible\n";
            return;
        }
        if (medkits <= 0) {
            cout << "no medkits left\n";
            return;
        }

        player.Heal(8);
        medkits = medkits - 1;
        cout << "healed\n";

        if (currentTarget->IsAlive()) {
            currentTarget->Attack(player);
        }
    }

    void Run() {
        while (true) {
            PrintState();

            if (!player.IsAlive()) {
                cout << "u lost\n";
                break;
            }

            if (AllEnemiesDefeated()) {
                cout << "u won\n";
                break;
            }

            cout << "1 - choose target\n";
            cout << "2 - attack\n";
            cout << "3 - use medkit\n";
            cout << "0 - esc\n";
            cout << "command: ";

            int command = 0;
            cin >> command;

            if (command == 0) {
                cout << "esc\n";
                break;
            } else if (command == 1) {
                HandleSelectTarget();
            } else if (command == 2) {
                HandleAttack();
            } else if (command == 3) {
                HandleHeal();
            } else {
                cout << "bruh\n";
            }
        }
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
