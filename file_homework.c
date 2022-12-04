#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
 
// Driver code
int main()
{
    FILE* data_file;
    FILE* output_file;

    char character;
    char buffer[10]; // numbers can only have 10 digits
    int *storage_vector;  
// in order to have a dynamic size vector, ill use a poiner to int 
// and later allocate the needed memory in the HEAP
    int size_of_vector;
    int char_counter = 0;  // how many total chars have been processed
    int position = 0;      // position of the given char in a given string that represents an int

    data_file = fopen("data.txt", "r"); // open in read mode
 
    if (NULL == data_file) {
        printf("file can't be opened \n");
    }
 
    do {
        character = fgetc(data_file);
        if(character == ' ' || character == EOF){
// values are separated by a space or the end of the file            
            buffer[position] = 0;
            position = 0;
            if(char_counter == 0){
                size_of_vector = atoi(buffer);
// once having the first value we can convert it to an int and allocate the space for the vector                
                assert(size_of_vector!=0);
// atoi is an unsafe function because it returns 0 when the convertion gives 0 or fails.
// in this case is usable because we should never have a 0 in the begin of the file
                storage_vector = malloc(sizeof(int)*size_of_vector);
            }else{
                storage_vector[char_counter-1] = atoi(buffer);
                // printf("buffer: %d\n", storage_vector[char_counter-1]);
            }
            char_counter++;

            assert(char_counter<=size_of_vector + 1);
// check if we are trying to process more data that we are supposed to            
        }else{
            buffer[position++] = character;
        }
    } while (character != EOF);

    fclose(data_file);

    int i;
    double average = 0;
    int max = storage_vector[0]; // pre asign first value as max
    for(i=0;i<size_of_vector;i++){
       average += storage_vector[i];
       if(max < storage_vector[i]) max = storage_vector[i];
    }
    average /= size_of_vector;

    char output_string[50];
    sprintf(output_string, "Average of stored numbers: %.2lf / Max number: %d\n", average, max);
    printf("%s", output_string);
    
    output_file = fopen("output_data.txt", "w+"); // open in write mode
    fprintf(output_file, &output_string);
    free(storage_vector); // free the vector allocation

    return 0;
}