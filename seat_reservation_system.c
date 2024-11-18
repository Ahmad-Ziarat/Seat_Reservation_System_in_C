#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUSES 1
#define MAX_BAGS 3
#define MAX_FARE 100
#define FARE_PER_STOP 15
#define TOTAL_STATIONS 6
#define MAX_NAME_LENGTH 50

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int cardBalance;
    int ticketNumber;
    int seatNumber;
    int busNumber;
    int numBags;
    int destinationStation;
    int duesPending;
} Passenger;

typedef struct
{
    char busName[MAX_NAME_LENGTH];
    int busNumber;
    int totalSeats;
    int availableSeats;
    int *reservedSeats;
    Passenger *passengers;
} Bus;

void initializeBus(Bus *bus)
{
    printf("\n****************************************\n");
    printf("__B U S  I N I T I A L I Z A T I O N__\n");
    printf("****************************************\n");
    
    printf("Enter bus name: ");
    scanf("%s", bus->busName);
    printf("Enter Bus Number: ");
    scanf("%d", &bus->busNumber);
    printf("Enter Total Seats: ");
    scanf("%d", &bus->totalSeats);
    bus->availableSeats = bus->totalSeats;
    bus->reservedSeats = (int *)malloc(bus->totalSeats * sizeof(int));
    bus->passengers = (Passenger *)malloc(bus->totalSeats * sizeof(Passenger));
    
    for (int i = 0; i < bus->totalSeats; i++)
    {
        bus->reservedSeats[i] = 0;
    }

    printf("Bus initialized successfully\n");
    printf("****************************************\n");
}

void displayMenu()
{
    printf("\n=== Bus Reservation System ===\n");
    printf("1. Reserve a Seat\n");
    printf("2. Cancel a Seat\n");
    printf("3. Swipe Card\n");
    printf("4. Show Bus Status\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void reserveSeat(Bus *bus, Passenger *passengers, int *numPassengers)
{
    printf("\nEnter Passenger Name: ");
    scanf("%s", passengers[*numPassengers].name);

    int numBags;
    while (1)
    {
        printf("Enter Number of Bags: ");
        scanf("%d", &numBags);
        if (numBags <= MAX_BAGS)
        {
            break;
        }
        else
        {
            printf("Sorry, only %d bags are allowed. Please try again.\n", MAX_BAGS);
        }
    }
    passengers[*numPassengers].numBags = numBags;

    int destinationStation;
    while (1)
    {
        printf("Enter Destination Station (1-%d): ", TOTAL_STATIONS);
        scanf("%d", &destinationStation);
        if (destinationStation >= 1 && destinationStation <= TOTAL_STATIONS)
        {
            break;
        }
        else
        {
            printf("Invalid station. Please try again.\n");
        }
    }
    passengers[*numPassengers].destinationStation = destinationStation;

    int fare = FARE_PER_STOP * destinationStation;
    printf("Fare: %d\n", fare);

    char payFromCard;
    while (1)
    {
        printf("Press 'y' to pay from card, 'n' to pay cash: ");
        scanf(" %c", &payFromCard);
        if (payFromCard == 'y' || payFromCard == 'n')
        {
            break;
        }
        else
        {
            printf("Invalid input. Please try again.\n");
        }
    }

    if (payFromCard == 'y')
    {
        passengers[*numPassengers].duesPending = fare;
        passengers[*numPassengers].cardBalance = 100;
        printf("******Dues are still pending. Kindly pay them from Swipe Card menu.******\n");
    }

    passengers[*numPassengers].seatNumber = bus->totalSeats - bus->availableSeats + 1;
    passengers[*numPassengers].busNumber = bus->busNumber;
    bus->availableSeats--;
    bus->reservedSeats[passengers[*numPassengers].seatNumber - 1] = 1;
    bus->passengers[passengers[*numPassengers].seatNumber - 1] = passengers[*numPassengers];

    printf("Seat ' %d ' reserved successfully for %s!\n", passengers[*numPassengers].seatNumber, passengers[*numPassengers].name);
    (*numPassengers)++;
}

void cancelSeat(Bus *bus, Passenger *passengers, int *numPassengers)
{
    char name[MAX_NAME_LENGTH];
    int seatNumber;
    char confirm;

    printf("Enter Passenger Name: ");
    scanf("%s", name);
    printf("Enter Seat Number to Cancel: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > bus->totalSeats || bus->reservedSeats[seatNumber - 1] == 0)
    {
        printf("Invalid seat number or seat not reserved.\n");
        return;
    }

    for (int i = 0; i < *numPassengers; i++)
    {
        if (strcmp(passengers[i].name, name) == 0 && passengers[i].seatNumber == seatNumber)
        {
            printf("Are you sure you want to cancel the reservation for %s? (y/n): ", name);
            scanf(" %c", &confirm);
            if (confirm == 'y')
            {
                bus->availableSeats++;
                bus->reservedSeats[seatNumber - 1] = 0;
                printf("Reservation for %s on seat %d has been cancelled.\n", name, seatNumber);
                passengers[i] = passengers[--(*numPassengers)]; // Remove passenger
                return;
            }
            else
            {
                printf("Cancellation aborted.\n");
                return;
            }
        }
    }
    printf("No reservation found for %s on seat %d.\n", name, seatNumber);
}

void swipeCard(Passenger *passengers, int numPassengers)
{
    char name[MAX_NAME_LENGTH];
    printf("Enter Passenger Name: ");
    scanf("%s", name);

    for (int i = 0; i < numPassengers; i++)
    {
        if (strcmp(passengers[i].name, name) == 0)
        {
            if (passengers[i].duesPending > 0)
            {
                if (passengers[i].cardBalance >= passengers[i].duesPending)
                {
                    passengers[i].cardBalance -= passengers[i].duesPending;
                    printf("Payment successful! Remaining balance: %d\n", passengers[i].cardBalance);
                    passengers[i].duesPending = 0;
                }
                else
                {
                    printf("Insufficient balance. Please recharge your card.\n");
                }
            }
            else
            {
                printf("No dues pending for %s.\n", name);
            }
            return;
        }
    }
    printf("No passenger found with the name %s.\n", name);
}

void showBusStatus(Bus *bus)
{
    printf("\n****************************************\n");
    printf("\t*__*__B U S  S T A T U S__*__*\n");
    printf("****************************************\n");
    
    printf("Bus Name: %s\n", bus->busName);
    printf("Bus Number: %d\n", bus->busNumber);
    printf("Total Seats: %d\n", bus->totalSeats);
    printf("Available Seats: %d\n", bus->availableSeats);
    printf("Reserved Seats:\n");
    for (int i = 0; i < bus->totalSeats; i++)
    {
        if (bus->reservedSeats[i] == 1)
        {
            printf("Seat %d: %s\n", i + 1, bus->passengers[i].name);
        }
        else
        {
            printf("\tSeat %d: Available\n", i + 1);
        }
    }
    printf("****************************************\n");
}

int main()
{
    Bus bus;
    Passenger passengers[100];
    int numPassengers = 0;

    // Initialize the bus
    initializeBus(&bus);

    int choice;
    while (1)
    {
        displayMenu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            reserveSeat(&bus, passengers, &numPassengers);
            break;
        case 2:
            cancelSeat(&bus, passengers, &numPassengers);
            break;
        case 3:
            swipeCard(passengers, numPassengers);
            break;
        case 4:
            showBusStatus(&bus);
            break;
        case 5:
            printf("Exiting... Thanks for visiting.\n ");
            free(bus.reservedSeats); // Free allocated memory
            free(bus.passengers);     // Free allocated memory
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}