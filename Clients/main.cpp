#define NOMINMAX
#include "client_manager.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include "Windows.h"
#include <pqxx/pqxx>


int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    try
    {
        pqxx::connection connection(
            "host=localhost "
            "port=5432 "
            "dbname=test "
            "user=postgres "
            "password=Postgres"
        );

        std::cout << "Подключение к PostgreSQL успешно!" << std::endl;

        ClientManager manager(connection);

        manager.createTables();

        int choice = -1;

        while (choice != 0)
        {
            std::cout << "\n===== МЕНЕДЖЕР КЛИЕНТОВ =====" << std::endl;
            std::cout << "1. Добавить клиента" << std::endl;
            std::cout << "2. Добавить телефон" << std::endl;
            std::cout << "3. Изменить данные клиента" << std::endl;
            std::cout << "4. Удалить телефон" << std::endl;
            std::cout << "5. Удалить клиента" << std::endl;
            std::cout << "6. Найти клиента" << std::endl;
            std::cout << "0. Выход" << std::endl;

            std::cout << "Выберите действие: ";

            if (!(std::cin >> choice))
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::cout << "Ошибка! Введите число." << std::endl;
                continue;
            }

            switch (choice)
            {
            case 1:
            {
                std::string name;
                std::string surname;
                std::string email;

                std::cout << "Введите имя: ";
                std::cin >> name;

                std::cout << "Введите фамилию: ";
                std::cin >> surname;

                std::cout << "Введите email: ";
                std::cin >> email;

                manager.addClient(name, surname, email);

                std::cout << "Клиент успешно добавлен!" << std::endl;
                break;
            }

            case 2:
            {
                int clientId;
                std::string phone;
                std::cout << "Введите ID клиента: ";
                if (!(std::cin >> clientId))
                {
                    std::cin.clear();
                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );
                    std::cout << "Ошибка! ID клиента должен быть числом."
                        << std::endl;
                    break;
                }
                std::cout << "Введите номер телефона: ";
                std::cin >> phone;
                manager.addPhone(clientId, phone);
                std::cout << "Телефон успешно добавлен!" << std::endl;
                break;
            }

            case 3:
            {
                int clientId;
                std::string name;
                std::string surname;
                std::string email;

                std::cout << "Введите ID клиента: ";

                if (!(std::cin >> clientId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "Ошибка! ID клиента должен быть числом."
                        << std::endl;

                    break;
                }

                std::cout << "Введите новое имя: ";
                std::cin >> name;

                std::cout << "Введите новую фамилию: ";
                std::cin >> surname;

                std::cout << "Введите новый email: ";
                std::cin >> email;

                manager.updateClient(clientId, name, surname, email);

                std::cout << "Данные клиента обновлены!" << std::endl;
                break;
            }

            case 4:
            {
                int phoneId;

                std::cout << "Введите ID телефона для удаления: ";

                if (!(std::cin >> phoneId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "Ошибка! ID телефона должен быть числом."
                        << std::endl;

                    break;
                }

                if (manager.deletePhone(phoneId))
                {
                    std::cout << "Телефон успешно удалён!" << std::endl;
                }
                else
                {
                    std::cout << "Телефон с таким ID не найден!" << std::endl;
                }

                break;
            }

            case 5:
            {
                int clientId;

                std::cout << "Введите ID клиента для удаления: ";

                if (!(std::cin >> clientId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "Ошибка! ID клиента должен быть числом."
                        << std::endl;

                    break;
                }

                if (manager.deleteClient(clientId))
                {
                    std::cout << "Клиент успешно удалён!" << std::endl;
                }
                else
                {
                    std::cout << "Клиент с таким ID не найден!" << std::endl;
                }

                break;
            }

            case 6:
            {
                int searchChoice;

                std::cout << "\n--- Поиск клиента ---" << std::endl;
                std::cout << "1. По имени" << std::endl;
                std::cout << "2. По фамилии" << std::endl;
                std::cout << "3. По email" << std::endl;
                std::cout << "4. По телефону" << std::endl;
                std::cout << "Выберите способ поиска: ";

                if (!(std::cin >> searchChoice))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout << "Ошибка! Введите номер способа поиска."
                        << std::endl;

                    break;
                }

                std::string value;

                std::cout << "Введите значение для поиска: ";
                std::cin >> value;

                if (searchChoice == 1)
                {
                    auto clients = manager.findClientByName(value);

                    if (clients.empty())
                    {
                        std::cout << "Клиенты с таким именем не найдены!"
                            << std::endl;
                    }
                    else
                    {
                        for (const auto& client : clients)
                        {
                            std::cout << client.clients_id << " | "
                                << client.name << " "
                                << client.surname << " | "
                                << client.email << std::endl;
                        }
                    }
                }

                else if (searchChoice == 2)
                {
                    auto clients = manager.findClientBySurname(value);

                    if (clients.empty())
                    {
                        std::cout << "Клиенты с такой фамилией не найдены!"
                            << std::endl;
                    }
                    else
                    {
                        for (const auto& client : clients)
                        {
                            std::cout << client.clients_id << " | "
                                << client.name << " "
                                << client.surname << " | "
                                << client.email << std::endl;
                        }
                    }
                }

                else if (searchChoice == 3)
                {
                    auto client = manager.findClientByEmail(value);

                    if (client.has_value())
                    {
                        std::cout << client->clients_id << " | "
                            << client->name << " "
                            << client->surname << " | "
                            << client->email << std::endl;
                    }
                    else
                    {
                        std::cout << "Клиент не найден!" << std::endl;
                    }
                }

                else if (searchChoice == 4)
                {
                    auto clients = manager.findClientByPhone(value);

                    if (clients.empty())
                    {
                        std::cout << "Клиенты с таким телефоном не найдены!"
                            << std::endl;
                    }
                    else
                    {
                        for (const auto& client : clients)
                        {
                            std::cout << client.clients_id << " | "
                                << client.name << " "
                                << client.surname << " | "
                                << client.email << std::endl;
                        }
                    }
                }
                else
                {
                    std::cout << "Неверный способ поиска!" << std::endl;
                }

                break;
            }

            case 0:
                std::cout << "Выход из программы" << std::endl;
                break;

            default:
                std::cout << "Неверный пункт меню!" << std::endl;
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}