#include <iostream>
#include "Engine.h"

using namespace std;

int main()
{
    Engine* engine = new Engine;
    while (!engine->getExit())
    {
        if (engine->getIsEngineCommand()) {
            engine->inputCommand();
            engine->executeCommand();
        }
        else{
            engine->getSolver()->inputCommand();
            engine->getSolver()->executeCommand();
        }
    }
    delete engine;
}