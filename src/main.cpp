#include "Disk.hpp"


int main() {
    std::cout << "Доброго времени суток. Перед Вами программа, которая позволяет управлять накопителем без использования стандартных файловых систем." << std::endl;
    std::cout << "Убедитесь, что запускаете программу с правами суперпользователя — большинство дистрибутивов не позволяют напрямую обращаться к диску без соответствующих разрешений." << std::endl;
    std::cout << "Будьте внимательны при указании накопителя. Ошибка может привести к повреждению системы и потере данных." << std::endl;
    std::cout << "Укажите путь к накопителю (например, — /dev/sdc или /dev/nvme0n6): ";

    std::string buff1, buff2;

    std::getline(std::cin, buff1);
    Disk disk = Disk(buff1);

    std::cout << "1| Отформатировать накопитель" << std::endl;
    std::cout << "2| Просмотреть содержимое накопителя (только для отформатированных накопителей!)" << std::endl;
    std::cout << "3| Добавить файл (только для отформатированных накопителей!)" << std::endl;
    std::cout << "4| Извлечь файл (только для отформатированных накопителей!)" << std::endl;
    std::cout << "5| Выйти" << std::endl;

    for (; ;) {
        for (int32_t i = 0; i < 4; i = i + 1) {
            std::cout << '\n';
        }
        std::cout.flush();
        std::cout << "Выберите действие: ";

        std::getline(std::cin, buff1);
        try {
            if (buff1 == "1") {
                disk.format();
                std::cout << "Накопитель был отформатирован." << std::endl;
            }
            else if (buff1 == "2") {
                std::cout << disk << std::endl;
            }
            else if (buff1 == "3") {
                std::cout << "Введите путь к файлу: ";
                std::getline(std::cin, buff1);
                disk.addFile(buff1);
                std::cout << "Файл был добавлен." << std::endl;
            }
            else if (buff1 == "4") {
                std::cout << "Введите имя извлекаемого файла: ";
                std::getline(std::cin, buff1);
                std::cout << "Введите путь, по которому нужно поместить извлекаемый файл: ";
                std::getline(std::cin, buff2);
                disk.getFile(buff1, buff2);
                std::cout << "Файл был извлечен." << std::endl;
            }
            else if (buff1 == "5") {
                std::cout << "Выход." << std::endl;
                return 0;
            }
            else {
                std::cout << "Неизвестное действие." << std::endl;
            }
        }
        catch (const char *e) {
            std::cout << e << std::endl;
            return 0;
        }
    }
}