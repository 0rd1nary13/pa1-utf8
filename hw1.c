#include <stdio.h>
#include <stdint.h>
#include <string.h>

int32_t is_ascii(char str[]) {
    for (int32_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < 0 || str[i] > 127) {
            return 0;
        }
    }
    return 1;
}

int32_t capitalize_ascii(char str[]) {
    int counter = 0;
    for (int32_t i = 0; str[i] != '\0'; i++) {
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
    for (int32_t i = 0; str[i] != '\0'; i++) {
        if ((str[i] & 0b11000000) != 0b10000000) {
            // If the byte is not a continuation byte, it is the start of a new character
            length++;
        }
    }
    return length;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int32_t byte_index = 0;
    int32_t codepoint_count = 0;
    for (int32_t i = 0; str[i] != '\0'; i++) {
        if ((str[i] & 0b11000000) != 0b10000000) {
            // If the byte is not a continuation byte, it is the start of a new character
            if (codepoint_count == cpi) {
                // If the code point index matches the desired index, return the byte index
                return byte_index;
            }
            codepoint_count++;
        }
        byte_index++;
    }
    // If the index is out of bounds, return -1
    return -1;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int32_t byte_start = codepoint_index_to_byte_index(str, cpi_start);
    int32_t byte_end = codepoint_index_to_byte_index(str, cpi_end);
    printf("%d",byte_end);
    printf("%d",byte_start);

    if (byte_start == -1 || byte_end == -1 || byte_start >= byte_end) {
        result[0] = '\0'; // Invalid indices, return an empty string
        return;
    }

    int32_t j = 0;
    for (int32_t i = byte_start; i < byte_end; i++) {
        result[j++] = str[i];
    }
    result[j] = '\0'; // Null-terminate the result string
}

int32_t convert_to_decimal(char k){
    if(width_from_start_byte(k)==1){
        return k&0b11111111;
    }else if(width_from_start_byte(k)==2){
        return k&0b00011111;
    }else if(width_from_start_byte(k)==3){
        return k&0b00001111;
    }else if(width_from_start_byte(k)==4){
        return k&0b00000111;
    }
    return  k&0b00111111;
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
    int32_t codepoint = convert_to_decimal(str[byte_index]);
    for (int32_t i = 1; i < width; i++) {
        codepoint = (codepoint << 6) | convert_to_decimal(str[byte_index + i]);
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
    printf("Is 🔥 ASCII? %d\n", is_ascii("🔥"));
    //    === Output ===
    //    Is 🔥 ASCII? 0

    printf("Is abcd ASCII? %d\n", is_ascii("abcd"));
    //    === Output ===
    //    Is abcd ASCII? 1

    int32_t ret = 0;
    char str[] = "abcd";
    ret = capitalize_ascii(str);

    printf("Capitalized String: %s\nCharacters updated: %d\n", str, ret);
    // === Output ===
    // Capitalized String: ABCD
    // Characters updated: 4

    char s[] = { 'H', 0xC3, 0xA9, 'y', 0 }; // same as { 'H', 0xC3, 0xA9, 'y', 0 },   é is start byte + 1 cont. byte
    printf("Width: %d bytes\n", width_from_start_byte(s[1])); // start byte 0xC3 indicates 2-byte sequence

    // === Output ===
    // Width: 2 bytes

    printf("Width: %d bytes\n", width_from_start_byte(s[2])); // start byte 0xA9 is a continuation byte, not a start byte

    // === Output ===
    // Width: -1

    char str1[] = "Joséph";
    printf("Length of string %s is %d\n", str1, utf8_strlen(str1));  // 6 codepoints, (even though 7 bytes)

    // === Output ===
    // Length of string Joséph is 6

    int32_t idx = 4;
    printf("Codepoint index %d is byte index %d\n", idx, codepoint_index_to_byte_index("Joséph", idx));

    // === Output ===
    // Codepoint index 4 is byte index 5

    char result[17];
    utf8_substring("🦀🦮🦮🦀🦀🦮🦮", 3, 7, result);
    printf("String: %s\nSubstring: %s\n", "🦀🦮🦮🦀🦀🦮🦮", result);
    // === Output ===
    // String: 🦀🦮🦮🦀🦀🦮🦮
    // Substring: 🦀🦀🦮🦮

    char str2[] = "Joséph";
    int32_t idx1 = 4;
    printf("Codepoint at %d in %s is %d\n", idx1, str2, codepoint_at(str2, idx1)); // 'p' is the 4th codepoint

    // === Output ===
    // Codepoint at 4 in Joséph is 112
}