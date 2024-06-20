#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <windows.h>

using namespace std;

class Passenger {
public:
    Passenger(double arrivalTime) : arrivalTime(arrivalTime) {}
    double getArrivalTime() const { return arrivalTime; }
private:
    double arrivalTime;
};

class Shuttle{
public:
    Shuttle(double arrivalTime, int freeSeats) : arrivalTime(arrivalTime), freeSeats(freeSeats) {}
    double getArrivalTime() const { return arrivalTime; }
    int getFreeSeats() const { return freeSeats; }
private:
    double arrivalTime;
    int freeSeats;
};

class BusStop {
public:
    BusStop(double avgPassengerArrival, double avgShuttleArrival, bool isTerminal)
        : avgPassengerArrival(avgPassengerArrival), avgShuttleArrival(avgShuttleArrival), isTerminal(isTerminal), totalWaitingTime(0), totalPassengers(0) {}

    void simulate(double simulationTime)
    {
        default_random_engine generator;
        exponential_distribution<double> passengerDistribution(1.0 / avgPassengerArrival);
        exponential_distribution<double> shuttleDistribution(1.0 / avgShuttleArrival);
        uniform_int_distribution<int> seatDistribution(0, 20);

        double currentTime = 0.0;
        priority_queue<double, vector<double>, greater<>> events;

        while (currentTime < simulationTime)
        {
            double nextPassengerTime = currentTime + passengerDistribution(generator);
            double nextShuttleTime = currentTime + shuttleDistribution(generator);

            if (nextPassengerTime < nextShuttleTime && nextPassengerTime < simulationTime)
            {
                events.push(nextPassengerTime);
                currentTime = nextPassengerTime;
                Passenger p(currentTime);
                passengers.push(p);
                totalPassengers++;
            }
            else if (nextShuttleTime < simulationTime)
            {
                events.push(nextShuttleTime);
                currentTime = nextShuttleTime;
                Shuttle s(currentTime, seatDistribution(generator));
                processShuttleArrival(s);
            }
            else
            {
                break;
            }
        }
        cout << "Среднее время ожидания: " << totalWaitingTime / totalPassengers << " минут" << endl;
    }

private:
    void processShuttleArrival(const Shuttle& shuttle)
    {
        int freeSeats = shuttle.getFreeSeats();
        while (!passengers.empty() && freeSeats > 0)
        {
            Passenger p = passengers.front();
            passengers.pop();
            totalWaitingTime += shuttle.getArrivalTime() - p.getArrivalTime();
            freeSeats--;
        }
    }

    double avgPassengerArrival;
    double avgShuttleArrival;
    bool isTerminal;
    double totalWaitingTime;
    int totalPassengers;
    queue<Passenger> passengers;
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    double avgPassengerArrival = 5.0; // average time between passenger arrivals in minutes
    double avgShuttleArrival = 15.0; // average time between shuttle arrivals in minutes
    bool isTerminal = false;
    double simulationTime = 480.0; // total simulation time in minutes (e.g., 8 hours)

    BusStop busStop(avgPassengerArrival, avgShuttleArrival, isTerminal);
    busStop.simulate(simulationTime);

    return 0;
}