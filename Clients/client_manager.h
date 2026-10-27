#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include <vector>

struct Client
{
    int clients_id;
    std::string name;
    std::string surname;
    std::string email;
};

class ClientManager
{
private:
    pqxx::connection& connection;

public:
    ClientManager(pqxx::connection& conn);

    void addClient(
        const std::string& name,
        const std::string& surname,
        const std::string& email
    );

    void addPhone(int clients_id, const std::string& phone);

    std::optional<Client> findClientByEmail(
        const std::string& email
    );

    void updateClient(
        int Clients_id,
        const std::string& name,
        const std::string& surname,
        const std::string& email
    );

    bool deletePhone(int phones_id);

    bool deleteClient(int clients_id);

    void createTables();

    std::vector<Client> findClientByName(const std::string& name);

    std::vector<Client> findClientBySurname(const std::string& surname);

    std::vector<Client> findClientByPhone(const std::string& phone);
};