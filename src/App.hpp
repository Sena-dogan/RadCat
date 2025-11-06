#pragma once
#include <stdlib.h>
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <functional>
#include "TimeManager.hpp"
#include "Debug.hpp"
#include "udpHandler.hpp"

using namespace std;

class Controller{
private:
    mutex dataMutex;
    thread LogicThread;
    float elapsedMS = 0.0f;

    std::queue<std::function<void()>> m_taskQueue;
    std::mutex m_taskMutex;

public:
    UDPHandler udpHandler;

    Controller() : udpHandler() { if(systemInitializor()) run(); }
    ~Controller(){}

    //Main Methods
    void run();
    void logic();
    void stop();
    bool systemInitializor();

    // Task queue methods
    void queueTask(std::function<void()> task);
    void processQueuedTasks();
    void processUDPData(string data);

    // Setup and config
    atomic<bool> isRunning{false};



};