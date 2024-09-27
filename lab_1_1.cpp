#include <immintrin.h>
#include <iostream>
#include <fstream>

//Разработать функцию, для нахождения поэлементного произведения двух массивов чисел типа float.
//Элементы и длина массивов берётся из файла с исходными данными. Все математические операции должны проводиться с помощью intrinsic-функций из наборов инструкций SSE1 – SSE4.1.
//Основные ошибки, такие как пустые входные массивы и нулевой размер массива должны обрабатываться.
//Функция должна возвращать статус выполнения: если ошибка –  число отличное от нуля, если ошибок не было – 0.
//Функция должна принимать на вход следующие параметры: два массива с числами из файла, массив для записи результата, размер входных массивов.


enum class EXIT_CODE {
    SUCCESS,
    ARR_IS_NULLPTR,
    INCORRECT_LEN
};


EXIT_CODE mulArrays(float* arr1, float* arr2, float* dst_arr, int len) {
    if (arr1 == nullptr || arr2 == nullptr) return EXIT_CODE::ARR_IS_NULLPTR;
    if (len < 1) return EXIT_CODE::INCORRECT_LEN;

    int increment = len & ~0x03;
    int i = 0;
    __m128 reg1, reg2;

    for (; i < increment; i += 4) {
        reg1 = _mm_loadu_ps(&arr1[i]);
        reg2 = _mm_loadu_ps(&arr2[i]);
        __m128 dst = _mm_mul_ps(reg1, reg2);
        _mm_storeu_ps(&dst_arr[i], dst);
    }
    for (; i < len; i += 1) {
        dst_arr[i] = arr1[i] * arr2[i];
    }
    return EXIT_CODE::SUCCESS;
}

int main() {
    std::string line;
    int len;
    std::ifstream in("input_two_array.txt");
    if (in.is_open())
    {
        in >> len;

        float* arr1 = new float[len]();
        float* arr2 = new float[len]();

        for (int i = 0; i < len; i++) {
            std::getline(in, line, ';');
            line.replace(line.find(','), 1, ".");
            float number = std::stof(line);
            arr1[i] = number;
        }

        for (int i = 0; i < len; i++) {
            std::getline(in, line, ';');
            line.replace(line.find(','), 1, ".");
            float number = std::stof(line);
            arr2[i] = number;
        }
        auto res = mulArrays(arr1, arr2, arr1, len);
        switch(res) {
            case EXIT_CODE::SUCCESS: {
                for (int i = 0; i < len; ++i) {
                    std::printf("%f, ", arr1[i]);
                }
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
        delete[] arr1;
        delete[] arr2;
    }
    in.close();
    return 0;
}
