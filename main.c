#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of cities and maximum length of city names
#define MAX_CITIES 100
#define MAX_CITY_NAME_LENGTH 50

// struct to represent an edge, connecting to cities in the graph (edge), for adjacency list
typedef struct Edge{
    int destination;
    int distance;
    struct Edge* next;
} Edge;

// struct to represent a city, a vertice in the graph, for adjacency list
typedef struct City{
    char name[MAX_CITY_NAME_LENGTH];
    Edge* edges;
} City;

// adds an edge, or in this case, a path from one city to another
void addEdge(Edge** head, int destination, int distance){
    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->destination = destination;
    newEdge->distance = distance;
    newEdge->next = *head;
    *head = newEdge;
}

// Add a city to the list of cities
void addCity(City* cities, int* numCities, char* name){
    strcpy(cities[*numCities].name, name);
    cities[*numCities].edges = NULL;
    (*numCities)++;
}

// Get the index of a city in the list of cities
int getCityIndex(City* cities, int numCities, char* name){
    for(int i = 0; i < numCities; i++){
        if(strcmp(cities[i].name, name) == 0)
            return i;
    }
    return -1;
}

City* createAdjancencyList(const char* cityListFilePath, const char* cityDistFilePath, int* numCities){
    // Allocate memory for cities array
    City* cities = (City*)malloc(MAX_CITIES * sizeof(City));
    if(!cities){
        printf("Failed to allocate memory for cities\n");
        return NULL;
    }

    // Initialize cities array
    for(int i = 0; i < MAX_CITIES; i++){
        cities[i].edges = NULL;
    }

    //Open city list file
    FILE* cityListFile = fopen(cityListFilePath, "r");
    if(!cityListFile){
        printf("Failed to open %s\n", cityListFilePath);
        free(cities);
        return NULL;
    }


    // read city names from city_list.dat into the city list
    char buffer[MAX_CITY_NAME_LENGTH];
    while(fgets(buffer, sizeof(buffer), cityListFile)){
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        addCity(cities, numCities, buffer);
    }
    // close the file, no longer needed.
    fclose(cityListFile);


    // Open city distances file
    FILE* cityDistFile = fopen(cityDistFilePath, "r");
    if(!cityDistFile){
        printf("Failed to open %s\n", cityDistFilePath);
        free(cities);
        return NULL;
    }

    // read through city distances, add to adjacency list.
    char city1[MAX_CITY_NAME_LENGTH], city2[MAX_CITY_NAME_LENGTH];
    int distance;
    while(fscanf(cityListFile, "%s %s %d", city1, city2, &distance) == 3){
        int index1 = getCityIndex(cities, *numCities, city1);
        int index2 = getCityIndex(cities, *numCities, city2);
        if(index1 == -1){
            printf("Unknown city %s in %s\n", city1, cityListFilePath);
            continue;
        }
        if(index2 == -1){
            printf("Unknown city %s in %s\n", city2, cityListFilePath);
            continue;
        }

        addEdge(&cities[index1].edges, index2, distance);
    }
    fclose(cityDistFile);
    return cities;

}


// main
int main() {
    int numCities = 0;
    City *cities = createAdjancencyList("/Users/ethanvirgil/Documents/GitHub/shortest-path-assignment-Ronak-Razavi/city_list.dat", "/Users/ethanvirgil/Documents/GitHub/shortest-path-assignment-Ronak-Razavi/city_distances.dat", &numCities);
    if (!cities) {
        printf("Failed to create adjacency list.\n");
        return 1;
    }
    for(int i=0; i<numCities; i++){
        printf("City: %s\n", cities[i].name);
        Edge* currentEdge = cities[i].edges;
        while(currentEdge){
            printf("\tConnects to: %s, Distance: %d\n", cities[currentEdge->destination].name, currentEdge->distance);
            currentEdge = currentEdge->next;
        }
    }

    // Free memory
    for (int i = 0; i < numCities; i++) {
        Edge* currentEdge = cities[i].edges;
        while(currentEdge){
            Edge* tempEdge = currentEdge;
            currentEdge = currentEdge->next;
            free(tempEdge);
        }
    }
    free(cities);

    return 0;


}
