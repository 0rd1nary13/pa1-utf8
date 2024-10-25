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
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if (str[i] >= 97 && str[i] <= 122) {
            str[i] = str[i] - 32;
        }
    }
    return 0;
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
    int32_t codepoint_at = 0;
    int32_t i = 0;
    while (i < cpi){
        if(utf8_strlen(str) == -1){
            return -1;
        }
        codepoint_at += width_from_start_byte(str[codepoint_at]);
        i += 1;
    }
    return codepoint_at;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int32_t byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int32_t byte_end = codepoint_index_to_byte_index(str, cpi_end);
    // printf("byte_start: %d\n", byte_start);
    // printf("byte_end: %d\n", byte_end);
    if(byte_start > byte_end){
        result[0] = '\0';
        return;
    }
    // // If the start or end index is out of bounds, return an empty string
    if (byte_start == -1 || byte_end == -1) {
        result[0] = '\0';
        return;
    }
    //change the string to the substring
    strncpy(result, &str[byte_start] , byte_end - byte_start);
    result[byte_end - byte_start] = '\0';
}

int32_t codepoint_at(char str[], int32_t cpi) {
    int32_t byte_index = codepoint_index_to_byte_index(str, cpi);
    if (byte_index == -1) {
        return -1;
    }
    int32_t width = width_from_start_byte((unsigned char)str[byte_index]);
    
    if (width == -1) {
        return -1;
    }

    int32_t codepoint = 0;
    unsigned char first_byte = str[byte_index];
    if(width == 1){
        codepoint = first_byte;
    }
    else if(width == 2){
        codepoint = ((first_byte & 0x1F) << 6) | (str[byte_index + 1] & 0x3F);
    }
    else if(width == 3){
        codepoint = ((first_byte & 0x0F) << 12) | ((str[byte_index + 1] & 0x3F) << 6) | (str[byte_index + 2] & 0x3F);
    }
    else if(width == 4){
        codepoint = ((first_byte & 0x07) << 18) | ((str[byte_index + 1] & 0x3F) << 12) | ((str[byte_index + 2] & 0x3F) << 6) | (str[byte_index + 3] & 0x3F);
    }

    return codepoint;
}

char is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t codepoint = codepoint_at(str, cpi);
    if (codepoint == -1) {
        return 0; // Invalid codepoint
    }

    // Check if the codepoint is within the specified animal emoji ranges
    if ((codepoint >= 128000 && codepoint <= 128063) || // 🐀 to 🐿️
        (codepoint >= 129408 && codepoint <= 129454)) { // 🦀 to 🦮
        return 1;
    }
    return 0;
}
void next_utf8_char(char str[], int32_t cpi, char result[]){
    int32_t byte_index = codepoint_index_to_byte_index(str, cpi);
    int32_t width = width_from_start_byte((unsigned char)str[byte_index]);
    if(width == 1){
        result[0] = str[byte_index];
        result[1] = '\0';
    }
    else if(width == 2){
        result[0] = str[byte_index];
        result[1] = str[byte_index + 1];
        result[2] = '\0';
    }
    else if(width == 3){
        result[0] = str[byte_index];
        result[1] = str[byte_index + 1];
        result[2] = str[byte_index + 2];
        result[3] = '\0';
    }
    else if(width == 4){
        result[0] = str[byte_index];
        result[1] = str[byte_index + 1];
        result[2] = str[byte_index + 2];
        result[3] = str[byte_index + 3];
        result[4] = '\0';
    }
}


int main(){

    char str[100];
    char result[100];
    char newstr[100];
    int8_t new_int[100];
    printf("Enter a UTF-8 encoded string: \n");
    fgets(str, 100, stdin);
//    fgets(new_int, 100, stdin);
    //result = str;
    for(int i = 0; i < 100; i++){
        result[i] = str[i];
        newstr[i] = str[i];
    }
    str[strcspn(str, "\n")] = 0;
    printf("Valid ASCII: %s\n", is_ascii(str) ? "true" : "false");
    capitalize_ascii(result);
    printf("Uppercased ASCII: \"%s\"\n", result);
    printf("Length in bytes: %ld\n", strlen(str));
    printf("Number of code points: %d\n", utf8_strlen(str));
    printf("Bytes per code point: ");
    for (int32_t i = 0; str[i] != '\0'; i+=1) {
        if(width_from_start_byte(str[i]) != -1){
            printf("%d ", width_from_start_byte(str[i]));
            }
    }
    printf("\n");

    utf8_substring(str, 0, 6, result);
    printf("Substring of the first 6 code points: \"%s\"\n", result);

    printf("Code points as decimal numbers: ");
    for (int32_t i = 0; i < utf8_strlen(newstr); i+=1) {
        printf("%d ", codepoint_at(newstr, i));
    }
    printf("\n");
    printf("Animal emojis: ");
    for (int32_t i = 0; i < utf8_strlen(str); i+=1) {
        if (is_animal_emoji_at(str, i)) {
           //print animal emoji codepoint
            int32_t byte_index = codepoint_index_to_byte_index(str, i);
            int32_t width = width_from_start_byte((unsigned char)str[byte_index]);
            for (int32_t j = 0; j < width; j++) {
                printf("%c", str[byte_index + j]);
            }
        }
    }
    printf("\n");
    int32_t idx = 3;
    next_utf8_char(str, idx, result);
    printf("Next Character of Codepoint at Index 3: %s\n",result);
    return 0;
    }
