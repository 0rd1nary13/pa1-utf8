#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int32_t is_ascii(char str[]) {
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if (str[i] < 0 || str[i] > 127) {
            return false;
        }
    }
    return true;
}

int32_t capitalize_ascii(char str[]) {
    int counter = 0;
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
            counter+=1;
        }
    }
    return counter;
}

int32_t width_from_start_byte(unsigned char start_byte) {
    // Check if the start byte represents a 1-byte UTF-8 character
    if ((start_byte & 0b10000000) == 0b00000000) {
        // The leading bit is 0, indicating a 1-byte character (0xxxxxxx)
        return 1;
    }
    // Check if the start byte represents a 2-byte UTF-8 character
    else if ((start_byte & 0b11100000) == 0b11000000) {
        // The leading bits are 110, indicating a 2-byte character (110xxxxx)
        return 2;
    }
    // Check if the start byte represents a 3-byte UTF-8 character
    else if ((start_byte & 0b11110000) == 0b11100000) {
        // The leading bits are 1110, indicating a 3-byte character (1110xxxx)
        return 3;
    }
    // Check if the start byte represents a 4-byte UTF-8 character
    else if ((start_byte & 0b11111000) == 0b11110000) {
        // The leading bits are 11110, indicating a 4-byte character (11110xxx)
        return 4;
    }
    // If none of the above, it is an invalid start byte for a UTF-8 character
    else {
        return -1;
    }
}

int32_t utf8_strlen(char str[]) {
    int32_t length = 0;
    //return -1 if the string is not valid UTF-8
    if (strlen(str) == 0) {
        return -1;
    }
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if ((str[i] & 0b11000000) != 0b10000000) {
            // If the byte is not a continuation byte, it is the start of a new character
            length+=1;
        }
    }
    return length;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int32_t byte_index = 0;
    int32_t codepoint_count = 0;
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if ((str[i] & 0b11000000) != 0b10000000) {
            // If the byte is not a continuation byte, it is the start of a new character
            if (codepoint_count == cpi) {
                // If the code point index matches the desired index, return the byte index
                return byte_index;
            }
            codepoint_count+=1;
        }
        byte_index+=1;
    }
    // If the index is out of bounds, return -1
    return -1;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int32_t byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int32_t byte_end = codepoint_index_to_byte_index(str, cpi_end);
    if(byte_start >= byte_end){
        result[0] = '\0';
        return;
    }
    // If the start or end index is out of bounds, return an empty string
    if (byte_start == -1 || byte_end == -1) {
        result[0] = '\0';
        return;
    }
    //change the string to the substring
    for (int32_t i = byte_start; i < byte_end; i+=1) {
        result[i - byte_start] = str[i];
    }
    result[byte_end - byte_start] = '\0';
    //printf("String: %s\nSubstring: %s\n", str, result);
}

int32_t codepoint_at(char str[], int32_t cpi){

    int32_t byte_index = codepoint_index_to_byte_index(str, cpi);

    if (byte_index == -1) {
        return -1;
    }
    int32_t width = width_from_start_byte(str[byte_index]);
    if (width == -1) {
        return -1;
    }
    int32_t codepoint = str[byte_index];
    for (int32_t i = 1; i < width; i+=1) {
        codepoint = (codepoint << 6) | (str[byte_index + i] & 0b00111111);
    }
    return codepoint;
}

char is_animal_emoji_at(char str[], int32_t cpi){
    int a = codepoint_at(str,cpi);
    if(a > 127999 && a < 128064 || a > 129407 && a < 129455){
        return 1;
    }
    return 0;
}

int main(){
//   ```  Enter a UTF-8 encoded string: My 🐩’s name is Erdős.
// Valid ASCII: false
// Uppercased ASCII: "MY 🐩’S NAME IS ERDőS."
// Length in bytes: 27
// Number of code points: 21
// Bytes per code point: 1 1 1 4 3 1 1 1 1 1 1 1 1 1 1 1 1 1 2 1 1
// Substring of the first 6 code points: "My 🐩’s"
// Code points as decimal numbers: 77 121 32 128041 8217 115 32 110 97 109 101 32 105 115 32 69 114 100 337 115 46
// Animal emojis: 🐩```
    char str[100];
    char result[100];
    printf("Enter a UTF-8 encoded string: ");
    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = 0;
    printf("Valid ASCII: %s\n", is_ascii(str) ? "true" : "false");
    printf("Uppercased ASCII: \"%s\"\n", str);
    printf("Length in bytes: %ld\n", strlen(str));
    printf("Number of code points: %d\n", utf8_strlen(str));
    printf("Bytes per code point: ");
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        printf("%d ", width_from_start_byte(str[i]));
    }
    printf("\n");
    utf8_substring(str, 0, 6, result);
    printf("Substring of the first 6 code points: \"%s\"\n", result);
    printf("Code points as decimal numbers: ");
    for (int32_t i = 0; i < utf8_strlen(str); i+=1) {
        printf("%d ", codepoint_at(str, i));
    }
    printf("\n");
    printf("Animal emojis: ");
    for (int32_t i = 0; i < utf8_strlen(str); i+=1) {
        if (is_animal_emoji_at(str, i)) {
            printf("%c", str[codepoint_index_to_byte_index(str, i)]);
        }
    }
    printf("\n");
    return 0;
    
}