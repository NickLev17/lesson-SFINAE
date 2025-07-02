#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <tuple>
#include <string>
using namespace std;

/// @brief Функция вывода ip адреса. Реализована через перегрузку шаблонных функций в сответсвии с идиомой SFINAE.
/// @tparam T В качестве параметра функция принимает целочисленный тип данных.
/// @param value Принимаемый тип данных.
/// @return Тип void , ничего не возвращает.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_ip(const T &value)
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

/// @brief Базовый шаблон по молчанию false
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

/// @brief Функция вывода ip адреса. Реализована через перегрузку шаблонных функций в сответсвии с идиомой SFINAE.
/// @tparam T В качестве параметра функция принимает строковый тип данных.
/// @param value Принимаемый тип данных.
/// @return Тип void , ничего не возвращает.
template <typename T>
typename std::enable_if<is_string<T>::value, void>::type
print_ip(const T &value)
{

cout << value << "\n";
}

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

/// @brief Cпециализация шаблона для типа вектор.
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
/// @tparam T В качестве параметра функция принимает тип данных вектор или список.
/// @param value Принимаемый тип данных.
/// @return Тип void , ничего не возвращает.
template <typename T>
typename std::enable_if<is_vector_or_list<T>::value, void>::type
print_ip(const T &value)
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

/// @brief Базовый шаблон по умолчанию false
/// @tparam T В качестве параметра функция принимает тип данных кортеж.
template <typename U, typename T>
struct is_tuple : std::false_type
{
};

/// @brief Специализация шаблна с переменным количеством аргументов и проверкой типа.
/// @tparam T В качестве параметра функция принимает произвольное количетво аргументов
template <typename T, typename... Args>
struct is_tuple<std::tuple<Args...>, T>
{
private:
template <typename...>
struct all_same : std::true_type
{
};

template <typename First, typename... Next>
struct all_same<First, Next...>
: std::conditional<std::is_same<First, T>::value, all_same<Next...>, std::false_type>::type
{
};

public:
static const bool value = all_same<Args...>::value;
};

/// @brief Вспомогательная функция для вывода ip адреса
template <typename T, std::size_t... Is>
void print_tuple_impl(const T &tpl, std::index_sequence<Is...>)
{

using mas = int[];
(void)mas{0, ((std::cout << (Is == 0 ? "" : ".") << std::get<Is>(tpl)), 0)...};
std::cout << std::endl;
}

/// @brief Функция вывода ip адреса
/// @tparam T В качестве параметра функция принимает тип данных кортеж
/// @param value Принимаемый тип данных.
/// @return Тип void , ничего не возвращает.
template <typename T>
typename std::enable_if<is_tuple<T, typename std::tuple_element<0, T>::type>::value, void>::type
print_ip(const T &value)
{
typedef typename std::tuple_element<0, T>::type FirstType;

static_assert(is_tuple<T, FirstType>::value,
"All tuple elements must be of the same type");

const size_t size = std::tuple_size<T>::value;

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
