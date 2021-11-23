#include "parseCourse.hpp"

void loadCourse(const char * path, GLfloat** green, int &greenSize, GLfloat*** bunkers, int &bunkerCount, int bunkerSizes[128]) {
    for (int* val = bunkerSizes; val < (bunkerSizes + 128); val++) {
        *val = 0;
    }
    int maxCoordCount = 0;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("File doesn't exist or I'm in the wrong directory \n");
        getchar();
        return;
    }

    while( 1 ){
        static int bunkerCoordCount = 0;
        int* activeCount;
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s \n", lineHeader);
    
        if(strcmp(lineHeader, "#") == 0) {
            
        } else if(strcmp(lineHeader, "Green") == 0) {
            activeCount = &greenSize;
        } else if (strcmp(lineHeader, "Bunker" ) == 0 ) {
            activeCount = &bunkerCoordCount;
            printf("found bunker\n");
            bunkerCount++;
            bunkerCoordCount = 0;
        } else if (strcmp(lineHeader, "p") == 0) {
            (*activeCount)++;
            if (activeCount == &bunkerCoordCount) {
                bunkerSizes[bunkerCount - 1] = bunkerCoordCount;
            }
            printf("%d\n", *activeCount);
        }
        if (res == EOF) break;
    }

    int greenVertexCount = 3 * greenSize;
    green = (GLfloat**)malloc(greenSize * sizeof(GLfloat*));
    if (!green) {
        fprintf(stderr, "Failed to allocate memory for green ptr, exiting...\n");
        exit;
    }
    for (int i = 0; i < greenSize; i++) {
        green[i] = (GLfloat*)calloc(3, sizeof(GLfloat));
        if (!green[i]) {
            fprintf(stderr, "Could not allocate memory for green multi-dim array, exiting...\n");
            exit;
        }
    }
    bunkers = (GLfloat***)malloc(bunkerCount * sizeof(GLfloat**));
    if (!bunkers) {
        fprintf(stderr, "Could not allocate memory for bunker multi-dim array, exiting...\n");
        exit;
    }
    for (int i = 0; i < bunkerCount; i++) {
        bunkers[i] = (GLfloat**)malloc(3 * bunkerSizes[i] * sizeof(GLfloat*));
        if (!bunkers[i]) {
            fprintf(stderr, "Could not allocate memory for bunker multi-dim array, part 2, exiting...\n");
            exit;
        }
        for (int j = 0; j < bunkerSizes[i]; j++) {
            bunkers[i][j] = (GLfloat*)calloc(3, sizeof(GLfloat));
            if (!bunkers[i]) {
                fprintf(stderr, "Could not allocate memory for bunker multi-dim array, part 3, exiting...\n");
                exit;
            }
        }
    }

    

    fseek(file, 0, SEEK_SET);

    char lineHeader[128];
    bool bunkerActive = false;
    bool greenActive = false;
    int bunker = 0;
    int coordCount = 0;
    int linecount = 0;
    while (1) {

        // read the first word of the line
        int res = fscanf(file, "%s \n", lineHeader);
        if(res == EOF) break;
        printf("linecount: %d\n", linecount);
        linecount++;

        if(strcmp(lineHeader, "#") == 0) {
            
        } else if(strcmp(lineHeader, "Green") == 0) {
            greenActive = true;
            bunkerActive = false;
            coordCount = 0;
        } else if (strcmp(lineHeader, "Bunker" ) == 0 ) {
            bunkerActive = true;
            greenActive = false;
            coordCount = 0;
            bunker++;
        } else if (strcmp(lineHeader, "p") == 0) {
            double c1, c2;
            fscanf(file, "%lf %lf\n", &c1, &c2);
            if (greenActive) {
                green[coordCount][0] = (GLfloat)c1;
                green[coordCount][1] = (GLfloat)c2;
            } else if (bunkerActive) {
                printf("bunker: %d, coordCount: %d\n", bunker, coordCount);
                bunkers[bunker - 1][coordCount][0] = (GLfloat)c1;
                bunkers[bunker - 1][coordCount][1] = (GLfloat)c2;
            } else {
                printf("what is going on here??? \n");
            }
            coordCount++;
        }
    }
}