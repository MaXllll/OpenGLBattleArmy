#include "Army.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "training.hpp"
#include "fstream"

#include "Filters/IFilters.hpp"

void unitTest()
{

}

int main(void)
{
    std::srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<std::unique_ptr<Army> > champions;
    try
    {
        std::ifstream in("Army_10_100.save");
        Army army = Army::load(in);
        std::cout << army << std::endl;
        champions.push_back(std::unique_ptr<Army>(new Army(army)));
    }
    catch(...)
    {
    }

    try
    {
        std::unique_ptr<Army> army = train(10, 100, 20, 10, 100, champions);
        std::cout << *army << std::endl;
        std::ofstream out("Army_10_100.save");
        army->save(out);
        out.flush();
        out.close();
    }
    catch (std::invalid_argument& e)
    {
        std::cout << "toto" << std::endl;
    }

    return 0;
}