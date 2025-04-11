#include <FlowerEngine/Inc/FlowerEngine.h>

using namespace FlowerEngine;
using namespace FlowerEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
    Student()
    {
        mId = ++sUniqueId;

        LOG("Stundent ID: %d", mId);
    }

    void SetName(const std::string& name)
    {
        mName = name;
    }
private:
    int mId = 0;
    std::string mName;
};
class DebitCard
{
public:
    DebitCard()
    {
        int randNum = rand() % 500 + 100;

        mAccoundNum = randNum;

        LOG("Acoount Number: %d", mAccoundNum);
    }

private:
    int mAccoundNum = 0;
};


class Enemy
{
public:
    Enemy()
    {
        int randHealth = rand() % 90 + 10;

        mHealth = randHealth;
        LOG("Enemy Health: %d", mHealth);
    }

private:
    int mHealth = 0;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
    TypedAllocator debitCardPool = TypedAllocator<DebitCard>("DebitCardPool", 50);
    TypedAllocator enemyPool = TypedAllocator<Enemy>("EnemyPool", 25);

    std::vector<Student*> students;
    std::vector<DebitCard*> debitCards;
    std::vector<Enemy*> enemies;
    //test new
    for (uint32_t i = 0; i < 100; ++i)
    {
        Student* newStudent = studentPool.New();
        students.push_back(newStudent);
    }

    //test deletion
    for (uint32_t i = 0; i < 60; ++i)
    {
        Student* student = students.back();
        studentPool.Delete(student);
        students.pop_back();
    }

    //test adding more
    for (uint32_t i = 0; i < 55; ++i)
    {
        Student* student = studentPool.New();
        students.push_back(student);
    }

    // Debit cards
    for (uint32_t i = 0; i < 50; ++i)
    {
        DebitCard* debitCard = debitCardPool.New();
        debitCards.push_back(debitCard);
    }

    for (uint32_t i = 0; i < 10; ++i)
    {
        DebitCard* debitCard = debitCards.back();
        debitCardPool.Delete(debitCard);
        debitCards.pop_back();
    }

    // Enemies
    for (uint32_t i = 0; i < 25; ++i)
    {
        Enemy* enemy = enemyPool.New();
        enemies.push_back(enemy);
    }

    for (uint32_t i = 0; i < 3; ++i)
    {
        Enemy* enemy = enemies.back();
        enemyPool.Delete(enemy);
        enemies.pop_back();
    }

    // clear all
    for (Student* student : students)
    {
        studentPool.Delete(student);
    }

    for (DebitCard* debitCard : debitCards)
    {
        debitCardPool.Delete(debitCard);
    }

    for (Enemy* enemy : enemies)
    {
        enemyPool.Delete(enemy);
    }

    students.clear();
    debitCards.clear();
    enemies.clear();

    return(0);
}