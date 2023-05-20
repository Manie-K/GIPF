#include <iostream>
#include "Engine.h"

using namespace std;

int main()
{
    Engine* engine = new Engine;
    while (!engine->getExit())
    {
        engine->inputCommand();
        engine->executeCommand();
    }
    delete engine;
}