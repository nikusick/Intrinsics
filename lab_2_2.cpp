#include <immintrin.h>
#include <iostream>
#include <fstream>

//Разработать функцию, для нахождения максимального элемента массива чисел типа float.
//Элементы и длина массива берётся из файла с исходными данными. Все математические операции должны проводиться с помощью intrinsic-функций из наборов инструкций SSE1 – SSE4.1.
//Основные ошибки, такие как пустые входные массивы и нулевой размер массива должны обрабатываться.
//Функция должна возвращать статус выполнения: если ошибка –  число отличное от нуля, если ошибок не было – 0.
//Функция должна принимать на вход следующие параметры: массив с числами из файла, переменную для записи максимума, размер входного массива.


enum class EXIT_CODE {
    SUCCESS,
    ARR_IS_NULLPTR,
    INCORRECT_LEN
};


EXIT_CODE getMaxElement(float* arr, float& max, int len) {
    if (arr == nullptr) return EXIT_CODE::ARR_IS_NULLPTR;
    if (len < 1) return EXIT_CODE::INCORRECT_LEN;
    int increment = len & ~0x03;
    int i;
    __m128 reg1, reg2;
    reg1 = _mm_loadu_ps(&arr[0]);
    for (i = 4; i < increment; i += 4) {
        reg2 = _mm_loadu_ps(&arr[i]);
        reg1 = _mm_max_ps(reg1, reg2);
        reg1 = _mm_max_ps(reg1, _mm_shuffle_ps(reg1, reg1, _MM_SHUFFLE(3, 2, 1, 0))); // max(0, 3), max(1, 2), max(2, 1), max(3, 0)
        reg1 = _mm_max_ps(reg1, _mm_shuffle_ps(reg1, reg1, _MM_SHUFFLE(1, 0, 2, 3))); // max(0, 1, 2, 3), max(0, 1, 2, 3), max(2, 1), max(3, 0)
    }
    _mm_store_ps(&max, reg1);
    for (; i < len; i += 1) {
        max = std::max(max, arr[i]);
    }
    return EXIT_CODE::SUCCESS;
}

int main() {
    std::string line;
    int len;
    std::ifstream in("input_one_array.txt");
    if (in.is_open())
    {
        in >> len;

        float* arr = new float[len]();

        for (int i = 0; i < len; i++) {
            std::getline(in, line, ';');
            line.replace(line.find(','), 1, ".");
            float number = std::stof(line);
            arr[i] = number;
        }
        float maxElement;
        auto res = getMaxElement(arr, maxElement, len);
        switch(res) {
            case EXIT_CODE::SUCCESS: {
                std::printf("%f", maxElement);
                break;
            }
            case EXIT_CODE::ARR_IS_NULLPTR: {
                std::cout << "Некорректный массив";
                break;
            }
            case EXIT_CODE::INCORRECT_LEN: {
                std::cout << "Некорректная длина";
                break;
            }
        }
        delete[] arr;
    }
    in.close();
    return 0;
}
