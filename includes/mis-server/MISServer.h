//
// Created by CIJhn on 11/27/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
#define MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H

#include <atomic>
#include <mutex>
#include <ctime>
#include <chrono>

#include "TCPServerSocket.h"
#include "mis-core/VirtualMachine.h"
#include "mis-core/parser.h"

namespace mis {
    struct MISServer {
        bool isTerminated();

        void terminated();

        void start();

        MISServer(TCPServerSocket *socket, VirtualMachine *virtualMachine, Parser *parser);

        virtual ~MISServer();

        const vector<Record *> &getHistory() const;

    private:
        class Worker {
            TCPSocket *socket;
            Parser *parser;
            VirtualMachine *virtualMachine;
            std::function<void(Worker *)> callback;
            Record *record;

        public:
            Worker(TCPSocket *socket, Parser *parser, VirtualMachine *virtualMachine,
                   const function<void(Worker *)> &callback);

            void start();

            virtual ~Worker();
        };

        struct Record {
            std::string ip;
            std::chrono::system_clock::time_point startTime;
            std::chrono::milliseconds duration;

            const string &getIp() const;

            std::string getDuration() const;

            std::string getStartTime() const;
        };

        TCPServerSocket *socket;
        std::atomic<bool> terminate;
        std::mutex mutex, startingLock;
        VirtualMachine *virtualMachine;
        Parser *parser;

        std::vector<Worker *> workingQueue;
        std::vector<Record *> history;

        void garbage(Worker *worker);


    };
}


#endif //MACHINEINSTRUCTIONSSIMULATOR_MISSERVER_H
