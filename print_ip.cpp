#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>
#include <string>
using namespace std;

/// \brief Базовый шаблон по молчанию false
/// @tparam T В качестве параметра функция принимает тип данных строка.
template <typename T, typename = void>
struct is_string : false_type
{
};

/// @brief Базовый шаблон true
/// @tparam T В качестве параметра функция принимает тип данных строка.
template <>
struct is_string<std::string> : true_type
{
};

/// @brief Обьединённая специализация шаблона для типо char* и const char*.
/// @tparam T В качестве параметра функция принимает тип данных строка си.
template <typename T>
struct is_string<T, std::enable_if_t<std::is_same_v<T, const char *> || is_same_v<T, char *>>> : std::true_type
{
};

/// @brief Базовый шаблон по молчанию false
/// @tparam T В качестве параметра функция принимает тип данных вектор.
template <typename T>
struct is_vector : false_type
{
};

/// @brief Cпециализация шаблона для типа вектор.
/// @tparam T В качестве параметра функция принимает тип данных вектор и аллоктор.
template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : true_type
{
};

/// @brief Базовый шаблон по молчанию false
/// @tparam T В качестве параметра функция принимает тип данных список.
template <typename T>
struct is_list : false_type
{
};

/// @brief Cпециализация шаблона для типа список.
/// @tparam T В качестве параметра функция принимает тип данных список и аллоктор.
template <typename T, typename Alloc>
struct is_list<std::list<T, Alloc>> : true_type
{
};

/// @brief Обьединенная специализация шаблона для типов вектор и список.
/// @tparam T В качестве параметра функция принимает типы данных вектор и список.
template <typename T>
struct is_vector_or_list : std::integral_constant<bool,
is_vector<T>::value || is_list<T>::value>
{
};

/// @brief Функция вывода ip адреса. Реализована через перегрузку шаблонных функций в сответсвии с идиомой SFINAE.
/// @tparam T В качестве параметра функция принимает различные типы данных: целочисленный, вектор, список, строки.
/// @param value Принимаемый тип данных.
/// @return Тип void , ничего не возвращает.
template <typename T>
typename std::enable_if<std::is_integral<T>::value ||
is_vector_or_list<T>::value ||
is_string<T>::value,
void>::type
print_ip(const T &value)
{

if constexpr (std::is_integral<T>::value)
{
size_t sizeType = sizeof(value);
const unsigned char *bytes = reinterpret_cast<const unsigned char *>(&value);

for (size_t i = 0; i < sizeType; ++i)
{
unsigned char b = bytes[sizeType - 1 - i];
cout << static_cast<unsigned int>(b);

if (i + 1 < sizeType)
{
cout << ".";
}
}

cout << "\n";
}
else if constexpr (is_vector_or_list<T>::value)
{

for (auto iter = value.begin(); iter != value.end();)
{

cout << *iter;
++iter;
if (iter != value.end())
{
cout << ".";
}
}
cout << "\n";
}
else if constexpr (is_string<T>::value)
{

for (const auto &v : value)
{
cout << v;
}
cout << "\n";
}
}

/// @brief Базовый шаблон по молчанию false
/// @tparam T В качестве параметра функция принимает тип данных кортеж.
template <typename U, typename T>
struct is_tuple : std::false_type
{
};

/// @brief Шаблон с переменным количеством аргументов.
/// @tparam T В качестве параметра функция принимает переменное количество аргументов.
template <typename T, typename... Args>
struct is_tuple<std::tuple<Args...>, T>
: std::conjunction<std::is_same<T, Args>...>
{
};

/// @brief Вспомогательная функция для вывода кортежа
/// @tparam T В качестве параметра функция принимает тип данных кортеж.
/// @return Тип void, ничего не возвращает
template <typename T, std::size_t... Is>
void print_tuple_impl(const T &tpl, std::index_sequence<Is...>)
{
((std::cout << (Is == 0 ? "" : ".") << std::get<Is>(tpl)), ...);
std::cout << std::endl;
}

/// @brief Функция вывода ip адреса.
/// @tparam T В качестве параметра функция принимает тип данных кортеж.
/// @return Тип void, ничего не возвращает
template <typename T>
typename enable_if<is_tuple<T, tuple_element_t<0, T>>::value, void>::type
print_ip(const T &value)
{
using FirstType = std::tuple_element_t<0, T>;

static_assert(is_tuple<T, FirstType>::value,
"All tuple elements must be of the same type");

constexpr size_t size = std::tuple_size_v<T>;
print_tuple_impl(value, std::make_index_sequence<size>{});
}

int main()
{

print_ip(int8_t{-1});
print_ip(int16_t{0});
print_ip(int32_t{2130706433});
print_ip(int64_t{8875824491850138409});

print_ip(string{"Hello, World!"});

print_ip(std::vector<int>{100, 200, 300, 400});
print_ip(std::list<short>{400, 300, 200, 100});

print_ip(std::make_tuple(123, 456, 789, 0));

return 0;
}
