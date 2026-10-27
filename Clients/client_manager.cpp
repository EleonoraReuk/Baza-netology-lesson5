#include "client_manager.h"

ClientManager::ClientManager(pqxx::connection& conn)
    : connection(conn)
{
}

void ClientManager::addClient(
    const std::string& name,
    const std::string& surname,
    const std::string& email)
{
    pqxx::work txn(connection);

    txn.exec(
        "INSERT INTO clients (name, surname, email) "
        "VALUES ($1, $2, $3)",
        pqxx::params{ name, surname, email }
    );

    txn.commit();
}

void ClientManager::addPhone(
    int clients_id,
    const std::string& phone)
{
    pqxx::work txn(connection);

    txn.exec(
        "INSERT INTO phones (clients_id, phone) "
        "VALUES ($1, $2)",
        pqxx::params{ clients_id, phone }
    );

    txn.commit();
}


void ClientManager::updateClient(
    int clients_id,
    const std::string& name,
    const std::string& surname,
    const std::string& email)
{
    pqxx::work txn(connection);

    txn.exec(
        "UPDATE clients "
        "SET name = $1, surname = $2, email = $3 "
        "WHERE clients_id = $4",
        pqxx::params{ name, surname, email, clients_id }
    );

    txn.commit();
}

bool ClientManager::deletePhone(int phones_id)
{
    pqxx::work txn(connection);

    auto result = txn.exec(
        "DELETE FROM phones "
        "WHERE phones_id = $1 "
        "RETURNING phones_id",
        pqxx::params{ phones_id }
    );

    bool deleted = !result.empty();

    txn.commit();

    return deleted;
}


bool ClientManager::deleteClient(int clients_id)
{
    pqxx::work txn(connection);

    auto result = txn.exec(
        "DELETE FROM clients "
        "WHERE clients_id = $1 "
        "RETURNING clients_id",
        pqxx::params{ clients_id }
    );

    bool deleted = !result.empty();

    txn.commit();

    return deleted;
}



std::optional<Client> ClientManager::findClientByEmail(
    const std::string& email)
{
    pqxx::read_transaction txn(connection);

    auto result = txn.exec(
        "SELECT clients_id, name, surname, email "
        "FROM clients WHERE email = $1",
        pqxx::params{ email }
    );

    if (result.empty())
    {
        return std::nullopt;
    }

    Client client{
        result[0]["clients_id"].as<int>(),
        result[0]["name"].as<std::string>(),
        result[0]["surname"].as<std::string>(),
        result[0]["email"].as<std::string>()
    };

    return client;
}


std::vector<Client> ClientManager::findClientByName(
    const std::string& name)
{
    pqxx::read_transaction txn(connection);

    auto result = txn.exec(
        "SELECT clients_id, name, surname, email "
        "FROM clients "
        "WHERE name = $1",
        pqxx::params{ name }
    );

    std::vector<Client> clients;

    for (const auto& row : result)
    {
        Client client{
        row["clients_id"].as<int>(),
        row["name"].as<std::string>(),
        row["surname"].as<std::string>(),
        row["email"].as<std::string>()
        };
        clients.push_back(client);
    }

    return clients;
}


std::vector<Client> ClientManager::findClientBySurname(
    const std::string& surname)
{
    pqxx::read_transaction txn(connection);

    auto result = txn.exec(
        "SELECT clients_id, name, surname, email "
        "FROM clients "
        "WHERE surname = $1",
        pqxx::params{ surname }
    );

    std::vector<Client> clients;

    for (const auto& row : result)
    {
        Client client{
        row["clients_id"].as<int>(),
        row["name"].as<std::string>(),
        row["surname"].as<std::string>(),
        row["email"].as<std::string>()
        };
        clients.push_back(client);
    }

    return clients;
}


std::vector<Client> ClientManager::findClientByPhone(
    const std::string& phone)
{
    pqxx::read_transaction txn(connection);

    auto result = txn.exec(
        "SELECT DISTINCT clients.clients_id, clients.name, "
        "clients.surname, clients.email "
        "FROM clients "
        "JOIN phones ON clients.clients_id = phones.clients_id "
        "WHERE phones.phone = $1",
        pqxx::params{ phone }
    );

    std::vector<Client> clients;

    for (const auto& row : result)
    {
        Client client{
            row["clients_id"].as<int>(),
            row["name"].as<std::string>(),
            row["surname"].as<std::string>(),
            row["email"].as<std::string>()
        };

        clients.push_back(client);
    }

    return clients;
}


void ClientManager::createTables()
{
    pqxx::work txn(connection);

    txn.exec(
        "CREATE TABLE IF NOT EXISTS clients ("
        "clients_id SERIAL PRIMARY KEY, "
        "name VARCHAR(100), "
        "surname VARCHAR(100), "
        "email VARCHAR(100)"
        ")"
    );

    txn.exec(
        "CREATE TABLE IF NOT EXISTS phones ("
        "phones_id SERIAL PRIMARY KEY, "
        "clients_id INTEGER NOT NULL, "
        "phone TEXT NOT NULL, "
        "FOREIGN KEY (clients_id) "
        "REFERENCES clients(clients_id) ON DELETE CASCADE"
        ")"
    );

    txn.commit();
}

