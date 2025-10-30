#include <iostream>

#ifdef PEDANTIC_ERR_MODE
int main() {
    char c = 0;
    std::cout << "Input chars: ";

    while (std::cin.get(c)) {
        if (static_cast<unsigned char>(c) < 32 || static_cast<unsigned char>(c) > 127) {
            continue;
        }

        switch (c) {
            // GCC расширение: диапазоны в case
            case 'A' ... 'Z':
                std::cout << "uppercase letter" << std::endl;
                break;
            
            case 'a' ... 'z':
                std::cout << "lowercase letter" << std::endl;
                break;
            
            case '0' ... '9':
                std::cout << "digit" << std::endl;
                break;
            
            // Знаки препинания через диапазоны (где это возможно)
            case '!' ... '/':
            case ':' ... '@':
            case '[' ... '`':
            case '{' ... '~':
                std::cout << "punctuation" << std::endl;
                break;
            
            default:
                std::cout << "other" << std::endl;
        }
    }
    
    return 0;
}
#endif


#ifndef PEDANTIC_ERR_MODE
int main() {
    char c = 0;
    std::cout << "Input chars: ";
    
    while (std::cin.get(c)) {
        // Проверяем, что символ в диапазоне от 32 до 127
        if (static_cast<unsigned char>(c) < 32 || static_cast<unsigned char>(c) > 127) {
            continue; // Пропускаем символы вне заданного диапазона
        }

        switch (c) {
            // Заглавные буквы: A-Z
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
            case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
            case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
            case 'Y': case 'Z':
                std::cout << "uppercase letter" << std::endl;
                break;
            
            // Строчные буквы: a-z
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
            case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
            case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
            case 'y': case 'z':
                std::cout << "lowercase letter" << std::endl;
                break;
            
            // Десятичные цифры: 0-9
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            case '8': case '9':
                std::cout << "digit" << std::endl;
                break;
            
            // Знаки препинания (основные)
            case '!': case '"': case '#': case '$': case '%': case '&': case '\'': case '(':
            case ')': case '*': case '+': case ',': case '-': case '.': case '/': case ':':
            case ';': case '<': case '=': case '>': case '?': case '@': case '[': case '\\':
            case ']': case '^': case '_': case '`': case '{': case '|': case '}': case '~':
                std::cout << "punctuation" << std::endl;
                break;
            
            // Прочие символы (включая пробел)
            default:
                std::cout << "other" << std::endl;
        }
    }
    
    return 0;
}
#endif