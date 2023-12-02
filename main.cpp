#include <cstdio>
#include <string>
#include <algorithm>

#define EXPECTED_ARG_COUNT 5

bool intagerValidator(const std::string& str){
    return !str.empty() && std::find_if(
        str.begin(), 
        str.end(), 
        [](unsigned char c) { 
            return !std::isdigit(c); 
        }) == str.end();
}

bool itterateDice(int *dice, const int count, const int maxValue) {
    for( int i = 0; i < count; i++ ) {
        if( dice[i] < maxValue ) {
            dice[i]++;
            return true;
        } else {
            dice[i] = 1;
            continue;
        }
    }

    return false;
}

int main(int argc, char **argv) {
    if (argc < EXPECTED_ARG_COUNT) {
        printf("Expected at least %d arguments but only got %d\n", EXPECTED_ARG_COUNT - 1, argc - 1);
        return -1;
    }

    std::string diceCountStr = argv[1];
    std::string diceMaxValueStr = argv[2];
    std::string comparisonType = argv[3];
    std::string valueStr = argv[4];

    if (!intagerValidator(diceCountStr)) {
        printf("Expected first argument to be an intager\n");
        return -1;
    }

    if (!intagerValidator(diceMaxValueStr)) {
        printf("Expected second argument to be an intager\n");
        return -1;
    }

    if (!intagerValidator(valueStr)) {
        printf("Expected fourth argument to be an intager\n");
        return -1;
    }

    int diceCount = std::stoi( diceCountStr );
    int diceMaxValue = std::stoi( diceMaxValueStr );

    int min = diceCount;
    int max = diceCount * diceMaxValue;

    // TODO valudate expected is within min & max
    // TODO put limits on number of dice

    int *dice = new int[diceCount];
    std::fill_n(dice, diceCount, 1);
    
    int totalsCountLength = max-min + 1;

    int *totalsCount = new int[totalsCountLength];
    std::fill_n(totalsCount, totalsCountLength, 0);

    int grandTotal = 0;


    do {
        int total = 0;
        for ( int i = 0; i < diceCount; i++ ) {
            //printf("%d ", dice[i]);
            total += dice[i];
        }
        //printf("\n");
        totalsCount[total - min] += 1;
    } while (itterateDice(dice, diceCount, diceMaxValue));

    printf("%d: %d\n", min, totalsCount[0]);
    printf("total: %d\n", grandTotal);
}