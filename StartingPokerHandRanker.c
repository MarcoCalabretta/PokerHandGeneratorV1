#include <math.h>
#include <stdio.h>
int hands[2118760] = {0};//50 choose 5
int temp[2118760] = {0};//for merge function
int numhands = sizeof(hands)/sizeof(int);
//a card is an int[2] where card[0] is the card's rank 2-14 and card[1] is the suit 1-4
int hand_score(int a[], int b[], int c[], int d[], int e[]);
void inverse_hand_score(int handscore, int *hand);
int five_of_seven(int a[], int b[], int c[], int d[], int e[], int f[], int g[]);
void all_hands(int a[], int b[], int *hand_data);//will edit all the ints in hand_data
void all_starting_hands(FILE *fp);//will edit all the ints in hand_data
void print_hand(int index, int *hand_arr, FILE *fp);
int array_eq(int a1[], int a2[]);
void merge_sort(int *arr, int start, int end);
void merge(int *arr, int start, int mid, int end);
void swap(int *ptr1, int *ptr2);
void main(){
    char *filename = "";
    //FILE *fp = fopen(FILL THIS IN WITH THE TEXT FILE YOU WANT TO OUTPUT TO, "w");
    all_starting_hands(fp);
    fclose(fp);
}
int hand_score(int a[2], int b[2], int c[2], int d[2], int e[2]){
    //This score will be filled in with the hand's score
    int score = 0;
    int hand_separator = pow(16,5);//16^5 is bigger than all of the individual card number calculations, so the multiple of 16^5 of a hand can be used
    //to distinguish between pair, two pair, etc.
    short flush, straight;// actually bool
    //make a 2-14 histogram of all card's ranks, will include 1 and 0 but both as empty values to make counting hella easy
    //this scheme makes the number 2 at position [2] etc
    int numranks = 14;
    int ranks[numranks];
    for(int i = 0; i <= numranks; i++){
        ranks[i] = 0;
    }
    ++ranks[a[0]];
    ++ranks[b[0]];
    ++ranks[c[0]];
    ++ranks[d[0]];
    ++ranks[e[0]];
    //Now the histogram is filled in, we can start doing shit with it

    //If flush (check if all suits are identical)

    if(a[1] == b[1] && a[1] == c[1] && a[1] == d[1] && a[1] == e[1])
        flush = 1;
    else
        flush = 0;

    //if straight (start at lowest non-zero value in histogram. if those 5 counting up are 1, then it's a straight)

    for(int i = 2; i <= numranks; i++){
        straight = 0;
        if(ranks[i] == 1){
            if(ranks[i] <11 && ranks[i+1] == 1 && ranks[i+2] == 1 && ranks[i+3] == 1 && ranks[i+4] == 1)
                straight = 1;
            break;
        }
    }
    //Checks specifically for A-5 straight edge case
    if(ranks[14] && ranks[2] && ranks[3] && ranks[4] && ranks[5]){
        straight = 1;
        ranks[14] = 0;//this makes sure that the straight score counter counts from the 5, not the A
        //Interesting side note: i didn't know this while originally writing this, but A-5 is actually the worst straight,
        //I thought it was tied for best bc it has an ace but its not.
    }

    if(flush || straight){
        if(!flush){
            //straight: scored highcard+4*16^5
            for(int i = numranks; i >= 0; i--){
                if(ranks[i]){
                    score = i+8*hand_separator;
                    break;
                }
            }
        }
        else if(!straight){
            //flush: scored (lowcard +16*2ndcard  +16^2*3rdcard  +16^3*4thcard +16^4*highcard)+5*16^5
            int pow16 = 0;
            for(int i = 2; i <= numranks; i++){
                if(ranks[i]){
                    score += i << 4*pow16++;
                }
            }
            score += 5*hand_separator;
        }
        else{
            //straight flush: scored highcard+8*16^5
            for(int i = numranks; i >= 0; i--){
                if(ranks[i]){
                    score = i+8*hand_separator;
                    break;
                }
            }
        }
    }
    else{
        int pair[2] = {0,0}, triple = 0, quad = 0;
        for(int i = 2; i <= numranks; i++){
            if(ranks[i] < 2){
                continue;
            }
            else if(ranks[i] == 2){
                if(!(pair[0]))
                    pair[0] = i;
                else
                    pair[1] = i;
            }
            else if(ranks[i] == 3){
                triple = i;
            }
            else if(ranks[i] == 4){
                quad = i;
                break;
            }
        }
        if(quad){
            //4kind: scored (1card+16*4card)+7*16^5
            for(int i = 2; i <= numranks; i++){
                if(ranks[i] == 1){
                    score = (i+16*quad)+7*hand_separator;
                    break;
                }
            }
        }
        else if(triple){
            if(pair[0]){
                //full house: scored (2card+16*3card)+6*16^5
                score = (pair[0]+16*triple)+6*hand_separator;
            }
            else{
                //3kind: scored (lowcard+16*highcard +16^2*3card)+3*16^5
                int pow2 = 0;
                for(int i = 2; i <= numranks; i++){
                    if(ranks[i] == 1){
                        score += i << pow2;
                        pow2 += 4;
                    }
                }
                score += triple << 8;
                score += 3*hand_separator;
            }
        }
        else if(pair[0]){
            if(pair[1]){
                //2pair: scored (1card+16*lowpair+16^2*highpair)+2*16^5
                for(int i = 2; i <= numranks; i++){
                if(ranks[i] == 1)
                    score = (i+16*pair[0]+16*16*pair[1])+2*hand_separator;
                    break;
                }
            }
            else{
                //1pair: scored (lowcard+16*midcard+16^2*highcard+16^3*2card)+1*16^5
                int pow2 = 0;
                for(int i = 2; i <= numranks; i++){
                    if(ranks[i] == 1){
                        score += i << pow2;
                        pow2 += 4;
                    }
                }
                score += pair[0] << 12;
                score += hand_separator;
            }
        }
        else{
            //high card: scored (lowcard+16*2ndcard+16^2*3rdcard+16^3*4thcard+16^4*highcard)+0*16^5
            int pow2 = 0;
            for(int i = 2; i <= numranks; i++){
                if(ranks[i]){
                    score += i << pow2;
                    pow2 += 4;
                }
            }
        }
    }
    return score;
}
void inverse_hand_score(int handscore, int *hand){
    hand[0] = handscore;
    int type = handscore/pow(16,5);//1 for pair, 2 for two pair, 7 for 4 of a kind, etc
    hand[1] = type;
    switch(type){
        case 0://high card
        case 5://flush or high card is just 5 different cards so you just return all 5 of them
            for(int i = 6; i>=2; i--){
                hand[i] = handscore%16;
                handscore /= 16;
            }
            break;
        case 1://pair
            for(int i = 6; i>=3; i--){
                hand[i] = handscore%16;
                handscore /= 16;
            }
            hand[2] = hand[3];
            break;
        case 2:// two pair
            for(int i = 6; i>=2; i-= 2){
                hand[i] = handscore%16;
                handscore /= 16;
            }
            hand[3] = hand[2];
            hand[5] = hand[4];
            break;
        case 3://3 of a kind
            for(int i = 6; i>=4; i--){
                hand[i] = handscore%16;
                handscore /= 16;
            }
            hand[2] = hand[3] = hand[4];
            break;
        case 6:// full house
            hand[5] = hand[6] = handscore%16;
            handscore /= 16;
            hand[2] = hand[3] = hand[4] = handscore%16;
            break;
        case 7:// four of a kind
            hand[6] = handscore%16;
            handscore /= 16;
            hand[2] = hand[3] = hand[4] = hand[5] = handscore%16;
            break;
        case 4://straight
        case 8://straight flush and flush are the same
            hand[2] = handscore%16;
            for(int i = 3; i<=6; i++)
                hand[i] = hand[2]-i+2;
            break;
    }
}
int five_of_seven(int a[], int b[], int c[], int d[], int e[], int f[], int g[]){
    int seven_cards[7][2] = {{a[0],a[1]},
                             {b[0],b[1]},
                             {c[0],c[1]},
                             {d[0],d[1]},
                             {e[0],e[1]},
                             {f[0],f[1]},
                             {g[0],g[1]}};
    int subset[5][2];
    int max_score = 0;
    int current_score;
    short subset_ind = 0;
    for(int i = 0; i < 7; i++){
        for(int j = i+1; j < 7; j++){
            subset_ind = 0;
            for(int k = 0; k < 7; k++){
                if(k == i || k == j){
                    continue;
                }
                subset[subset_ind][0] = seven_cards[k][0];
                subset[subset_ind][1] = seven_cards[k][1];
                subset_ind++;
            }
            current_score = hand_score(subset[0],subset[1],subset[2],subset[3],subset[4]);
            if(current_score > max_score)
                max_score = current_score;
        }
    }
    return max_score;
}
void all_hands(int a[], int b[], int *hand_data){
    int *hand0;
    hand0 = hand_data;
    int deck[52][2];//every possible card in a 52 card deck
    int numcards = sizeof(deck)/sizeof(deck[0]);
    int rank = 2, suit = 1;
    //initializes deck
    for(int i = 0; i<numcards; i++){
        deck[i][0] = rank;
        deck[i][1] = suit;
        rank++;
        if(rank>14){
            rank = 2;
            suit++;
        }
    }
    //loops through all possible 5 card hands from the deck, not including the starting pair of cards a and b
    //total iterations is the length of hands[]
    for(int i = 0; i<numcards; i++){
        if(array_eq(a,deck[i]) || array_eq(b,deck[i]))
            continue;
        for(int j = i+1; j<numcards; j++){
            if(array_eq(a,deck[j]) || array_eq(b,deck[j]))
                continue;
            for(int k = j+1; k<numcards; k++){
                if(array_eq(a,deck[k]) || array_eq(b,deck[k]))
                    continue;
                for(int l = k+1; l<numcards; l++){
                    if(array_eq(a,deck[l]) || array_eq(b,deck[l]))
                        continue;
                    for(int m = l+1; m<numcards; m++){
                        if(array_eq(a,deck[m]) || array_eq(b,deck[m]))
                            continue;
                        *hand_data = five_of_seven(a,b,deck[i],deck[j],deck[k],deck[l],deck[m]);
                        hand_data++;//moves to the next spot in the hand_data array
                    }
                }
            }
        }
    }
}
void all_starting_hands(FILE *fp){
    int a[2], b[2];
    b[1] = a[1] = 1;
    for(int i = 1; i<=2; i++){
        for(int j = 2; j<=14; j++){
            a[0] = j;
            for(int k = j+(2-i); k<=14; k++){
                b[0] = k;
                all_hands(a,b,&hands[0]);
                merge_sort(&hands[0],0,numhands-1);
                printf("%d\n", k);
                //print a bunch of hands from various places
                fprintf(fp,"\n%d and %d %s results:\n", a[0], b[0], b[1]-a[1] ? "off-suit" : "on-suit");
                print_hand(numhands/8,&hands[0],fp);
                print_hand(numhands/4,&hands[0],fp);
                print_hand(numhands/3,&hands[0],fp);
                print_hand(numhands/2,&hands[0],fp);
                print_hand(2*numhands/3,&hands[0],fp);
                print_hand(3*numhands/4,&hands[0],fp);
                print_hand(7*numhands/8,&hands[0],fp);
                print_hand(9*numhands/10,&hands[0],fp);
                print_hand(19*numhands/20,&hands[0],fp);
                print_hand(99*numhands/100,&hands[0],fp);
            }
        }
        b[1] = 2;
    }
}
void print_hand(int index, int *hand_arr, FILE *fp){
    int newhand[7];
    inverse_hand_score(hand_arr[index], &newhand[0]);
    fprintf(fp, "%.2f ", (float) index/ (float) numhands);
    for(int i = 0; i<7; i++)
        fprintf(fp, "%d, ", *(newhand+i));
    fprintf(fp, "\n");
}
//compares if two integer arrays are exactly the same: same elements, same order
//Used rn for comparing if two cards (of the form int, int for rank, suit) are the same
//Used in the all_hands function
int array_eq(int a1[], int a2[]){
    if(sizeof(a1) == sizeof(a2)){
        for(int i = 0; i<(sizeof(a1)/sizeof(*a1)); i++){
            if(a1[i] != a2[i])
                return 0;
        }
        return 1;
    }
    else
        return 0;
}
void merge_sort(int *arr, int start, int end){
    if(start<end){
        int mid = (start+end)/2;
        merge_sort(arr,start,mid);
        merge_sort(arr,mid+1,end);
        merge(arr,start,mid+1,end);
    }
}
//mid starts the second part to be sorted: i.e. sortedlist2[0] would be ar arr[mid]
//end is included in the arrays to be merged
void merge(int *arr, int start, int mid, int end){
    for(int n = start; n<=end; n++)
        temp[n] = arr[n];
    int i = start, j = mid;
    while(i<mid && j<=end){
        if(arr[i]<arr[j]){
            temp[i+j-mid] = arr[i];
            i++;
        }
        else{
            temp[i+j-mid] = arr[j];
            j++;
        }
    }
    for(;i<mid;i++)
        temp[i+j-mid] = arr[i];
    for(;j<=end;j++)
        temp[i+j-mid] = arr[j];
    for(int n = start; n<=end; n++)
        arr[n] = temp[n];
}
void swap(int *ptr1, int *ptr2){
    int temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}
